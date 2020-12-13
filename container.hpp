#pragma once

#include <memory>

// chain of list

template <typename T> struct Cons {
  Cons(const T &value) : car(value), cdr(nullptr) {}
  T car;
  Cons<T> *cdr;
};

using intC = Cons<int>;

// forward_list

template <typename T, typename Allocator = std::allocator<Cons<T>>>
class my_container {
  Allocator data_allocator;
  size_t data_count;
  Cons<T> *data_head;
  Cons<T> *data_rest;

public:
  my_container() : data_count(0), data_head(nullptr), data_rest(nullptr){};

  ~my_container() { clear(); }

  void append(const T &element);

  T &at(size_t index);

  size_t size() const { return data_count; }

  bool empty() const { return data_count == 0; }

  void clear();
};

template <typename T, typename Allocator>
void my_container<T, Allocator>::append(const T &element) {
  auto data = data_allocator.allocate(1);
  if (empty()) {
    data_head = data;
    data_rest = data;
  } else {
    data_rest->cdr = data;
    data_rest = data;
  }

  data_allocator.construct(data_rest, element);

  data_count++;
}

template <typename T, typename Allocator>
void my_container<T, Allocator>::clear() {

  {
    Cons<T> *next_node = nullptr;
    Cons<T> *data_node = data_head;

    while (data_node) {
      next_node = data_node->cdr;
      data_allocator.destroy(data_node);
      data_allocator.deallocate(data_node, 1);
      data_node = next_node;
    }
  }

  data_count = 0;
  data_head = nullptr;
  data_rest = nullptr;
}

template <typename T, typename Allocator>
T &my_container<T, Allocator>::at(size_t index) {
  if (index >= size()) {
    throw std::out_of_range("out of range");
  }

  Cons<T> *data_node = data_head;
  for (size_t i = 0; i != index; i++) {
    data_node = data_node->cdr;
  }

  return data_node->car;
}
