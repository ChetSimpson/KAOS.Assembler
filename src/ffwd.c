/*****************************************************************************
	ffwd.c	- forward reference manager

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "proto.h"


#define MAX_FORWARD_REFS	32768

typedef struct
{
	u_int16		cfn;
	u_int32		line;
} ForwardRef;

static ForwardRef refs[MAX_FORWARD_REFS];
static int		refsCount = 0;
static int		refsMax = 0;
static u_int16	Ffn = 0;						/* forward ref file #					*/
static u_int32	F_ref = 0;						/* next line with forward ref			*/



bool FwdRefIsRecord(const u_int32 lineNumber, const int fileno)
{
	if(lineNumber == F_ref && fileno == Ffn)
	{
		return true;
	}
	return false;

}


void FwdRefInit(void)
{
	refsCount = 0;
	refsMax = 0;
}

/*
 *      FwdRefReinit --- reinitialize forward ref file
 */
void FwdRefReinit(EnvContext *ctx)
{
	refsCount = 0;
	if(0 != refsMax)
	{
		FwdRefNext(ctx);
	}
}

/*
 *      FwdRefMark --- mark current file/line as containing a forward ref
 */
void FwdRefMark(EnvContext *ctx)
{
	ASSERT(refsCount < MAX_FORWARD_REFS);
	refs[refsCount].cfn = Cfn;
	refs[refsCount].line = ctx->m_LineNumber;

	refsCount++;
	refsMax++;
}

/*
 *      FwdRefNext --- get next forward ref
 */
void FwdRefNext(EnvContext *ctx)
{
	ASSERTX(refsCount <= refsMax);
	Ffn = refs[refsCount].cfn;
	F_ref = refs[refsCount].line;
	refsCount++;
}

/*
 *  FwdRefDone --- closes & deletes forward reference file
 */
void FwdRefDone(void)
{
	refsCount = 0;
	refsMax = 0;
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
