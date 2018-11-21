/*****************************************************************************
	config.h	- Declarations for system and operational configurations

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>


#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif


#define ERROR_BASE	-1

#ifndef __cplusplus
typedef enum
{
	false,
	true
} bool;
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4127)	/* Turn off warning for conditional expressions that are always true */
#pragma warning(disable: 4100)	/* Turn off warning for arguments that are not used */
#endif


#ifdef _WIN32
typedef unsigned char	u_char;
#endif
typedef short			int16;
typedef	unsigned short	u_int16;
typedef long			int32;
typedef unsigned long	u_int32;


#if defined(_WIN32) || defined(WIN32)
#define NORETURN	__declspec(noreturn)
#define MAXPATH		_MAX_PATH
#define MAXDRIVE	_MAX_DRIVE
#define MAXDIR		_MAX_DIR
#define MAXFILE		_MAX_FNAME
#define MAXEXT		_MAX_EXT
#define fnsplit		_splitpath
#define fnmerge		_makepath
#define creat		_creat
#define open		_open
#define close		_close
#define read		_read
#define write		_write
#define lseek		_lseek
#define filelength	_filelength
#define setmode		_setmode
#define unlink		_unlink
#define O_BINARY	_O_BINARY
#define O_RDWR		_O_RDWR
#define S_IREAD		_S_IREAD
#define S_IWRITE	_S_IWRITE
#else
#define NORETURN
#undef stricmp
#undef strnicmp
int stricmp(const char *, const char *);
int strnicmp(const char *, const char *, size_t sz);
#define MAXPATH		256
#define MAXDRIVE	256
#define MAXDIR		256
#define MAXFILE		256
#define MAXEXT		256

#endif	/* WIN32 || _WIN32 */

#ifndef FILENAME_MAX
#define FILENAME_MAX	260		/* longest file or path name allowed */
#endif

#define FILE_DEPTH		 32		/* Maximum depth for including files */
#define MAX_ERRORS		250		/* Maximum number of errors before exit */
#define MAX_WARNINGS	100		/* Maximum number of warnings before exit */
#define MAX_FILES		 64		/* Maximum number of files in list to assemble */
#define MAX_SEARCHPATHS	 64		/* Maximum number of search paths */
#define MAX_CONDITIONS	 64		/* Maximum number of contional assembly statements */
#define MAX_BUFFERSIZE	512     /* size of buffers */
#define MAX_LABELSIZE	 64		/* longest label size */
#define EMIT_LIMIT	   4096		/* Size of emit buffer used before flushing to file */
#define PMIT_LIMIT		 12		/* Bytes collected for listing */
#define MAX_VAR_LENGTH	 16		/* Max macro variable name length */
#define MAX_FCB_REPEATS	512		/* Maximum number of characters in an FCB repeating array */
#define MAX_TITLESIZE	 64		/* Max characters allowed in title */
/*      Character Constants     */
#define TAB		'\t'
#define SPACE	' '
#define EOS		'\0'

#define MIN_5BITS	-16
#define MAX_5BITS	15
#define MIN_BYTE	-128
#define MAX_BYTE	127
#define MAX_UBYTE	255

#define MIN_WORD	-32768
#define MAX_WORD	32767

/* We place this here in order to eliminate the cross dependency between
	context.h and cpu.h */
typedef enum
{
	CPU_NONE,
	CPU_6809,
	CPU_6309
} CPUTYPE;

/* Output file types */
typedef enum
{
	UNKNOWN = -1,
	S19FILE,		/* S-Record file */
	MOTBIN,			/* Motorola/RS-DOS binary file */
	OS9BIN,			/* OS-9 Binary file */
	ROMBIN,			/* Padded ROM binary file */
	MODBIN,			/* Loadable module */
	RAWBIN,			/* Raw binary file */
	ROFBIN,			/* OS-9 relocatable object file */
	OBJBIN			/* Prop object file */
} OUTPUT_TYPE;


#endif	/* CONFIG_H */


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
