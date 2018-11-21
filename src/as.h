/*****************************************************************************
	as.h	- Assembler wide definitions

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/

/*
 *      machine independent definitions and global variables
 */
#ifndef AS_H
#define AS_H

#include "config.h"
#include "macro.h"
#include "symtab.h"
#include "pseudo.h"
#include "cpu.h"
#include "table9.h"
#include "util.h"
#include "error.h"
#include "output.h"
#include "context.h"


typedef struct _Export
{
	struct _Export	*next;
	char			*name;
} Export;


/* global variables */


/*
	various line counters
*/
extern u_int16		Cfn;			/* Current file number 1...n			*/


/*
	internal/external options
*/

extern int			Cpflag;			/* print cumulative cycles flag			*/
extern int			outputROMSize;	/* Size of the output ROM				*/
/*
	file handling pointers/counters
*/
extern u_int16		asmFileCount;	/* Number of files to assemble			*/
extern char			*filelist[];	/* list of files to assemble			*/
extern char			*ipathlist[];	/* search paths for include/lib pseudo	*/


/*
	misc pointers and other data
*/
extern char			**Argv;			/* pointer to file names				*/


/*
	Structures
*/
extern bool			instruct;
extern Struct		*laststructfound;

/* More misc */
extern char			*includeDirectories[256];
extern int			includeCount;
extern char			*outputDirectory;
extern char			*outputFilename;



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
