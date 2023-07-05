%{
  #include <stdio.h>
  #include <algorithm>

  #include "Object.h"
  #include "ObjectPath.h"
  #include "PlaceholderFragment.h"
  #include "TextFragment.h"
  #include "Expression.h"
  #include "PathPart.h"
  #include "interpreter.cpp"
  #include "types.h"

  int yylex(void);
  void yyerror(char const *);
  Object* newObject(std::string* objName
                   ,std::vector<Fragment*>* fragments
                   ,subobjects_map* subObjects
                   ,std::vector<std::string>* parameters
                   ,std::vector<Expression*>* expressions
                   );

  subobjects_map* addObject(Object* obj, subobjects_map* previous);

  Object* root = NULL;
  std::string* newline_str = new std::string("\n");
%}

%union {
  Object* object;
  std::string* ObjName;
  std::string* Placeholder;
  std::string* Text;
  std::vector<std::string>* ParameterList;
  subobjects_map* subObjects;
  std::vector<Fragment*>* fragments;
  Fragment* fragment;
  std::vector<Expression*>* expressions;
  ObjectPath* object_path;
  Expression* expression;
  int Int;
}

%token SEMICOLON
%token ASSIGN
%left PLUS MINUS
%left MULT DIV
%left OR
%left AND
%left EQ LE GE NE LT GT
%token <Int> INT
%token LCB RCB ELAB LAB RAB LSB RSB LRB RRB COMMA NEWLN COLON DOT
%token <ObjName> OBJNAME
%token FUNC
%token <Placeholder> PLACEHOLDER
%token <Text> TEXT

%start program

%type<subObjects> objects
%type<object> object
%type<fragments> fragments
%type<fragments> single_line_fragments
%type<fragment> placeholder
%type<object_path> varname
%type<ParameterList> parameter_list
%type<ParameterList> parameters
%type<expression> expression
%type<expressions> expressions
%type<expressions> inner_expressions

%%

program:
  objects { root = new Object();
            root->objName = std::string("ROOT");
            root->subObjects = *$1;}

objects:
  object newlines          {$$ = addObject($1, new subobjects_map());}
| object newlines objects  {$$ = addObject($1, $3);}
  ;

/* Zero or any number */
newlines:
  /* empty */ {}
| NEWLN newlines {}
  ;

object:
  LAB OBJNAME parameter_list expressions RAB NEWLN fragments NEWLN ELAB OBJNAME RAB {$$ = newObject($2, $7, NULL, $3, $4);}
| LAB OBJNAME parameter_list expressions RAB NEWLN fragments NEWLN objects ELAB OBJNAME RAB {$$ = newObject($2, $7, $9, $3, $4);}
| LAB OBJNAME parameter_list expressions RAB single_line_fragments ELAB OBJNAME RAB {$$ = newObject($2, $6, NULL, $3, $4);}
| LAB OBJNAME parameter_list expressions RAB NEWLN objects ELAB OBJNAME RAB {$$ = newObject($2, NULL, $7, $3, $4);}
  ;

expressions:
  /* empty */                      { $$ = new std::vector<Expression*>;}
| LCB inner_expressions RCB        { $$ = $2; }
  ;

inner_expressions:
  /* empty */                            { $$ = new std::vector<Expression*>;}
| expression SEMICOLON inner_expressions { $3->push_back($1); $$ = $3;}
| OBJNAME DOT OBJNAME ASSIGN expression SEMICOLON inner_expressions { $7->push_back(new Assign(*$1, *$3, $5)); $$ = $7;}
  ;

expression:
  INT                        { $$ = new Int($1);}
| OBJNAME DOT OBJNAME        { $$ = new Variable(*$1, *$3);}
| FUNC DOT OBJNAME           { $$ = new Func(*$3);}
| expression PLUS expression { $$ = new Add($1, $3);}
| expression MINUS expression { $$ = new Sub($1, $3);}
| expression MULT expression  { $$ = new Mult($1, $3);}
| expression DIV expression   { $$ = new Div($1, $3);}
| expression EQ expression   { $$ = new Eq($1, $3);}
| expression LE expression   { $$ = new Le($1, $3);}
| expression GE expression   { $$ = new Ge($1, $3);}
| expression LT expression   { $$ = new Lt($1, $3);}
| expression GT expression   { $$ = new Gt($1, $3);}
| expression NE expression   { $$ = new Ne($1, $3);}
| expression AND expression  { $$ = new And($1, $3);}
| expression OR expression   { $$ = new Or($1, $3);}
  ;

parameter_list:
  /* empty */               {$$ = new std::vector<std::string>;}
| LRB parameters RRB        {$$ = $2;}
  ;

parameters:
  OBJNAME                   {$$ = new std::vector<std::string>; $$->push_back(*$1); }
| parameters COMMA OBJNAME  {$1->push_back(*$3); $$ = $1;}
  ;

fragments:
   /* empty */              {$$ = new std::vector<Fragment*>;}
| fragments NEWLN           {$1->push_back(new TextFragment(newline_str)); $$ = $1;}
| fragments TEXT            {$1->push_back(new TextFragment($2)); $$ = $1;}
| fragments placeholder     {$1->push_back($2); $$ = $1;}
  ;

single_line_fragments:
   /* empty */                      {$$ = new std::vector<Fragment*>;}
| single_line_fragments TEXT        {$1->push_back(new TextFragment($2)); $$ = $1;}
| single_line_fragments placeholder {$1->push_back($2); $$ = $1;}
  ;

placeholder:
  LSB varname RSB               {$$ = new PlaceholderFragment($2);}
| LSB OBJNAME COLON varname RSB {$$ = new PlaceholderFragment($4, $2);}
  ;

varname:
  OBJNAME parameter_list {$$ = new ObjectPath(); $$->push_next_object(*$1, *$2);}
| varname DOT OBJNAME parameter_list {$1->push_next_object(*$3, *$4); $$ = $1;}
  ;

%%

subobjects_map* addObject(Object* obj, subobjects_map* previous){
  subobjects_map_iterator existing = previous->find(obj->objName);

  // If this name doesn't already exsit, create it, and add the object:
  if (existing == previous->end()) {
    std::vector<Object*>* v = new std::vector<Object*>;
    v->push_back(obj);
    previous->insert(std::pair<std::string, std::vector<Object*>>(obj->objName, *v));

  // Otherwise just add the object to the list:
  } else {
    existing->second.push_back(obj);
  }
  return previous;
}

Object* newObject(std::string* objName
                 ,std::vector<Fragment*>* fragments
                 ,subobjects_map* subObjects
                 ,std::vector<std::string>* parameters
                 ,std::vector<Expression*>* expressions
                 ){

  // Create object
  Object* obj = new Object();
  if (objName)    obj->objName = *objName;
  if (fragments)  obj->fragments = *fragments;
  if (subObjects) obj->subObjects = *subObjects;
  if (parameters) obj->parameters = *parameters;
  if (expressions) obj->expressions = *expressions;
  return obj;
}

int main(int argc, char **argv){
  yyparse();
  int seed = argc > 1 ? std::stoi(argv[1]) : 0;
  interpStory(root, seed);
  return 0;
}

void yyerror(char const *s){
  fprintf(stderr, "Error: %s\n", s);
}
