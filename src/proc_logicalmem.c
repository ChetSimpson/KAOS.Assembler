/*****************************************************************************
	proc_logicalmem.c	- Process logical memory ops

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "error.h"
#include "util.h"
#include "proc_util.h"
#include "proto.h"


void ProcLogicalMem(EnvContext *ctx, const Mneumonic *op)
{
	int32		result;
	int			amode;
	int32		logValue;

	/* get first argument */
	Evaluate(ctx, &logValue, EVAL_NORMAL, NULL);

	if(logValue > 0xff)
	{
		warning(ctx, WARN_VALUE_TRUNCATED, "immediate value for logical operation too big - value truncated");
	}

	logValue &= 0xff;


	/*
		Here we support 3 dialects of the logical mem operation:
	
			,	-	H.K. dialect
			;	-	CCASM Dialect
			:	-	CLS-97 dialect
	*/
	if(',' != *ctx->m_Ptr && ';' != *ctx->m_Ptr && ':' != *ctx->m_Ptr)
	{
		/* FIXME - warn compat */
		error(ctx, ERR_SYNTAX, "value for logical memory operation must be followed by a comma, semi-colon, or colon");
		return;
	} 

	ctx->m_Ptr++;
	ctx->m_Operand = ctx->m_Ptr;	/* Move string down to start of buffer */

	amode = GetAddrMode(ctx);

	switch(amode)
	{
	case INDEXED:
		EmitOpCode(ctx, op, 0x60);
		EmitOpPostByte(ctx, lobyte(logValue));			/* send out logresult */
		ProcIndexed(ctx, NULL, 0);
		break;

	case INDIRECT:
		EmitOpCode(ctx, op, 0x60);
		EmitOpPostByte(ctx, lobyte(logValue));			/* send out logresult */
		ProcIndexed(ctx, NULL, 0);

		break;

	case IMMEDIATE:
		error(ctx, ERR_INVALID_MODE_FOR_OPERATION, "immediate addressing mode not allowed for '%s'", op->mnemonic);
		break;

	case EXTENDED:
	case DIRECT:
		Evaluate(ctx, &result, EVAL_NORMAL, NULL);

		if(true == ctx->m_ForceByte || true == IsAddressInDirectPage(loword(result)))
		{
			EmitOpCode(ctx, op, 0);
			EmitOpPostByte(ctx, lobyte(logValue));
			EmitOpPostByte(ctx, lobyte(result));
			ctx->m_CycleCount += 2;
		}
		else
		{
			EmitOpCode(ctx, op, 0x70);
			EmitOpPostByte(ctx, lobyte(logValue));
			EmitOpAddr(ctx, loword(result));
			ctx->m_CycleCount += 3;
		}
		break;

	default:
		internal_error((ctx, "ProcLogicalMem(): Unknown addressing mode!"));
		break;
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
