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
                   );

  subobjects_map* addObject(Object* obj, subobjects_map* previous);

  Object* root = NULL;
%}

%union {
  Object* object;
  std::string* ObjName;
  std::string* Text;
  subobjects_map* subObjects;
  std::vector<Fragment*>* fragments;
}

%token ELAB LAB RAB LSB RSB
%token <ObjName> OBJNAME
%token <Text> TEXT

%start program

%type<subObjects> objects
%type<object> object
%type<fragments> fragments

%%

program:
  objects { root = new Object();
            root->objName = std::string("ROOT");
            root->subObjects = *$1;}

objects:
  object          {$$ = addObject($1, new subobjects_map());}
| object objects  {$$ = addObject($1, $2);}
  ;

object:
  LAB OBJNAME RAB fragments ELAB OBJNAME RAB {$$ = newObject($2, $4, NULL);}
| LAB OBJNAME RAB fragments objects ELAB OBJNAME RAB {$$ = newObject($2, $4, $5);}
  ;


fragments:
   /* empty */              {$$ = new std::vector<Fragment*>;}
| fragments TEXT            {$1->push_back(new TextFragment($2)); $$ = $1;}
// If TEXT doesn't contain a character that is illegal in OBJNAME, it will be
// parsed as an OBJNAME, which should be allowed:
| fragments OBJNAME         {$1->push_back(new TextFragment($2)); $$ = $1;}
| fragments LSB OBJNAME RSB {$1->push_back(new PlaceholderFragment($3)); $$ = $1;}
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
                 ){

  // Create object
  Object* obj = new Object();
  if (objName)    obj->objName = *objName;
  if (fragments)  obj->fragments = *fragments;
  if (subObjects) obj->subObjects = *subObjects;
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
