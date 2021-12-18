#include "PathPart.h"

PathPart::PathPart(std::string path_name, std::vector<std::string> parameter_names) {
  this->path_name = path_name;
  this->parameter_names = parameter_names;
}

