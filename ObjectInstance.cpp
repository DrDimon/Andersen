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

std::string ObjectInstance::render(Object* root, ObjectPath path, std::string variable_name) {

  std::string next_subobject = path.pop_next_object();

  // If this is the final object in the path, it should be evaluated:
  if (next_subobject == "") {
    if(!is_evaluated) {
      std::vector<Fragment*> fragments = object->get_fragments();
      for (auto elem : fragments) {
        ObjectInstance* instance;
        variable_name = elem->get_variableName();

        if ( variable_name == "") {
          instance = new ObjectInstance(object);
        } else {
          instance = get_named_objectinst(variable_name);
        }

        value += elem->render(root, instance);
        // TODO: delete instance if it is not named.
      }
      is_evaluated = true;
    }

    return value;
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

    return next_object_instance->render(root, path);
  }

  // Otherwise return the render of the subobject
  return existing_instance->second->render(root, path);

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
