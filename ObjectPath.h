#ifndef OBJECT_PATH_H
#define OBJECT_PATH_H

#include <string>
#include <vector>

#include "Expression.h"
#include "PathPart.h"

class ObjectPath {
  std::string full_path;
  std::vector<PathPart*> object_path = std::vector<PathPart*>();
  unsigned int current_index = 0;

  public:
    ObjectPath(){};

    void push_next_object(std::string object_name, std::vector<std::string> parameter_names);
    PathPart* pop_next_object();
    PathPart* current_object();

    void print();
    void reset() {current_index = 0; };
    /*
     * Return true if this path originates from the root ie. is absolute.
     */
    bool is_root_path() {return !object_path.empty() && object_path[0]->get_path_name() == "ROOT";};

    std::string to_string() { return full_path; }

    int size() { return object_path.size(); };
    PathPart* get_path_part(int index);
};

#endif
