#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <vector>
#include <iostream>

#include "types.h"

class ObjectPath;
class Fragment;

class Object {

  public:
    std::string objName;
    std::vector<Fragment*> fragments;
    std::vector<std::string> parameters;
    subobjects_map subObjects;
    std::vector<Expression*> expressions;
    unsigned int counter = 0;

  std::vector<Fragment*> get_fragments() {
    return fragments;
  }

  /*
   * Print the object and fragments recursively without resolving placeholders.
   * This is mostly for debugging purposes.
   */
  void print();

  void print_expressions(inst_subobjects_map namedObjects);

  /*
   * Return a random subobject with the given name. It should be the actual
   * object name, not a path that includes '.'.
   */
  Object* get_random_subobject(std::string name, inst_subobjects_map namedObjects);
  std::vector<std::string> get_next_subobject_params(std::string name);

  std::vector<std::string> get_parameters() {return parameters;};
  std::string obj_name() {return objName; };

  bool validate_expressions(inst_subobjects_map namedObjects);
  void execute_expressions(inst_subobjects_map namedObjects);
  void increment_count() { counter++; }
  unsigned int get_count() const { return counter; }
};

#endif
