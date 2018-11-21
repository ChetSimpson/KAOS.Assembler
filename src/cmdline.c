/*****************************************************************************
	cmdline.c	- options parser

	Copyright (C) 1996, 1997, 2014, 2018, by Chet Simpson, Et al.

	This file is distributed under the MIT License. See notice at the end
	of this file.

*****************************************************************************/
/*
*/
#include "as.h"
#include "proto.h"
#include "context.h"

static void banner(void);


static void banner(void)
{
	fprintf(stderr, "KAOS Assembler V0.0\n");
	fprintf(stderr, "Copyright (c) 1996, 1997, 2004, 2018, by Chet Simpson\n");
}

static void Usage(void)
{
	fprintf(stderr, "Usage:  %s [files] [-options]\n", Argv[0]);
	fprintf(stderr, "Example:  %s file1.asm file2.asm -xref -bin\n\n", Argv[0]);
}


static void poption(const char *opt, const char *desc)
{
	size_t tabs;

	fputc('-', stderr);
	fputs(opt, stderr);
	tabs = (30 - strlen(opt)) / 8;
	while(tabs > 0)
	{
		fputc(TAB, stderr);
		tabs--;
	}
	fprintf(stderr, "%s\n",  desc);
}


static void pheader(const char *hdr)
{
	fprintf(stderr, "\n%s\n", hdr);
	fputs("-------------------------------------------------------------------------------\n", stdout);
}

static void help(void)
{
	Usage();

	/* General options */
	pheader("General KASM options");
	poption("help", "Displays this list");
	poption("?", "Displays this list");
	poption("6809", "Allow only 6809 Opcodes");
	poption("D", "Define symbol");
#ifdef FIXME_LATER_OR_IMPLEMENT
	poption("I", "Add include directory");
#endif
	poption("C", "Use case sensative labels");
	poption("silent", "Run in silent mode");
	poption("-no-warn", "Disable warnings");

	/* Output options */
	pheader("Output file options");
	poption("O", "Set output directory");
	poption("o", "Set output name");
	poption("bin", "Generate CoCo RS-DOS binary file");
	poption("s19", "Generate S-record file");
	poption("rom", "Generate a padded ROM file");
	poption("raw", "Generate raw binary file");
	poption("mod", "Generate shared object module");
	poption("os9", "Generate Microware OS-9 module");
	poption("rof", "Generate Microware Relocatable Object File");
	poption("obj", "Generate KASM object file");
	poption("noout", "Assemble only, do not generate output file");

	/* Compatibility options */
	pheader("Compatibilty options");
	poption("-mode-rma", "Assemble in RMA compatibility mode");
	poption("-mode-macro80c", "Assemble in Macro-80c compatibility mode");
	poption("-mode-edtasm", "Assemble in EDTASM+ compatibility mode");
	poption("-mode-edtasm6309", "Assemble in EDTASM6309 compatibility mode");
	poption("-mode-ccasm", "Assemble in CCASM compatibility mode");
	poption("-warn-portable", "Warn of source code portability issues");
	poption("-force-zero-offset", "Enable explicit 0 index offset");
	poption("-force-pc-relative", "Use relative offsets for both PC and PCR");
	poption("-disable-macros", "Disable use of macros");
	poption("-disable-locals", "Disable use of local labels");
	poption("-strict-locals", "Enable use of strict local label rules");
	poption("-ignore-case", "Ignore case on symbols");
	poption("-enable-precedence", "Enable operator precedence in expressions");
	

	/* Listing options */
	pheader("Assembler listing options");
	poption("list", "Generate formatted source file listing to stdout");
	poption("nolist", "Source output disabled");
	poption("cycle", "Cycle count enabled");
	poption("slist", "Generate formatted list of labels");
	poption("xref", "Cross refence table disabled");
	poption("expand", "Macro definitions will be expanded in listing");
	poption("opt", "Display number of lines that could be optmized");
	poption("alert", "Display source lines that could be displayed");
	poption("noln", "Do not print line numbers in listing");
	poption("noopdata", "Do not print opcode data in listing");
	poption("cm", "Comment out macros in listing");

#if 0
	pheader("CCASM compatible command line options");
	poption("l", "Generate formatted source file listing to stdout");
	poption("s", "Generate formatted list of labels");
	poption("sa", "ignored");
	poption("sr", "ignored");
	poption("nr", "Generate output file with no records");
	poption("d", "ignored");
	poption("bin", "Generate CoCo RS-DOS binary file");
	poption("rom[=size]", "Generate a padded ROM file with optional size");
#endif

	fprintf(stderr, "\nSome options can be enabled or disabled from within source files\n");
	fprintf(stderr, "Please refer to the on-line manual for a list of those options.\n");

	exit(0);
}



