%{

#include <iostream>
#include "types.h"
#include "y.tab.h"

int expression_context;

%}

%s SPLACEHOLDER STAG STEXT SEXPR SINC SINCNAME

%%

BEGIN STEXT;

"<@"                              { BEGIN SINCNAME; }
<SINC>"</@"[a-zA-Z_][a-zA-Z0-9_]*">" { BEGIN STEXT; std::cout << "to text\n"; return EINCLUDE;}
<SINC>[a-zA-Z0-9_/.][a-zA-Z0-9_/.]*     { std::cout << "include objname " << yytext << "\n";
                                    yyin = fopen( yytext, "r");
                                    if(!yyin) {
                                      std::cout << "invalid include: " << yytext << "\n";
                                      exit(0);
                                    }
                                    yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE));
                                    BEGIN STEXT;
                                  }
<SINCNAME>">"                     { BEGIN SINC; }
<SINCNAME>[a-zA-Z_][a-zA-Z0-9_]*     { std::cout << "including " << yytext << "\n"; yylval.ObjName = new std::string(yytext); return SINCLUDE; }
<<EOF>>                           {
                                    std::cout << "ending file\n";
                                    yypop_buffer_state();
                                    if(!YY_CURRENT_BUFFER) {
                                      yyterminate();
                                    }
                                    BEGIN SINC;
                                  }

<SEXPR>";"     { return SEMICOLON; }
<SEXPR>[0-9]+  { yylval.Int = std::stoi(yytext); return INT; }
<SEXPR>" "*"=="    { return EQ; }
<SEXPR>" "*"<="    { return LE; }
<SEXPR>" "*">="    { return GE; }
<SEXPR>" "*"!="    { return NE; }
<SEXPR>" "*"="     { return ASSIGN; }
<SEXPR>" "*"<"     { return LT; }
<SEXPR>" "*">"     { return GT; }
<SEXPR>" "*"+"     { return PLUS; }
<SEXPR>" "*"-"     { return MINUS; }
<SEXPR>" "*"*"     { return MULT; }
<SEXPR>" "*"/"     { return DIV; }
<SEXPR>" "*"&&"    { return AND; }
<SEXPR>" "*"||"    { return OR; }
<SEXPR>" "* /*ignore spaces in expressions*/

" "*<                          { BEGIN STAG;         return LAB;}
" "*"</"                       { BEGIN STAG;         return ELAB;}
">"                            { BEGIN STEXT;        return RAB;}
"["                            { BEGIN SPLACEHOLDER; return LSB;}
"]"                            { BEGIN STEXT;        return RSB;}
<SPLACEHOLDER,STAG>" "*"{"     { expression_context = YY_START; BEGIN SEXPR; return LCB;}
<SEXPR>" "*"}"                 { BEGIN expression_context; return RCB;}
"\n"                           { return NEWLN;}
<SPLACEHOLDER>":"              { return COLON;}
<SPLACEHOLDER,STAG,SEXPR>"."   { return DOT;}
<SPLACEHOLDER,STAG>" "*"("" "* { return LRB; }
<SPLACEHOLDER,STAG>" "*")"" "* { return RRB; }
<SPLACEHOLDER,STAG>" "*","" "* { return COMMA; }
" "+"//"[^\n]*\n            /* Ignore comments: This is a full line comment, which is removed including the newline. */
"//"\n                      /* Ignore comments: Ending the line with '//' removes the newline. */
"//"[^\n]*                  /* Ignore comments: A comment at the end of a line does not remove the newline */

<SPLACEHOLDER,STAG,SEXPR>[a-zA-Z_][a-zA-Z0-9_]* { yylval.ObjName = new std::string(yytext);
                                     return OBJNAME; }
<SPLACEHOLDER>@                  { std::cout << "found @\n"; return INCLUDE; }
<STEXT>[^<[\n]+                    { std::cout << "text: " << yytext << "\n"; yylval.ObjName = new std::string(yytext);
                                     return TEXT;}

%%

int yywrap(void){
  return 1;
}
