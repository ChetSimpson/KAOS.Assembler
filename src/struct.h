/*****************************************************************************
	struct.h	- Declarations for the struct handler

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#ifndef STRUCT_H
#define STRUCT_H

#include "config.h"
#include "input.h"


typedef struct _element Element;
typedef struct _struct Struct;


/* Element entry */
struct _element
{
	char		name[MAX_LABELSIZE];
	int			offset;		/* Offset into the structure of the element */
	int			size;		/* Size of a single element */
	int			count;		/* Number of elements */
	Struct		*child;		/* Child structures */
	Element		*next;
};

/* Structure entry */
struct _struct
{
	char		name[MAX_LABELSIZE];
	int			size;
	Element		*el_head;	/* First element in the list */
	Element		*el_tail;	/* Last element in the list */
	Struct		*next;
};


void StructCreate(EnvContext *ctx, const char *structName);
void StructAddElement(EnvContext *ctx, const char *elementName, const char *elementType);
Struct *StructLookup(const char *);
Element *StructGetElement(const Struct *, const char *);
void UnionCreate(EnvContext *ctx, const char *unionName);
void UnionEnd(EnvContext *ctx);

#endif	/* STRUCT_H */


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
