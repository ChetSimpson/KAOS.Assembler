/*****************************************************************************
	output_bin.c	- code emitting for RS-DOS BIN files

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "proto.h"

#if 1
#define MAX_BIN_RECORD_SIZE		32768
#else
#define MAX_BIN_RECORD_SIZE		255
#endif

static u_int16	E_pc;							/* PC at beginning of collection	*/
static u_int16	E_Total;						/* total # bytes for one line			*/
static u_char	E_Bytes[MAX_BIN_RECORD_SIZE];	/* Emitted held bytes					*/
static FILE		*outputFile = NULL;
FILEGEN(bin, bin, bin, false);


static void filegen_bin_flush(EnvContext *ctx, u_int16 regpc)
{
	ASSERTX(2 == ctx->m_Pass);
	ASSERTX(NULL != outputFile);
	ASSERTX(true == ctx->m_Misc.Oflag);

	if(0 != E_Total)
	{
		fprintf(outputFile, "%c%c%c%c%c", 0x00, hibyte(E_Total), lobyte(E_Total), hibyte(E_pc), lobyte(E_pc));
		fwrite(E_Bytes, 1, E_Total, outputFile);
	}
	E_pc = regpc;
	E_Total = 0;
}


static void filegen_bin_addbyte(EnvContext *ctx, u_char val, u_int16 regpc)
{
	ASSERTX(2 == ctx->m_Pass);
	ASSERTX(NULL != outputFile);
	ASSERTX(true == ctx->m_Misc.Oflag);

	E_Bytes[E_Total++] = val;

	if(MAX_BIN_RECORD_SIZE == E_Total)
	{
		filegen_bin_flush(ctx, regpc);
	}

}

static void filegen_bin_finish(EnvContext *ctx, u_int16 regpc)
{
	filegen_bin_flush(ctx, regpc);
	fprintf(outputFile, "%c%c%c%c%c", 0xff, 0x00, 0x00, hibyte(ctx->m_EntryPoint), lobyte(ctx->m_EntryPoint));
	outputFile = NULL;
}

static void filegen_bin_init(EnvContext *ctx, FILE *outFile)
{
	ASSERTX(NULL == outputFile);
	ASSERTX(true == ctx->m_Misc.Oflag);

	outputFile = outFile;
	E_Total = 0;
	E_pc = 0;

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
