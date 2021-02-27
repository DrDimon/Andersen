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

  /*
   * Print the object and fragments recursively without resolving placeholders.
   * This is mostly for debugging purposes.
   */
  void print();

  /*
   * Render the object to a string. This includes resolving placeholders.
   */
  std::string render(Object* root, ObjectPath path);

};

#endif
