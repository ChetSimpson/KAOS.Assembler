/*****************************************************************************
	proc_general.c	- Process general addressing modes

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "error.h"
#include "util.h"
#include "proc_util.h"
#include "proto.h"


/*
 *	  ProcAddrMode --- process general addressing mode stuff
 */
void ProcGeneral(EnvContext *ctx, const Mneumonic *op)
{
	ADDR_MODE mode;

	/* pickup indicated addressing mode */
	mode = GetAddrMode(ctx);

	switch(op->optype)
	{
	case IMMEDIATE:
		if(IMMEDIATE != mode)
		{
			error(ctx, ERR_INVALID_MODE_FOR_OPERATION, "'%s' requires immediate addressing mode", op->mnemonic);
			return;
		}
		break;

	case NOIMM:
		if(IMMEDIATE == mode)
		{
			error(ctx, ERR_INVALID_MODE_FOR_OPERATION, "'%s' cannot be used with the immediate addressing mode", op->mnemonic);
			return;
		}
		break;

	case INDEXED:
		if(INDEXED != mode && INDIRECT != mode)
		{
			error(ctx, ERR_INVALID_MODE_FOR_OPERATION, "'%s' requires an indexed addressing mode", op->mnemonic);
			return;
		}
		ProcIndexed(ctx, op, 0);
		return;
		break;

	default:
		break;
	}

	switch(mode)
	{
	case IMMEDIATE:
		ProcImmediate(ctx, op);
		break;

	case INDEXED:
	case INDIRECT:
		ProcIndexed(ctx, op, 0x20);
		break;

	case DIRECT:
		ProcDirect(ctx, op);
		break;

	case EXTENDED:
		ProcExtended(ctx, op);
		break;

	default:
		internal_error((ctx, "Unknown Addressing Mode"));
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
