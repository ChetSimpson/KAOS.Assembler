/*****************************************************************************
	output.h	- Declarations for output handlers

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#ifndef OUTPUT_H
#define OUTPUT_H

#include "config.h"
#include "table9.h"
#include "context.h"



typedef enum
{
	SEGMENT_INVALID = -1,
	SEGMENT_CODE,
	SEGMENT_DATA,
	SEGMENT_BSS,
} SEGMENT;


typedef void (*FILEGEN_INIT_FUNC)(EnvContext *ctx, FILE *outFile);
typedef void (*FILEGEN_ADDBYTE_FUNC)(EnvContext *ctx, const u_char val,const  u_int16 pcreg);
typedef void (*FILEGEN_FINISH_FUNC)(EnvContext *ctx, const u_int16 regpc);
typedef void (*FILEGEN_FLUSH_FUNC)(EnvContext *ctx, const u_int16 regpc);

#define FILEGEN(type, iface, ext, fill)													\
	static void filegen_##type##_init(EnvContext *ctx, FILE *outFile);					\
	static void filegen_##type##_addbyte(EnvContext *ctx, u_char val, u_int16 pcreg);	\
	static void filegen_##type##_finish(EnvContext *ctx, u_int16 regpc);				\
	static void filegen_##type##_flush(EnvContext *ctx, u_int16 regpc);					\
	const OutputIFace filegen_##iface = {												\
		#ext,																			\
		fill,																			\
		filegen_##type##_init,															\
		filegen_##type##_addbyte,														\
		filegen_##type##_finish,														\
		filegen_##type##_flush,															\
	};


typedef struct
{
	const char				*extension;
	bool					fill_uninitdata;
	FILEGEN_INIT_FUNC		filegen_init;
	FILEGEN_ADDBYTE_FUNC	filegen_addbyte;
	FILEGEN_FINISH_FUNC		filegen_finish;
	FILEGEN_FLUSH_FUNC		filegen_flush;
} OutputIFace;

extern const OutputIFace filegen_S19;
extern const OutputIFace filegen_bin;
extern const OutputIFace filegen_os9;
extern const OutputIFace filegen_raw;
extern const OutputIFace filegen_rom;
extern const OutputIFace filegen_mod;
extern const OutputIFace filegen_rof;
extern const OutputIFace filegen_obj;

void SetSegment(EnvContext *info, SEGMENT segment);
void BeginSegment(EnvContext *info, SEGMENT seg);
void EndSegment(EnvContext *info);
SEGMENT GetCurrentSegment(EnvContext *ctx);


void EmitOpCode(EnvContext *ctx, const Mneumonic *op, const u_char modifier);
void EmitOpCodeEx(EnvContext *ctx, const u_char op);
void EmitOpCode2(EnvContext *ctx, const u_char op1, const u_char op2);
void EmitOpPostByte(EnvContext *ctx, const u_char postop);
void EmitOpDataByte(EnvContext *ctx, const u_char data);
void EmitOpDataWord(EnvContext *ctx, const u_int16 data);
void EmitOpDataLong(EnvContext *ctx, const u_int32 data);
void EmitOpAddr(EnvContext *ctx, const u_int16 addr);
void EmitOpRelAddrByte(EnvContext *ctx, const u_char addr);
void EmitOpRelAddrWord(EnvContext *ctx, const u_int16 addr);



void EmitDataByte(EnvContext *ctx, const u_char data);
void EmitDataWord(EnvContext *ctx, const u_int16 data);
void EmitDataLong(EnvContext *ctx, const u_int32 data);
void EmitUninitData(EnvContext *ctx, const u_int16 size);


bool OpenOutput(EnvContext *ctx, const char *name, const bool forceExt);
void FlushOutput(EnvContext *ctx);
void CloseOutput(EnvContext *ctx);
void PrintLine(EnvContext *ctx, const char* filename, const char *lineBuffer, const bool forceComment);
void PrintCycles(EnvContext *ctx, const int cycles);


#endif	/* OUTPUT_H */


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
