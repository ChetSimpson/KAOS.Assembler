/*****************************************************************************
	pesudo.h	- Declarations for pseudo ops

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
#ifndef PSEUDO_H
#define PSEUDO_H


typedef enum
{
	NULL_OP,	/* null pseudo op				*/
	PRINTDP,

	MOD,		/* Define OS-9 module header	*/
	OS9,	 	/* OS9 system call directive	*/
	VSECT,		/* Begin variable section		*/
	PSECT,		/* Begin program section		*/
	ENDSECT,	/* ctx->m_Finished section		*/

	SEGCODE,	/* Code segment					*/
	SEGDATA,	/* Data segment					*/
	SEGBSS,		/* Uninit data segment			*/
	IMPORT,		/* Import symbol				*/
	EXPORT,		/* Export symbol				*/

	EQU,		/* Equate						*/

	FILL,		/* block fill constant bytes	*/
	FCB,		/* Form Constant Bytes			*/
	FDB,		/* Form Double Bytes (words)	*/
	FQB,		/* Form quad bytes				*/
	FCC,		/* Form Constant Characters		*/
	FCS,		/* Form command string			*/
	FCZ,		/* Form null terminated string	*/
	FCN,		/* "                         "  */
	FCR,
	ZMB,		/* Zero memory bytes			*/
	ZMD,		/* Zero memory words			*/
	ZMQ,		/* Zero memory quads			*/
	RMB,		/* Reserve Memory Bytes			*/
	RMD,		/* Reserve Memory words			*/
	RMQ,		/* Reserve Memory quads			*/

	RAW,	 	/* including raw binary data	*/
	LIB,	  	/* include file into assembly	*/

	ALIGN_EVEN,	/* Align PC on even byte boundry */
	ALIGN_ODD,	/* Align PC on odd byte boundry	*/
	ALIGN,		/* Align PC on arbitrary boundry */

	NAMESPACE,	/* Begin namespace				*/
	ENDNS,		/* ctx->m_Finished namespace	*/

	UNION,		/* Begin union					*/
	ENDUNION,	/* ctx->m_Finished union		*/

	OPT, 		/* assembler option				*/
	PAGE,		/* new page						*/

	ORG,		/* Origin						*/
	END,		/* end directive				*/

	MACRO,	 	/* define macro block			*/
	ENDM,	 	/* mark end of macro block		*/

	STRUCT,		/* Struct						*/
    STRUCTDEC,	/* Structure declaration		*/
	ENDSTRUCT,	/* ctx->m_Finished of structure */

	SETDP,		/* assume DP value is this		*/
	PUSHDP,		/* Saves the current set DP val	*/
	POPDP,		/* Gets the previous DP val		*/

	IF,		 	/* if (expression != 0)			*/
	IFNDEF,		/* if !(symbol defined)			*/
	IFDEF,		/* if (symbol defined)			*/
	IFN,	 	/* if (expression != 0)			*/
	IFEQ,	 	/* if (expression = 0)			*/
	IFGT,	 	/* if (expression > 0)			*/
	IFGE,	 	/* if (expression >= 0)			*/
	IFLT,	 	/* if (expression < 0)			*/
	IFLE,	 	/* if (expression <= 0)			*/
	IFNE,	 	/* if (expression != 0)			*/
	IFP1,	 	/* if (assembler pass = 1)		*/
	IFP2,	 	/* if (assembler pass = 1)		*/
	COND,		/* if (expression != 0)			*/
	ELSE,		/* Conditional else				*/
	ENDIF,	 	/* conditional assembly			*/
	ENDC,		/* end cond statement			*/
	ENDP,    	/* end (ifp1 || ifp2)			*/

	TITLE,		/* Set title					*/
	NAME,		/* Output name					*/

	/* Macro-80c directives */
	FAIL,
	LIST,
	NLST,
	REORG,
	OP_DEX,
	OP_INX,
	CLRD_6809



} PSEUDO_OP;




#endif	/* PSEUDO_H */


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
