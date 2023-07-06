#include "Expression.h"
#include "Object.h"

#include <iostream>

#include "ObjectInstance.h"

void Assign::execute(const Object* obj, inst_subobjects_map namedObjects) {

  ObjectInstance* obj_inst = namedObjects[objname];

  if( !obj_inst ) {
    std::cerr << "Object not found: " << objname << std::endl;
    exit(1);
  }

  int value = expression->eval_int(obj, namedObjects);
  obj_inst->set_variable(varname, value);

}

int Variable::eval_int(const Object* obj, inst_subobjects_map namedObjects) {

  // First, we find the object in the named object list.
  // This is the list of parameters given in the placeholder.
  inst_subobjects_map_iterator object = namedObjects.find(objname);

  // The object must exist:
  if( object == namedObjects.end() ){
    std::cerr << "Variable not found: " << objname << std::endl;
    exit(1);
  }

  // Return the integer variable if it exist, otherwise 0 is returned, which is
  // perfect:
  return object->second->get_variable(varname);
}

int Func::eval_int(const Object* obj, inst_subobjects_map namedObjects) {
  std::cout << obj->get_count() << "\n";
  return obj->get_count();
}