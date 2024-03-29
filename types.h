#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <string>
#include <map>

class Object;
class ObjectPath;
class ObjectInstance;
class Expression;
class PathPart;

typedef std::map<std::string, std::vector<Object*>> subobjects_map;
typedef std::map<std::string, std::vector<Object*>>::iterator subobjects_map_iterator;

typedef std::map<std::string, ObjectInstance*> inst_subobjects_map;
typedef std::map<std::string, ObjectInstance*>::iterator inst_subobjects_map_iterator;

class Fragment {
  public:
    virtual void print() {};
    /*
     * Render the object to a string replacing placeholders with text.
     * args are
     * Object* root: the root which includes all other objects.
     * Object* current_object: The object that is currently being rendered.
     */
    virtual std::string render(Object*, ObjectInstance*, std::map<PathPart*, std::vector<ObjectInstance*>>) {return "";};
    virtual std::string get_variableName() {return "";};
    virtual std::map<PathPart*, std::vector<std::string>> get_parameters() {return std::map<PathPart*, std::vector<std::string>>();};
};

#endif
