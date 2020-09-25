/*****************************************************************************
	proc_extended.c	- Process extended addressing ops

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "error.h"
#include "util.h"
#include "proc_util.h"
#include "proto.h"


void ProcExtended(EnvContext *ctx, const Mneumonic *op)
{
	int32 result;
	u_char modifier;
	bool status;

	status = Evaluate(ctx, &result, EVAL_NORMAL, NULL);
	if(false == status && 2 == ctx->m_Pass)
	{
		ctx = ctx;
	}

	if(LDQ_OPIMM == op->opcode && PAGE2 == op->page)
	{
		modifier = lobyte((LDQ_OPOTH - LDQ_OPIMM) - 0x10);
	}
	else
	{
		modifier = 0;
	}

	if(false == ctx->m_ForceWord && true == IsAddressInDirectPage(loword(result)))
	{
		ctx->m_ForceByte = true;
	}

	else if(false == ctx->m_ForceWord && (result >= 0 && result <= 0xFF))
	{
		ctx->m_ForceByte = true;
	}
	
	else if(false == ctx->m_ForceByte)
	{
		ctx->m_ForceWord = true;
	}
	
	else if(true == ctx->m_ForceByte && true == ctx->m_ForceWord)
	{
		warning(ctx, WARN_BUGALERT, "possible assembler bug! conflict deciding between direct or extended mode.");
		warning(ctx, WARN_BUGALERT, "forcing to extended addressing mode");
		ctx->m_ForceWord = true;
		ctx->m_ForceByte = false;
	}


	if (ctx->m_Pass == 2 && op->opcode == 0x8D)
	{
		int		dist;

		dist = (int)result - (GetPCReg() + 3);
		if(dist >= MIN_BYTE && dist <= MAX_BYTE)
		{
			ctx->m_OptimizeLine = true;
		}
	}

	if(true == ctx->m_ForceByte)
	{
		ASSERTX(false == ctx->m_ForceWord);
		EmitOpCode(ctx, op, modifier + 0x10);
		EmitOpPostByte(ctx, lobyte(result));	/* FIXME - emitopaddrbyte */
		ctx->m_CycleCount += 2;
	}
	else
	{
		ASSERTX(false == ctx->m_ForceByte);
		EmitOpCode(ctx, op, modifier + 0x30);
		EmitOpAddr(ctx, loword(result));
		ctx->m_CycleCount += 3;
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
