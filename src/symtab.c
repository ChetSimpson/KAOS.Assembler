/*****************************************************************************
	symtab.c	- Symbol Table management

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "proto.h"
#include "label.h"


static Symbol	*root;			/* root node of the tree */
static int		localoccur;		/* occurance of local labels			*/
static int		localcount;
static char		cNameSpace[MAX_LABELSIZE + 1];


void SetNamespace(EnvContext *ctx, const char *ns)
{
	strcpy(cNameSpace, ns);
}

void ClearNamespace(EnvContext *ctx)
{
	if(0 == cNameSpace[0])
	{
		warning(ctx, WARN_NONAMESPACE, "namespace not set");
		return;
	}

	cNameSpace[0] = 0;
}

bool IsInNamespace()
{
	return(0 != *cNameSpace);
}

void ResetLocalLabels(EnvContext *ctx)
{
	localoccur = 0;
	localcount = 0;
}


void InitSymbolTable(EnvContext *ctx)
{
	root = NULL;
	ResetLocalLabels(ctx);

	BeginSegment(ctx, SEGMENT_CODE);
	if(CPU_6309 == ctx->m_CPUType)
	{
		AddSymbol(ctx, "__CPU_6309__", true, SYM_INTERNAL_VALUE, NULL, 0);
	}
	else
	{
		AddSymbol(ctx, "__CPU_6809__", true, SYM_INTERNAL_VALUE, NULL, 0);
	}

#ifdef _DEBUG
	AddSymbol(ctx, "__KASM_DEBUG__", true, SYM_INTERNAL_VALUE, NULL, 0);
#endif

	AddSymbol(ctx, "__KASM__", true, SYM_INTERNAL_VALUE, NULL, 0);

	EndSegment(ctx);
}


/*
 *  DumpSymTable --- prints the symbol table in alphabetical order
 */
void DumpSymTable2(EnvContext *ctx, Symbol *ptr)
{
	if (ptr != NULL)
	{
		DumpSymTable2(ctx, ptr->Lnext);
		if('#' != *ptr->name)
		{
			const char *type;

			switch(ptr->type)
			{
			case SYM_INTERNAL_VALUE:
				type = NULL;
				break;

			case SYM_ADDRESS:
				type = "";
				break;
			case SYM_VALUE:
				type = "Constant";
				break;

			case SYM_STRUCTURE:
				type = "Structure definition";
				break;

			case SYM_STRUCTDATA:
				type = "Data";
				break;

			case SYM_UNION:
				type = "Union";
				break;

			default:
				ASSERTX("Unknown symbol type is list");
				type = "Unknown";
				break;
			}

			if(NULL != type)
			{
				if(((u_int32)ptr->value) > 0xffff)
				{
					fprintf(stdout, "%-16s %08lux  %s\n", ptr->name, (u_int32)ptr->value, type);
				}
				else
				{
					fprintf(stdout, "%-16s     %04x\t  %s\n", ptr->name, (u_int16)ptr->value, type);
				}

				fflush(stdout);
			}
		}
		DumpSymTable2(ctx, ptr->Rnext);
	}
}

void DumpSymTable(EnvContext *ctx)
{
	printf ("\n\nSymbol table:\n------------------------------\n");
	DumpSymTable2(ctx, root);
}



/*
 *  DumpCrossRef  --  prints the cross reference table
 */
void DumpCrossRef2(Symbol *point)
{
	Line *tp;

	int i = 1;
	if (point != NULL)
	{
		DumpCrossRef2(point->Lnext);

		if('#' != *point->name)
		{
			fprintf(stdout, "%-16s %04x *", point->name, (u_int16)point->value);
			tp = point->L_list;
			while (tp != NULL)
			{
				if(i++ > 10)
				{
					i = 1;
					fprintf(stdout, "\n                       ");
				}
				fprintf(stdout, "%04d ",tp->L_num);
				tp = tp->next;
			}
			fprintf(stdout, "\n");
		}
		DumpCrossRef2(point->Rnext);
	}
}

void DumpCrossRef()
{
	printf ("\n\nSymbol table cross reference:\n------------------------------\n");
	DumpCrossRef2(root);
}


int GetNextLocalLabel()
{
	if(0 != localcount)
	{
		localcount = 0;
		localoccur++;
	}

	return localoccur;
}

int GetLocalLabel()
{
	return localoccur;
}

/*----------------------------------------------------------------------------
	proclabel --- Preprocess a local label
----------------------------------------------------------------------------*/
static bool proclabel(EnvContext *ctx, const char *str, char *newname, const bool includens)
{
	size_t length = strlen(str);
	bool isLocal;

	/* Make sure label is correct length */
	if(length > MAX_LABELSIZE)
	{
		return(false);
	}

	isLocal = HasLocalLabelChar(ctx, str);

	if(true == isLocal)
	{	/* if first character of label is an @ symbol, its a local */

		localcount++;

		if(length > MAX_LABELSIZE - 7)
		{
			error(ctx, ERR_GENERAL, "local label too long");
			return(false);
		}

		if(true == IsMacroOpen())
		{
			sprintf(newname,"#m%s-%d", str, GetMacroLocalLabel());
		}
		else
		{
			sprintf(newname,"#s%s-%d", str, GetLocalLabel());
		}
	}
	else
	{
		*newname = 0;

		if(true == includens)
		{
			if(':' == *str)
			{
				str++;
			}
			
			else if(0 != cNameSpace[0])
			{
				strcpy(newname, cNameSpace);
				strcat(newname, ":");
			}
		}

		strcat(newname, str);
	}

	return(true);
}


