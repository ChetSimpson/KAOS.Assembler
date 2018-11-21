/*****************************************************************************
	util.c	- implements stricmp

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.
	
	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#include "as.h"
#include "proto.h"

bool IsCommentChar(const EnvContext *ctx, const char ch)
{
	if('*' == ch || (true == ctx->m_Compat.m_SemicolonComment && ';' == ch))
	{
		return true;
	}

	return false;
}



#ifndef _WIN32

char *strlwr(char *str)
{
	char *save;

	save = str;

	while(*str)
	{
		*str = tolower(*str);
		str++;
	}

	return str;
}


int stricmp(const char *src, const char *dst)
{
	while(0 != *src && 0 != *dst)
	{
		if(tolower(*src) != tolower(*dst))
		{
			break;
		}
		src++;
		dst++;
	}

	return tolower(*src) - tolower(*dst);
}

int strnicmp(const char *src, const char *dst, size_t sz)
{
	if(sz < 1)
	{
		return 0;
	}


	while(0 != *src && 0 != *dst && sz > 0)
	{
		if(tolower(*src) != tolower(*dst))
		{
			break;
		}
		src++;
		dst++;
		sz--;
	}

	return tolower(*src) - tolower(*dst);
}
#endif



/*----------------------------------------------------------------------------
	any --- does str contain c?
----------------------------------------------------------------------------*/
static bool any(const char c, const char *str)
{
	while(*str != EOS)
	{
		if(*str++ == c)
		{
			return(true);
		}
	}
	return(false);
}


/*----------------------------------------------------------------------------
	head --- is str2 the head of str1?
----------------------------------------------------------------------------*/
bool head(const char *str1, const char *str2)
{
	while(*str1 != EOS && *str2 != EOS)
	{
		if(tolower(*str1) != tolower(*str2) )
		{
			break;
		}
		str1++;
		str2++;
	}

	if(*str1 == *str2)
	{
		return(true);
	}

	if(*str2 == EOS)
	{
		if(any(*str1," \t\n,+-];*"))
		{
			return(true);
		}
	}
	return(false);
}

/*----------------------------------------------------------------------------
	IsWS --- is character whitespace?
----------------------------------------------------------------------------*/
bool IsWS(const char ch)
{
	if(TAB == ch || SPACE == ch)
	{
		return(true);
	}
	return(false);
}

/*----------------------------------------------------------------------------
	AllocMem --- allocate memory
----------------------------------------------------------------------------*/
void *AllocMem(const size_t nbytes)
{
	void *ptr;

	ptr = malloc(nbytes);
	if(ptr == NULL)
	{
		internal_error((NULL, "out of memory"));
	}
	return(ptr);
}


void *CAllocMem(const size_t count, const size_t nbytes)
{
	void *ptr;

	ptr = calloc(count, nbytes);
	if(ptr == NULL)
	{
		internal_error((NULL, "out of memory"));
	}
	return(ptr);
}


/*----------------------------------------------------------------------------
	SkipWS --- move pointer to next non-whitespace char
----------------------------------------------------------------------------*/
char *SkipWS(char *ptr)
{
	while(SPACE == *ptr || TAB == *ptr) ptr++;
	return(ptr);
}




#ifndef _WIN32

#include <sys/stat.h>

int filelength(int fno)
{
	struct stat s;

	fstat(fno, &s);

	return s.st_size;
}

enum
{
	DRIVE,
	DIRECTORY,
	FILENAME,
	EXTENSION,
	WILDCARDS
};


static char *max_ptr(char *p1, char *p2)
{
	if (p1 > p2) return p1;
	else return p2;
}


int fnsplit (const char *path, char *drive, char *dir, char *name, char *ext)
{
	int flags = 0, len;
	const char *pp, *pe;

	if (drive) *drive = '\0';
	if (dir) *dir = '\0';
	if (name) *name = '\0';
	if (ext) *ext = '\0';

	pp = path;

	if ((isalpha(*pp) || strchr("[\\]^_`", *pp)) && (pp[1] == ':'))
	{
		flags |= DRIVE;
		if (drive)
		{
			strncpy(drive, pp, 2);
			drive[2] = '\0';
		}
		pp += 2;
	}

	pe = max_ptr(strrchr(pp, '\\'), strrchr(pp, '/'));
	if (pe) 
	{ 
		flags |= DIRECTORY;
		pe++;
		len = pe - pp;
		if (dir)
		{
			strncpy(dir, pp, len);
			dir[len] = '\0';
		}
		pp = pe;
	}
	else
	{
		pe = pp;
	}

	/* Special case: "c:/path/." or "c:/path/.." These mean FILENAME, not EXTENSION.  */
	while (*pp == '.') ++pp;
	if (pp > pe)
	{
		flags |= FILENAME;
		if (name)
		{
			len = pp - pe;
			strncpy(name, pe, len);
			name[len] = '\0';
		}
	}

	pe = strrchr(pp, '.');
	if (pe)
	{
		flags |= EXTENSION;
		if (ext) strcpy(ext, pe);
	}
	else pe = strchr( pp, '\0');

	if (pp != pe)
	{
		flags |= FILENAME;
		len = pe - pp;
		if (name)
		{
			strncpy(name, pp, len);
			name[len] = '\0';
		}
	}

	if (strcspn(path, "*?[") < strlen(path)) flags |= WILDCARDS;

	return flags;
}


void fnmerge(char *path, const char *drive, const char *dir, const char *name, const char *ext)
{
	*path = '\0';
	if (drive && *drive)
	{
		path[0] = drive[0];
		path[1] = ':';
		path[2] = 0;
	}
	if (dir && *dir)
	{
		char last_dir_char = dir[strlen(dir) - 1];

		strcat(path, dir);
		if (last_dir_char != '/' && last_dir_char != '\\')
			strcat(path, strchr(dir, '\\') ? "\\" : "/");
	}
	if (name) strcat(path, name);
	if (ext && *ext)
	{
		if (*ext != '.') strcat(path, ".");
		strcat(path, ext);
	}
}

#endif


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
