#include "Heap/BinaryHeap.h"

int main() {
  blp::BinaryHeap<int> bh;
  for (uint32_t i{0}; i < 10; ++i) {
    bh.Push(10 - i);
  }
  bh.PrintDebugMsg();
  bh.Pop();
  bh.PrintDebugMsg();
  bh.Pop();
  bh.PrintDebugMsg();

  return 0;
}