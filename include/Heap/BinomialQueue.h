#ifndef INCLUDE_HEAP_BINOMINAL_QUEUE_H_
#define INCLUDE_HEAP_BINOMINAL_QUEUE_H_

#include "Base/Node.h"

namespace blp {

template <typename T>
class _BinominalQueueNode : public _NodeBase<T> {
 private:
  typedef _NodeBase<T> _Base;

 public:
  typedef typename _Base::value_type value_type;
  typedef typename _Base::pointer pointer;
  typedef typename _Base::const_pointer const_pointer;

 public:
  template <typename... Args,
            typename = std::enable_if_t<std::is_constructible_v<T, Args...>>>
  _BinominalQueueNode(Args&&... args) : _Base{std::forward<Args>(args)...} {}

 private:
  std::vector<_BinominalQueueNode> children;
};

template <typename T, typename Node = _BinominalQueueNode<T>>
class BinominalQueue {
 public:
 private:
};

};  // namespace blp

#endif  // INCLUDE_HEAP_BINOMINAL_QUEUE_H_