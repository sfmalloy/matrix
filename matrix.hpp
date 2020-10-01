///\author Sean Malloy
///\name   matrix.hpp
///\brief  Generic matrix class for performing various matrix operations.
/**********************************************************************/
// Macro guard
#ifndef MATRIX_HPP
#define MATRIX_HPP

/**********************************************************************/
// System includes
#include <cstddef>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <limits>
#include <cmath>

/**********************************************************************/
namespace mat 
{
  template<typename T>
  class matrix
  {
  public:
    // type aliases
    using iterator = T*;
    using const_iterator = const T*;

    // default ctor
    matrix()
      : m_rows(0),
        m_cols(0),
        m_size(0),
        m_matrix(nullptr)
    {
    }

    // size ctor
    matrix(size_t rows, size_t cols)
      : m_rows(rows),
        m_cols(cols),
        m_size(rows * cols),
        m_matrix(new T[m_size])
    {
    }

    // size ctor
    matrix(size_t rows, size_t cols, T init)
      : m_rows(rows),
        m_cols(cols),
        m_size(rows * cols),
        m_matrix(new T[m_size])
    {
      for (auto& elem : *this)
        elem = init;
    }

    // copy ctor
    matrix(const matrix& m)
      : m_rows(m.rows()),
        m_cols(m.cols()),
        m_size(m.size()),
        m_matrix(new T[m_size])
    {
      if (this != &m)
        std::copy(m.begin(), m.end(), begin());
    }

    // dtor
    ~matrix()
    {
      if (m_matrix != nullptr)
        delete[] m_matrix;
    }

    matrix&
    operator=(const matrix& m)
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
    swapRows(size_t r1, size_t r2)
    {
      if (r1 >= m_rows || r2 >= m_rows)
        return;      
      for (size_t j = 0; j < m_cols; ++j)
        std::swap((*this)(r1, j), (*this)(r2, j));
    }

    // Adds scalar * r1 to r2, changing the values in r2
    void
    addRows(size_t r1, size_t r2, T scalar = T(1))
    {
      if (r1 >= m_rows || r2 >= m_rows)
        return;
      for (size_t j = 0; j < m_cols; ++j) 
      {
        if (almostEqual((*this)(r2, j), -scalar * (*this)(r1, j)))
					(*this)(r2, j) = 0;
        else
					(*this)(r2, j) += scalar * (*this)(r1, j);
      }
    }

    void 
    multiplyRow(size_t r, T scalar)
    {
      for (size_t j = 0; j < m_cols; ++j)
        if ((*this)(r, j) != 0)
          (*this)(r, j) *= scalar;
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

    // matrix addition
    matrix&
    operator+=(const matrix& other)
    {
      if (m_rows == other.rows() && m_cols == other.cols())
      {
        for (size_t i = 0; i < other.rows(); ++i)
          for (size_t j = 0; j < other.cols(); ++j)
            (*this)(i, j) += other(i, j);
      }
      else
        std::cerr << "Incompatible matrices, cannot add";
      

      return *this;
    }

    // matrix addition
    matrix
    operator+(const matrix& other) const
    {
      return matrix(*this) += other;
    }
    
    // matrix subtraction
    matrix&
    operator-=(const matrix& other)
    {
      if (m_rows == other.rows() && m_cols == other.cols())
      {
        for (size_t i = 0; i < other.rows(); ++i)
          for (size_t j = 0; j < other.cols(); ++j)
            (*this)(i, j) -= other(i, j);
      }

      return *this;
    }

    // matrix subtraction
    matrix
    operator-(const matrix& other) const
    {
      return matrix(*this) -= other;
    }

    // matrix multiplication
    matrix&
    operator*=(const matrix& other)
    {
      if (m_cols == other.rows())
      {
        matrix result(m_rows, other.cols(), T(0));
        for (size_t i = 0; i < result.rows(); ++i)
          for (size_t j = 0; j < result.cols(); ++j)
            for (size_t k = 0; k < m_cols; ++k)
              result(i, j) += (*this)(i, k) * other(k, j);
          
        *this = result;
      }
      else
        std::cerr << "Cannot multiply, returning first matrix\n";

      return *this;
    }

    // matrix multiplication
    matrix
    operator*(const matrix& other) const
    {
      return matrix(*this) *= other;
    }

    // Scalar multiplication
    matrix&
    operator*=(const T& k)
    {
      for (auto& elem : *this)
        elem *= k;
      
      return *this;
    }

    // Scalar multiplication
    matrix
    operator*(const T& k) const
    {
      return matrix(*this) *= k;
    }

    // Scalar multiplication
    friend matrix
    operator*(const T& k, const matrix& M)
    {
      return M * k;
    }

    bool
    operator==(const matrix& other)
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
    operator==(const matrix& other) const
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

    bool
    almostEqual(T a, T b)
    {
      T diff = std::fabs(a - b);
      return diff <= std::numeric_limits<T>::epsilon() * std::fabs(a + b) 
              || std::fabs(a - b) < std::numeric_limits<T>::min();
    }
  };
  /**********************************************************************/
  // Global functions

