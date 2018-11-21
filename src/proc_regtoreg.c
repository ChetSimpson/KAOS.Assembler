/*****************************************************************************
	proc_regtoreg.c	- Process register to register ops

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "error.h"
#include "util.h"
#include "proc_util.h"
#include "proto.h"


void ProcRegToReg(EnvContext *ctx, const Mneumonic *op)
{
	REGISTER	src;
	REGISTER	dst;
	u_char		byte;

	EmitOpCode(ctx, op, 0);

	src = GetRegister(ctx, true, true);
	if(REG_ERROR == src)
	{
		error(ctx, ERR_SRCREG_REQUIRED, "invalid source register for '%s'", op->mnemonic);
		EmitOpPostByte(ctx, 0);
		return;
	}

	if(*ctx->m_Ptr++ != ',')
	{
		error(ctx, ERR_SYNTAX, "missing comma");
		EmitOpPostByte(ctx, 0);
		return;
	}

	ctx->m_Ptr = SkipConstWS(ctx->m_Ptr);

	dst = GetRegister(ctx, true, true);
	if(REG_ERROR == dst)
	{
		error(ctx, ERR_SRCREG_REQUIRED, "invalid destination register for '%s'", op->mnemonic);
		EmitOpPostByte(ctx, 0);
		return;
	}

	/* Check to warn of PCR/PC compatibility */
	if(REG_PCR == src)
	{
		warning(ctx, WARN_REGTOREG_PCR, "PCR as source register assumed to mean PC");
		src = REG_PC;
	}

	/* Check to warn of PCR/PC compatibility */
	if(REG_PCR == dst)
	{
		warning(ctx, WARN_REGTOREG_PCR, "PCR as destination register assumed to mean PC");
		dst = REG_PC;
	}

	/* Check for 0 register as dest usage */
	if(REG_ZERO == dst)
	{
		warning(ctx, WARN_REGTOREG_CONSTDST, "cannot transfer value into read only register '0'");
	}
	
	/* Check for 00 register as dest usage */
	if(REG_ZEROZERO == dst)
	{
		warning(ctx, WARN_REGTOREG_CONSTDST, "cannot transfer value into read only register '00'");
	}

	if(REG_ZERO != src && REG_ZEROZERO != src)
	{
		if((src >= 8 && dst <= 7) || (src <= 7 && dst >= 8))
		{
			warning(ctx, WARN_REGTOREG_SIZEMISMATCH, "register size mismatch for '%s'", op->mnemonic);
		}
	}
	else
	{
		if(REG_PC != dst && REG_V != dst)
		{
			warning(ctx, WARN_OPTIMIZE, "optimization warning for using '0' register");
		}
	}

	byte = (u_char)(((src & REG_LSN_MASK) << 4) | (dst & REG_LSN_MASK));

	EmitOpPostByte(ctx, byte);
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
