#include "ObjectInstance.h"

#include <iostream>
#include "Object.h"
#include "ObjectPath.h"

ObjectInstance::ObjectInstance(Object* obj, ObjectInstance* rootInstance) {
  if(!obj) {
    std::cerr << "Object instance cannot be created without an object" << std::endl;
    exit(1);
  }
  object = obj;
  this->rootInstance = rootInstance ? rootInstance : this;
  //this.includedFrom = includedFrom;
}

std::string ObjectInstance::render(Object* root
                                  ,ObjectPath path
                                  ,std::map<PathPart*, std::vector<ObjectInstance*>> parameters
                                  ,std::string variable_name
                                  ) {

  std::cout << "rendering " << root->objName << ": " << root << "\n";
  path.print();

  PathPart* current_path = path.current_object();
  PathPart* next_subobject = path.pop_next_object();

  // If this is the final object in the path, it should be evaluated:
  if (next_subobject == NULL) {
    std::string evaluation_key = get_evaluation_key(parameters[current_path]);
    std::map<std::string, std::string>::iterator evaluation_it = evaluations.find(evaluation_key);

    if(evaluation_it != evaluations.end()) {
      return evaluation_it->second;
    }

    std::string result = "";
    move_parameters_to_namedObjects(parameters[current_path], object->get_parameters());

    // Render each fragment:
    std::vector<Fragment*> fragments = object->get_fragments();
    for (auto elem : fragments) {
      ObjectInstance* instance;
      variable_name = elem->get_variableName();

      if ( variable_name == "") {
        instance = new ObjectInstance(object, rootInstance);
      } else {
        instance = get_named_objectinst(variable_name);
      }
      std::map<PathPart*, std::vector<ObjectInstance*>> parameters = create_parameter_list(elem->get_parameters());

      result += elem->render(root, instance, parameters);
      // TODO: delete instance if it is not named.
    }

    evaluations.emplace(evaluation_key, result);
    return result;
  }

  // Otherwise we recurse through the path:
  inst_subobjects_map_iterator existing_instance = subObjects.find(next_subobject->get_path_name());

  // If we didn't find the object that we are looking for according to the
  // path, instantiate a new to save the instance:
  if (existing_instance == subObjects.end() ) {
    ObjectInstance* next_object_instance;

    if (next_subobject->get_path_name() == "ROOT") {
      next_object_instance = new ObjectInstance(root, rootInstance);
    } else {
      std::vector<std::string> next_obj_params = object->get_next_subobject_params(next_subobject->get_path_name());
      move_parameters_to_namedObjects(parameters[next_subobject], next_obj_params);
      Object* next_object = object->get_random_subobject(next_subobject->get_path_name(), namedObjects);
      next_object_instance = new ObjectInstance(next_object, rootInstance);
    }
    subObjects.insert(std::pair<std::string, ObjectInstance*>(next_subobject->get_path_name(), next_object_instance));

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
    result = new ObjectInstance(object, rootInstance);
    namedObjects.insert(std::pair<std::string, ObjectInstance*>(name, result));
  }

  return result;
}

void ObjectInstance::move_parameters_to_namedObjects(std::vector<ObjectInstance*> parameters, std::vector<std::string> param_names) {

  // Check that we got the expected number of parameters:
  if (parameters.size() != param_names.size()) {
    std::cerr << "Parameter list of wrong size in " << object->objName << std::endl;
    std::cerr << "Expected parameters: ";
    for( std::string p : param_names ) {
      std::cerr << p << ", ";
    }
    std::cerr << std::endl << "Received parameters: ";
    for( ObjectInstance* o : parameters ) {
      std::cerr << o->get_object()->obj_name() << ", ";
    }
    std::cerr << std::endl;
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

std::map<PathPart*, std::vector<ObjectInstance*>> ObjectInstance::create_parameter_list(std::map<PathPart*, std::vector<std::string>> parameter_names) {
  std::map<PathPart*, std::vector<ObjectInstance*>> result;

  std::map<PathPart*, std::vector<std::string>>::iterator it;
  for( it = parameter_names.begin(); it != parameter_names.end(); it++) {
    std::vector<ObjectInstance*> individual_result = std::vector<ObjectInstance*>(it->second.size());
    for (unsigned int i=0; i<it->second.size(); i++) {
      individual_result[i] = get_named_objectinst(it->second[i]);
    }
    result.insert(std::pair<PathPart*, std::vector<ObjectInstance*>>(it->first, individual_result));
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

int ObjectInstance::get_variable(std::string var_name) {
  std::map<std::string, int>::iterator var_it = variables.find(var_name);

  if (var_it != variables.end()) {
    return var_it->second;
  }

  return 0;
}

void ObjectInstance::set_variable(std::string var_name, int value) {
  variables[var_name] = value;
  std::map<std::string, int>::iterator var_it = variables.find(var_name);

  if (var_it != variables.end()) {
    var_it->second = value;
  } else {
    variables.insert(std::pair<std::string, int>(var_name, value));
  }
}

ObjectInstance* ObjectInstance::getOrSetIncludedInstance(std::string includeName, Object* includedObject) {

  ObjectInstance* result = includedInstances[includeName];
  if (result) return result;

  result = new ObjectInstance(includedObject, NULL);
  includedInstances[includeName] = result;

  return result;
}
