/*****************************************************************************
	output_mod.c	- code emitting loadable shared modules

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "proto.h"

#define MAX_MOD_RECORD_SIZE		512

static FILE		*outputFile = NULL;
static u_int16	E_Total;						/* total # bytes for one line			*/
static u_char	E_Bytes[MAX_MOD_RECORD_SIZE];	/* Emitted held bytes					*/
FILEGEN(mod, mod, mod, true);

static void filegen_mod_flush(EnvContext *ctx, u_int16 regpc)
{
	ASSERTX(2 == ctx->m_Pass);
	ASSERTX(NULL != outputFile);
	ASSERTX(true == ctx->m_Misc.Oflag);

	fwrite(E_Bytes, 1, E_Total, outputFile);
	E_Total = 0;
}


static void filegen_mod_addbyte(EnvContext *ctx, u_char val, u_int16 regpc)
{
	ASSERTX(2 == ctx->m_Pass);
	ASSERTX(NULL != outputFile);
	ASSERTX(true == ctx->m_Misc.Oflag);

	E_Bytes[E_Total++] = val;

	if(MAX_MOD_RECORD_SIZE == E_Total)
	{
		filegen_mod_flush(ctx, regpc);
	}

}

static void filegen_mod_finish(EnvContext *ctx, u_int16 regpc)
{
	filegen_mod_flush(ctx, regpc);
	outputFile = NULL;
}

static void filegen_mod_init(EnvContext *ctx, FILE *outFile)
{
	ASSERTX(NULL == outputFile);
	ASSERTX(true == ctx->m_Misc.Oflag);

	outputFile = outFile;
	E_Total = 0;

	/* Write out the header */
	fprintf(outputFile, "%c%c%c%c", 0x80, 0xc0, 0x55, 0x85);

	/* Write out the exports */
	EmitExports(ctx, outputFile);

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
