#include "PlaceholderFragment.h"

#include <iostream>
#include "Object.h"
#include "ObjectInstance.h"

PlaceholderFragment::PlaceholderFragment( std::string* str, std::string* varName ){
  path = ObjectPath(*str);
  if (varName) variableName = *varName;
}

void PlaceholderFragment::print(){
  std::cout << "pf: ";
  path.print();
}

std::string PlaceholderFragment::render(Object* root, ObjectInstance* current_object) {
  path.reset(); // TODO: this should probably not be shared...

  return current_object->render(root, path, variableName);
}
