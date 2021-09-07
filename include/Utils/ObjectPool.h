#ifndef UTILS_OBJECTPOOL_H_
#define UTILS_OBJECTPOOL_H_

namespace blp {

template <typename T>
struct _Node {
 public:
  typedef T value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;

 private:
  using node_pointer = _Node*;

 public:
  template <typename... Args>
  _Node(Args&&... args) : next(nullptr) {
    _p = new (_buf) T(std::forward<Args>(args)...);
  }
  ~_Node() { _Reset(); }

  void PrintBuf() {
    std::size_t length = sizeof(T);
    for (auto i{0}; i < length; ++i) {
      std::uint8_t tmp = _buf[i];
      std::cout << static_cast<std::uint32_t>(tmp) << " ";
    }
    std::cout << std::endl;
  }

 private:
  _Node(const _Node&) = delete;
  _Node& operator=(const _Node&) = delete;

  void _Reset() { _p->~T(); }

 private:
  std::uint8_t _buf[sizeof(T)];
  pointer _p;
};

}  // namespace blp

#endif  // UTILS_OBJECTPOOL_H_