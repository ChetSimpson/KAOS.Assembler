/*****************************************************************************
	proto.h	- Function prototypes

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#ifndef PROTO_H
#define PROTO_H

#include "context.h"
#include "table9.h"
#include "symtab.h"


void Params(EnvContext *ctx, int argc, char **argv);

/* prototypes for functions in as.c */


/*------------------------------------------------------------------------
	eval.c
------------------------------------------------------------------------*/
typedef enum
{
	EVAL_NORMAL,
	EVAL_NOERRORS,
	EVAL_INDEXED_FCB
} EVAL_TYPE;

#ifdef __cplusplus
bool Evaluate(EnvContext *line, u_int32 *retResult, EVAL_TYPE evalType, Symbol **retSym);
#endif

bool Evaluate(EnvContext *line, int32 *result, EVAL_TYPE evalType, Symbol **retSym);


/*------------------------------------------------------------------------
	ffwd.c
------------------------------------------------------------------------*/
void FwdRefInit(void);
void FwdRefReinit(EnvContext *ctx);
void FwdRefMark(EnvContext *ctx);
void FwdRefNext(EnvContext *ctx);
void FwdRefDone(void);
bool FwdRefIsRecord(const u_int32 lineNumber, const int fileno);

void ProcOpcode(EnvContext *line, const Mneumonic *mnu);
void ProcGeneral(EnvContext *line, const Mneumonic *op);
void ProcDirect(EnvContext *line, const Mneumonic *op);
void ProcIndexed(EnvContext *line, const Mneumonic *opinfo, u_char modifier);
void ProcInherent(EnvContext *line, const Mneumonic *op);
void ProcImmediate(EnvContext *line, const Mneumonic *op);
void ProcExtended(EnvContext *line, const Mneumonic *op);
void ProcLogicalMem(EnvContext *line, const Mneumonic *op);
void ProcPushPull(EnvContext *line, const Mneumonic *op);
void ProcBitTransfer(EnvContext *line, const Mneumonic *op);
void ProcRegToReg(EnvContext *line, const Mneumonic *op);
void ProcShortBranch(EnvContext *line, const Mneumonic *op);
void ProcLongBranch(EnvContext *line, const Mneumonic *op);
void ProcMemXfer(EnvContext *line, const Mneumonic *op);
void ProcMemXfer2(EnvContext *line, const Mneumonic *op);


void ProcPseudo(EnvContext *line, const Mneumonic *op);


bool ParseLine(LineBuffer *line, EnvContext *operandPtr);
bool ProcessLine(EnvContext *operandPtr);

#endif	/* PROTO_H */


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
