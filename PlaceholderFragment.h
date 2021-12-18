#ifndef PLACEHOLDERFRAGMENT_H
#define PLACEHOLDERFRAGMENT_H

#include "ObjectPath.h"
#include "types.h"

class ObjectInstance;

class PlaceholderFragment: public Fragment {
  ObjectPath path;
  std::string variableName;

  public:
    PlaceholderFragment( ObjectPath* object_path = NULL
                        ,std::string* varName = NULL
                         );
    void print() override;
    std::string render(Object* root
                      ,ObjectInstance* current_object
                      ,std::map<PathPart*, std::vector<ObjectInstance*>> parameters
                      ) override;
    std::string get_variableName() override {return variableName;};
    std::map<PathPart*, std::vector<std::string>> get_parameters() override;
};

#endif
