#include <iostream>

#include "Linear/List.h"

int main() {
  blp::List<int> li;
  for (int i{0}; i < 10; ++i) {
    li.push_back(i);
  }
  std::cout << li << std::endl;

  blp::List<int> li1 = li;
  std::cout << li1 << std::endl;

  auto sz = li1.size();
  for (auto i{0}; i < sz; ++i) {
    li1.erase(li1.begin());
  }
  std::cout << li1 << std::endl;

  for (int i{0}; i < 10; ++i) {
    li1.push_front(i);
  }
  std::cout << li1 << std::endl;

  li1.resize(5);
  std::cout << li1 << std::endl;
  li1.resize(10);
  std::cout << li1 << std::endl;

  li1.clear();
  std::cout << li1 << std::endl;

  li1.assign(10, 1);
  std::cout << li1 << std::endl;
  li1.assign(li.begin(), li.end());
  std::cout << li1 << std::endl;

  return 0;
}