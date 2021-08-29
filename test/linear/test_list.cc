#include <iostream>

#include "Linear/List.h"
#include "Utils/ObjectPool.h"

struct TS {
 public:
  TS(int i) {
    std::cout << i << std::endl;
    std::cout << "TS Constructor\n";
  }
  ~TS() { std::cout << "TS Destructor\n"; }
};

int main() {
  blp::_Node<blp::_ListNode<TS>> nts(1);
  nts.PrintBuf();
  nts.~_Node();
  nts.PrintBuf();
  return 0;
}