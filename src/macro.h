/*****************************************************************************
	macro.h	- Declarations for the macro handler

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#ifndef MACRO_H
#define MACRO_H

#include "config.h"
#include "input.h"


/*
	Macro processing pointers, flags and counters
*/

bool OpenMacro(EnvContext *info, const char *name, const char *macroargs);
void CloseMacro(void);
void CreateMacro(EnvContext *info, const char *macroname, const char *macroargs);
void EndMacro(EnvContext *line);
void AddLineToMacro(EnvContext *line, const char *macroline);
char *GetMacroLine(EnvContext *info, char *outBuffer);
bool IsMacroOpen();
bool IsProcessingMacro();
void ResetMacroLocalLabels();
int GetNextMacroLocalLabel();
int GetMacroLocalLabel();
bool OnFirstMacroLine();


#endif	/* MACRO_H */


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
