/*****************************************************************************
	error.c	- Error reporting routines

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "error.h"
#include "as.h"
#include "input.h"

/*----------------------------------------------------------------------------
	fatal --- fatal error handler
----------------------------------------------------------------------------*/

NORETURN static void dofatal(EnvContext *ctx, int error, const char *fmt, va_list list)
{
	if(GetOpenFileCount() > 0 && NULL != ctx)
	{
		fprintf(stderr, "%s(%lu) : ", GetCurrentFilePathname(), ctx->m_LineNumber);
	}
	vfprintf(stderr, fmt, list);
	fprintf(stderr, "\n");
	exit(CRITICAL_FATAL);
}

NORETURN void fatal(EnvContext *ctx, const char *str, ...)
{
	va_list list;

	va_start(list, str);

	dofatal(ctx, CRITICAL_FATAL, str, list);
}


/*----------------------------------------------------------------------------
	error --- error in a line. print line number and error
----------------------------------------------------------------------------*/
const char *internal_fname;
int internal_lineno;
NORETURN void internal_ex(EnvContext *ctx, const char *fmt, ...)
{
	va_list list;

	va_start(list, fmt);

	if(NULL != ctx)
	{
		fprintf(stderr, "%s\n", ctx->m_Line);
	}
	fprintf(stderr, "%s(%lu) : Internal assembler error: ", GetCurrentFilePathname(), ctx->m_LineNumber);
	vfprintf(stderr, fmt, list);
	fputc('\n', stderr);
	if(NULL != internal_fname)
	{
		fprintf(stderr, "kasm file %s line %d\n", internal_fname, internal_lineno);
	}
	fprintf(stderr, "\n");
	exit(CRITICAL_INTER);
}



/*----------------------------------------------------------------------------
	error --- error in a line. print line number and error
----------------------------------------------------------------------------*/
void error(EnvContext *ctx, ERROR_NUMBER errnum, const char *str, ...)
{
	va_list list;


	va_start(list, str);

	if(NULL != ctx)
	{
		fprintf(stderr, "%s(%lu) : error A%4d : ", GetCurrentFilePathname(), ctx->m_LineNumber, errnum + 2100);
	}

	
	switch(errnum)
	{
	case ERR_SYNTAX:
		fprintf(stderr, "syntax error : ");
		break;

	case ERR_INVALID_MODE_FOR_OPERATION:
		fprintf(stderr, "invalid mode for operation : ");
		break;

	case ERR_PHASING:
		fprintf(stderr, "phasing error : ");
		break;

	case ERR_INVALID_REG_FOR_OPERATION:
		fprintf(stderr, "invalid register ");
		break;

	case ERR_INVALID_MODE_FOR_REGISTER:
		fprintf(stderr, "invalid mode for register : ");
		break;

	case ERR_MISMATCHED_COND:
		fprintf(stderr, "mismatched conditional directive : ");
		break;

	default:
		break;
	}

	vfprintf(stderr, str, list);
	fputc('\n', stderr);

	if(NULL != ctx)
	{
		fprintf(stderr, "%s\n", ctx->m_Line);
	}	ctx->m_ErrorCount++;

	if(ctx->m_ErrorCount > MAX_ERRORS)
	{
		va_list args;

		va_start(args, str);
		dofatal(ctx, CRITICAL_ERRORS, "Too many errors.  Stopping assembly", args);
	}
}




/*----------------------------------------------------------------------------
	warn --- trivial error in a line. print line number and error
----------------------------------------------------------------------------*/
void warning(EnvContext *ctx, WARNING_NUMBER warn, const char *str, ...)
{
	va_list list;

	/* Check for disabled warnings */
	if(false == ctx->m_Compat.m_Warn && WARN_COMPAT == warn)
	{
		return;
	}

	if(true == ctx->m_DisableWarnings)
	{
		if(WARN_COMPAT != warn)
		{
			return;
		}
	}

	if(2 == ctx->m_Pass && false == ctx->m_ListingFlags.m_OptEnabled)
	{
		return;    /* if not listing, don't */
	}

	va_start(list, str);

	/* repeat the warnings   */
	fprintf(stderr, "%s(%lu): warning : ", GetCurrentFilePathname(), ctx->m_LineNumber);
	vfprintf(stderr, str, list);
	fprintf(stderr, "\n");
	ctx->m_WarningCount++;
}


/*----------------------------------------------------------------------------
	note --- puts note and cumulative line # in listing
----------------------------------------------------------------------------*/
void note(EnvContext *ctx, const char *str, ...)
{
	if(true == ctx->m_ListingFlags.m_OptEnabled && 2 == ctx->m_Pass)
	{
		va_list list;
		va_start(list, str);

		fprintf(stderr, "%s(%lu): note : ", GetCurrentFilePathname(), ctx->m_LineNumber);
		vfprintf(stderr, str, list);
		fprintf(stderr, "\n");
	}
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
