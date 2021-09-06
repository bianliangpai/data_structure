#ifndef INCLUDE_UTILS_NODE_H_
#define INCLUDE_UTILS_NODE_H_

template <typename T>
class _NodeBase {
 public:
  typedef T value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;

 public:
  template <typename... Args,
            typename = std::enable_if_t<std::is_constructible_v<T, Args...>>>
  _NodeBase(Args&&... args) {
    _element = new T(std::forward<Args>(args)...);
  }
  _NodeBase(const T& value) { _element = new T(value); }
  _NodeBase(T&& value) : { _element = std::addressof(value); }
  _NodeBase(const _NodeBase& other) { _element = new T(*other); }
  _NodeBase(_NodeBase&& other) {
    _element = other._element;
    other._element = nullptr;
  }
  ~_NodeBase() {
    delete _element;
    _element = pointer();
  }

  reference operator*() const { return *_element; }

 private:
  T* _element;
};

#endif  // INCLUDE_UTILS_NODE_H_