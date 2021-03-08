#include "ObjectInstance.h"

#include <iostream>
#include "Object.h"
#include "ObjectPath.h"

ObjectInstance::ObjectInstance(Object* obj) {
  if(!obj) {
    std::cerr << "Object instance cannot be created without an object" << std::endl;
    exit(1);
  }
  object = obj;
}

std::string ObjectInstance::render(Object* root
                                  ,ObjectPath path
                                  ,std::vector<ObjectInstance*> parameters
                                  ,std::string variable_name
                                  ) {

  std::string next_subobject = path.pop_next_object();

  // If this is the final object in the path, it should be evaluated:
  if (next_subobject == "") {
    std::string evaluation_key = get_evaluation_key(parameters);
    std::map<std::string, std::string>::iterator evaluation_it = evaluations.find(evaluation_key);

    if(evaluation_it != evaluations.end()) {
      return evaluation_it->second;
    }

    std::string result = "";
    move_parameters_to_namedObjects(parameters);

    // Render each fragment:
    std::vector<Fragment*> fragments = object->get_fragments();
    for (auto elem : fragments) {
      ObjectInstance* instance;
      variable_name = elem->get_variableName();

      if ( variable_name == "") {
        instance = new ObjectInstance(object);
      } else {
        instance = get_named_objectinst(variable_name);
      }
      std::vector<ObjectInstance*> parameters = create_parameter_list(elem->get_parameters());

      result += elem->render(root, instance, parameters);
      // TODO: delete instance if it is not named.
    }

    evaluations.emplace(evaluation_key, result);
    return result;
  }

  // Otherwise we recurse through the path:
  inst_subobjects_map_iterator existing_instance = subObjects.find(next_subobject);

  // If we didn't find the object, instantiate a new:
  if (existing_instance == subObjects.end() ) {
    ObjectInstance* next_object_instance;

    if (next_subobject == "ROOT") {
      next_object_instance = new ObjectInstance(root);
    } else {
      Object* next_object = object->get_random_subobject(next_subobject);
      next_object_instance = new ObjectInstance(next_object);
    }
    subObjects.insert(std::pair<std::string, ObjectInstance*>(next_subobject, next_object_instance));

    return next_object_instance->render(root, path, parameters);
  }

  // Otherwise return the render of the subobject
  return existing_instance->second->render(root, path, parameters);

}

ObjectInstance* ObjectInstance::get_named_objectinst(std::string name) {

  ObjectInstance* result;
  inst_subobjects_map_iterator named_instance = namedObjects.find(name);

  if (named_instance != namedObjects.end()) {
    result = named_instance->second;
  } else {
    result = new ObjectInstance(object);
    namedObjects.insert(std::pair<std::string, ObjectInstance*>(name, result));
  }

  return result;
}

void ObjectInstance::move_parameters_to_namedObjects(std::vector<ObjectInstance*> parameters) {

  // Check that we got the expected number of parameters:
  std::vector<std::string> param_names = object->get_parameters();
  if (parameters.size() != param_names.size()) {
    std::cerr << "Parameter list of wrong size in " << object->objName << std::endl;
    exit(1);
  }

  // Move the passed parameters to the namedObjects map, so that they are used as variables:
  for (unsigned int i=0; i<parameters.size(); i++) {
    // If the variable name already exist, it must be from a previous
    // evaluation of this object.  It may be different, so we replace it
    // with the new variable.  This allow using different parameters, ie if
    // we call OBJ(X,Y) and later OBJ(Y,X) from the same object, the
    // results would be the same, since the variable names in the final
    // object would already be defined.
    inst_subobjects_map_iterator it = namedObjects.find(param_names[i]);
    if (it != namedObjects.end()) namedObjects.erase(it);
    namedObjects.insert(std::pair<std::string, ObjectInstance*>(param_names[i], parameters[i]));
  }
}

std::vector<ObjectInstance*> ObjectInstance::create_parameter_list(std::vector<std::string> parameter_names) {

  std::vector<ObjectInstance*> result(parameter_names.size());
  for (unsigned int i=0; i<parameter_names.size(); i++) {
    result[i] = get_named_objectinst(parameter_names[i]);
  }

  return result;

}

std::string ObjectInstance::get_evaluation_key(std::vector<ObjectInstance*> parameters) {
  std::string key = "";
  for (ObjectInstance* inst : parameters) {
    key += std::to_string((long int)inst);
    key += "-";
  }

  return key;
}
