#include "ObjectPath.h"

#include <iostream>
#include <sstream>

ObjectPath::ObjectPath(std::string path){
  full_path = path;
  std::string next;
  std::stringstream path_stream = std::stringstream(path);
  while(std::getline(path_stream, next, '.')) {
    object_path.push_back(next);
  }
}

std::string ObjectPath::pop_next_object() {
  if( current_index == object_path.size() ){
      return "";
  }

  current_index++;
  return object_path[current_index-1];
}

void ObjectPath::print() {
  std::cout << "full path: " << full_path << std::endl;
  std::cout << "index: " << current_index << ": " << object_path[current_index] << std::endl;
}
