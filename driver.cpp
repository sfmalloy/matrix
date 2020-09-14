// This driver is for testing purposes and is not complete
// or intented for regular use.

#include <iostream>
#include <random>
#include <vector>

#include "Matrix.hpp"

using Type = float;

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
  mat::Matrix<Type> A(6, 4);
  std::vector<Type> v 
  {
    2,4,-6,30,
    -1,-1,4,-18,
    -2,-6,5,-28,
    2,4,-6,30,
    -1,-1,4,-18,
    -2,-6,5,-28
  };

  fillSpecific(A, v);

  std::cout << "Matrix A:\n" <<  A << '\n';
  std::cout << "Row Echelon A:\n" << mat::rowEchelon(A) << '\n';
  std::cout << "Reduced row echelon A:\n" << mat::reducedRowEchelon(A) << '\n';
  
  A = mat::transpose(A);
  return 0;
}