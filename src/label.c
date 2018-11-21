/*****************************************************************************
	label.c	- Label/Symbol helper routines

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "label.h"
#include "as.h"


bool IsLocalLabelChar(EnvContext *ctx, const char ch)
{
	return('@' == ch || (false == ctx->m_Compat.m_StrictLocals && '?' == ch));
}


bool HasLocalLabelChar(EnvContext *ctx, const char *str)
{
	if(false == IsLabelStart(ctx, *str))
	{
		return false;
	}

	while(*str && false == IsWS(*str))
	{
		if(true == IsLocalLabelChar(ctx, *str))
		{
			return true;
		}
		str++;
	}
	return false;
}


bool IsLabelStart(EnvContext *ctx, const char ch)
{
	return('_' == ch || true == IsLocalLabelChar(ctx, ch) || isalpha(ch));
}


bool IsLabelChar(EnvContext *ctx, const char ch)
{
	return(IsLabelStart(ctx, ch) || '.' == ch || isdigit(ch));
}


bool IsStructLabelStart(EnvContext *ctx, const char ch)
{
	return('_' == ch || isalpha(ch));
}


bool IsStructLabelChar(EnvContext *ctx, const char ch)
{
	return('_' == ch || isalnum(ch));
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
