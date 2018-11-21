/*****************************************************************************
	cpu.c	- Implementation for 6809/6309 CPU specifics

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "cpu.h"
#include "error.h"
#include "util.h"

#define MAX_DP_STACK	256

static u_int16	regtenPC;                  	/* Program Counter              		*/
static u_int16	regtenOldPC;              	/* Program Counter at beginning 		*/
static u_int16	regtenDP;				/* storage to DP contents assumption 	*/
static u_char	dpStack[MAX_DP_STACK];
static int		dpStackPtr = 0;


void SetDPReg(const u_char val)
{
	regtenDP = (val & 0xff);
}


void InitCPU(void)
{
	SetPCReg(0);
	SetOldPCReg(0);
	regtenDP = 0x0000;
	dpStackPtr = 0;
}


u_char GetDPReg(void)
{
	return lobyte(regtenDP);
}

void SetPCReg(const u_int16 val)
{
	regtenPC = val;
}

void SetOldPCReg(const u_int16 val)
{
	regtenOldPC = val;
}

u_int16 GetPCReg(void)
{
	return regtenPC;
}


u_int16 GetOldPCReg(void)
{
	return regtenOldPC;
}

u_int16 BumpPCReg(const u_int16 val)
{
	regtenPC = regtenPC + val;
	return regtenPC;
}

void PushDPReg(EnvContext *ctx)
{
	if(dpStackPtr >= MAX_DP_STACK)
	{
		error(ctx, ERR_GENERAL, "too many Direct Page values saved");
	}
	else
	{
		dpStack[dpStackPtr] = (u_char)GetDPReg();
	}

	dpStackPtr++;
}

void PopDPReg(EnvContext *ctx)
{
	if(0 == dpStackPtr)
	{
		warning(ctx, WARN_DPSTACK, "no DP value has been saved : value of DP remains unchanged");
	}
	else
	{

		/* Restore DP only if it's within the stack */
		if(dpStackPtr < MAX_DP_STACK)
		{
			SetDPReg(dpStack[dpStackPtr]);
		}

		dpStackPtr--;
	}
}


bool IsAddressInDirectPage(const u_int16 addr)
{
	return(hibyte(addr) == GetDPReg());
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
