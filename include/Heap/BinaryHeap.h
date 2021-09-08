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
  BinaryHeap(const BinaryHeap& other) : _c{other._c}, _comp{other._comp} {}
  BinaryHeap(BinaryHeap&& other) { Swap(std::move(other)); }
  BinaryHeap(const_iterator beg, const_iterator end) : _c{beg, end}, _comp{} {}
  BinaryHeap& operator=(const BinaryHeap& other) {
    _c = other._c;
    _comp = other._comp;
  }
  BinaryHeap& operator=(BinaryHeap&& other) {
    _c = std::move(other._c);
    _comp = std::move(other._comp);
  }
  void swap(BinaryHeap& other) {
    std::swap(_c, other._c);
    std::swap(_comp, other._comp);
  }

  const_reference top() { return _c.front(); }
  bool empty() { return _c.empty(); }

  void push(const value_type& value) {
    _c.push_back(value);
    _UpHeap();
  }
  void push(value_type&& value) {
    _c.push_back(std::move(value));
    _UpHeap();
  }
  template <typename... Args>
  void emplace(Args&&... args) {
    _c.emplace_back(std::forward<Args>(args)...);
    _UpHeap();
  }
  void pop() {
    if (_c.size() == 0) {
      return;
    }
    std::swap(_c[0], _c[_c.size() - 1]);
    _c.pop_back();
    _DownHeap();
  }

  friend std::ostream& operator<<(std::ostream& os, const BinaryHeap& bh) {
    std::function<void(std::string, size_type, bool)> recursive_print =
        [&](const std::string& prefix, size_type index, bool is_left) {
          if (index > bh._c.size()) {
            return;
          }

          os << prefix;
          os << (is_left ? "|--" : "L--");
          os << bh._c[index - 1] << std::endl;

          recursive_print(prefix + (is_left ? "|   " : "    "), index * 2,
                          true);
          recursive_print(prefix + (is_left ? "|   " : "    "), index * 2 + 1,
                          false);
        };

    recursive_print("", 1, false);
    return os;
  }

 private:
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