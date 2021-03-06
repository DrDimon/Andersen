#ifndef OBJECTPATH_H
#define OBJECTPATH_H

#include <string>
#include <vector>

#include "types.h"

class ObjectPath;
class Fragment;

class Object {

  public:
    std::string objName;
    std::vector<Fragment*> fragments;
    subobjects_map subObjects;

  std::vector<Fragment*> get_fragments() {
    return fragments;
  }

  /*
   * Print the object and fragments recursively without resolving placeholders.
   * This is mostly for debugging purposes.
   */
  void print();

  /*
   * Return a random subobject with the given name. It should be the actual
   * object name, not a path that includes '.'.
   */
  Object* get_random_subobject(std::string name);

};

#endif
