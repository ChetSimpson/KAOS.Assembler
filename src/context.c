/*****************************************************************************
	context.c	- Context helper functions

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "context.h"
#include "error.h"
#include "proto.h"
#include "util.h"
#include "label.h"
#include "table9.h"


bool GetConditionalState(EnvContext *ctx)
{
	bool condition;

	/* If the current if/else/endif condition is */
	if(NO_CONDITION == ctx->m_Cond.m_Count)
	{
		condition = true;
	}
	else
	{
		condition = ctx->m_Cond.m_Cond[ctx->m_Cond.m_Count].m_State;
	}

	return condition;
}


PSEUDO_OP GetConditionalOp(EnvContext *ctx)
{
	ASSERTX(NO_CONDITION != ctx->m_Cond.m_Count);

	return ctx->m_Cond.m_Cond[ctx->m_Cond.m_Count].m_Op;
}


int GetConditionalCount(EnvContext *ctx)
{
	return ctx->m_Cond.m_Count;
}


void PushConditionalState(EnvContext *ctx, const char *opname, PSEUDO_OP op)
{
	int32	result;
	bool	cond;

	if(IFP1 != op && IFP2 != op && IFDEF != op && IFNDEF != op )
	{
		Evaluate(ctx, &result, 1 == ctx->m_Pass ? EVAL_NOERRORS : EVAL_NORMAL, NULL);
	}
	else
	{
		result = 0;
	}

	switch(op)
	{
	case IFP1:	/* If assembler pass = 1 */
		cond = (ctx->m_Pass == 1 ? true : false);
		break;

	case IFP2:	/* If assembler pass = 2 */
		cond = (ctx->m_Pass == 2 ? true : false);
		break;

	case IFEQ:
		cond = (result == 0 ? true : false);
		break;

	case IFNE:	/* Not equal */
		cond = (result != 0 ? true : false);
		break;

	case IFLT:	/* Less than */
		cond = (result < 0 ? true : false);
		break;

	case IFLE:	/* Less than or equal */
		cond = (result <= 0 ? true : false);
		break;

	case IFGT:	/* Greater than */
		cond = (result > 0 ? true : false);
		break;

	case IFGE:	/* Creater than or equal */
		cond = (result >= 0 ? true : false);
		break;

	case IF:	/* if value is not 0, assemble */
	case COND:
		cond = (result != 0 ? true : false);
		break;

	case IFN:	/* if value is 0, assemble */
		cond = (result == 0 ? true : false);
		break;

	case IFDEF:
	case IFNDEF:
		{
			char label[MAX_LABELSIZE * 3];
			char *dst;
			Symbol *sym;

			if(false == IsLabelStart(ctx, *ctx->m_Ptr))
			{
				error(ctx, ERR_SYNTAX, "invalid label for %s conditional expression", opname);
				return;
			}

			dst = label;
			while(true == IsLabelChar(ctx, *ctx->m_Ptr))
			{
				*(dst++) = *(ctx->m_Ptr++);
			}
			*dst = 0;

		
			sym = FindSymbol(ctx, label, true, false);
			if(IFDEF == op)
			{
				cond = (NULL != sym ? true : false);
			}
			else
			{
				cond = (NULL == sym ? true : false);
			}
		}
		break;

	default:
		cond = false;	/* Make compiler happy */
		internal_error((ctx, "error processing conditional directive for %s", opname));
	}

	if(ctx->m_Cond.m_Count >= MAX_CONDITIONS)
	{
		error(ctx, ERR_GENERAL, "too many conditional directives");
	}

	ctx->m_Cond.m_Count++;
	ctx->m_Cond.m_Cond[ctx->m_Cond.m_Count].m_Op = op;
	ctx->m_Cond.m_Cond[ctx->m_Cond.m_Count].m_State = cond;
}


void PopConditionalState(EnvContext *ctx)
{
	ASSERTX(NO_CONDITION != ctx->m_Cond.m_Count);

	if(ctx->m_Cond.m_Count >= 0)
	{
		ctx->m_Cond.m_Count--;
	}
}


bool ToggleConditionalState(EnvContext *ctx)
{
	int count;
	bool state;

	ASSERTX(NO_CONDITION != ctx->m_Cond.m_Count);

	/* Get the count */
	count = ctx->m_Cond.m_Count;

	/* Get the current state */
	state = ctx->m_Cond.m_Cond[count].m_State;

	/* toggle it */
	state = (false == state ? true : false);

	/* Save the new state */
	ctx->m_Cond.m_Cond[count].m_State = state;

	/* Return the new state */
	return state;
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
