#ifndef PLACEHOLDERFRAGMENT_H
#define PLACEHOLDERFRAGMENT_H

#include "ObjectPath.h"
#include "types.h"

class PlaceholderFragment: public Fragment {
  ObjectPath path;

  public:
    PlaceholderFragment( std::string* str );
    void print() override;
    std::string render(Object* root) override;
};

#endif
