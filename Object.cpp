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

std::string Object::render(Object* root, ObjectPath path){

  std::string next_subobject = path.pop_next_object();

  if (next_subobject == "") {
    std::string result;
    for (auto elem : fragments) {
      result += elem->render(root, this);
    }
    return result;
  }

  subobjects_map_iterator possible_objects = subObjects.find(next_subobject);

  // If we didn't find the object, return an error:
  if (possible_objects == subObjects.end() || possible_objects->second.empty()) {
    std::cerr << "Object not found:" << std::endl;
    path.print();
    return "[ERROR]"; // TODO
  }

  // Otherwise return the render of a random subobject:
  std::vector<Object*> valid_objects = possible_objects->second;
  unsigned int index = std::rand() % valid_objects.size();
  Object* next_object = valid_objects[index];
  return next_object->render(root, path);

}
