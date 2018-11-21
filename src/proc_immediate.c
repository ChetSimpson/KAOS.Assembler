/*****************************************************************************
	proc_immediate.c	- Process immediate addressing modes

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "error.h"
#include "util.h"
#include "proc_util.h"
#include "proto.h"




void ProcImmediate(EnvContext *ctx, const Mneumonic *op)
{
	int32 result;

	ctx->m_Ptr++;
	Evaluate(ctx, &result, EVAL_NORMAL, NULL);


	/* Emit the opcode */
	if(LDQ_OPIMM == op->opcode && PAGE2 == op->page)
	{
		EmitOpCodeEx(ctx, op->opcode);
	}
	else
	{
		EmitOpCode(ctx, op, 0);
	}

	switch(op->immsize)
	{
	case BIT_8:
		if(result > 0xff)
		{
			warning(ctx, WARN_VALUE_TRUNCATED, "immediate value too large for operation - value truncated");
		}
		EmitOpDataByte(ctx, lobyte(result));
		break;

	case BIT_16:
		if(0 != hiword(result) && 0xffff != hiword(result))
		{
			warning(ctx, WARN_VALUE_TRUNCATED, "immediate value is too large for operation - value truncated");
		}
		EmitOpDataWord(ctx, loword(result));
		break;

	case BIT_32:
		EmitOpDataLong(ctx, result);
		break;

	case BIT_0:
		ASSERT(0);
		internal_error((ctx, "ProcImmediate(): Unknown error encountered!"));
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
