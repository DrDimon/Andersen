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

std::string PlaceholderFragment::render(Object* root, Object* current_object) {
  path.reset(); // TODO: this should probably not be shared...

  if (path.is_root_path()) {
      std::string next_object = path.pop_next_object();
      return root->render(root, path);
  }

  return current_object->render(root, path);
}
