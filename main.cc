#include <iostream>

#include "Linear/List.h"

int main() {
  blp::List<int> li;
  for (int i{0}; i < 10; ++i) {
    li.push_back(i);
  }
  for (auto e : li) {
    std::cout << e << std::endl;
  }
  return 0;
}