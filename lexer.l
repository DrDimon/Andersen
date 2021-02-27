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

[a-zA-Z0-9\.]+              { yylval.ObjName = new std::string(yytext);
                              return OBJNAME; }
[a-zA-Z0-9][a-zA-Z0-9" "]*  { yylval.ObjName = new std::string(yytext);
                              return TEXT; }

%%

int yywrap(void){
  return 1;
}
