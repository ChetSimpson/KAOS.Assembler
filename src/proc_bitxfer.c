/*****************************************************************************
	proc_bitxfer.c	- Process bit transfer ops

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "error.h"
#include "util.h"
#include "proc_util.h"
#include "proto.h"

void ProcBitTransfer(EnvContext *ctx, const Mneumonic *op)
{
	int32	result;
	REGISTER reg;
	u_char opcode;


	EmitOpCode(ctx, op, 0);		/* send out page 3 prebyte */

	reg = GetRegister(ctx, false, false);

	switch (reg)
	{
	case REG_CC:
		opcode = 0x00;
		break;
	case REG_ACCA:
		opcode = 0x40;
		break;
	case REG_ACCB:
		opcode = 0x80;
		break;
	default:
		opcode = 0;	/* Make compiler happy */
		error(ctx, ERR_INVALID_REG_FOR_OPERATION, "in operand for '%s'", op->mnemonic);
		break;
	}

	if (*ctx->m_Ptr++ != ',')
	{
		error(ctx, ERR_SYNTAX, "invalid format in operand for '%s'", op->mnemonic);
		return;
	}

	Evaluate(ctx, &result, EVAL_NORMAL, NULL);

	if (result > 7)
	{
		error(ctx, ERR_VALUE_TOO_BIG, "source bit number out of range in operand for '%s'", op->mnemonic);
		return;
	}

	opcode |= (result << 0x03);		/* shift register to correct postiton */

	if (*ctx->m_Ptr++ != ',')
	{
		error(ctx, ERR_SYNTAX, "invalid format in operand for '%s'", op->mnemonic);
		return;
	}

	Evaluate(ctx, &result, EVAL_NORMAL, NULL);

	if (result > 7)
	{
		error(ctx, ERR_VALUE_TOO_BIG, "destination bit number out of range in operand for '%s'", op->mnemonic);
		return;
	}

	opcode |= (result );		/* shift register to correct postiton */

	if (*ctx->m_Ptr++ != ',')
	{
		error(ctx, ERR_SYNTAX, "invalid format in operand for '%s'");
		return;
	}

	if (*ctx->m_Ptr == '#')
	{
		error(ctx, ERR_INVALID_MODE_FOR_OPERATION, "'%s' does not support immediate mode addressing", op->mnemonic);
		return;
	}

	Evaluate(ctx, &result, EVAL_NORMAL, NULL);

	if (((int)result) >= 0x100)
	{
		error(ctx, ERR_INVALID_MODE_FOR_OPERATION, "'%s' does not support extended mode addressing", op->mnemonic);
		return;
	}

	EmitOpPostByte(ctx, opcode);
	EmitOpDataByte(ctx, lobyte(result));
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
