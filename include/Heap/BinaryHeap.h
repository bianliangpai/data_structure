#ifndef HEAP_BINRARY_HEP_H_
#define HEAP_BINRARY_HEP_H_

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace blp {

template <typename T, typename Compare = std::less<T>,
          typename Container = std::vector<T>>
class BinaryHeap {
 private:
  typedef BinaryHeap<T, Compare, Container> this_type;
  typedef typename Container::iterator iterator;
  typedef const iterator const_iterator;

 public:
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

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
  void Swap(BinaryHeap& rhs) {
    std::swap(_c, rhs._c);
    std::swap(_comp, rhs._comp);
  }

  const_reference Top() { return _c.front(); }
  bool Empty() { return _c.empty(); }

  void Push(const value_type& value) {
    _c.push_back(value);
    _UpHeap();
  }
  void Push(value_type&& value) {
    _c.push_back(std::move(value));
    _UpHeap();
  }
  template <typename... Args>
  void Emplace(Args&&... args) {
    _c.emplace_back(std::forward<Args>(args)...);
    _UpHeap();
  }
  void Pop() {
    if (_c.size() == 0) {
      return;
    }
    std::swap(_c[0], _c[_c.size() - 1]);
    _c.pop_back();
    _DownHeap();
  }

  void PrintDebugMsg() {
    _PrintBinaryHeap("", 1, false);
    std::cout << std::endl;
  };

 private:
  void _PrintBinaryHeap(const std::string& prefix, size_type index,
                        bool is_left) {
    if (index > _c.size()) {
      return;
    }

    std::cout << prefix;
    std::cout << (is_left ? "|--" : "L--");
    std::cout << _c[index - 1] << std::endl;

    _PrintBinaryHeap(prefix + (is_left ? "|   " : "    "), index * 2, true);
    _PrintBinaryHeap(prefix + (is_left ? "|   " : "    "), index * 2 + 1,
                     false);
  }

  void _UpHeap() {
    if (_c.size() <= 1) {
      return;
    }
    for (size_type i = _c.size(); i > 1; i /= 2) {
      if (_comp(_c[i - 1], _c[i / 2 - 1])) {
        std::swap(_c[i - 1], _c[i / 2 - 1]);
      } else {
        break;
      }
    }
  }

  void _DownHeap() {
    if (_c.size() <= 1) {
      return;
    }
    for (size_type i = 1; i <= _c.size();) {
      size_type next_i{0};
      if (2 * i <= _c.size()) {
        if (2 * i + 1 <= _c.size()) {
          next_i = _comp(_c[2 * i - 1], _c[2 * i]) ? 2 * i : 2 * i + 1;
        } else {
          next_i = 2 * i;
        }
      } else {
        break;
      }

      if (!_comp(_c[i - 1], _c[next_i - 1])) {
        std::swap(_c[i - 1], _c[next_i - 1]);
        i = next_i;
      } else {
        break;
      }
    }
  }

 private:
  Container _c;
  Compare _comp;
};

};  // namespace blp

#endif  // HEAP_BINRARY_HEP_H_