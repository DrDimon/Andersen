#ifndef OBJECTINSTANCE_H
#define OBJECTINSTANCE_H

#include <string>
#include "types.h"

class Object;
class ObjectPath;

class ObjectInstance {
  Object* object;
  inst_subobjects_map subObjects;
  inst_subobjects_map namedObjects;
  bool is_evaluated = false;
  std::string value;

  public:
    ObjectInstance(Object* obj);
    std::string render(Object* root, ObjectPath path, std::string variable_name = "");

    /*
     * Return the pointer to a named object instance with the given name.
     * If one does not already exist, it is created and returned.
     */
    ObjectInstance* get_named_objectinst(std::string name);
};

#endif
