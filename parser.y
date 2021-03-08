%{
  #include <stdio.h>
  #include <algorithm>

  #include "Object.h"
  #include "PlaceholderFragment.h"
  #include "TextFragment.h"
  #include "interpreter.cpp"
  #include "types.h"

  int yylex(void);
  void yyerror(char const *);
  Object* newObject(std::string* objName
                   ,std::vector<Fragment*>* fragments
                   ,subobjects_map* subObjects
                   ,std::vector<std::string>* parameters
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
}

%token ELAB LAB RAB LSB RSB LRB RRB COMMA NEWLN COLON DOT
%token <ObjName> OBJNAME
%token <Placeholder> PLACEHOLDER
%token <Text> TEXT

%start program

%type<subObjects> objects
%type<object> object
%type<fragments> fragments
%type<fragments> single_line_fragments
%type<fragment> placeholder
%type<ObjName> varname
%type<ParameterList> parameter_list
%type<ParameterList> parameters

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
  LAB OBJNAME parameter_list RAB NEWLN fragments NEWLN ELAB OBJNAME RAB {$$ = newObject($2, $6, NULL, $3);}
| LAB OBJNAME parameter_list RAB NEWLN fragments NEWLN objects ELAB OBJNAME RAB {$$ = newObject($2, $6, $8, $3);}
| LAB OBJNAME parameter_list RAB single_line_fragments ELAB OBJNAME RAB {$$ = newObject($2, $5, NULL, $3);}
| LAB OBJNAME parameter_list RAB NEWLN objects ELAB OBJNAME RAB {$$ = newObject($2, NULL, $6, $3);}
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
  LSB varname parameter_list RSB               {$$ = new PlaceholderFragment($2, $3);}
| LSB OBJNAME COLON varname parameter_list RSB {$$ = new PlaceholderFragment($4, $5, $2);}
  ;

varname:
  OBJNAME {$$ = $1;}
| OBJNAME DOT varname  {$1->append("."); $1->append(*$3); $$ = $1;}
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
                 ){

  // Create object
  Object* obj = new Object();
  if (objName)    obj->objName = *objName;
  if (fragments)  obj->fragments = *fragments;
  if (subObjects) obj->subObjects = *subObjects;
  if (parameters) obj->parameters = *parameters;
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
