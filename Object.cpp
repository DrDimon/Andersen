#include "Object.h"

#include <iostream>
#include "ObjectPath.h"
#include "Expression.h"

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

void Object::print_expressions(inst_subobjects_map namedObjects) {
  for(auto expression : expressions) {
    std::cout << expression->to_string(namedObjects);
  }
  std::cout << "\n";
}

std::vector<std::string> Object::get_next_subobject_params(std::string name) {

  std::vector<Object*> sub_objects = subObjects[name];

  // If we didn't find the object, return an error:
  if (sub_objects.size() == 0) {
    std::cerr << "Object not found: " << name << std::endl;
    exit(1);
  }

  return sub_objects[0]->get_parameters();
}

Object* Object::get_random_subobject(std::string name, inst_subobjects_map namedObjects) {

  subobjects_map_iterator possible_objects = subObjects.find(name);

  // If we didn't find the object, return an error:
  if (possible_objects == subObjects.end() || possible_objects->second.empty()) {
    std::cerr << "Object not found: " << name << std::endl;
    exit(1);
  }

  std::vector<Object*> valid_objects = std::vector<Object*>(possible_objects->second.size());
  int num_added = 0;
  for(auto obj : possible_objects->second) {
    if(obj->validate_expressions(namedObjects)) {
      valid_objects[num_added] = obj;
      num_added++;
    }
  }

  // If we didn't add any objects, we return an error:
  if(!num_added){
    std::cerr << "All objects are invalid due to expressions: " << name << std::endl;
    for(auto obj : possible_objects->second) {
      obj->print_expressions(namedObjects);
    }
    exit(1);
  }

  unsigned int index = std::rand() % num_added;

  // First we execute any assignments in the choosen object:
  valid_objects[index]->execute_expressions(namedObjects);

  // Then return it:
  return valid_objects[index];
}

bool Object::validate_expressions(inst_subobjects_map namedObjects) {
  for (Expression* expression : expressions){
    if(!expression->eval_bool(this, namedObjects)) return false;
  }
  return true;
}

void Object::execute_expressions(inst_subobjects_map namedObjects) {
  for (Expression* expression : expressions) {
    expression->execute(this, namedObjects);
  }
}
