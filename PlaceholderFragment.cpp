#include "PlaceholderFragment.h"

#include <iostream>
#include "Object.h"
#include "ObjectPath.h"
#include "ObjectInstance.h"

PlaceholderFragment::PlaceholderFragment(ObjectPath* object_path
                                        ,std::string* varName
                                        ){
  if (object_path) path = *object_path;
  if (varName) variableName = *varName;
}

void PlaceholderFragment::print(){
  std::cout << "pf: ";
  path.print();
}

std::string PlaceholderFragment::render(Object* root
                                       ,ObjectInstance* current_object
                                       ,std::map<PathPart*, std::vector<ObjectInstance*>> parameters
                                       ) {
  path.reset(); // TODO: this should probably not be shared...

  return current_object->render(root, path, parameters, variableName);
}

std::map<PathPart*, std::vector<std::string>> PlaceholderFragment::get_parameters() {

  std::map<PathPart*, std::vector<std::string>> parameter_map = std::map<PathPart*, std::vector<std::string>>();

  for( int i=0; i<path.size(); i++ ) {
    PathPart* path_part = path.get_path_part(i);
    std::vector<std::string> param_names = path_part->get_parameter_names();
    parameter_map.insert(std::pair<PathPart*, std::vector<std::string>>(path_part, param_names));
  }

  return parameter_map;
}
