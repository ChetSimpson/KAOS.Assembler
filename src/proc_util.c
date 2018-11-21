/*****************************************************************************
	proc_util.c	- OPcode processing utils

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "cpu.h"

/*
 *	  GetRegister --- return register number of *line->m_Ptr
 */
REGISTER GetRegister(EnvContext *line, const bool allowZeroText, const bool allowZeroReg)
{
	if(head(line->m_Ptr,"d"))
	{
		line->m_Ptr++;
		return(REG_ACCD);
	}

	if(head(line->m_Ptr,"x"))
	{
		line->m_Ptr++;
		return(REG_INDX);
	}

	if(head(line->m_Ptr,"y"))
	{
		line->m_Ptr++;
		return(REG_INDY);
	}

	if(head(line->m_Ptr,"u"))
	{
		line->m_Ptr++;
		return(REG_USTACK);
	}

	if(head(line->m_Ptr,"s"))
	{
		line->m_Ptr++;
		return(REG_SSTACK);
	}

	if(head(line->m_Ptr,"pc"))
	{
		line->m_Ptr += 2;
		return(REG_PC);
	}


	if(head(line->m_Ptr,"w"))
	{
		line->m_Ptr++;
		return(REG_ACCW);		/* 6309 register */
	}

	if(head(line->m_Ptr,"v"))
	{
		line->m_Ptr++;
		return(REG_V);			/* 6309 register */
	}

	if(head(line->m_Ptr,"a"))
	{
		line->m_Ptr++;
		return(REG_ACCA);
	}

	if(head(line->m_Ptr,"b"))
	{
		line->m_Ptr++;
		return(REG_ACCB);
	}

	if(head(line->m_Ptr,"cc"))
	{
		line->m_Ptr += 2;
		return(REG_CC);
	}

	if(head(line->m_Ptr,"dp"))
	{
		line->m_Ptr += 2;
		return(REG_DP);
	}

	if(true == allowZeroReg)
	{
		if(true == allowZeroText && head(line->m_Ptr, "zero"))
		{
			line->m_Ptr += 4;
			return REG_ZERO;
		}

		if(head(line->m_Ptr,"o"))
		{
			line->m_Ptr++;
			return(REG_ZERO);		/* 6309 register */
		}

		if(head(line->m_Ptr,"0"))
		{
			line->m_Ptr++;
			return(REG_ZERO);		/* 6309 register */
		}

		if(head(line->m_Ptr,"00"))
		{
			line->m_Ptr += 2;
			return(REG_ZEROZERO);		/* 6309 register */
		}

	}

	if(head(line->m_Ptr,"e"))
	{
		line->m_Ptr++;
		return(REG_ACCE);		/* 6309 register */
	}

	if(head(line->m_Ptr,"f"))
	{
		line->m_Ptr++;
		return(REG_ACCF);		/* 6309 register */
	}

	if(head(line->m_Ptr,"pcr"))
	{
		line->m_Ptr += 3;
		return(REG_PCR);
	}

	return(REG_ERROR);
}



const char *GetRegisterName(const REGISTER r)
{
#define DOREG(x, y)	case x: return y;
	switch(r)
	{
	DOREG(REG_ACCD, "d");
	DOREG(REG_INDX, "x");
	DOREG(REG_INDY, "y");
	DOREG(REG_USTACK, "u");
	DOREG(REG_SSTACK, "s");
	DOREG(REG_PC, "pc");
	DOREG(REG_ACCW, "w");		/* 6309 register */
	DOREG(REG_V, "v");			/* 6309 register */
	DOREG(REG_ACCA, "a");
	DOREG(REG_ACCB, "b");
	DOREG(REG_CC, "cc");
	DOREG(REG_DP, "dp");
	DOREG(REG_ZERO, "0");		/* 6309 register */
	DOREG(REG_ZEROZERO, "00");	/* 6309 register */
	DOREG(REG_ACCE, "e");		/* 6309 register */
	DOREG(REG_ACCF, "f");		/* 6309 register */
	DOREG(REG_PCR, "pcr");
	default:
		return "unknown";
	}
#undef DOREG
}



/*
 *	  GetAddrMode --- determine addressing mode from operand field
 */

#define IS_STACK(a)		(reg == REG_SSTACK || reg == REG_USTACK)
#define IS_INDEX(a)		(reg == REG_INDX || reg == REG_INDY)
#define IS_PC(a)		(reg == REG_PC || reg == REG_PCR)

ADDR_MODE GetAddrMode(EnvContext *line)
{
	const char *p;
	REGISTER reg;

	if( *line->m_Operand  == '#' )
	{
		return(IMMEDIATE);		  /* immediate addressing */
	}

	if('[' == *line->m_Operand)
	{
		return INDIRECT;
	}

	p = line->m_Operand;

	/* Check for comma */
	while(false == IsWS(*p) && EOS != *p)
	{
		if(*p == ',')
		{
			return(INDEXED);	/* indexed addressing */
		}

		p++;
	}

	p = line->m_Operand;

	if(*p == '>')
	{
		return(EXTENDED);
	}

	if(*p == '<' )
	{
		return(DIRECT);
	}

	if( *p == '[' )
	{
		p++;
	}

	/* Skip index decrement */
	while(*p == '-')
	{
		p++;
	}

	reg = GetRegister(line, false, false);

	/* indexed addressing */
	if(IS_STACK(reg) || IS_INDEX(reg) || IS_PC(reg) )
	{
		return(INDEXED);
	}

	 /* Sanity check for indirect addressing */
	if( *line->m_Operand == '[')
	{
		return(INDIRECT);
	}

	return(EXTENDED);
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
