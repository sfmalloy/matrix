///\author Sean Malloy
///\name   Matrix.hpp
///\brief  Matrix class for performing various matrix operations.
/**********************************************************************/
// Macro guard
#ifndef MATRIX_HPP
#define MATRIX_HPP

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
    : m_rows{m.rows()},
      m_cols{m.cols()},
      m_size{m.size()},
      m_matrix{new T[m_size]}
  {
    if (this != &m)
      std::copy(m.begin(), m.end(), begin());
  }

  // dtor
  ~Matrix()
  {
    if (m_matrix != nullptr)
      delete[] m_matrix;
  }

  Matrix&
  operator=(const Matrix& m)
  {
    if (this != &m)
    {
      std::copy(m.begin(), m.end(), begin());
      m_size = m.size();
      m_rows = m.rows();
      m_cols = m.cols();
    }
    return *this;
  }

  size_t
  rows()
  {
    return m_rows;
  }

  size_t
  rows() const
  {
    return m_rows;
  }

  size_t
  cols()
  {
    return m_cols;
  }

  size_t
  cols() const
  {
    return m_cols;
  }

  size_t
  size()
  {
    return m_size;
  }

  size_t
  size() const
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
    return m_matrix + m_size;
  }

  const_iterator
  end() const
  {
    return m_matrix + m_size;
  }

  void
  transpose()
  {
    Matrix transposed(m_cols, m_rows);
    
    for (size_t i = 0; i < m_rows; ++i)
      for (size_t j = 0; j < m_cols; ++j)
        transposed(i, j) = this(j, i);

    this = transposed;
  }

  void
  swapRows(size_t r1, size_t r2)
  {

  }

  T&
  operator()(const size_t& row, const size_t& col)
  {
    return m_matrix[(m_cols * row) + col];
  }

  T&
  operator()(const size_t& row, const size_t& col) const
  {
    return m_matrix[(m_cols * row) + col];
  }

  // Matrix addition
  Matrix
  operator+(const Matrix<T>& other)
  {
    return *this;
  }

  // Matrix multiplication
  Matrix
  operator*(const Matrix<T>& other)
  {
    std::cout << "matrix multiplication\n";
    return *this;
  }

  bool
  operator==(const Matrix<T>& other)
  {
    return false;
  }
  
private:
  size_t m_rows;
  size_t m_cols;
  size_t m_size;

  T* m_matrix;
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

template <typename T>
std::ostream&
operator<<(std::ostream& output, const Matrix<T>& M)
{
  for (size_t i = 0; i < M.rows(); ++i)
  {
    for (size_t j = 0; j < M.cols(); ++j)
    {
      output << M(i, j) << ' ';
    }
    output << '\n';
  }

  return output;
}


#endif // MATRIX_HPP