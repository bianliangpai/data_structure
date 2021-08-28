#include <iostream>

#include "Heap/BinaryHeap.h"

int main() {
  blp::BinaryHeap<int> bh;
  for (uint32_t i{0}; i < 10; ++i) {
    bh.Push(10 - i);
  }
  std::cout << "build tree:\n";
  bh.PrintDebugMsg();
  bh.Pop();
  std::cout << "first pop:\n";
  bh.PrintDebugMsg();
  bh.Pop();
  std::cout << "second pop:\n";
  bh.PrintDebugMsg();

  return 0;
}