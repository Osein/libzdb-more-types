/*
 * Copyright (C) 2008 Tildeslash. All rights reserved.
 * Use is subject to license terms.
 */

%{
  /*
   * Use for filtering text from a C-header file. Filter text between
   * //<< and //>> or from //<< and to eof. '#' may be used instead of 
   * '//' if applicable.
   * 
   * Usage: filterh < file > filtered-file
   */
#include <stdio.h>
#include <string.h>
%}

author		^.*"@author".*"\n"
version		^.*"@version".*"\n"
%x CPP BPP

%%

{author}	;
{version}	;

"//<<"	        { BEGIN(CPP); }
"#<<"	        { BEGIN(BPP); }

<CPP>{
	"//>>".*"\n"    { BEGIN(INITIAL); }
	[\000-\377]     ;
}

<BPP>{
	"#>>".*"\n"     { BEGIN(INITIAL); }
	[\000-\377]     ;
}

<INITIAL>.      { fprintf(yyout, "%s", yytext); }

%%