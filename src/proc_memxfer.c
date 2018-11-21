/*****************************************************************************
	proc_memxfer.c	- Process memory transfer ops

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "error.h"
#include "util.h"
#include "proc_util.h"
#include "proto.h"


typedef enum
{
	RXFER_NONE,
	RXFER_ADD,
	RXFER_SUB,
} RXFER_OP;

static RXFER_OP GetMemXferOp(EnvContext *ctx)
{
	RXFER_OP op;

	if('+' == *ctx->m_Ptr)
	{
		op = RXFER_ADD;
		ctx->m_Ptr++;
	}
	
	else if(*ctx->m_Ptr == '-')
	{
		op = RXFER_SUB;
		ctx->m_Ptr++;
	}
	else
	{
		op = RXFER_NONE;
	}

	return op;
}


void ProcMemXfer2(EnvContext *ctx, const Mneumonic *op)
{
	REGISTER	srcReg;	/* source register */
	REGISTER	dstReg;	/* destination register */
	u_char		postop;

	srcReg = GetRegister(ctx, false, true);	/* get first register */
	if(REG_ERROR == srcReg)
	{
		error(ctx, ERR_INVALID_SRCREG, "invalid register for memory transfer operand");
		return;
	}


	if(*ctx->m_Ptr++ != ',')
	{
		error(ctx, ERR_SYNTAX, "missing ','");
		return;
	}

	dstReg = GetRegister(ctx, false, true);		/* get first register */
	if(REG_ERROR == dstReg)
	{
		error(ctx, ERR_INVALID_DSTREG, "invalid register for memory transfer operand");
		return;
	}

	postop = ((lobyte(srcReg) & 0x0f) << 0x04) | (lobyte(dstReg) & 0x0f);

	EmitOpCode(ctx, op, 0);
	EmitOpPostByte(ctx, postop);
}


void ProcMemXfer(EnvContext *ctx, const Mneumonic *op)
{
	REGISTER	srcReg;	/* source register */
	REGISTER	dstReg;	/* destination register */
	RXFER_OP	srcOp;	/* Source operation */
	RXFER_OP	dstOp;	/* Destination operation */
	u_char		postop;
	u_char		opcode;

	srcReg = GetRegister(ctx, false, true);	/* get first register */
	if(REG_ERROR == srcReg)
	{
		error(ctx, ERR_INVALID_SRCREG, "invalid register for '%s'", op->mnemonic);
		return;
	}

	srcOp = GetMemXferOp(ctx);

	if(*ctx->m_Ptr++ != ',')
	{
		error(ctx, ERR_SYNTAX, "missing ',' in memory transfer operand");
		return;
	}

	dstReg = GetRegister(ctx, false, true);		/* get first register */
	if(REG_ERROR == dstReg)
	{
		error(ctx, ERR_INVALID_DSTREG, "invalid register for '%s'", op->mnemonic);
		return;
	}

	dstOp = GetMemXferOp(ctx);

	postop = ((lobyte(srcReg) & 0x0f) << 0x04) | (lobyte(dstReg) & 0x0f);
	opcode = op->opcode;

	if(RXFER_ADD == srcOp && RXFER_ADD == dstOp)
	{
		opcode += 0;
	}
	else if(RXFER_SUB == srcOp && RXFER_SUB == dstOp)
	{
		opcode += 1;
	}
	else if(RXFER_ADD == srcOp && RXFER_NONE == dstOp)
	{
		opcode += 2;
	}
	else if(RXFER_NONE == srcOp && RXFER_ADD == dstOp)
	{
		opcode += 3;
	}
	else
	{
		error(ctx, ERR_INVALID_MODE_FOR_OPERATION, "invalid direction for '%s'", op->mnemonic);
	}

	EmitOpCode2(ctx, PAGE3, opcode);
	EmitOpPostByte(ctx, postop);
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
