#include <iostream>
#include <random>

#include "Matrix.hpp"

using Type = float;

template <typename T>
void
fillRandom(mat::Matrix<T>& M)
{
  std::random_device seed_gen;
  std::mt19937 random_gen(seed_gen());
  std::uniform_int_distribution<int> dist(-9, 9);

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

int
main()
{
  mat::Matrix<float> A(5, 4);
  fillRandom(A);

  std::cout << "Matrix A:\n" <<  A << '\n';
  std::cout << "Row Echelon A:\n" << mat::rowEchelon(A) << '\n';

  std::cout << "A.isRowEchelonForm(): " << std::boolalpha << A.isRowEchelonForm() << '\n'
            << "Row Echelon is row echelon" << mat::rowEchelon(A).isRowEchelonForm() << '\n';

  return 0;
}