#ifndef TEXTFRAGMENT_H
#define TEXTFRAGMENT_H

#include <string>
#include "types.h"

class TextFragment: public Fragment {
  std::string text;

  public:
    TextFragment( std::string* str );
    void print() override;
    std::string render(Object*, ObjectInstance*, std::vector<ObjectInstance*>) override;
};

#endif
