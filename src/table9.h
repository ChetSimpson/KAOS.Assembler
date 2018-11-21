/*****************************************************************************
	table9.h	- Declarations for opcodes

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#ifndef TABLE9_H
#define TABLE9_H


/* an entry in the mnemonic table */
#include "config.h"
#include "cpu.h"


typedef struct
{
	int		m_Immediate;
	int		m_Direct;
	int		m_Indexed;
	int		m_Extended;
	int		m_Inherent;
} CycleCount;

typedef struct
{
	u_int16		compatMask;		/* The level this mneumonic is allowed */
	CPUTYPE		cputype;		/* CPU type (6809/6309) */
	char		*mnemonic;		/* its name */
	u_char		immsize;		/* Datasize for immediate addressing mode */
	char		optype;			/* its class */
	u_char		page;			/* opcode page */
	u_char		opcode;			/* its base opcode */
	CycleCount	cycles6809;		/* its base # of cycles */
	CycleCount	cycles6309;		/* its base # of cycles */
} Mneumonic;

void InitOpcodeTable();

const Mneumonic *mne_look(EnvContext *ctx, const char *str);
int GetCycleCount(EnvContext *ctx, const Mneumonic *op, const ADDR_MODE mode, const CPUTYPE cpu);


#endif	/* TABLE9_H */


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
