#include <iostream>

#include "Matrix.hpp"

int
main()
{
  Matrix<int> a(5, 5);

  for (size_t i = 0; i < 5; ++i)
    for (size_t j = 0; j < 5; ++j)
      a(i, j) = 5;

  Matrix<int> b = a;

  for (auto it = b.begin(); it != b.end(); ++it)
    std::cout << *it << ' ';

  return 0;
}