void Params(EnvContext *ctx, int argc, char **argv)
{
	char *ptr;
	char symname[50];
	u_int32 symval;
	char *opt;
	int count;
	bool flag;
	asmFileCount = 0;

	ctx->m_Compat.m_AsmMode = ASM_MODE_KASM;

	for(count = 1; count < argc; count++)
	{
		if(*argv[count] == '-')
		{
			flag = false;
			opt = argv[count]+1;

			if('-' == *opt)
			{
				opt++;

				if(0 == stricmp(opt, "mode-edtasm"))
				{
					ctx->m_Compat.m_AsmMode = ASM_MODE_EDTASM;
				}

				else if(0 == stricmp(opt, "mode-edtasm6309"))
				{
					ctx->m_Compat.m_AsmMode = ASM_MODE_EDTASM6309;
				}

				else if(0 == stricmp(opt, "mode-rma"))
				{
					ctx->m_Compat.m_AsmMode = ASM_MODE_RMA;
				}

				else if(0 == stricmp(opt, "mode-macro80c"))
				{
					ctx->m_Compat.m_AsmMode = ASM_MODE_MACRO80C;
				}
	
				else if(0 == stricmp(opt, "warn-portable"))
				{
					ctx->m_Compat.m_Warn = true;
				}

				else if(0 == stricmp(opt, "force-zero-offset"))
				{
					ctx->m_Compat.m_ForceZeroOffset = true;
				}

				else if(0 == stricmp(opt, "force-pc-relative"))
				{
					ctx->m_Compat.m_ForcePCR = true;
				}

				else if(0 == stricmp(opt, "strict-locals"))
				{
					ctx->m_Compat.m_StrictLocals = true;
				}

				else if(0 == stricmp(opt, "disable-macros"))
				{
					ctx->m_Compat.m_DisableMacros = true;
				}

				else if(0 == stricmp(opt, "disable-locals"))
				{
					ctx->m_Compat.m_DisableLocals = true;
				}

				else if(0 == stricmp(opt, "ignore-case"))
				{
					ctx->m_Compat.m_IgnoreCase = true;
				}

				else if(0 == stricmp(opt, "enable-precedence"))
				{
					ctx->m_Compat.m_UsePrecedence = true;
				}

				else if(0 == stricmp(opt, "no-warn"))
				{
					ctx->m_DisableWarnings = true;
				}

				else
				{
					fatal(NULL, "Invalid option '%s'", argv[count]);
				}
			}
	
			else if(0 == stricmp(opt, "6809"))
			{
				ctx->m_CPUType = CPU_6809;
			}

			else if(0 == stricmp(opt, "s19"))
			{
				ctx->m_OutputType = S19FILE;
			}

			else if(0 == stricmp(opt, "bin"))
			{
				ctx->m_OutputType = MOTBIN;
			}

			else if(0 == stricmp(opt, "os9"))
			{
				ctx->m_OutputType = OS9BIN;
			}

			else if(0 == strnicmp(opt, "rom", 3))
			{
				if(0 != opt[3])
				{
					opt += 3;
					if('=' == *opt)
					{
						opt++;
						outputROMSize = atoi(opt) * 1024;
						if(outputROMSize < 2048)
						{
							fatal(NULL, "Invalid ROM size. Valid sizes are 2, 4, 8, 16, etc.");
						}
					}
					else
					{
						fatal(NULL, "Invalid option '%s'\n", argv[count]);
					}
				}
				ctx->m_OutputType = ROMBIN;
			}

			else if(0 == stricmp(opt, "rof"))
			{
				ctx->m_OutputType = ROFBIN;
			}

			else if(0 == stricmp(opt, "mod"))
			{
				ctx->m_OutputType = MODBIN;
			}

			else if(0 == stricmp(opt, "raw"))
			{
				ctx->m_OutputType = RAWBIN;
			}

			else if(0 == stricmp(opt, "obj"))
			{
				ctx->m_OutputType = OBJBIN;
			}

			else if(0 == stricmp(opt, "list"))
			{
				ctx->m_ListingFlags.m_CmdEnabled = true;
				ctx->m_ListingFlags.m_OptEnabled = true;

			}

			else if(0 == stricmp(opt, "cycle"))
			{
				ctx->m_Misc.Cflag = true;
			}

			else if(0 == stricmp(opt, "slist"))
			{
				ctx->m_Misc.Sflag = true;
			}

			else if(0 == stricmp(opt, "xref"))
			{
				ctx->m_ListingFlags.m_ListSymbols = true;
			}

			else if(0 == stricmp(opt, "noout"))
			{
				ctx->m_Misc.Oflag = false;
			}

			else if(0 == stricmp(opt, "expand"))
			{
				ctx->m_ListingFlags.m_ExpandMacros = true;
			}

			else if(0 == stricmp(opt, "opt"))
			{
				ctx->m_Misc.OptFlag = true;
			}

			else if(0 == stricmp(opt, "alert"))
			{
				ctx->m_Misc.OptShow = true;
			}

			else if(0 == stricmp(opt, "noln"))
			{
				ctx->m_ListingFlags.OptNoLineNumbers = true;
			}

			else if(0 == stricmp(opt, "noopdata"))
			{
				ctx->m_ListingFlags.OptNoOpData = true;
			}

			else if(0 == stricmp(opt, "cm"))
			{
				ctx->m_ListingFlags.OptCommentMacros = true;
			}

			else if(0 == stricmp(opt, "silent"))
			{
				ctx->m_SilentMode = true;
			}


			else
			{
				switch(*opt)
				{
				case '?':
					help();
					break;

				case 'I':	/* Add include directory */
					if(opt[1] == '=')
					{
						includeDirectories[includeCount++] = &opt[2];
					}
					else
					{
						includeDirectories[includeCount++] = &opt[2];
					}
					break;
					break;

				case 'D':	/* Add #define */
					ptr = symname;
					opt++;

					while(*opt != '=' && *opt != 0)
					{
						*(ptr++) = *(opt++);
					}

					if(*opt == '=')
					{
						symval = atoi(opt+1);
					}

					else
					{
						symval = 0;
					}

					*ptr = 0;
					/*	CLS-FIXED: Changed NULL to ctx as symbol management requires an active context. */
					BeginSegment(ctx, SEGMENT_DATA);
					AddSymbol(ctx, symname, symval, SYM_VALUE, NULL, 0);
					EndSegment(ctx);
					break;

				case 'C':	/* Case sensative labels */
					break;

				case 'O':	/* Set output directory */
					if(opt[1] == '=')
					{
						outputDirectory = &opt[2];
					}

					else
					{
						outputDirectory = &opt[1];
					}
					break;

				case 'o':
					if(opt[1] == '=')
					{
						outputFilename = &opt[2];
					}

					else
					{
						outputFilename = &opt[1];
					}
					break;

				default:
					flag = true;
					break;
				}

				/* check for misc and help options */
				if(flag)
				{
					banner();
					fprintf(stderr, "[%s] ", argv[count]);
					fprintf(stderr, "Invalid option.  Use -help or -? for a list of options\n\n");
					exit(CRITICAL_USER);
				}
			}

		}
		else
		{
			filelist[asmFileCount++] = argv[count];
		}
	}

	if(asmFileCount == 0)
	{
		Usage();
		exit(ERR_GENERAL);
	}

	if(MODBIN == ctx->m_OutputType)
	{
		InitExports(ctx);
	}

	switch(ctx->m_Compat.m_AsmMode)
	{
	case ASM_MODE_KASM:
		break;

	case ASM_MODE_EDTASM:
		ctx->m_CPUType = CPU_6809;
		ctx->m_Compat.m_AsmOpMask = ASM_EDTASM;
		ctx->m_Compat.m_DisablePCIndex = true;
		break;

	case ASM_MODE_EDTASM6309:
		ctx->m_Compat.m_AsmOpMask = ASM_EDTASMX;
		ctx->m_Compat.m_DisablePCIndex = true;
		ctx->m_Compat.m_SemicolonComment = false;
		break;

	case ASM_MODE_RMA:
		ctx->m_Compat.m_AsmOpMask = ASM_RMA;
		ctx->m_Compat.m_SemicolonComment = false;
		break;

	case ASM_MODE_CCASM:
		ctx->m_Compat.m_AsmOpMask = ASM_CCASM;
		ctx->m_Compat.m_ForceZeroOffset = true;
		ctx->m_Compat.m_ForcePCR = true;
		ctx->m_Compat.m_SemicolonComment = false;
		break;

	case ASM_MODE_MACRO80C:
		ctx->m_Compat.m_AsmOpMask = ASM_MACRO80;
		ctx->m_Compat.m_StrictLocals = true;
		ctx->m_Compat.m_UsePrecedence = true;
		ctx->m_Compat.m_SemicolonComment = false;
		break;
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