  // Gaussian elimination
  template <typename T>
  matrix<T>
  rowEchelon(matrix<T> A)
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
        A.multiplyRow(currRow, 1.0 / leadingElement);

      for (size_t i = currRow + 1; i < A.rows(); ++i)
      {
        T elem = A(i, currCol);
        if (elem != 0)
          A.addRows(currRow, i, -1.0 * elem);
      }

			std::cout << A;
    }

    return A;
  }

  // Gauss-Jordan elimination
  template <typename T>
  matrix<T>
  reducedRowEchelon(matrix<T> A)
  {
    A = rowEchelon(A);

    for (size_t currBottomRow = A.rows() - 1; currBottomRow > 0; --currBottomRow)
    {
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
        for (size_t i = 0; i < currBottomRow; ++i)
          A.addRows(currBottomRow, i, -A(i, leadingOne));
    }

    return A;
  }

  template <typename T>
  matrix<T>
  transpose(const matrix<T>& A)
  {
    matrix<T> transposed(A.cols(), A.rows());
    
    for (size_t i = 0; i < A.rows(); ++i)
      for (size_t j = 0; j < A.cols(); ++j)
        transposed(j, i) = A(i, j);

    return transposed;
  }

  template <typename T>
  matrix<T>
  inverse(matrix<T>& A)
  {
    if (A.rows() != A.cols())
    {
      std::cerr << "Inverse does not exist.\n";
      return A;
    }

    matrix<T> augmented(A.rows(), 2 * A.cols());
    
    for (size_t i = 0; i < A.rows(); ++i)
    {
      for (size_t j = 0; j < A.cols(); ++j)
        augmented(i, j) = A(i, j);
      
      for (size_t j = A.cols(); j < augmented.cols(); ++j)
        augmented(i, j) = j - A.cols() == i;
    }

    matrix<T> reduced = reducedRowEchelon(augmented);
    matrix<T> inverse(A.rows(), A.cols());

    for (size_t i = 0; i < reduced.rows(); ++i)
      for (size_t j = A.cols(); j < reduced.cols(); ++j)
        inverse(i, j - A.cols()) = reduced(i, j);

    return inverse;
  }

  template <typename T>
  matrix<T>
  augment(const matrix<T>& A, const matrix<T>& B)
  {
    if (A.rows() != B.rows())
    {
      std::cerr << "Number of rows not equal.\n";
      return A;
    }

    matrix<T> augmented(A.rows(), A.cols() + B.cols());

    for (size_t i = 0; i < A.rows(); ++i)
    {
      size_t j;
      for (j = 0; j < A.cols(); ++j)
        augmented(i, j) = A(i, j);
      for ( ; j < augmented.cols(); ++j)
        augmented(i, j) = B(i, j - A.cols());
    }

    return augmented;
  }

  template <typename T>
  std::ostream&
  operator<<(std::ostream& output, const matrix<T>& M)
  {
    output << std::left;
    for (size_t i = 0; i < M.rows(); ++i)
    {
      for (size_t j = 0; j < M.cols(); ++j)
        output << std::setw(10) << M(i, j) << ' ';
      output << '\n';
    }

    return output;
  }
} // namespace mat

#endif // MATRIX_HPP
