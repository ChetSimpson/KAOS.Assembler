/*****************************************************************************
	cpu.h	- Declarations for CPU specific information

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#ifndef CPU_H
#define CPU_H

#include "context.h"

#define BIT_0	0
#define BIT_8	8
#define BIT_16	16
#define BIT_32	32

/*      Opcode Classes          */
typedef enum
{
	ADDR_ERROR = ERROR_BASE,
	INDIRECT = 0,		/* Indirect addressing [$xxxx]		*/
	EXTENDED,			/* extended							*/
	DIRECT,				/* Direct Page access				*/
	INDEXED,			/* Indexed only						*/
	INHERENT,			/* Inherent							*/
	IMMEDIATE,			/* Immediate only					*/
	GENERAL,			/* General addressing				*/
	REL_SHORT,			/* Short Relative					*/
	REL_LONG,			/* Long Relative					*/
	NOIMM,				/* General except for Immediate		*/
	RTOR,				/* Register To Register				*/
	REGLIST,			/* Register List					*/
	GRP2,				/* Group 2 (Read/Modify/Write)		*/
	LOGMEM,				/* 6309 logical memory ops			*/
	BTM,				/* Bit transfer and manipulation	*/
	MRTOR,				/* TFM memory transfers				*/
	MRTOR2,				/* TFM memory transfers - CCASM		*/
	PSEUDO				/* Pseudo ops						*/
} ADDR_MODE;




/* MC6809 specific processing */

/*
0000 - D (A:B)    1000 - A
0001 - X          1001 - B
0010 - Y          1010 - CCR
0011 - U          1011 - DPR
0100 - S          1100 - 0
0101 - PC         1101 - 0
0110 - W          1110 - E
0111 - V          1111 - F
*/
/* register names */
typedef enum
{
	REG_ERROR = ERROR_BASE,
	REG_ACCD = 0,
	REG_INDX,
	REG_INDY,
	REG_USTACK,
	REG_SSTACK,
	REG_PC,
	REG_ACCW,
	REG_V,
	REG_ACCA,
	REG_ACCB,
	REG_CC,
	REG_DP,
	REG_ZERO,
	REG_ZEROZERO,
	REG_ACCE,
	REG_ACCF,
	REG_PCR,
	REG_LSN_MASK = 0x0f,
	REG_MSN_MASK = 0xf0
} REGISTER;

#define NOPAGE			0x00
#define PAGE2			0x10
#define PAGE3			0x11
#define IPBYTE			0x9F	/* extended indirect postbyte */
#define OP_SWI			0x3F
#define LDQ_OPIMM		0xcd
#define	LDQ_OPOTH		0xdc
#define OP_PSHS			0x34
#define OP_PSHU			0x36
#define OP_PULS			0x35
#define OP_PULU			0x37

#define OP_PSHS_U_1		OP_PSHS
#define OP_PSHS_U_2		0x40
#define OP_TFR_S_U1		0x1f
#define OP_TFR_S_U2		0x43
#define OP_TFR_U_S1		0x1f
#define OP_TFR_U_S2		0x34
#define OP_PULS_U_PC1	OP_PULS
#define OP_PULS_U_PC2	0xc0
#define OP_LEAS_8BIT_1	0x32
#define OP_LEAS_8BIT_2	0xe8



void InitCPU(void);
u_int16 BumpPCReg(const u_int16 val);
u_char GetDPReg();
u_int16 GetPCReg(void);
u_int16 GetOldPCReg(void);
void SetPCReg(const u_int16 val);
void SetOldPCReg(const u_int16 val);
void SetDPReg(const u_char val);
void PushDPReg(EnvContext *ctx);
void PopDPReg(EnvContext *ctx);
bool IsAddressInDirectPage(const u_int16 addr);


#endif	/* CPU_H */


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
