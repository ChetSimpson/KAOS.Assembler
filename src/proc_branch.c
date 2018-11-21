/*****************************************************************************
	proc_branch.c	- Process branch ops

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "error.h"
#include "util.h"
#include "proc_util.h"
#include "proto.h"


void ProcShortBranch(EnvContext *ctx, const Mneumonic *op)
{
	int32	result;
	int		dist;
	bool	status;

	status = Evaluate(ctx, &result, EVAL_NORMAL, NULL);

	if(true == status)
	{
		dist = (int)result - (GetPCReg() + 2);
	}
	else
	{
		dist = -2;
	}

	EmitOpCode(ctx, op, 0);

	if(ctx->m_Pass == 2 && (dist > MAX_BYTE || dist < MIN_BYTE))
	{
		error(ctx, ERR_GENERAL, "branch destination is out of range", dist);
		dist = -2;
	}

	EmitOpRelAddrByte(ctx, lobyte(dist));
}


void ProcLongBranch(EnvContext *ctx, const Mneumonic *op)
{
	bool	status;
	int32	result;
	int		offset;
	int		dist;

	if(NOPAGE == op->page)
	{
		offset = 3;
	}
	else
	{
		offset = 4;
	}

	status = Evaluate(ctx, &result, EVAL_NORMAL, NULL);

	dist = (int)result - (GetPCReg() + offset);

	if((dist >= MIN_BYTE) && (dist <= MAX_BYTE))
	{
		ctx->m_OptimizeLine = true;
	}

	EmitOpCode(ctx, op, 0);
	EmitOpRelAddrWord(ctx, loword(dist));
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
