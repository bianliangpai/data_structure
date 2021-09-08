#include <iostream>

#include "Heap/BinaryHeap.h"

int main() {
  blp::BinaryHeap<int> bh;
  for (uint32_t i{0}; i < 10; ++i) {
    bh.push(10 - i);
  }
  std::cout << "build tree:\n";
  std::cout << bh << std::endl;
  bh.pop();
  std::cout << "first pop:\n";
  std::cout << bh << std::endl;
  bh.pop();
  std::cout << "second pop:\n";
  std::cout << bh << std::endl;

  return 0;
}