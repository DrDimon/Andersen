#ifndef PLACEHOLDERFRAGMENT_H
#define PLACEHOLDERFRAGMENT_H

#include "ObjectPath.h"
#include "types.h"

class ObjectInstance;

class PlaceholderFragment: public Fragment {
  ObjectPath path;
  std::string variableName;

  public:
    PlaceholderFragment( std::string* str, std::string* varName = NULL );
    void print() override;
    std::string render(Object* root, ObjectInstance* current_object) override;
    std::string get_variableName() override {return variableName;};
};

#endif
