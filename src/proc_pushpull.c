/*****************************************************************************
	proc_pushpull.c	- Process stack push/pull ops

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "error.h"
#include "util.h"
#include "proc_util.h"
#include "proto.h"



/* convert tfr/exg reg number into psh/pul format */
static int	 ppRegBits[17] =	{ 0x06,		/* REG_ACCD */
								  0x10,		/* REG_INDX */
								  0x20,		/* REG_INDY */
								  0x40,		/* REG_USTACK */
								  0x40,		/* REG_SSTACK */
								  0x80,		/* REG_PC */
								  0x00,		/* REG_ACCW *INVALID */
								  0x00,		/* REG_V *INVALID */
								  0x02,		/* REG_ACCA */
								  0x04,		/* REG_ACCB */
								  0x01,		/* REG_CC */
								  0x08,		/* REG_DP */
								  0x00,		/* REG_ZERO		* Invalid */
								  0x00,		/* REG_ZEROZERO	* Invalid */
								  0x00,		/* REG_ACCE		* Invalid */
								  0x00,		/* REG_ACCF		* Invalid */
								  0x80		/* REG_PCR		* Invalid */
								};
static int	 ppRegCycles[17]=	{ 0x02,
								  0x02,
								  0x02,
								  0x02,
								  0x02,
								  0x02,
								  0x00,
								  0x00,
								  0x01,
								  0x01,
								  0x01,
								  0x01,
								  0x00,
								  0x00,
								  0x00,
								  0x00,
								  0x00
								};



 void ProcPushPull(EnvContext *ctx, const Mneumonic *op)
{
	REGISTER	reg;
	u_char		pbyte;

	ctx->m_CycleCount = GetCycleCount(ctx, op, IMMEDIATE, ctx->m_CPUType);

	if(EOS == *ctx->m_Ptr)
	{
		error(ctx, ERR_SYNTAX, "No registers specified for %s operation", op->mnemonic);
		return;
	}

	pbyte = 0;
	do
	{
		/* ctx->m_Ptr = SkipConstWS(ctx->m_Ptr); */

		reg = GetRegister(ctx, false, false);
		switch(reg)
		{
		case REG_ERROR:
			error(ctx, ERR_ILLEGAL_REGISTER_NAME, "invalid register supplied for '%s'", op->mnemonic);
			return;

		case REG_PCR:
			if(false == ctx->m_Compat.m_DisablePCIndex)
			{
				warning(ctx,
						WARN_COMPAT,
						"Some assemblers do not accept '%s' as a valid register for '%s' operations",
						GetRegisterName(reg),
						op->mnemonic);
				break;
			}

		case REG_ACCW:
		case REG_V:
		case REG_ZERO:
		case REG_ZEROZERO:
		case REG_ACCE:
		case REG_ACCF:
			error(ctx, ERR_ILLEGAL_REGISTER_NAME, "illegal register '%s' supplied for '%s'", GetRegisterName(reg), op->mnemonic);
			break;

		case REG_SSTACK:
			if(op->opcode == OP_PSHS || op->opcode == OP_PULS)
			{
				error(ctx, ERR_ILLEGAL_REGISTER_NAME, "illegal register '%s' supplied for '%s'", GetRegisterName(reg), op->mnemonic);
			}

			break;

		case REG_USTACK:
			if(op->opcode == OP_PSHU || op->opcode == OP_PULU)
			{
				error(ctx, ERR_ILLEGAL_REGISTER_NAME, "illegal register '%s' supplied for '%s'", GetRegisterName(reg), op->mnemonic);
			}

			break;

		default:
			/* Make the compiler happy */
			break;
		}

		pbyte |= ppRegBits[reg];
		ctx->m_CycleCount += ppRegCycles[reg];

	}
	while(*(ctx->m_Ptr++) == ',' );

	EmitOpCode(ctx, op, 0);
	EmitOpPostByte(ctx, pbyte);
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
