/*****************************************************************************
	export.c	- Symbol export management

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "proto.h"

static Export	*exportList = NULL;
static Export	*exportTail = NULL;
static int		exportCount = 0;
static int		exportMMUPage = -1;
static u_int16	exportAddress;


void SetExportAddress(const int mmuPage, const u_int16 address)
{
	exportMMUPage = mmuPage;
	exportAddress = address;
}

void InitExports(EnvContext *ctx)
{
	ASSERT(MODBIN == ctx->m_OutputType);
	AddExport(NULL, "initmod");
}

void AddExport(EnvContext *ctx, const char *name)
{
	Export *exp;

	ASSERTX(1 == ctx->m_Pass);

	/* See if the export already exists */
	exp = exportList;
	while(NULL != exp)
	{
		if(0 == strcmp(exp->name, name))
		{
			warning(ctx, WARN_DBLEXPORT, "%s already exported");
			return;
		}
		exp = exp->next;
	}

	/* Allocate a new export struct */
	exp = (Export*)AllocMem(sizeof(Export));
	exp->name = (char*)AllocMem(strlen(name) + 1);

	/* Create it */
	strcpy(exp->name, name);
	exp->next = NULL;


	/* Add it to the list */
	if(NULL == exportTail)
	{
		exportList = exp;
		exportTail = exp;
	}
	else
	{
		exportTail->next = exp;
		exportTail = exp;
	}

	exportCount++;
}


static void EmitExportWord(FILE *output, u_int16 word)
{
	fputc((word & 0xff00) >> 8, output);
	fputc((word & 0xff), output);
}

void EmitExports(EnvContext *ctx, FILE *output)
{
	Export *exp;

	ASSERTX(MODBIN == ctx->m_OutputType);

	if(-1 == exportMMUPage)
	{
		fputc(0x00, output);
		fputc(0x00, output);
	}
	else
	{
		fputc(0xff, output);
		fputc(exportMMUPage, output);
	}
	EmitExportWord(output, exportAddress);
	EmitExportWord(output, loword(exportCount));

	/* Write the exports */
	exp = exportList;
	while(NULL != exp)
	{
		Symbol *sym;

		sym = FindSymbol(NULL, exp->name, true, true);
		if(NULL == sym)
		{
			error(ctx, ERR_UNDEFINED, "symbol '%s' was declared for export but is not defined", exp->name);
		}
		else
		{
			/* Write the export */
			EmitExportWord(output, (u_int16)sym->value);
			fputc((int)strlen(exp->name), output);
			fputs(exp->name, output);
		}

		exp = exp->next;
	}


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
