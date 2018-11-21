/*****************************************************************************
	error.h	- Declarations for error reporting

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#ifndef ERROR_H
#define ERROR_H

#include "config.h"
#include "context.h"
/*
	Critical errors

*/

#define CRITICAL_USER	1	/* An error from the user */
#define CRITICAL_FATAL	2	/* Fatal error encountered */
#define CRITICAL_INTER	3	/* Internal compiler error */
#define CRITICAL_ERRORS	4	/* Too many errors reported */


typedef enum
{
	ERR_ERROR = -1,
	ERR_SUCCESS = 0,
	ERR_GENERAL,
	ERR_SYNTAX,
	ERR_SUBSCRIPT_OUT_OF_RANGE,
	ERR_INVALID_LABEL,
	ERR_LABEL_TOO_LONG,
	ERR_VALUE_TOO_BIG,
	ERR_UNDEFINED,
	ERR_REDEFINED,
	ERR_PHASING,	
	ERR_INVALID_MODE_FOR_OPERATION,
	ERR_INVALID_MODE_FOR_REGISTER,
	ERR_INVALID_REG_FOR_OPERATION,
	ERR_INVALID_SRCREG,
	ERR_INVALID_DSTREG,
	ERR_SRCREG_REQUIRED,
	ERR_DSTREG_REQUIRED,
	ERR_ILLEGAL_REGISTER_NAME,
	ERR_MISMATCHED_COND,
	ERR_INVALID_MEMBER,
} ERROR_NUMBER;

typedef enum
{
	WARN_LINESIZE,
	WARN_MISMATCHED_COND,
	WARN_COMPAT,		/* Compatibility issues */
	WARN_OS9SYSCALL,
	WARN_BUGALERT,
	WARN_PSECTUSAGE,
	WARN_VSECTUSAGE,
	WARN_DBLEXPORT,
	WARN_ENDSECTUSAGE,
	WARN_NONAMESPACE,
	WARN_DPRANGE,
	WARN_DPSTACK,
	WARN_EMPTYSTRUCT,
	WARN_ENTRYPOINT,
	WARN_LABELSIZE,
	WARN_OPTIMIZE,
	WARN_REGTOREG_SIZEMISMATCH,
	WARN_REGTOREG_CONSTDST,
	WARN_REGTOREG_PCR,
	WARN_OS9HEADER,
	WARN_VALUE_TRUNCATED,
	WARN_PREDEF,
	WARN_ROMSIZE,
	WARN_UNINITUSAGE,
	WARN_MACROPARAMS,
	WARN_SEGMENTUSAGE, 
} WARNING_NUMBER;

NORETURN void internal_ex(EnvContext *ctx, const char *str, ...);
extern const char *internal_fname;
extern int internal_lineno;
#define internal_error(x)	\
	{ internal_fname = __FILE__;\
	  internal_lineno = __LINE__;\
	  internal_ex x; }
#define ASSERTX(x)		if(!(x)) { internal_error((ctx, "%s", #x)); }


NORETURN void fatal(EnvContext *ctx, const char *str, ...);
void error(EnvContext *ctx, const ERROR_NUMBER errorno, const char *str, ...);
void warning(EnvContext *ctx, const WARNING_NUMBER warn, const char *str, ...);
void note(EnvContext *ctx, const char *str, ...);

#endif	/* ERROR_H */


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
