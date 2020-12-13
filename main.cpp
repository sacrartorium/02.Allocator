#ifndef __PRETTY_FUNCTION__
#include "pretty.h"
#endif

#define USE_PRETTY 1

#include "allocator.hpp"
#include "container.hpp"

#include <iostream>
#include <map>

unsigned fact(unsigned base) {
  if (base == 0)
    return 1;
  else
    return base * fact(base - 1);
}

int main() {

  const int size = 10;

  {
    /// map<int, int> + std allocator

    auto map = std::map<int, int>{};

    for (int key = 0; key < size; key++) {
      map[key] = fact(key);
    }

    /*for (const auto &[key, value] : map) {
      std::cout << key << " " << value << std::endl;
    }*/
  }

  {
    /// map<int, int> + my allocator

    auto map = std::map<int, int, std::less<int>,
                        my_allocator<std::pair<const int, int>, size>>{};

    for (int key = 0; key < size; key++) {
      map[key] = fact(key);
    }

    for (auto key = 0; key < size; key++) {
      std::cout << key << " " << map.at(key) << std::endl;
    }
  }

  {
    /// my container + std allocator

    auto container = my_container<int>{};

    for (int key = 0; key < size; key++)
      container.append(key);

    /*for (int key = 0; key < size; key++) {
      std::cout << container.at(key) << std::endl;
    }*/
  }

  {
    /// my container + my allocator

    auto container = my_container<int, my_allocator<intC, size>>{};

    for (int key = 0; key < size; key++)
      container.append(key);

    for (int key = 0; key < size; key++) {
      std::cout << container.at(key) << std::endl;
    }
  };

  return 0;
}
