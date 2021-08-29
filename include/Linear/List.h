#ifndef LINEAR_LIST_H_
#define LINEAR_LIST_H_

#include <algorithm>
#include <list>
#include <memory>
#include <type_traits>

namespace blp {

template <typename T>
class _ListNode {
 public:
  typedef T value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;

 public:
  template <typename... Args>
  _ListNode(Args&&... args) : next{nullptr} {
    _element = new T(std::forward<Args>(args)...);
  }
  _ListNode(const T& value) { _element = new T(value); }
  _ListNode(T&& value) { _element = new T(std::move(value)); }
  ~_ListNode() {
    delete _element;
    _element = pointer();
    next = nullptr;
  }

  const_pointer Get() const { return _element; }

  _ListNode* next;

 private:
  T* _element;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const _ListNode<T>& ln) {
  std::cout << static_cast<void*>(ln.next);
  std::cout << "->";
  std::cout << static_cast<void*>(const_cast<T*>(ln.Get()));
  return os;
}

template <typename T, typename Node = _ListNode<T>,
          typename Allocator = std::allocator<Node>>
class List {
 private:
  typedef List<T, Node, Allocator> this_type;
  typedef Allocator _allocator_type;

 public:
  typedef T value_type;
  typedef std::size_t size_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  class iterator {
   public:
    typedef Node value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;

   public:
    iterator(const_pointer p) : _p{p} {}
    iterator(const iterator& rhs) : _p{rhs._p} {}
    typename this_type::reference operator*() { return *(_p->Get()); }
    typename this_type::pointer operator->() { return _p->Get(); }
    iterator& operator++() {
      _p = _p->next;
      return *this;
    }
    bool operator==(const iterator& rhs) { return _p == rhs._p; }
    bool operator!=(const iterator& rhs) { return !(*this == rhs); }

   private:
    pointer _p;
  };
  typedef const iterator const_iterator;

 public:
  List() : _end(nullptr) { _begin = _end; }
  ~List() { clear(); }

  bool Empty() const { return (_begin == _end); }

  size_type Size() const {
    size_type size = 0;
    for (iterator it = _begin; it != _end; ++it) {
      size += 1;
    }
    return size;
  }

  void Clear() {
    iterator it = _begin;
    while (it != _end) {
      iterator tmp = it;
      ++it;

      delete std::addressof(*it);
    }
    _begin = _end;
  }

  template <typename V,
            typename = std::enable_if_t<std::is_convertible_v<T, V>>>
  iterator Insert(const iterator& pos, V&& value) {
    for (iterator it = _begin; it != _end; ++it) {
      iterator tmp = it;
      if (++tmp == pos) {
        Node* node = new Node(std::forward<V>(value));
        node->next = it->next;
        it->next = node;
        break;
      }
    }
  }

  template <typename... Args>
  iterator Emplace(const_iterator pos, Args&&... args) {
    return Insert(pos, std::move({std::forward<Args>(args)...}));
  }

  void erase();
  void push_back();
  void emplace_back();
  void pop_back();
  void push_front();
  void emplace_front();
  void pop_front();
  void resize();
  void swap();

  void merge();
  void splice();
  void remove();
  void reserve();
  void unique();
  void sort();

 private:
  iterator _begin;
  iterator _end;

  _allocator_type _allocator;
};

}  // namespace blp

#endif  // LINEAR_LIST_H_