static int CompareSymbol(EnvContext *ctx, const char *str1, const char *str2)
{
	int retval;

	if(true == ctx->m_Compat.m_IgnoreCase)
	{
		retval = stricmp(str1, str2);
	}
	else
	{
		retval = strcmp(str1, str2);
	}

	return retval;
}
	

static Symbol *FindSymbol2(EnvContext *ctx, const char *name)
{
	Symbol	*list;

	/* Point to the root of the label tree */
	list = root;

	/* Find the label */
	while(list)
	{
		int result;
		result = CompareSymbol(ctx, name, list->name);
		if(0 == result)
		{
			return (list);
		}
		
		else if(result < 0)
		{
			list = list->Lnext;
		}

		else
		{
			list = list->Rnext;
		}
	}

	return (NULL);
}

Symbol *FindSymbol(EnvContext *ctx, const char *name, const bool noerror, const bool onlyns)
{
	Symbol *sym;
	char newname[MAX_LABELSIZE * 3];

	/* Check to see if label is symantically correct */
	if(false == proclabel(ctx, name, newname, true))
	{
		return NULL;
	}

	sym = FindSymbol2(ctx, newname);
	if(NULL == sym && false == onlyns)
	{
		if(false == proclabel(ctx, name, newname, false))
		{
			return NULL;
		}

		sym = FindSymbol2(ctx, newname);
	}

	if (NULL == sym && 2 == ctx->m_Pass && false == noerror)
	{
#ifdef _DEBUG
		error(ctx, ERR_UNDEFINED, "'%s' (%s) not defined", name, newname);
#else
		error(ctx, ERR_UNDEFINED, "'%s' not defined", name);
#endif
	}

	return sym;

}

/*----------------------------------------------------------------------------
	Symbol --- add a symbol to the table
----------------------------------------------------------------------------*/
Symbol *AddSymbol(EnvContext *ctx,
				  const char *labelIn,
				  const int val,
				  const SYMBOL_TYPE type,
				  Struct *astruct,
				  const int stcount)
{
	Line *lp;
	Symbol *new_sym;
	Symbol *list;
	Symbol *save;
	char newname[MAX_LABELSIZE * 3];

	if(false == IsLabelStart(ctx, *labelIn) && 0 != stricmp("?rts", labelIn))
	{
		internal_error((ctx, "illegal symbol mame"));
	}

	if(false == proclabel(ctx, labelIn, newname, true))
	{
		return NULL;
	}


	new_sym = FindSymbol2(ctx, newname);
	if(NULL != new_sym)
	{
		if(SYM_INTERNAL_VALUE == new_sym->type)
		{
			if(val != new_sym->value && 1 == ctx->m_Pass)
			{
				warning(ctx, WARN_PREDEF, "The value for predefined symbol '%s' cannot be changed", labelIn);
			}
		}
		else
		{
			if(1 == ctx->m_Pass)
			{
				error(ctx, ERR_REDEFINED, "'%s' was previously defined on line %d", labelIn, new_sym->L_list->L_num);
				return new_sym;
			}

			if(ctx->m_Pass == 2)
			{
				if(new_sym->value == val)
				{
					return new_sym;
				}

				else
				{
					error(ctx, ERR_PHASING, "symbol %s was defined as %04X and is now %04X", labelIn, new_sym->value, val);
					return new_sym;
				}
			}

		}
		return new_sym;
	}

	if(ctx->m_Pass == 2)
	{
		error(ctx, ERR_UNDEFINED, "'%s' not defined", labelIn);
		return NULL;
	}


	/* enter new symbol */
	new_sym = (Symbol*)AllocMem(sizeof(Symbol));
	new_sym->name = (char*)AllocMem(strlen(newname) + 1);
	strcpy(new_sym->name, newname);
	switch(type)
	{
	case SYM_STRUCTURE:
	case SYM_STRUCTDATA:
	case SYM_UNION:
		ASSERT(NULL != astruct);
		if(NULL == astruct)
		{
			internal_error((ctx, "Structure not passed in AddSymbol()"));
		}
		new_sym->astruct = astruct;
		new_sym->stcount = stcount;
		break;

	case SYM_ADDRESS:
	case SYM_VALUE:
		ASSERT(NULL == astruct);
		if(NULL != astruct)
		{
			internal_error((ctx, "Structure passed in AddSymbol()"));
		}
		
		new_sym->astruct = NULL;
		new_sym->stcount = 0;
		break;

	default:
		ASSERTX("Unknown symbol type");
		break;
	}

	new_sym->segment = GetCurrentSegment(ctx);
	new_sym->type = type;
	new_sym->value = val;
	new_sym->Lnext = NULL;
	new_sym->Rnext = NULL;
	new_sym->L_list = lp = (Line*)AllocMem(sizeof(Line));;
	lp->L_num = ctx->m_LineNumber;
	lp->next = NULL;

	if(root == NULL)
	{
		root = new_sym;
	}

	else
	{
		list = root;
		save = NULL;
		while(list != NULL)
		{
			save = list;
			if(CompareSymbol(ctx, newname, list->name) < 0)
			{
				list = list->Lnext;
			}
			else
			{
				list = list->Rnext;
			}
		}

		if(save == NULL)
		{
			internal_error((ctx, "unable to install symbol"));
		}

		
		if(CompareSymbol(ctx, newname, save->name) < 0)
		{
			save->Lnext = new_sym;
		}

		else
		{
			save->Rnext = new_sym;
		}
	}

	return new_sym;
}


/*****************************************************************************
	
	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson
	
	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:
	
	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
	
*****************************************************************************/
