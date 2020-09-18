///\author Sean Malloy
///\name   main.cpp
///\brief  Matrix class interpreter for general use.
/**********************************************************************/
// System includes
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <random>

/**********************************************************************/
// Local includes
#include "matrix.hpp"

/**********************************************************************/
// Using declarations
using elem_t = float;
using tokenlist_t = std::vector<std::string>;

/**********************************************************************/
// Global variables
std::unordered_map<std::string, mat::matrix<elem_t>> g_matrices;

/**********************************************************************/
// Command declarations

void
printMatrix(tokenlist_t tokens);

void
transpose(tokenlist_t tokens);

void
inverse(tokenlist_t tokens);

void
rowEchelon(tokenlist_t tokens);

void
reducedRowEchelon(tokenlist_t tokens);

void
swapRows(tokenlist_t tokens);

void
addRows(tokenlist_t tokens);

void
multiplyRow(tokenlist_t tokens);

void
equalExpression(tokenlist_t tokens);
/**********************************************************************/
// Helper function declarations

template <typename T>
mat::matrix<T>
getIdentity(size_t size);

template <typename T>
mat::matrix<T>
getRandom(size_t rows, size_t cols, int lower, int upper);

template <typename T>
mat::matrix<T>
getRandom(size_t rows, size_t cols, int lower, int upper, unsigned long seed);

/**********************************************************************/

int
main()
{
  g_matrices = std::unordered_map<std::string, mat::matrix<elem_t>>();

  std::string line;
  while (std::getline(std::cin, line))
  {
    std::stringstream tokenize(line);
    std::string token;
    tokenlist_t tokens;

    while (std::getline(tokenize, token, ' '))
      tokens.push_back(token);

    if (tokens.size() > 0)
    {
      if (tokens[0] == "exit")
        exit(0);
      else if (tokens[0] == "reset")
        g_matrices = std::unordered_map<std::string, mat::matrix<elem_t>>();
      else if (tokens[0] == "print")
        printMatrix(tokens);
      else if (tokens[0] == "transpose")
        transpose(tokens);
      else if (tokens[0] == "inverse")
        inverse(tokens);
      else if (tokens[0] == "row_echelon")
        rowEchelon(tokens);
      else if (tokens[0] == "reduced_row_echelon")
        reducedRowEchelon(tokens);
      else if (tokens[0] == "swap_rows")
        swapRows(tokens);
      else if (tokens[0] == "add_rows")
        addRows(tokens);
      else if (tokens[0] == "multiply_row")
        multiplyRow(tokens);
      else if (tokens.size() > 1 && tokens[1] == "=")
        equalExpression(tokens);
      else
        std::cerr << "No such command.\n" << '\n';
    }  
  }

  return 0;
}

void
printMatrix(tokenlist_t tokens)
{
  std::string name = tokens[1];
  if (g_matrices.find(name) != g_matrices.end())
  {
    mat::matrix<elem_t> A = g_matrices.at(name);
    std::cout << A;
  }
  else
    std::cout << "Matrix not found\n";
  
  std::cout << '\n';
}

void
transpose(tokenlist_t tokens)
{

}

void
inverse(tokenlist_t tokens)
{

}

void
rowEchelon(tokenlist_t tokens)
{

}

void
reducedRowEchelon(tokenlist_t tokens)
{

}

void
swapRows(tokenlist_t tokens)
{

}

void
addRows(tokenlist_t tokens)
{

}

void
multiplyRow(tokenlist_t tokens)
{

}

