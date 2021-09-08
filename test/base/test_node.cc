#include <iostream>

#include "Base/Node.h"

struct Test {
  void f() { std::cout << "Test::f() -> " << this << std::endl; }

  friend std::ostream& operator<<(std::ostream& os, const Test& t) {
    os << "I'm struct Test";
    return os;
  }
};

int main() {
  blp::_NodeBase<Test> nb{};
  std::cout << *nb << std::endl;
  nb->f();
  std::cout << &nb << std::endl;
  return 0;
}