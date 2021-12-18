#ifndef PATH_PART_H
#define PATH_PART_H

#include <string>
#include <vector>

  class PathPart {
    private:
      std::string path_name;
      std::vector<std::string> parameter_names;
    public:
      PathPart(std::string path_name, std::vector<std::string> parameter_names);
      std::string get_path_name() {return path_name;};
      std::vector<std::string> get_parameter_names() {return parameter_names;};
  };

#endif
