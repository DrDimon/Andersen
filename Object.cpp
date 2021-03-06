#include "Object.h"

#include <iostream>
#include "ObjectPath.h"

void Object::print() {
  std::cout << objName << std::endl;

  for (auto elem : fragments) {
    elem->print();
  }

  for (auto object_list : subObjects) {
    std::cout << "objects with name " << object_list.first << ":" << std::endl;
    for (auto object : object_list.second) {
      object->print();
    }
  }
}

Object* Object::get_random_subobject(std::string name) {

  subobjects_map_iterator possible_objects = subObjects.find(name);

  // If we didn't find the object, return an error:
  if (possible_objects == subObjects.end() || possible_objects->second.empty()) {
    std::cerr << "Object not found: " << name << std::endl;
    exit(1);
  }

  std::vector<Object*> valid_objects = possible_objects->second;
  unsigned int index = std::rand() % valid_objects.size();
  return valid_objects[index];
}
