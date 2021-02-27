#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <vector>

class ObjectPath {
  std::string full_path;
  std::vector<std::string> object_path;
  unsigned int current_index = 0;

  public:
    ObjectPath(){};
    ObjectPath(std::string path);

    std::string pop_next_object();

    void print();
    void reset() {current_index = 0; };
    /*
     * Return true if this path originates from the root ie. is absolute.
     */
    bool is_root_path() {return !object_path.empty() && object_path[0] == "ROOT";};

    std::string to_string() { return full_path; }
};

#endif
