#ifndef INCLUDEFRAGMENT_H
#define INCLUDEFRAGMENT_H

#include "ObjectPath.h"
#include "types.h"

class ObjectInstance;

class IncludeFragment: public Fragment {
  std::string includeName;
  ObjectPath path;
  std::string variableName;

  public:
    IncludeFragment( std::string* includeName = NULL
                     ,ObjectPath* object_path = NULL
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
