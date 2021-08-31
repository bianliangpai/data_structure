#include <iostream>

#include "Linear/List.h"

template <typename T>
void Print(const blp::List<T>& l) {
  for (auto e : l) {
    std::cout << e << "->";
  }
  std::cout << std::endl;
}

int main() {
  blp::List<int> li;
  for (int i{0}; i < 10; ++i) {
    li.push_back(i);
  }
  Print(li);

  blp::List<int> li1 = li;
  Print(li1);

  auto sz = li1.size();
  for (auto i{0}; i < sz; ++i) {
    li1.erase(li1.begin());
  }
  Print(li1);

  for (int i{0}; i < 10; ++i) {
    li1.push_front(i);
  }
  Print(li1);

  li1.resize(5);
  Print(li1);
  li1.resize(10);
  Print(li1);

  li1.clear();
  Print(li1);

  li1.assign(10, 1);
  Print(li1);
  li1.assign(li.begin(), li.end());
  Print(li1);

  return 0;
}