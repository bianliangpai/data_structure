#ifndef LINEAR_LIST_H_
#define LINEAR_LIST_H_

#include <list>
#include <memory>

namespace blp {

template <typename T>
struct Node {
 private:
  using container_pointer = Node*;

 public:
  Node() : next(nullptr) {}

 private:
  Node(const Node&) = delete;
  Node& operator=(const Node&) = delete;

 public:
  T obj;
  container_pointer next;
};

template <typename T, typename Node = Node<T>,
          typename NodeAllocator = std::allocator<Node>,
          typename ElemAllocator = std::allocator<T>>
class List {
 private:
  using value_type = Node;
  using allocator_type = NodeAllocator;
  using allocator_traits = std::allocator_traits<allocator_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = typename allocator_traits::pointer;
  using const_pointer = typename allocator_traits::const_pointer;

 public:
  List() : begin_(nullptr), end_(nullptr) {}
  ~List() { clear(); }

  bool empty() const { return (begin_ == nullptr); }

  size_type size() const {
    size_type size = 0;
    for (pointer it = begin_; it != end_; it = it->next) {
      size += 1;
    }
    return size;
  }

  void clear() {
    pointer it = begin_;
    while (it != end_) {
      pointer tmp = it;
      it = it->next;
      DestroyNode(tmp);
    }
    begin_ = nullptr;
  }

  template <typename T>
  pointer insert(const_pointer pos, T&& value) {
    for (pointer it = begin_; it != end_; it = it->next) {
      if (it->next == pos) {
        InsertImpl(it);
        break;
      }
    }
  }

  template <class... Args>
  pointer emplace(const_pointer pos, Args&&... args) {
    std::unique_ptr<T> pobj{nullptr};
    ElemAllocator::construct(elem_allocator_, pobj.get(),
                             std::forward<Args>(args)...);

    return insert(pos, *pobj.get());
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
  template <typename... Args>
  void ConstructNode(pointer& new_node, Args&&... args) {
    allacator_traits::construct(allocaltor_, new_node,
                                std::forward<Args>(args)...);
  }
  void DestroyNode(pointer node) {
    allacator_traits::destroy(allocaltor_, node);
  }

  void InsertImpl(pointer p, const T& value) {
    pointer new_node{nullptr};
    AllocateNode(new_node);
    new_node->obj = value;
    new_node->next = p->next;
    p->next = new_node;
  }
  void InsertImpl(pointer p, T&& value) {
    pointer new_node{nullptr};
    AllocateNode(new_node);
    new_node->obj = std::move(value);
    new_node->next = p->next;
    p->next = new_node;
  }

  void ElemAssign(pointer new_node, const T& value) { new_node->obj = value; }

  void ElemAssign(pointer new_node, T&& value) {
    new_node->obj = std::move(value);
  }

 private:
  size_type size_;
  pointer begin_;
  pointer end_;

  allocator_type allocator_;
  ElemAllocator elem_allocator_;
};

}  // namespace blp

#endif  // LINEAR_LIST_H_