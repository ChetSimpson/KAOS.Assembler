/*****************************************************************************
	output_listing.c	- formatted listing output routines

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "proto.h"

/*----------------------------------------------------------------------------
	PrintLine --- pretty print input line
----------------------------------------------------------------------------*/
void PrintLine(EnvContext *ctx, const char *lineBuffer, bool forceComment)
{
	int			i;
	const char	*ptr;
	char		optchar;

	i = 0;

	if(true == forceComment)
	{
		return;
	}

	if(ctx->m_OptimizeLine == true)
	{
		optchar = '>';
	}
	else
	{
		optchar = SPACE;
	}

	if(false == ctx->m_ListingFlags.OptNoLineNumbers)
	{
		fprintf(stdout, "%5d%c ", (int)ctx->m_LineNumber, optchar);
	}

	if(false == ctx->m_ListingFlags.OptNoOpData)
	{
		if(ctx->m_ListingFlags.P_total || true == ctx->m_ListingFlags.P_force || true == IsMacroOpen())
		{
			fprintf(stdout, "%04x ", GetOldPCReg());
		}

		else
		{
			fprintf(stdout, "     ");
		}

		i = 0;

		if(i < ctx->m_ListingFlags.P_total)
		{
			if(ctx->m_ListingFlags.P_total > 1 && (PAGE2 == ctx->m_ListingFlags.P_bytes[0] || PAGE3 == ctx->m_ListingFlags.P_bytes[0]))
			{
				fprintf(stdout, "%02X", ctx->m_ListingFlags.P_bytes[i++]);
			}
			fprintf(stdout, "%02X", ctx->m_ListingFlags.P_bytes[i++]);

			/*
			fputc(SPACE, stdout);
			if(1 == i)
			{
				fputc(SPACE, stdout);
				fputc(SPACE, stdout);
			}
			*/
		}

		for(;i < ctx->m_ListingFlags.P_total && i < 6 ;i++)
		{
			fprintf(stdout, "%02x", lobyte(ctx->m_ListingFlags.P_bytes[i]));
		}

		for(; i < 6; i++)
		{
			fprintf(stdout, "  ");
		}

		fprintf(stdout, "  ");

		if(ctx->m_Misc.Cflag && ctx->m_CycleCount)
		{
			fprintf(stdout, "[%2d] ", (int)ctx->m_CycleCount);
		}

		else
		{
			fprintf(stdout, "       ");
		}
	}

	if(true == forceComment)
	{
		putchar('*');
	}

	ptr = lineBuffer;
	while( *ptr != EOS )
	{
		putchar(*ptr++);   /* just echo the line back out */
	}

	if(false == ctx->m_ListingFlags.OptNoOpData)
	{
		for(; i < ctx->m_ListingFlags.P_total; i++)
		{
			if(0 == (i % 6))
			{
				fprintf(stdout, "\n            ");
			}
			fprintf(stdout, "%02x", ctx->m_ListingFlags.P_bytes[i]);
		}
	}
	fprintf(stdout, "\n");
}

/*----------------------------------------------------------------------------
	PrintCycles --- print # of cycles counted so far
----------------------------------------------------------------------------*/
void PrintCycles(EnvContext *ctx, const int cycles)
{
	if(ctx->m_Pass == 2 && 0 != cycles)
	{
		fprintf(stdout, "  ctx->m_CycleCount Counted:  %d\n\n", cycles);
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
