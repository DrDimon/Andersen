#include <algorithm>
#include <iostream>
#include <time.h>

#include "Object.h"
#include "ObjectInstance.h"
#include "ObjectPath.h"

int interpStory(Object* root, unsigned int seed = 0){

  if (!seed) seed = time(NULL);
  std::cout << "Seed: " << seed << std::endl;
  std::srand(seed);

//  root->print();
  ObjectPath path = ObjectPath("TOP");
//  std::cout << "Render result:" << std::endl;

  ObjectInstance root_instance = ObjectInstance(root);
  std::vector<ObjectInstance*> parameters;
  std::string result = root_instance.render(root, path, parameters);
  std::cout << result << std::endl;
  return 0;
}
