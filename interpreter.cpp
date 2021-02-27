#include <algorithm>
#include <iostream>
#include <time.h>

#include "Object.h"
#include "ObjectPath.h"

int interpStory(Object* root){

  std::srand(time(NULL));

  root->print();
  ObjectPath path = ObjectPath("TOP");
  std::string result = root->render(root, path);
  std::cout << "Render result:" << std::endl;
  std::cout << result << std::endl;
  return 0;
}
