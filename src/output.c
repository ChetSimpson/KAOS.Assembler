/*****************************************************************************
	output.c	- code emitting

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "proto.h"
#include "output.h"

#define MAX_SEGMENTS	64

/*
	Added for OS-9 Module files
*/
static FILE			*outputFile = NULL;
/*
	output (emit) byte buffers and counters
*/
const OutputIFace	*filegen = NULL;			/* File generation interface pointer	*/
static SEGMENT		segments[MAX_SEGMENTS];		/* Segment stack						*/
static int			segmentCount = 0;			/* Current segment stack pointer		*/


void BeginSegment(EnvContext *ctx, SEGMENT seg)
{
	segments[segmentCount++] = seg;
}


void EndSegment(EnvContext *ctx)
{
	if(0 == segmentCount)
	{
		error(ctx, ERR_GENERAL, "not currently in a segment");
		return;
	}
}


SEGMENT GetCurrentSegment(EnvContext *ctx)
{
	if(0 == segmentCount)
	{
		internal_error((ctx, "no segment defined for GetCurrentSegment()"));
	}
	return segments[segmentCount - 1];
}


void SetSegment(EnvContext *ctx, SEGMENT segment)
{
	if(0 == segmentCount)
	{
		segmentCount++;
	}

	ASSERTX(1 == segmentCount);
	segments[0] = segment;
}


/*----------------------------------------------------------------------------
	emit --- emit a byte to code file
----------------------------------------------------------------------------*/
static void Emit(EnvContext *ctx, u_char byte)
{
	BumpPCReg(1);

	if(1 != ctx->m_Pass)
	{
		if(ctx->m_ListingFlags.m_OptEnabled && (ctx->m_ListingFlags.P_total < PMIT_LIMIT))
		{
			ctx->m_ListingFlags.P_bytes[ctx->m_ListingFlags.P_total++] = byte;
		}

		if(true == ctx->m_Misc.Oflag && NULL != filegen)
		{
			filegen->filegen_addbyte(ctx, byte, GetPCReg());
		}
	}
}

/*----------------------------------------------------------------------------
	eword --- Emit a word to code file
----------------------------------------------------------------------------*/
void EmitOpCode(EnvContext *ctx, const Mneumonic *op, const u_char mod)
{
	if(NOPAGE != op->page)
	{
		Emit(ctx, op->page);
	}

	Emit(ctx, op->opcode + mod);
}

void EmitOpCodeEx(EnvContext *ctx, const u_char op1)
{
	Emit(ctx, op1);
}

void EmitOpCode2(EnvContext *ctx, const u_char op1, const u_char op2)
{
	Emit(ctx, op1);
	Emit(ctx, op2);
}


void EmitOpRelAddrWord(EnvContext *ctx, const u_int16 addr)
{
	Emit(ctx, hibyte(addr));
	Emit(ctx, lobyte(addr));
}



void EmitDataByte(EnvContext *ctx, const u_char data)
{
	Emit(ctx, data);
}

void EmitDataWord(EnvContext *ctx, const u_int16 data)
{
	Emit(ctx, hibyte(data));
	Emit(ctx, lobyte(data));
}

void EmitDataLong(EnvContext *ctx, const u_int32 data)
{
	EmitDataWord(ctx, hiword(data));
	EmitDataWord(ctx, loword(data));
}


void EmitOpPostByte(EnvContext *ctx, const u_char postop)
{
	Emit(ctx, postop);
}


void EmitOpDataByte(EnvContext *ctx, const u_char data)
{
	Emit(ctx, data);
}

void EmitOpDataWord(EnvContext *ctx, const u_int16 data)
{
	Emit(ctx, hibyte(data));
	Emit(ctx, lobyte(data));
}

void EmitOpDataLong(EnvContext *ctx, const u_int32 data)
{
	EmitOpDataWord(ctx, hiword(data));
	EmitOpDataWord(ctx, loword(data));
}

void EmitOpAddr(EnvContext *ctx, const u_int16 addr)
{
	Emit(ctx, hibyte(addr));
	Emit(ctx, lobyte(addr));
}

void EmitOpRelAddrByte(EnvContext *ctx, const u_char data)
{
	Emit(ctx, data);
}


void EmitUninitData(EnvContext *ctx, const u_int16 size)
{
	FlushOutput(ctx);
	BumpPCReg(size);
	FlushOutput(ctx);
}




void FlushOutput(EnvContext *ctx)
{
	if(NULL != filegen)
	{
		filegen->filegen_flush(ctx, GetPCReg());
	}
}



bool OpenOutput(EnvContext *ctx, const char *name, const bool forceExt)
{
	char	*fext;
	char	outputName[MAXPATH];
	char	odrive[MAXDRIVE];
	char	odirect[MAXDIR];
	char	ofile[MAXFILE];
	char	oext[MAXEXT];

	ctx->m_ListingFlags.P_total = 0;
	fext = NULL;


	/* added for coco binary output */
	switch(ctx->m_OutputType)
	{
	case S19FILE:
		filegen = &filegen_S19;
		break;

	case MOTBIN:
		filegen = &filegen_bin;
		break;

	case OS9BIN:
		filegen = &filegen_os9;
		break;

	case ROFBIN:
		filegen = &filegen_rof;
		break;

	case RAWBIN:
		filegen = &filegen_raw;
		break;

	case ROMBIN:
		filegen = &filegen_rom;
		break;

	case MODBIN:
		filegen = &filegen_mod;
		break;

	case OBJBIN:
		filegen = &filegen_obj;
		break;

	default:
		internal_error((ctx, "unknown output type"));
	}

	ASSERTX(NULL != filegen);

	fnsplit(name, odrive, odirect, ofile, oext);
	if(*oext == 0 || true == forceExt)
	{
		ASSERTX(NULL != oext);
		strcpy(oext, filegen->extension);
	}


	/* Get output directory */
	if(outputDirectory)
	{
		fnmerge(outputName, "", outputDirectory, ofile, oext);
	}
	else
	{
		fnmerge(outputName, odrive, odirect, ofile, oext);
	}

	if(ctx->m_SilentMode == false)
	{
		fprintf(stderr, "Output file:%s\n", outputName);
	}

	if(S19FILE != ctx->m_OutputType)
	{
		outputFile = fopen(outputName, "wb");
	}
	else
	{
		outputFile = fopen(outputName, "wt");
	}

	if(outputFile == NULL)
	{
		fatal(ctx, "unable to open '%s' for output", outputName);
	}

	filegen->filegen_init(ctx, outputFile);

	return true;
}


void CloseOutput(EnvContext *ctx)
{
	FlushOutput(ctx);

	if(NULL != filegen)
	{
		filegen->filegen_finish(ctx, GetPCReg());
	}

	if(NULL != outputFile)
	{
		fclose(outputFile);
		outputFile = NULL;
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
