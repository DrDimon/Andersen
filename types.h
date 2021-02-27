#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <string>
#include <map>

class Object;

typedef std::map<std::string, std::vector<Object*>> subobjects_map;
typedef std::map<std::string, std::vector<Object*>>::iterator subobjects_map_iterator;

class Fragment {
  public:
    virtual void print() {};
    virtual std::string render(Object*) {return "";};
};

#endif
