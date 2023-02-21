/*************************************************************************
  > File Name:     use_module.cc
  > Author:        Landerl Young
  > Mail:          LanderlYoung@gmail.com
  > Created Time:  Tue Feb 21 13:40:56 2023
 ************************************************************************/

#include <iostream>

import mymodule;

int main() {
  std::cout << "plus:" << mymodule::add(1, 2)
            << std::endl;

  return 0;
}

