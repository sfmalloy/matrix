#include <iostream>
#include <random>
#include <vector>

#include "Matrix.hpp"

using Type = double;

template <typename T>
void
fillRandom(mat::Matrix<T>& M)
{
  std::random_device seed_gen;
  std::mt19937 random_gen(seed_gen());
  std::uniform_int_distribution<int> dist(-2, 2);

  for (auto& elem : M)
    elem = dist(random_gen);
}

template <typename T>
void
fillSequential(mat::Matrix<T>& M)
{
  T count(0);

  for (auto& elem : M)
    elem = ++count;
}

template <typename T>
void
fillSpecific(mat::Matrix<T>& M, std::vector<T>& elems)
{
  auto it = elems.begin();
  for (auto& elem : M)
    elem = *(it++);
}

int
main()
{
  mat::Matrix<Type> A(5, 4);
  std::vector<Type> v {
    -1,-9,-8,1,
    1,2,-8,5,
    -9,5,7,1,
    5,-5,-7,3,
    -6,7,5,3
  };
  fillRandom(A);

  std::cout << "Matrix A:\n" <<  A << '\n';
  std::cout << "Row Echelon A:\n" << mat::rowEchelon(A) << '\n';
  // std::cout << "Reduced row echelon A:\n" << mat::reducedRowEchelon(A) << '\n';
  return 0;
}