void
equalExpression(tokenlist_t tokens)
{
  std::string name = tokens[0];
  std::string keyword = tokens[2];
  if (keyword == "identity")
  {
    size_t size = std::stoi(tokens[3]);
    g_matrices[name] = getIdentity<elem_t>(size);
  }
  else if (keyword == "random")
  {
    size_t rows = std::stoul(tokens[3]);
    size_t cols = std::stoul(tokens[4]);
    size_t lowerBound = std::stoul(tokens[5]);
    size_t upperBound = std::stoul(tokens[6]);
    
    if (tokens.size() >= 8)
    {
      unsigned long seed = std::stoul(tokens[7]);
      g_matrices[name] = getRandom<elem_t>(rows, cols, lowerBound, upperBound, seed);
    }
    else
      g_matrices[name] = getRandom<elem_t>(rows, cols, lowerBound, upperBound);

  }
  else if (keyword[0] == '[')
  {
    // Concatenate list together into one string to tokenize list easier
    std::string listStr;
    for (size_t i = 0; i < tokens.size(); ++i)
      if (tokens[i].back() != '=' && i > 0)
        listStr += tokens[i];

    // Count number of '[' and ']', if not equal then break
    // Also count number of commas to figure out number of columns for later
    int openCount = 0;
    int closeCount = 0;
    int commaCount = 0;
    std::string numStr;
    for (const char& c : listStr)
    {
      if (c == '[')
        openCount += 1;
      else if (c == ']')
        closeCount += 1;
      else if (c == ',' && closeCount == 0)
        commaCount += 1;

      if (c != '[' && c != ']')
        numStr.push_back(c);
    }

    if (openCount != closeCount)
    {
      std::cerr << "Missing " << (openCount < closeCount ? '[' : ']') << ".\n";
      return;
    }

    // By doing the above counting loop we now know the number of rows and columns
    size_t numRows = openCount - 1;
    size_t numCols = commaCount + 1;
    
    std::stringstream tokenize(numStr);
    std::string num;
    mat::matrix<elem_t> A(numRows, numCols);
    auto it = A.begin();

    std::cout << numRows << ' ' << numCols << '\n';

    // TODO: don't use stof and check the type to see if using double or float
    while (std::getline(tokenize, num, ','))
      *(it++) = std::stof(num);
    g_matrices[name] = A;
  }
  else if (g_matrices.find(keyword) != g_matrices.end())
    g_matrices[name] = g_matrices[keyword];
}

template <typename T>
mat::matrix<T>
getIdentity(size_t size)
{
  mat::matrix<T> A(size, size);
  for (size_t i = 0; i < size; ++i)
    A(i, i) = 1;

  return A;
}

template <typename T>
mat::matrix<T>
getRandom(size_t rows, size_t cols, int lower, int upper)
{
  std::random_device seed;
  return getRandom<T>(rows, cols, lower, upper, seed());
}

template <typename T>
mat::matrix<T>
getRandom(size_t rows, size_t cols, int lower, int upper, unsigned long seed)
{
  std::mt19937 gen(seed);
  std::uniform_int_distribution<int> dist(lower, upper);
  
  mat::matrix<T> A(rows, cols);
  for (T& elem : A)
    elem = dist(gen);

  return A;
}

/*
 * Command list
 * 
 * Equals operations
 * To print any result after doing an equals operation, use print command
 * <name> = identity <order>
 * <name> = random <rows> <cols> <lower_bound> <upper_bound> [<seed>]
 * <name> = [[0,1,2,3...N],[0,1,2,3,...N],...]
 * <name> = <command_that_returns_matrix>
 * <name> = <mathematical_expression>
 * <name> = <other_name>
 *
 * Commands that return a matrix that can be saved into a new variable
 * or overwrite another variable. If commands are just ran, result is
 * printed and nothing is permanently changed.
 * 
 * transpose <name>
 * inverse <name>
 * row_echelon <name>
 * reduced_row_echelon <name>
 *
 * Commands that do not return a matrix, but instead change the
 * state of a given matrix.
 * 
 * swap_rows <name> <r1> <r2>
 * add_rows <name> <r1> <r2> [<scalar>]
 * multiply_row <name> <r> <scalar>
 * 
 * Misc commands
 * print <name>
 * reset
 * exit
 */