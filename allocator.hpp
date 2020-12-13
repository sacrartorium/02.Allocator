#pragma once

#include <memory>
#include <unordered_set>

// Storage for pointers

template <typename T> class my_storage {
  std::unordered_set<T *> storage;

public:
  my_storage() = default;

  void append(T *pointer, size_t size) {
    for (size_t i = 0; i < size; i++) {
      storage.insert(pointer + i);
    }
  }

  void remove(T *pointer, size_t size) {
    for (size_t i = 0; i < size; i++) {
      auto ptr = pointer + i;
      auto iter = storage.find(ptr);

      if (iter != storage.end()) {
        storage.erase(ptr);
      }
    }
  }

  bool empty() { return storage.empty(); }

  void clear() { storage.clear(); }

  void reserve(size_t size) { storage.reserve(size); }
};

// Allocator

template <typename T, size_t Capacity> class my_allocator {
  T *reserve(std::size_t size);

  T *data_start;
  T *data_used;
  T *data_end;

  my_storage<T> data_storage;

public:
  using value_type = T;

  template <typename U> struct rebind {
    using other = my_allocator<U, Capacity>;
  };

  my_allocator() : data_start(nullptr), data_used(nullptr), data_end(nullptr) {}

  template <typename U, size_t CapacityOther>
  my_allocator(const my_allocator<U, CapacityOther> &)
      : data_start(nullptr), data_used(nullptr), data_end(nullptr) {}

  ~my_allocator() {
    data_storage.clear();

    if (data_start)
      free(data_start);
  }

  T *allocate(std::size_t size);
  void deallocate(T *p, size_t size);

  template <typename U, typename... Args>
  void construct(U *p, Args &&... args) {
    new (p) U(std::forward<Args>(args)...);
  }

  template <typename U> void destroy(U *pointer) { pointer->~U(); }
};

template <typename T, size_t Capacity>
T *my_allocator<T, Capacity>::allocate(std::size_t size) {
  // allocate
  if (data_start == nullptr && size <= Capacity) {
    {
      data_start = reserve(sizeof(T) * Capacity);
      data_storage.reserve(Capacity);
    }
    data_storage.append(data_start, size);
    data_used = data_start + size;
    data_end = data_start + Capacity;
    return data_start;
  }
  // reused
  else if (data_used + size <= data_end) {
    data_storage.append(data_used, size);
    data_used += size;
    return data_used - size;
  } else {
    throw std::bad_alloc();
  }
}

template <typename T, size_t Capacity>
void my_allocator<T, Capacity>::deallocate(T *p, size_t size) {
  data_storage.remove(p, size);

  if (data_storage.empty()) {
    data_used = data_start;
  }
}

template <typename T, size_t Capacity>
T *my_allocator<T, Capacity>::reserve(std::size_t size) {
  void *ptr = std::malloc(size);
  if (ptr)
    return reinterpret_cast<T *>(ptr);

  throw std::bad_alloc();
}
