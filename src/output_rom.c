/*****************************************************************************
	output_rom.c	- code emitting for ROM and RAW files

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "proto.h"

#define MAX_ROM_RECORD_SIZE	512

static FILE			*outputFile = NULL;
static u_int16		E_Total;						/* total # bytes for one line			*/
static u_char		E_Bytes[MAX_ROM_RECORD_SIZE];	/* Emitted held bytes					*/



FILEGEN(rom, rom, rom, true);
FILEGEN(rom, raw, raw, true);



static void filegen_rom_init(EnvContext *ctx, FILE *outFile)
{
	ASSERTX(NULL == outputFile);
	outputFile = outFile;
	E_Total = 0;
}


static void filegen_rom_addbyte(EnvContext *ctx, u_char val, u_int16 regpc)
{
	ASSERTX(2 == ctx->m_Pass);
	ASSERTX(NULL != outputFile);
	ASSERTX(true == ctx->m_Misc.Oflag);

	E_Bytes[E_Total++] = val;

	if(MAX_ROM_RECORD_SIZE == E_Total)
	{
		filegen_rom_flush(ctx, regpc);
	}
}


static void filegen_rom_flush(EnvContext *ctx, u_int16 regpc)
{
	ASSERTX(2 == ctx->m_Pass);
	ASSERTX(NULL != outputFile);
	ASSERTX(true == ctx->m_Misc.Oflag);

	if(E_Total > 0)
	{
		if(true == ctx->m_Misc.Oflag)
		{
			fwrite(E_Bytes, 1, E_Total, outputFile);
		}
		E_Total = 0;
	}
}


static void filegen_rom_finish(EnvContext *ctx, u_int16 regpc)
{
	ASSERTX(2 == ctx->m_Pass);
	ASSERTX(NULL != outputFile);
	ASSERTX(true == ctx->m_Misc.Oflag);

	/* Fluish the output */
	filegen_rom_flush(ctx, regpc);

	if(ROMBIN == ctx->m_OutputType)
	{
		int length;

		length = ftell(outputFile);
		if(length > outputROMSize)
		{
			warning(ctx, WARN_ROMSIZE, "ROM file is %d bytes larger than requested ROM size", length - outputROMSize);
		}
		
		else
		{
			while(length < outputROMSize)
			{
				fputc(0, outputFile);
				length++;
			}
		}
	}

	outputFile = NULL;
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
