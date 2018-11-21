/*****************************************************************************
	util.h	- Declarations for various utility functions

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#ifndef UTIL_H
#define UTIL_H

#include <assert.h>

#ifdef _DEBUG
#define ASSERT(x)	assert(x)
#else
#define ASSERT(x)
#endif


#ifndef _WIN32
int fnsplit (const char *path, char *drive, char *dir, char *name, char *ext);
void fnmerge (char *path, const char *drive, const char *dir, const char *name, const char *ext);
int filelength(int fno);
char *strlwr(char *str);
#endif


void *AllocMem(const size_t nbytes);
void *CAllocMem(const size_t count, const size_t nbytes);

char *SkipWS(char *ptr);
#define SkipConstWS(x)	((const char*)SkipWS((char*)x))

bool IsWS(const char c);
bool head(const char *str1, const char *str2);

/*----------------------------------------------------------------------------
	lobyte --- return low byte of an int
----------------------------------------------------------------------------*/
#define lobyte(i)	(((u_char)(i)) & 0xff)
#define loword(i)	(((u_int16)(i)) & 0xffff)

/*----------------------------------------------------------------------------
	hibyte --- return high byte of an int
----------------------------------------------------------------------------*/
#define hibyte(i)	((u_char)(((i) >> 8) & 0xff))
#define hiword(i)	((u_int16)(((i) >> 16) & 0xffff))


#define IsBinary(x)			('0' == (x) || '1' == (x))
#define IsOctal(x)			((x) >= '0' && (x) <= '7')
#define IsDigit(x)			(0 != isdigit(x))
#define IsHex(x)			(((x) >= 'a' && (x) <= 'f') || ((x) >= 'A' && (x) <= 'F'))
#define IsHexDecimal(x)		(true == IsDigit(x) || true == IsHex(x))
bool IsCommentChar(const EnvContext *ctx, const char ch);

#endif	/* UTIL_H */


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
