#include "ObjectPath.h"

#include <iostream>
#include <sstream>

void ObjectPath::push_next_object(std::string object_name, std::vector<std::string> parameter_names) {
  full_path = full_path + (full_path == "" ? "" : ".") + object_name;
  PathPart* new_part = new PathPart(object_name, parameter_names);
  object_path.push_back(new_part);
}

PathPart* ObjectPath::pop_next_object() {
  current_index++;
  return get_path_part(current_index-1);
}

PathPart* ObjectPath::current_object() {
  if( current_index > object_path.size() || current_index == 0){
      return NULL;
  }

  return get_path_part(current_index-1);
}
void ObjectPath::print() {
  std::cout << "full path: " << full_path << std::endl;
  PathPart* pp = get_path_part(current_index);
  std::cout << "index: " << current_index <<  ": " << (pp ? pp->get_path_name() : "<NONE>") << std::endl;
}

PathPart* ObjectPath::get_path_part(int index) {
  if (index >= (int)object_path.size()) return NULL;
  return object_path[index];
};
