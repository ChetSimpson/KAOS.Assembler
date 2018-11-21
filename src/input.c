/*****************************************************************************
	input.c	- File input routines

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "proto.h"
#include "os9.h"
#include "label.h"

typedef struct
{
	char	fullpath[MAXDRIVE+MAXPATH+MAXDIR+MAXFILE+MAXEXT];
	char	drive[MAXDRIVE];
	char	dir[MAXDIR];
	char	filename[MAXFILE];
	char	ext[MAXEXT];
} PATH;

typedef struct
{
	PATH	path;
	FILE	*file;
	int		line_num;
} RECUR_FILES;


typedef struct
{
	 char drive[MAXDRIVE];
	 char dir[MAXDIR];
	 char file[MAXFILE];
	 char ext[MAXEXT];
} DIRDEF;

static FILE			*inputFile = NULL;				/* Current input file structure			*/
static RECUR_FILES	openedFiles[FILE_DEPTH];		/* recursive file list 				*/
static PATH			curfilepath;					/* split path information of current file */
static int			filecount = 0;					/* number of files currently opened		*/


int GetCurrentInputFileSize()
{
	return (NULL == inputFile ? 0 : filelength(fileno(inputFile)));
}



/*
 *	ProcessLine --- determine mnemonic class and act on it
 */

bool OpenInputFile(EnvContext *ctx, const char *filename, const bool binary)
{
	PATH	*cfp = &curfilepath;
	PATH	temp;
	char	tcb[MAXPATH];
	char	filename_new[MAXPATH];
	int		i;


	if(filecount == FILE_DEPTH)
	{
		error(ctx, ERR_GENERAL, "Cannot opened more than %i files at one time\n", FILE_DEPTH);
		return false;
	}

	if(filecount != 0)
	{
		/* create a path if one is already defined */
		fnsplit(filename, temp.drive, temp.dir, temp.filename, temp.ext);
		/* if drive is not specified, use the one of the current file */
		if(temp.drive[0] == 0)
		{
			/* merge the specified path with the path of the current file */
			strcpy(tcb, cfp->dir);	/* Get the path of the current file */
			strcat(tcb, temp.dir);	/* Get the path of the file to be opened */
			fnmerge(filename_new, temp.drive, tcb, temp.filename, temp.ext);
		}

		filename = filename_new;
	}

	for(i = 0; i < filecount + 1; i++)
	{
		if(0 == stricmp(openedFiles[i].path.fullpath, filename))
		{
			error(ctx, ERR_GENERAL, "'%s' is already in the input stream", filename);
			return false;
		}
	}

	openedFiles[filecount + 1].file = fopen(filename, true == binary ? "rb" : "r");
	if(NULL == openedFiles[filecount+1].file)
	{
		fprintf(stderr, "%s can't open %s\n", Argv[0], filename);
	}
	else
	{
		openedFiles[filecount].line_num = ctx->m_LineNumber;

		filecount++;

		cfp = &curfilepath;
		fnsplit(filename, cfp->drive, cfp->dir, cfp->filename, cfp->ext);

		cfp = &openedFiles[filecount].path;
		fnsplit(filename, cfp->drive, cfp->dir, cfp->filename, cfp->ext);

		strcpy(openedFiles[filecount].path.fullpath, filename);

		inputFile = openedFiles[filecount].file;	/* add file handle to list */
		ctx->m_LineNumber = 0;

		return true;
	}

	return false;
}


int CloseInputFile(EnvContext *ctx)
{
	PATH *tp, *cfp = &curfilepath;

	if(true == IsProcessingMacro())
	{
		error(ctx, ERR_GENERAL, "unterminated macro at end of file");
	}


	if(openedFiles == 0)
	{
		fprintf(stderr, "kasm: no files opened\n");
		return false;
	}
	fclose(openedFiles[filecount].file);
	memset(&openedFiles[filecount], 0, sizeof(RECUR_FILES));

	filecount--;
	inputFile = openedFiles[filecount].file;
	tp = &openedFiles[filecount].path;
	memcpy(cfp, tp, sizeof(PATH));
	ctx->m_LineNumber = openedFiles[filecount].line_num;
	return(true);
}

u_char ReadInputByte()
{
	return (u_char)fgetc(inputFile);
}

int GetOpenFileCount()
{
	return filecount;
}

const char *GetCurrentFilePathname()
{
	return openedFiles[filecount].path.fullpath;
}


bool ReadInputLine(EnvContext *ctx, char *lineBuffer, const int maxLength)
{
	char *dst;
	int count;
	int ch;

	ASSERTX(maxLength > 0);

	count = 0;
	dst = lineBuffer;
	ch = 0;

	while(count < (maxLength - 1))
	{

		ch = fgetc(inputFile);

		if(EOF == ch)
		{
			break;
		}

		if('\n' == ch)
		{
			ch = fgetc(inputFile);
			if('\r' != ch)
			{
				ungetc(ch, inputFile);
			}
			break;
		}

		if('\r' == ch)
		{
			break;
		}

		*(dst++) = (char)ch;
		count++;
	}

	*dst = 0;

	if(0 == count && EOF == ch)
	{
		return false;
	}

	if(count == maxLength)
	{
		while(EOF != ch)
		{
			ch = fgetc(inputFile);
			if(ch == '\n' || ch == '\r')
			{
				break;
			}
		}
		warning(ctx, WARN_LINESIZE, "input line is too long and has been truncated to %d characters", maxLength);
		return false;
	}


	return true;
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
