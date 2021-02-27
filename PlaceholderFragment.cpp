#include "PlaceholderFragment.h"

#include <iostream>
#include "Object.h"

PlaceholderFragment::PlaceholderFragment( std::string* str ){
  path = ObjectPath(*str);
}

void PlaceholderFragment::print(){
  std::cout << "pf: ";
  path.print();
}

std::string PlaceholderFragment::render(Object* root) {
  path.reset(); // TODO: this should probably not be shared...
  std::string next_object = path.pop_next_object();

  if (next_object == "ROOT") {
      return root->render(root, path);
  }
  // TODO:
  std::cout << "TODO: ERROR!!!! not implemented yet!" << std::endl;
  return path.to_string();
}
