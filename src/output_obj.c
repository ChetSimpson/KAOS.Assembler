/*****************************************************************************
	output_obj.c	- code emitting for relocatable object files

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "proto.h"


FILEGEN(obj, obj, obj, false);

void filegen_obj_init(EnvContext *ctx, FILE *outFile)
{
}


void filegen_obj_addbyte(EnvContext *ctx, u_char val, u_int16 regpc)
{
}


void filegen_obj_finish(EnvContext *ctx, u_int16 regpc)
{
}


void filegen_obj_flush(EnvContext *ctx, u_int16 regpc)
{
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