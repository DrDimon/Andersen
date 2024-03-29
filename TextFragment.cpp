#include "TextFragment.h"

#include <iostream>

TextFragment::TextFragment( std::string* str ){
  text = std::string(*str);
}

void TextFragment::print() {
  std::cout << "tf: " << text << std::endl;
}

std::string TextFragment::render(Object*, ObjectInstance*, std::map<PathPart*, std::vector<ObjectInstance*>>) {
  return text;
}
