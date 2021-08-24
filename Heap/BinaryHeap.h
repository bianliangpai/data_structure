#ifndef HEAP_BINRARY_HEP_H_
#define HEAP_BINRARY_HEP_H_

#include <algorithm>
#include <vector>

namespace blp {

template <typename T, typename Compare = std::less<T>,
          typename Container = std::vector<T>>
class BinaryHeap {
 private:
  typedef BinaryHeap<T, Compare, Container> this_type;

 public:
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef typename Container::iterator iterator;
  typedef const iterator const_iterator;

 public:
  BinaryHeap() : _c{}, _comp{} {}
  BinaryHeap(const BinaryHeap& rhs) : _c{rhs._c}, _comp{rhs._comp} {}
  BinaryHeap(BinaryHeap&& rhs) { Swap(std::move(rhs)); }
  BinaryHeap(const_iterator beg, const_iterator end) : _c{beg, end}, _comp{} {}
  BinaryHeap& operator=(const BinaryHeap& rhs) {
    _c = rhs._c;
    _comp = rhs._comp;
  }
  BinaryHeap& operator=(BinaryHeap&& rhs) {
    _c = std::move(rhs._c);
    _comp = std::move(rhs._comp);
  }
  Swap(BinaryHeap& rhs) {
    std::swap(_c, rhs._c);
    std::swap(_comp, rhs._comp);
  }

  const_referece Top() { return _c.front(); }
  bool Empty() { return _c.empty(); }

  void Push(const value_type& value) {
    // pass
  }
  void Push(value_type&& value) {
    // pass
  }
  template <typename... Args>
  void Emplace(Args&&... args) {
    // pass
  }
  void Pop() {
    // pass
  }

 private:
  Container _c;
  Compare _comp;
};

};  // namespace blp

#endif  // HEAP_BINRARY_HEP_H_