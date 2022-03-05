#include "IncludeFragment.h"

#include <iostream>
#include "Object.h"
#include "ObjectPath.h"
#include "ObjectInstance.h"

IncludeFragment::IncludeFragment( std::string* includeName
                                 ,ObjectPath* object_path
                                 ,std::string* varName
                                 ){
  // TODO error hvis null:
  this->includeName = *includeName;
  if (object_path) path = *object_path;
  if (varName) variableName = *varName;
}

void IncludeFragment::print(){
  std::cout << "if: " << includeName << ": " ;
  path.print();
}

std::string IncludeFragment::render(Object* root
                                   ,ObjectInstance* current_object
                                   ,std::map<PathPart*, std::vector<ObjectInstance*>> parameters
                                   ) {

  include_map_iterator includedObject = root->includedObjects.find(includeName);

  if (includedObject == root->includedObjects.end()) {
    std::cerr << "Included file was not found: " << includeName << std::endl;
    exit(-1);
  }

  ObjectInstance* newInstance = current_object->get_rootInstance()->getOrSetIncludedInstance( includeName, includedObject->second);

  return newInstance->render(includedObject->second, path, parameters);
}

// TODO hvad skal den her? lige nu er den bare kopieret.
std::map<PathPart*, std::vector<std::string>> IncludeFragment::get_parameters() {

  std::map<PathPart*, std::vector<std::string>> parameter_map = std::map<PathPart*, std::vector<std::string>>();

  for( int i=0; i<path.size(); i++ ) {
    PathPart* path_part = path.get_path_part(i);
    std::vector<std::string> param_names = path_part->get_parameter_names();
    parameter_map.insert(std::pair<PathPart*, std::vector<std::string>>(path_part, param_names));
  }

  return parameter_map;
}
