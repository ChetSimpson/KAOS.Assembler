/*****************************************************************************
	symtab.h	- declaration for the symbol table

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#ifndef SYMTAB_H
#define SYMTAB_H

#include "config.h"
#include "struct.h"
#include "output.h"

/* Label types */
typedef enum
{
	SYM_STRUCTURE = 0,
    SYM_STRUCTDATA,
	SYM_UNION,
	SYM_ADDRESS,
	SYM_VALUE,
	SYM_INTERNAL_VALUE,
	SYM_IMPORTED
} SYMBOL_TYPE;


typedef struct _symbol Symbol;
typedef struct _line Line;



/* linked list to hold line numbers */
struct _line
{
	int 	L_num;		/* line number */
	Line	 *next;		/* pointer to next node */
};


/* basic symbol table entry */
struct _symbol
{
	SEGMENT		segment;	/* Data segment the symbol was declared in */
	SYMBOL_TYPE	type;		/* Type of symbol */
	char		*name;		/* symbol name */
	int32		value;		/* Value of the symbol */
	Struct		*astruct;	/* Pointer to structure */
	int			stcount;	/* Structure count */
	Symbol		*Lnext;		/* left node of the tree leaf */
	Symbol		*Rnext;		/* right node of the tree leaf */
	Line		*L_list;	/* pointer to linked list of line numbers */
};

/*------------------------------------------------------------------------
	symtab.c
------------------------------------------------------------------------*/
void InitSymbolTable(EnvContext *ctx);
void ResetLocalLabels(EnvContext *ctx);
Symbol *AddSymbol(EnvContext *ctx, const char *str, const int val, const SYMBOL_TYPE type, Struct *astruct, const int astructCount);
Symbol *FindSymbol(EnvContext *ctx, const char *name, const bool noerror, const bool onlyns);
void AddExport(EnvContext *info, const char *name);
void EmitExports(EnvContext *ctx, FILE *outputFile);
void InitExports(EnvContext *ctx);
void SetExportAddress(const int mmuPage, const u_int16 address);
int GetNextLocalLabel();
void DumpSymTable(EnvContext *ctx);
void DumpCrossRef();
void SetNamespace(EnvContext *ctx, const char *ns);
void ClearNamespace(EnvContext *ctx);
bool IsInNamespace();


#endif	/* SYMTAB_H */


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
