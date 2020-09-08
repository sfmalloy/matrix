///\author Sean Malloy
///\name   Matrix.hpp
///\brief  Matrix class for performing various matrix operations.
/**********************************************************************/
// System includes
#include <cstddef>
#include <iostream>
#include <iterator>

/**********************************************************************/
// Local includes

/**********************************************************************/

template<typename T>
class Matrix
{
public:
  // type aliases
  using iterator = T*;
  using const_iterator = const T*;

  using reference = T&;
  using const_reference = const T&;

  using size_type = size_t;
  using difference_type = ptrdiff_t;

  // default ctor
  Matrix()
    : m_rows{0},
      m_cols{0},
      m_size{0},
      m_matrix{nullptr}
  {
  }

  // size ctor
  Matrix(size_t rows, size_t cols)
    : m_rows{rows},
      m_cols{cols},
      m_size{rows * cols},
      m_matrix{new T[m_size]}
  {
  }

  // copy ctor
  Matrix(const Matrix& m)
    : m_rows{m.numRows()},
      m_cols{m.numCols()},
      m_size{m.size()}
  {
    
  }

  // dtor
  ~Matrix()
  {
    delete[] this;
  }

  size_t
  numRows()
  {
    return m_rows;
  }

  size_t
  numCols()
  {
    return m_cols;
  }

  size_t
  size()
  {
    return m_size;
  }

  iterator
  begin()
  {
    return m_matrix;
  }

  const_iterator
  begin() const
  {
    return m_matrix;
  }

  iterator
  end()
  {
    return m_matrix + size();
  }

  const_iterator
  end() const
  {
    return m_matrix + size();
  }

  void
  transpose()
  {

  }

  void
  swapRows(size_t r1, size_t r2)
  {

  }

  // Matrix addition
  Matrix
  operator+(const Matrix<T>& other)
  {
    std::cout << "matrix addition\n";
    return *this;
  }

  // Matrix multiplication
  Matrix
  operator*(const Matrix<T>& other)
  {
    std::cout << "matrix multiplication\n";
    return *this;
  }
  
private:
  T* m_matrix;

  size_t m_capacity, m_rows, m_cols, m_size;
};

// Scalar multiplication k * M
template <typename T>
Matrix<T>
operator*(const int& k, const Matrix<T>& M)
{
  std::cout << "scalar multiplication by " << k << '\n';
  return M;
}

// Scalar multiplication M * k
template <typename T>
Matrix<T>
operator*(const Matrix<T>& M, const int& k)
{
  std::cout << "scalar multiplication by " << k << '\n';
  return M;
}


