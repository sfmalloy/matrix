///\author Sean Malloy
///\name   Matrix.hpp
///\brief  Generic Matrix class for performing various matrix operations.
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
#include <limits>
#include <cmath>

// REMOVE BEFORE RELEASE
#include <cstdio>

/**********************************************************************/
namespace mat 
{
  template<typename T>
  class Matrix
  {
  public:
    // type aliases
    using iterator = T*;
    using const_iterator = const T*;

    // default ctor
    Matrix()
      : m_rows(0),
        m_cols(0),
        m_size(0),
        m_matrix(nullptr)
    {
    }

    // size ctor
    Matrix(size_t rows, size_t cols)
      : m_rows(rows),
        m_cols(cols),
        m_size(rows * cols),
        m_matrix(new T[m_size])
    {
    }

    // copy ctor
    Matrix(const Matrix& m)
      : m_rows(m.rows()),
        m_cols(m.cols()),
        m_size(m.size()),
        m_matrix(new T[m_size])
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

    // Adds factor * r1 to r2, changing the values in r2
    void
    addRows(size_t r1, size_t r2, T factor)
    {
      for (size_t j = 0; j < m_cols; ++j) {
        // When using floats, this if statement is required so
        // rounding errors don't occur.
        T a = (*this)(r2, j);
        T b = -factor * (*this)(r1, j);
        T diff = std::fabs(a - b);
        
        // See https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
        if (diff <= std::numeric_limits<T>::epsilon() * std::fabs(a + b) * 2 
            || std::fabs(a - b) < std::numeric_limits<T>::min())
          (*this)(r2, j) = 0;
        else
          (*this)(r2, j) += factor * (*this)(r1, j);
      }
    }

    void 
    multiplyRow(size_t r, T factor)
    {
      for (size_t j = 0; j < m_cols; ++j)
      {
        if ((*this)(r, j) != 0)
          (*this)(r, j) *= factor;
      }
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
      if (m_rows == other.rows() && m_cols == other.cols())
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
      if (m_rows == other.rows() && m_cols == other.cols())
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
      if (m_cols == other.rows())
      {
        Matrix result(m_rows, other.cols());
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

    // Scalar multiplication
    Matrix&
    operator*=(const T& k)
    {
      for (auto& elem : *this)
        elem *= k;
      
      return *this;
    }

    // Scalar multiplication
    Matrix
    operator*(const T& k) const
    {
      return Matrix(*this) *= k;
    }

    // Scalar multiplication
    friend Matrix
    operator*(const T& k, const Matrix& M)
    {
      return M * k;
    }

    bool
    operator==(const Matrix& other)
    {
      if (m_rows != other.rows() || m_cols != other.cols())
        return false;

      for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
          if ((*this)(i, j) != other(i, j))
            return false;

      return true;
    }
    
    bool
    operator==(const Matrix& other) const
    {
      return *this == other;
    }

    bool
    isRowEchelonForm()
    {
      size_t prevCol = 0;
      for (size_t i = 0; i < m_rows; ++i)
      {
        for (size_t j = 0; j < m_cols; ++j)
        {
          T elem = (*this)(i, j);
          if (elem == 1 && j > prevCol)
          {
            prevCol = j;
            break;
          }
          else if (elem != 0)
            return false;
        }
      }

      return true;   
    }

  private:
    size_t m_rows;
    size_t m_cols;
    size_t m_size;

    T* m_matrix;
  };
  /**********************************************************************/
  // Global functions

  // Gaussian elimination
  template <typename T>
  Matrix<T>
  rowEchelon(Matrix<T> A)
  {
    if (A.isRowEchelonForm())
      return A;

    for (size_t currTopRow = 0; currTopRow < A.rows(); ++currTopRow)
    {
      size_t currRow = currTopRow;
      size_t currCol = 0;
      
      bool found = false;
      for (size_t j = 0; j < A.cols(); ++j)
      {
        for (size_t i = currTopRow; i < A.rows(); ++i)
        {
          if (A(i, j) != 0)
          {
            found = true;
            currRow = i;
            currCol = j;
            break;
          }
        }

        if (found)
          break;
      }

      if (!found)
        return A;

      if (currRow != currTopRow)
      {
        A.swapRows(currRow, currTopRow);
        currRow = currTopRow;
      }

      T leadingElement = A(currRow, currCol);
      if (leadingElement != 1)
        A.multiplyRow(currRow, 1 / leadingElement);

      for (size_t i = currRow + 1; i < A.rows(); ++i)
      {
        // std::cout << i << '\n';
        T elem = A(i, currCol);
        if (elem != 0)
          A.addRows(currRow, i, -elem);
      }
    }

    return A;
  }

  // TODO
  template <typename T>
  Matrix<T>
  reducedRowEchelon(Matrix<T> A)
  {
    A = rowEchelon(A);

    for (size_t currBottomRow = A.rows() - 1; currBottomRow > 0; --currBottomRow)
    {
      // check to see if row is all zeros
      bool allZeros = true;
      size_t leadingOne = 0;
      for (size_t j = 0; j < A.cols(); ++j)
      {
        if (A(currBottomRow, j) != 0)
        {
          allZeros = false;
          leadingOne = j;
          break;
        }
      }

      if (!allZeros)
      {
        for (size_t i = 0; i < currBottomRow; ++i)
          A.addRows(currBottomRow, i, -A(i, leadingOne));
      }
    }

    return A;
  }

  // TODO
  template <typename T>
  Matrix<T>
  inverse(Matrix<T>& A)
  {
    return A;
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
} // namespace mat

#endif // MATRIX_HPP