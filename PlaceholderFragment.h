#ifndef PLACEHOLDERFRAGMENT_H
#define PLACEHOLDERFRAGMENT_H

#include "ObjectPath.h"
#include "types.h"

class ObjectInstance;

class PlaceholderFragment: public Fragment {
  ObjectPath path;
  std::string variableName;
  std::vector<std::string> parameters;

  public:
    PlaceholderFragment( std::string* str
                        ,std::vector<std::string>* params = NULL
                        ,std::string* varName = NULL
                         );
    void print() override;
    std::string render(Object* root
                      ,ObjectInstance* current_object
                      ,std::vector<ObjectInstance*> parameters
                      ) override;
    std::string get_variableName() override {return variableName;};
    std::vector<std::string> get_parameters() override {return parameters;};
};

#endif
