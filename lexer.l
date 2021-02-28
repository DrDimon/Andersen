%{

#include <iostream>
#include "types.h"
#include "y.tab.h"

%}

%s SPLACEHOLDER STAG

%%


" "*<                       { BEGIN STAG;         return LAB;}
" "*"</"                    { BEGIN STAG;         return ELAB;}
">"                         { BEGIN INITIAL;      return RAB;}
"["                         { BEGIN SPLACEHOLDER; return LSB;}
"]"                         { BEGIN INITIAL;      return RSB;}
"\n"                        { return NEWLN;}
" "+"//"[^\n]*\n            /* Ignore comments: This is a full line comment, which is removed including the newline. */
"//"\n                      /* Ignore comments: Ending the line with '//' removes the newline. */
"//"[^\n]*                  /* Ignore comments: A comment at the end of a line does not remove the newline */

<STAG>[a-zA-Z0-9]+           { yylval.ObjName = new std::string(yytext);
                              return OBJNAME; }
<SPLACEHOLDER>[a-zA-Z0-9\.]+ { yylval.Placeholder = new std::string(yytext);
                              return PLACEHOLDER; }
[a-zA-Z0-9" ",\-\.]+         { yylval.ObjName = new std::string(yytext);
                              return TEXT; }

%%

int yywrap(void){
  return 1;
}
