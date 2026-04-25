#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include <cstddef>
#include <new>
#include <type_traits>
#include <stdexcept>
#include <utility>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a
 * list.
 */
template <typename T> class list {
protected:
  struct node {
    node *prev;
    node *next;
    typename std::aligned_storage<sizeof(T), alignof(T)>::type storage;
    T *val_ptr() { return reinterpret_cast<T *>(&storage); }
    const T *val_ptr() const { return reinterpret_cast<const T *>(&storage); }
  };

protected:
  node *head; // sentinel node
  size_t n;

  static node *alloc_node() { return static_cast<node *>(::operator new(sizeof(node))); }
  static void free_node(node *p) { ::operator delete(p); }

  template <typename U>
  node *make_node(U &&value) {
    node *p = alloc_node();
    // linkers will be set by insert
    p->prev = p->next = nullptr;
    ::new (static_cast<void *>(p->val_ptr())) T(std::forward<U>(value));
    return p;
  }

  void destroy_node(node *p) noexcept {
    // do not call on head
    p->val_ptr()->~T();
    free_node(p);
  }

  void init_empty() {
    head = alloc_node();
    head->prev = head->next = head; // sentinel; T is not constructed in head
    n = 0;
  }

  /**
   * insert node cur before node pos
   * return the inserted node cur
   */
  node *insert(node *pos, node *cur) {
    // pos may be head (end)
    cur->next = pos;
    cur->prev = pos->prev;
    pos->prev->next = cur;
    pos->prev = cur;
    ++n;
    return cur;
  }
  /**
   * remove node pos from list (no need to delete the node)
   * return the removed node pos
   */
  node *erase(node *pos) {
    // pos must not be head
    node *nxt = pos->next;
    pos->prev->next = pos->next;
    pos->next->prev = pos->prev;
    --n;
    return nxt;
  }

public:
  class const_iterator;
  class iterator {
  private:
    node *ptr;
    const list *owner;

    friend class list;
    friend class const_iterator;

  public:
    iterator() : ptr(nullptr), owner(nullptr) {}
    iterator(node *p, const list *o) : ptr(p), owner(o) {}

    iterator operator++(int) {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    iterator &operator++() {
      if (!ptr) throw std::runtime_error("iterator increment on null");
      ptr = ptr->next;
      return *this;
    }
    iterator operator--(int) {
      iterator tmp = *this;
      --(*this);
      return tmp;
    }
    iterator &operator--() {
      if (!ptr) throw std::runtime_error("iterator decrement on null");
      ptr = ptr->prev;
      return *this;
    }

    T &operator*() const {
      if (!ptr || ptr == owner->head) throw std::runtime_error("dereference end");
      return *(ptr->val_ptr());
    }
    T *operator->() const noexcept { return ptr ? ptr->val_ptr() : nullptr; }

    bool operator==(const iterator &rhs) const { return ptr == rhs.ptr; }
    bool operator==(const const_iterator &rhs) const;

    bool operator!=(const iterator &rhs) const { return ptr != rhs.ptr; }
    bool operator!=(const const_iterator &rhs) const;
  };

  /**
   * TODO
   * has same function as iterator, just for a const object.
   * should be able to construct from an iterator.
   */
  class const_iterator {
  private:
    node *ptr;
    const list *owner;
    friend class list;

  public:
    const_iterator() : ptr(nullptr), owner(nullptr) {}
    const_iterator(node *p, const list *o) : ptr(p), owner(o) {}
    const_iterator(const iterator &it) : ptr(it.ptr), owner(it.owner) {}

    const_iterator operator++(int) {
      const_iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    const_iterator &operator++() {
      if (!ptr) throw std::runtime_error("iterator increment on null");
      ptr = ptr->next;
      return *this;
    }
    const_iterator operator--(int) {
      const_iterator tmp = *this;
      --(*this);
      return tmp;
    }
    const_iterator &operator--() {
      if (!ptr) throw std::runtime_error("iterator decrement on null");
      ptr = ptr->prev;
      return *this;
    }

    const T &operator*() const {
      if (!ptr || ptr == owner->head) throw std::runtime_error("dereference end");
      return *(ptr->val_ptr());
    }
    const T *operator->() const noexcept { return ptr ? ptr->val_ptr() : nullptr; }

    bool operator==(const const_iterator &rhs) const { return ptr == rhs.ptr; }
    bool operator==(const iterator &rhs) const { return ptr == rhs.ptr; }

    bool operator!=(const const_iterator &rhs) const { return ptr != rhs.ptr; }
    bool operator!=(const iterator &rhs) const { return ptr != rhs.ptr; }
  };

  // Define cross comparisons declared in iterator
  bool it_eq(const iterator &a, const const_iterator &b) const { return a.ptr == b.ptr; }

  // Constructs
  list() { init_empty(); }
  list(const list &other) { init_empty(); for (auto it = other.cbegin(); it.ptr != other.head; ++it) push_back(*it); }
  /** Destructor */
  virtual ~list() { clear(); free_node(head); head = nullptr; }
  /** Assignment operator */
  list &operator=(const list &other) {
    if (this == &other) return *this;
    clear();
    for (auto it = other.cbegin(); it.ptr != other.head; ++it) push_back(*it);
    return *this;
  }
  /** access the first / last element */
  const T &front() const {
    if (empty()) throw std::runtime_error("list empty");
    return *(head->next->val_ptr());
    }
  const T &back() const {
    if (empty()) throw std::runtime_error("list empty");
    return *(head->prev->val_ptr());
    }
  /** returns an iterator to the beginning. */
  iterator begin() { return iterator(head->next, this); }
  const_iterator cbegin() const { return const_iterator(head->next, this); }
  /** returns an iterator to the end. */
  iterator end() { return iterator(head, this); }
  const_iterator cend() const { return const_iterator(head, this); }
  /** checks whether the container is empty. */
  virtual bool empty() const { return n == 0; }
  /** returns the number of elements */
  virtual size_t size() const { return n; }

  /** clears the contents */
  virtual void clear() {
    if (!head) return;
    node *cur = head->next;
    while (cur != head) {
      node *nxt = cur->next;
      // unlink not strictly necessary during destruction
      destroy_node(cur);
      cur = nxt;
    }
    head->next = head->prev = head;
    n = 0;
  }
  /** insert value before pos (pos may be the end() iterator) */
  virtual iterator insert(iterator pos, const T &value) {
    if (pos.owner != this) throw std::runtime_error("iterator from another container");
    node *cur = make_node(value);
    insert(pos.ptr, cur);
    return iterator(cur, this);
  }
  /** remove the element at pos (the end() iterator is invalid) */
  virtual iterator erase(iterator pos) {
    if (pos.owner != this) throw std::runtime_error("iterator from another container");
    if (pos.ptr == head) throw std::runtime_error("erase end iterator");
    node *nxt = erase(pos.ptr);
    destroy_node(pos.ptr);
    return iterator(nxt, this);
  }
  /** adds an element to the end */
  void push_back(const T &value) { insert(end(), value); }
  /** removes the last element */
  void pop_back() {
    if (empty()) throw std::runtime_error("list empty");
    iterator it(head->prev, this);
    (void)erase(it);
  }
  /** inserts an element to the beginning. */
  void push_front(const T &value) { insert(begin(), value); }
  /** removes the first element. */
  void pop_front() {
    if (empty()) throw std::runtime_error("list empty");
    iterator it(head->next, this);
    (void)erase(it);
  }
};

// Define cross comparison methods outside to reference const_iterator
template <typename T>
bool list<T>::iterator::operator==(const typename list<T>::const_iterator &rhs) const { return ptr == rhs.ptr; }

template <typename T>
bool list<T>::iterator::operator!=(const typename list<T>::const_iterator &rhs) const { return ptr != rhs.ptr; }

} // namespace sjtu

#endif // SJTU_LIST_HPP
