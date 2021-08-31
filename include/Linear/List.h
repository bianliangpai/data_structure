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
  std::cout << *(ln.Get());
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
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;

   private:
    typedef Node wrapper_value_type;
    typedef wrapper_value_type* wrapper_pointer;
    typedef const wrapper_value_type* const_wrapper_pointer;
    typedef wrapper_value_type& wrapper_reference;
    typedef const wrapper_value_type& const_wrapper_reference;

   public:
    iterator(const_wrapper_pointer wp = nullptr)
        : _wp{const_cast<wrapper_pointer>(wp)} {}
    iterator(const_wrapper_reference other) : _wp{other._wp} {}
    reference operator*() const { return *const_cast<pointer>(_wp->Get()); }
    pointer operator->() const { return _wp->Get(); }
    iterator& operator++() {
      _wp = _wp->next;
      return *this;
    }
    bool operator==(const iterator& other) const { return _wp == other._wp; }
    bool operator!=(const iterator& other) const { return !(*this == other); }

    const_wrapper_pointer get() const { return _wp; }
    void set_next(const iterator& next_iterator) {
      _wp->next = next_iterator._wp;
    }
    void set_next(wrapper_pointer next_wp) { _wp->next = next_wp; }
    const_wrapper_pointer get_next() const { return _wp->next; }

   private:
    wrapper_pointer _wp;
  };
  typedef const iterator const_iterator;

 public:
  List() : _begin(nullptr), _end(nullptr) { _begin = _end; }
  List(const List& other) : _begin(nullptr), _end(nullptr) {
    _begin = _end;
    for (value_type value : other) {
      push_back(value);
    }
  }
  List(List&& other) { swap(std::move(other)); }
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
    swap(std::move(other));
  }

  void assign(size_type count, const T& value) {
    clear();
    for (size_type i{0}; i < count; ++i) {
      push_back(value);
    }
  }
  template <typename InputIt,
            typename = std::void_t<typename InputIt::value_type>>
  void assign(InputIt first, InputIt last) {
    clear();
    for (InputIt iit = first; iit != last; ++iit) {
      push_back(*iit);
    }
  }

  reference front() const { return *_begin; }
  reference back() const {
    for (iterator it = _begin; it != _end; ++it) {
      if (it.get_next() == _end) {
        return *it;
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
      i.set_next(pos);
      _begin = i;
      return i;
    }

    for (iterator it = _begin; it != _end; ++it) {
      iterator tmp{it.get_next()};
      if (tmp == pos) {
        iterator i{new Node(std::forward<V>(value))};
        i.set_next(it.get_next());
        it.set_next(i);
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
      iterator tmp{it.get_next()};
      if (tmp == pos) {
        it.set_next(pos.get_next());
        _Destroy(pos);
        return ++it;
      }
    }

    throw std::out_of_range("invalid pos parameter!");
    return _end;
  }
  iterator erase(const_iterator beg, const_iterator end) {
    for (iterator it = beg; it != end && it != _end;) {
      it = erase(it);
    }
    return end;
  }

  template <typename V,
            typename = std::enable_if_t<std::is_convertible_v<V, T>>>
  void push_back(V&& value) {
    insert(_end, std::forward<V>(value));
  }

  template <typename... Args>
  auto emplace_back(Args&&... args)
      -> std::enable_if_t<std::is_constructible_v<T, Args...>> {
    push_back(std::move({std::forward<Args>(args)...}));
  }

  void pop_back() {
    for (iterator it = _begin; it != _end; ++it) {
      if (it.get_next() == _end) {
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
        iterator tmp{it.get_next()};
        erase(tmp, _end);
        it.set_next(_end);
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
  void _Destroy(iterator it) { delete it.get(); }

 private:
  iterator _begin;
  iterator _end;
};

}  // namespace blp

#endif  // LINEAR_LIST_H_