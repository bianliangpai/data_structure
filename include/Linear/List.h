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
  template <typename... Args,
            typename = std::enable_if_t<std::is_constructible_v<T, Args...>>>
  _ListNode(Args&&... args) : next{nullptr} {
    _element = new T(std::forward<Args>(args)...);
  }
  _ListNode(const T& value) : next{nullptr} { _element = new T(value); }
  _ListNode(T&& value) : next{nullptr} { _element = new T(std::move(value)); }
  _ListNode(const _ListNode& other) : next{other.next} {
    _element = new T(*other._element);
  }
  _ListNode(_ListNode&& other) : next{other.next}, _element{other._element} {}
  ~_ListNode() {
    delete _element;
    _element = pointer();
    next = nullptr;
  }

  // operator T() { return *_element; }

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
    iterator(const_pointer p) : _p{const_cast<pointer>(p)} {}
    iterator(const iterator& rhs) : _p{rhs._p} {}
    reference operator*() const { return *_p; }
    pointer operator->() const { return _p; }
    iterator& operator++() {
      _p = _p->next;
      return *this;
    }
    bool operator==(const iterator& rhs) const { return _p == rhs._p; }
    bool operator!=(const iterator& rhs) const { return !(*this == rhs); }

   private:
    pointer _p;
  };
  typedef const iterator const_iterator;

 public:
  List() : _begin(nullptr), _end(nullptr) { _begin = _end; }
  ~List() { clear(); }

  iterator begin() const { return _begin; }
  iterator end() const { return _end; }

  List& operator=(const List& other) {
    clear();
    for (value_type value : other) {
      push_back(value);
    }
    return *this;
  }
  List& operator=(List&& other) {
    clear();
    swap(other);
  }

  void assign(size_type count, const T& value) {
    clear();
    for (size_type i{0}; i < count; ++i) {
      push_back(value);
    }
  }
  template <typename InputIt>
  void assign(InputIt first, InputIt last) {
    clear();
    for (InputIt iit = first; iit != last; ++iit) {
      push_back(*iit);
    }
    push_back(*last);
  }

  reference front() { return *((*_begin).Get()); }
  reference back() {
    for (iterator it = _begin; it != _end; ++it) {
      iterator tmp{it};
      if (++tmp == _end) {
        return *((*it).Get());
      }
    }
  }

  bool empty() const { return (_begin == _end); }

  size_type size() const {
    size_type size = 0;
    for (iterator it = _begin; it != _end; ++it) {
      size += 1;
    }
    return size;
  }

  void clear() {
    iterator it = _begin;
    while (it != _end) {
      iterator tmp = it;
      ++it;

      _Destroy(tmp);
    }
    _begin = _end;
  }

  template <typename V,
            typename = std::enable_if_t<std::is_convertible_v<V, T>>>
  iterator insert(const iterator& pos, V&& value) {
    if (pos == _begin) {
      iterator i{new Node(std::forward<V>(value))};
      i->next = std::addressof(*pos);
      _begin = i;
      return i;
    }

    for (iterator it = _begin; it != _end; ++it) {
      iterator tmp = it;
      if (++tmp == pos) {
        iterator i{new Node(std::forward<V>(value))};
        i->next = it->next;
        it->next = std::addressof(*i);
        return i;
      }
    }

    throw std::out_of_range("invalid pos parameter!");
    return _end;
  }

  template <typename... Args>
  iterator emplace(const_iterator pos, Args&&... args) {
    return insert(pos, std::move({std::forward<Args>(args)...}));
  }

  iterator erase(const_iterator pos) {
    if (pos == _begin) {
      iterator tmp{_begin};
      ++_begin;
      _Destroy(tmp);
      return _begin;
    }

    for (iterator it = _begin; it != _end; ++it) {
      iterator tmp = it;
      if (++tmp == pos) {
        iterator i = it->next;
        it->next = pos->next;
        _Destroy(i);
        return tmp;
      }
    }

    throw std::out_of_range("invalid pos parameter!");
    return _end;
  }
  iterator erase(const_iterator beg, const_iterator end) {
    for (iterator it = beg; it != end && it != _end; ++it) {
      erase(it);
    }
  }

  template <typename V,
            typename = std::enable_if_t<std::is_convertible_v<V, T>>>
  void push_back(V&& value) {
    insert(_end, std::forward<V>(value));
  }

  template <typename... Args>
  auto emplace_back(Args&&... args)
      -> std::enable_if_t<std::is_constructible_v<T, Args...>, void> {
    push_back(std::move({std::forward<Args>(args)...}));
  }

  void pop_back() {
    for (iterator it = _begin; it != _end; ++it) {
      iterator tmp{it};
      if (++tmp == _end) {
        erase(it);
      }
    }
  }

  template <typename V,
            typename = std::enable_if_t<std::is_convertible_v<V, T>>>
  void push_front(V&& value) {
    insert(_begin, value);
  }

  template <typename... Args>
  reference emplace_front(Args&&... args) {
    insert(_begin, {std::forward<Args>(args)...});
    return *_begin;
  }

  void pop_front() { erase(_begin); }

  void resize(size_type count, const T& value = T()) {
    size_type counter{0};
    for (iterator it = _begin; it != _end; ++it) {
      if (++counter >= count) {
        iterator tmp{it};
        erase(++tmp, _end);
        it->next = std::addressof(_end);
        return;
      }
    }

    size_type remaining = count - counter;
    for (size_type i = 0; i < remaining; ++i) {
      push_back(value);
    }
  }

  void swap(List& other) { std::swap(_begin, other._begin); }

 private:
  void _Destroy(iterator it) { delete std::addressof(*it); }

 private:
  iterator _begin;
  iterator _end;
};

}  // namespace blp

#endif  // LINEAR_LIST_H_