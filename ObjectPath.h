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

    std::string to_string() { return full_path; }
};

#endif
