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
#include <algorithm>

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
      if (m_matrix != nullptr)
        delete[] m_matrix;
      m_matrix = new T[m.rows() * m.cols()];

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
        transposed(j, i) = (*this)(i, j);

    *this = transposed;
  }

  void
  swapRows(size_t r1, size_t r2)
  {
    for (size_t j = 0; j < m_cols; ++j)
      std::swap((*this)(r1, j), (*this)(r2, j));
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
  Matrix&
  operator+=(const Matrix& other)
  {
    if (this->rows() == other.rows() && this->cols() == other.cols())
    {
      for (size_t i = 0; i < other.rows(); ++i)
        for (size_t j = 0; j < other.cols(); ++j)
          (*this)(i, j) += other(i, j);
    }

    return *this;
  }

  // Matrix addition
  Matrix
  operator+(const Matrix& other) const
  {
    return Matrix(*this) += other;
  }
  
  // Matrix subtraction
  Matrix&
  operator-=(const Matrix& other)
  {
    if (this->rows() == other.rows() && this->cols() == other.cols())
    {
      for (size_t i = 0; i < other.rows(); ++i)
        for (size_t j = 0; j < other.cols(); ++j)
          (*this)(i, j) -= other(i, j);
    }

    return *this;
  }

  // Matrix subtraction
  Matrix
  operator-(const Matrix& other) const
  {
    return Matrix(*this) -= other;
  }

  // Matrix multiplication
  Matrix&
  operator*=(const Matrix& other)
  {
    if (this->cols() == other.rows())
    {
      Matrix result{this->rows(), other.cols()};
      for (size_t i = 0; i < result.rows(); ++i)
        for (size_t j = 0; j < result.cols(); ++j)
          for (size_t k = 0; k < this->cols(); ++k)
            result(i, j) += (*this)(i, k) * other(k, j);
        
      *this = result;
    }

    return *this;
  }

   // Matrix multiplication
  Matrix
  operator*(const Matrix& other) const
  {
    return Matrix(*this) *= other;
  }

  // TODO
  // Scalar *=
  Matrix
  operator*=(const T& other)
  {
    return *this;
  }

  bool
  operator==(const Matrix& other)
  {
    return false;
  }
  
  bool
  operator==(const Matrix& other) const
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
operator*(const T& k, const Matrix<T>& M)
{
  return M;
}

// Scalar multiplication M * k
template <typename T>
Matrix<T>
operator*(const Matrix<T>& M, const T& k)
{
  return M;
}

template <typename T>
std::ostream&
operator<<(std::ostream& output, const Matrix<T>& M)
{
  for (size_t i = 0; i < M.rows(); ++i)
  {
    for (size_t j = 0; j < M.cols(); ++j)
      output << M(i, j) << ' ';
    output << '\n';
  }

  return output;
}


#endif // MATRIX_HPP