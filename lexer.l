%{

#include <iostream>
#include "types.h"
#include "y.tab.h"

%}

%%

"<"                         { return LAB;}
"</"                        { return ELAB;}
">"                         { return RAB;}
"["                         { return LSB;}
"]"                         { return RSB;}
" "+"//"[^\n]*\n            /* Ignore comments: This is a full line comment, which is removed including the newline. */
"//"\n                      /* Ignore comments: Ending the line with '//' removes the newline. */
"//"[^\n]*                  /* Ignore comments: A comment at the end of a line does not remove the newline */

[a-zA-Z0-9\.]+              { yylval.ObjName = new std::string(yytext);
                              return OBJNAME; }
[a-zA-Z0-9][a-zA-Z0-9" "]*  { yylval.ObjName = new std::string(yytext);
                              return TEXT; }

%%

int yywrap(void){
  return 1;
}
