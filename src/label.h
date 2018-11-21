/*****************************************************************************
	label.h	- Interfaces for Label/Symbol helper functions

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#ifndef LABEL_H
#define LABEL_H

#include "config.h"
#include "context.h"

bool IsLocalLabelChar(EnvContext *ctx, const char ch);
bool IsLabelStart(EnvContext *ctx, const char ch);
bool IsLabelChar(EnvContext *ctx, const char ch);
bool IsStructLabelStart(EnvContext *ctx, const char ch);
bool IsStructLabelChar(EnvContext *ctx, const char ch);
bool HasLocalLabelChar(EnvContext *ctx, const char *str);
#define IsExportLabelStart IsStructLabelStart
#define IsExportLabelChar IsStructLabelChar


#endif	/* LABEL_H */


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
