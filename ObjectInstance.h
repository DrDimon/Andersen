#ifndef OBJECTINSTANCE_H
#define OBJECTINSTANCE_H

#include <string>
#include "types.h"

class Object;
class ObjectPath;

class ObjectInstance {
  Object* object;
  inst_subobjects_map subObjects;
  inst_subobjects_map namedObjects;
  std::map<std::string, std::string> evaluations;
  std::map<std::string, int> variables;

  public:
    ObjectInstance(Object* obj);
    std::string render(Object* root
                      ,ObjectPath path
                      ,std::map<PathPart*, std::vector<ObjectInstance*>> parameters
                      ,std::string variable_name = ""
                      );

    /*
     * Return the pointer to a named object instance with the given name.
     * If one does not already exist, it is created and returned.
     */
    ObjectInstance* get_named_objectinst(std::string name);

    int get_variable(std::string var_name);
    void set_variable(std::string var_name, int value);

    Object* get_object() { return object; };

  private:

    /*
     * Take a list of parameters, and insert them in the namedObjects map.
     * This way the parameters can be used as normal variables.
     *
     * If a ObjectInstance with a given name already exist, it is overwritten.
     */
    void move_parameters_to_namedObjects(std::vector<ObjectInstance*> parameters, std::vector<std::string> param_names);

    /*
     * Create a list of parameters for rendering a placeholder.
     *
     * The function takes a list of parameternames, and return a list of
     * ObjectInstance* for each name.
     *
     * The ObjectInstance may be a previously named variable (or parameter), or
     * it may be newly created to put in the list.
     *
     * These parameters are added to namedObjects, and are considered a
     * variable like any other.
     */
    std::map<PathPart*, std::vector<ObjectInstance*>> create_parameter_list(std::map<PathPart*, std::vector<std::string>> parameter_names);

    /*
     * Create a string that is considered a unique key for that particular
     * parameters.
     *
     * If there are no parameters, an empty string is returned.
     *
     * This is used as key in the evaluations map, to get a previous evaluation
     * of those parameters.
     */
    std::string get_evaluation_key(std::vector<ObjectInstance*> parameters);

};

#endif
