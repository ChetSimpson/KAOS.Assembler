/*****************************************************************************
	do9.c	- Instruction assembly

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "proto.h"
#include "proc_util.h"




const char *GetAddressingModeName(ADDR_MODE mode)
{
	const char *name;

	switch(mode)
	{
	case INDIRECT:	name = "indirect";			break;	/* Indirect addressing [$xxxx]		*/
	case EXTENDED:	name = "extended";			break;	/* extended							*/
	case DIRECT:	name = "direct";			break;	/* Direct Page access				*/
	case INDEXED:	name = "indexed";			break;	/* Indexed only						*/
	case INHERENT:	name = "inherent";			break;	/* Inherent							*/
	case IMMEDIATE:	name = "immediate";			break;	/* Immediate only					*/
	case GENERAL:	name = "general";			break;	/* General addressing				*/
	case REL_SHORT:	name = "short relative";	break;	/* Short Relative					*/
	case REL_LONG:	name = "long relative";		break;	/* Long Relative					*/
	case NOIMM:		name = "no immediate";		break;	/* General except for Immediate		*/
	case RTOR:		name = "reg to reg";		break;	/* Register To Register				*/
	case REGLIST:	name = "push/pull";			break;	/* Register List					*/
	case GRP2:		name = "Group2";			break;	/* Group 2 (Read/Modify/Write)		*/
	case LOGMEM:	name = "logical memory";	break;	/* 6309 logical memory ops			*/
	case BTM:		name = "bit transfer";		break;	/* Bit transfer and manipulation	*/
	case MRTOR:		name = "reg to reg2";		break;	/* TFM memory transfers				*/
	case MRTOR2:	name = "reg to reg2";		break;	/* TFM memory transfers - CCASM		*/
	default:
		name = "UNKNOWN";
		break;
	}

	return name;
}


static void ProcGroup2(EnvContext *ctx, const Mneumonic *op)
{
	int32		result;
	ADDR_MODE	mode;
	bool		status;

	/* pickup indicated addressing mode */
	mode = GetAddrMode(ctx);

	if(INDEXED == mode || INDIRECT == mode)
	{
		ProcIndexed(ctx, op, 0x60);
		return;
	}

	if(DIRECT != mode && EXTENDED != mode)
	{
		error(ctx, ERR_INVALID_MODE_FOR_OPERATION, "%s addressing mode not allowed for %s", GetAddressingModeName(mode), op->mnemonic);
		return;
	}

	status = Evaluate(ctx, &result, EVAL_NORMAL, NULL);

	if (ctx->m_Pass == 2 && mode == EXTENDED && op->opcode == 0x0E)
	{
		int		dist;

		dist = (int)result - (GetPCReg() + 2);
		if(dist >= MIN_BYTE && dist <= MAX_BYTE)
		{
			ctx->m_OptimizeLine = true;
		}
	}


	if(true == ctx->m_ForceByte || (false == ctx->m_ForceWord && true == IsAddressInDirectPage(loword(result))))
	{
		EmitOpCode(ctx, op, 0);
		EmitOpDataByte(ctx, lobyte(result));
		ctx->m_CycleCount += 2;
		return;
	}

	/* Default to extended */
	EmitOpCode(ctx, op, 0x70);
	EmitOpAddr(ctx, loword(result));
	ctx->m_CycleCount += 3;

}


/*
 *	  ProcOpcode --- process mnemonic
 *
 *	Called with the base opcode and it's class. operandPtr points to
 *	the beginning of the operand field.
 */
void ProcOpcode(EnvContext *ctx, const Mneumonic *op)
{
	ctx->m_OptimizeLine = false;
	
	switch(op->optype)
	{
	case GENERAL:
	case NOIMM:
	case IMMEDIATE:
	case INDEXED:
		ProcGeneral(ctx, op);
		break;


	case INHERENT:
		/* inherent addressing */
		ProcInherent(ctx, op);
		break;
		
	case REL_SHORT:
		/* short relative branches */
		ProcShortBranch(ctx, op);
		break;
		
	case REL_LONG:
		/* long relative branches */
		ProcLongBranch(ctx, op);
		break;

	case RTOR:
		/* tfr and exg */
		ProcRegToReg(ctx, op);
		break;
		
	case REGLIST:
		/* pushes and pulls */
		ProcPushPull(ctx, op);
		break;
		
	case GRP2:
		ProcGroup2(ctx, op);
		break;
		
	case LOGMEM:
		ProcLogicalMem(ctx, op);
		break;

	case BTM:
		ProcBitTransfer(ctx, op);
		break;

	case MRTOR:
		ProcMemXfer(ctx, op);
		break;

	case MRTOR2:
		ProcMemXfer2(ctx, op);
		break;

	default:
		internal_error((ctx, "Error in Mnemonic table (op=%s / %d)", op->mnemonic, op->opcode));
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
