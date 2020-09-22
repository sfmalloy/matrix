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
#include <stack>
#include <cctype>

/**********************************************************************/
// Local includes
#include "matrix.hpp"

/**********************************************************************/
// Using declarations
using elem_t = float;
using tokenlist_t = std::vector<std::string>;
using matmap_t = std::unordered_map<std::string, mat::matrix<elem_t>>;

/**********************************************************************/
// Global variables
matmap_t g_matrices;

/**********************************************************************/
// Global constants

const std::string g_newMatFunctions[4]
{
  "transpose",
  "inverse",
  "row_echelon",
  "reduced_row_echelon"
};

const std::unordered_map<char, int> g_operatorPrecedence
{
  {'+', 1},
  {'-', 1},
  {'*', 2},
  {'^', 3},
  {'(', 4},
  {')', 4}
};

/**********************************************************************/
// Command declarations

void
printMatrix(const tokenlist_t& tokens);

template <typename T>
mat::matrix<T>
transpose(const tokenlist_t& tokens);

template <typename T>
mat::matrix<T>
inverse(const tokenlist_t& tokens);

template <typename T>
mat::matrix<T>
rowEchelon(const tokenlist_t& tokens);

template <typename T>
mat::matrix<T>
reducedRowEchelon(const tokenlist_t& tokens);

void
swapRows(const tokenlist_t& tokens);

void
addRows(const tokenlist_t& tokens);

template <typename T>
void
multiplyRow(const tokenlist_t& tokens);

template <typename T>
void
equalExpression(const tokenlist_t& tokens);

/**********************************************************************/
// Helper function declarations

template <typename T>
mat::matrix<T>
doCommand(const tokenlist_t& tokens);

template <typename T>
mat::matrix<T>
getIdentity(size_t size);

template <typename T>
mat::matrix<T>
getRandom(size_t rows, size_t cols, int lower, int upper);

template <typename T>
mat::matrix<T>
getRandom(size_t rows, size_t cols, int lower, int upper, unsigned long seed);

template <typename T>
mat::matrix<T>
evaluate(const tokenlist_t& tokens);

void
printUsage(const std::string& usage);

void
printError(const std::string& error);

tokenlist_t
toPostfix(tokenlist_t tokens);

bool
isOperator(char token);

/**********************************************************************/

// TODO allow for file input
int
main()
{
  g_matrices = matmap_t();

  std::string line;
  std::cout << "mat> ";
  while (std::getline(std::cin, line))
  {
    std::stringstream tokenize(line);
    std::string token;
    tokenlist_t tokens;

    while (std::getline(tokenize, token, ' '))
      tokens.push_back(token);

    if (tokens.size() > 0)
      doCommand<elem_t>(tokens);
    
    std::cout << "mat> ";
  }

  return 0;
}

template <typename T>
mat::matrix<T>
doCommand(const tokenlist_t& tokens)
{
  if (tokens[0] == "exit")
    exit(0);
  else if (tokens[0] == "reset")
    g_matrices = matmap_t();
  else if (tokens[0] == "print")
    printMatrix(tokens);
  else if (tokens[0] == "transpose")
    return transpose<T>(tokens);
  else if (tokens[0] == "inverse")
    return inverse<T>(tokens);
  else if (tokens[0] == "row_echelon")
    return rowEchelon<T>(tokens);
  else if (tokens[0] == "reduced_row_echelon")
    return reducedRowEchelon<T>(tokens);
  else if (tokens[0] == "swap_rows")
    swapRows(tokens);
  else if (tokens[0] == "add_rows")
    addRows(tokens);
  else if (tokens[0] == "multiply_row")
    multiplyRow<T>(tokens);
  else if (tokens.size() > 1 && tokens[1] == "=")
    equalExpression<T>(tokens);
  else if (g_matrices.find(tokens[0]) != g_matrices.end() || std::isdigit(tokens[0][0]))
    return evaluate<T>(tokens);
  else
    std::cerr << "No such command.\n" << '\n';
  
  return mat::matrix<T>();
}

void
printMatrix(const tokenlist_t& tokens)
{
  if (tokens.size() != 2)
  {
    printUsage("print <name>");
    return;
  }

  std::string name = tokens[1];
  if (g_matrices.find(name) != g_matrices.end())
  {
    mat::matrix<elem_t> A = g_matrices.at(name);
    std::cout << A;
  }
  else
    std::cout << "Matrix not found\n";
}

template <typename T>
mat::matrix<T>
transpose(const tokenlist_t& tokens)
{
  if (tokens.size() != 2)
  {
    printUsage("transpose <name>");
    return mat::matrix<T>();
  }

  std::string name = tokens[1];
  if (g_matrices.find(name) != g_matrices.end())
    return mat::transpose(g_matrices.at(name));
  else
  {
    return mat::matrix<T>();
  }

}

template <typename T>
mat::matrix<T>
inverse(const tokenlist_t& tokens)
{
  if (tokens.size() != 2)
  {
    printUsage("inverse <name>");
    return;
  }

  std::string name = tokens[1];
  if (g_matrices.find(name) != g_matrices.end())
    return mat::inverse(g_matrices.at(name));
  else
    printUsage("No such matrix");
  
  return mat::matrix<T>();
}

template <typename T>
mat::matrix<T>
rowEchelon(const tokenlist_t& tokens)
{
  std::string name = tokens[1];
  if (g_matrices.find(name) != g_matrices.end())
    return mat::rowEchelon(g_matrices.at(name));
  return mat::matrix<T>();
}

template <typename T>
mat::matrix<T>
reducedRowEchelon(const tokenlist_t& tokens)
{
  std::string name = tokens[1];
  if (g_matrices.find(name) != g_matrices.end())
    return mat::reducedRowEchelon(g_matrices.at(name));
  return mat::matrix<T>();
}

void
swapRows(const tokenlist_t& tokens)
{
  std::string name = tokens[1];
  size_t r1 = std::stoul(tokens[2]);
  size_t r2 = std::stoul(tokens[3]);
  if (g_matrices.find(name) != g_matrices.end())
    g_matrices.at(name).swapRows(r1, r2);
}

void
addRows(const tokenlist_t& tokens)
{
  std::string name = tokens[1];
  size_t r1 = std::stoul(tokens[2]);
  size_t r2 = std::stoul(tokens[3]);
  if (g_matrices.find(name) != g_matrices.end())
    g_matrices.at(name).addRows(r1, r2);
}

template <typename T>
void
multiplyRow(const tokenlist_t& tokens)
{
  std::string name = tokens[1];
  size_t row = std::stoul(tokens[2]);
  T scalar = std::stof(tokens[3]);
  if (g_matrices.find(name) != g_matrices.end())
    g_matrices.at(name).multiplyRow(row, scalar);
}


// FIXME check size of tokens to prevent seg faults when giving too few args
template <typename T>
void
equalExpression(const tokenlist_t& tokens)
{
  std::string name = tokens[0];
  std::string keyword = tokens[2];
  if (keyword == "identity")
  {
    if (tokens.size() != 4)
    {
      printUsage("identity <size>");
      return;
    }
    size_t size = std::stoi(tokens[3]);
    g_matrices[name] = getIdentity<T>(size);
  }
  else if (keyword == "random")
  {
    if (tokens.size() != 7 || tokens.size() != 8)
    {
      printUsage("random <rows> <cols> <lower_bound> <upper_bound> [<seed>]");
      return;
    }

    size_t rows = std::stoul(tokens[3]);
    size_t cols = std::stoul(tokens[4]);
    size_t lowerBound = std::stoul(tokens[5]);
    size_t upperBound = std::stoul(tokens[6]);
    
    if (tokens.size() == 8)
    {
      unsigned long seed = std::stoul(tokens[7]);
      g_matrices[name] = getRandom<T>(rows, cols, lowerBound, upperBound, seed);
    }
    else
      g_matrices[name] = getRandom<T>(rows, cols, lowerBound, upperBound);

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
      printError("Missing " + (openCount < closeCount ? '[' : ']'));
      return;
    }

    // By doing the above counting loop we now know the number of rows and columns
    size_t numRows = openCount - 1;
    size_t numCols = commaCount + 1;
    
    std::stringstream tokenize(numStr);
    std::string num;
    mat::matrix<T> A(numRows, numCols);
    auto it = A.begin();

    // FIXME: Don't use only stof. Check the type to see if using double or float (or int I guess).
    while (std::getline(tokenize, num, ','))
      *(it++) = std::stof(num);
    g_matrices[name] = A;
  } 
  else if (std::find(std::begin(g_newMatFunctions), std::end(g_newMatFunctions), keyword) != std::end(g_newMatFunctions) 
    || g_matrices.find(keyword) != g_matrices.end())
    g_matrices[name] = doCommand<T>(tokenlist_t(tokens.begin() + 2, tokens.end()));
  else
    printError("Invalid expression");
}

template <typename T>
mat::matrix<T>
getIdentity(size_t size)
{
  mat::matrix<T> A(size, size);
  for (size_t i = 0; i < A.rows(); ++i)
    for (size_t j = 0; j < A.cols(); ++j)
      A(i, j) = i == j;

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

template <typename T>
mat::matrix<T>
evaluate(const tokenlist_t& tokens)
{
  tokenlist_t postfix = toPostfix(tokens);

  for (const auto& t : postfix)
    std::cout << t << ' ';
  std::cout << '\n';

  return mat::matrix<T>();
}

// supported operators in order: ^ * + -
tokenlist_t
toPostfix(tokenlist_t tokens)
{
  for (const auto& t : tokens)
    std::cout << t << '\n';

  // Setup for conversion algorithm
  std::stack<std::string> stack;
  stack.push("(");
  tokens.push_back(")");

  tokenlist_t expression;
  for (const auto& t : tokens)
  {
    if (t[0] == '(')
      stack.push(t);
    
    if (isOperator(t[0]) || t[0] == ')')
    {
      int precedence = g_operatorPrecedence.at(t[0]);
      if (isOperator(stack.top()[0]))
      {
        int currPrecedence = g_operatorPrecedence.at(stack.top()[0]);
        while (precedence >= currPrecedence && stack.top()[0] != '(')
        {
          expression.push_back(stack.top());
          stack.pop();
          currPrecedence = g_operatorPrecedence.at(stack.top()[0]);
        }
      }
      if (isOperator(t[0]))
        stack.push(t);
      else
        stack.pop();      
    }
    else
      expression.push_back(t);
  }

  return expression;
}

void
printUsage(const std::string& usage)
{
  std::cerr << "Usage: " << usage << '\n';
}

void
printError(const std::string& error)
{
  std::cerr << error << '\n';
}

bool
isOperator(char token)
{
  return token == '^' || token == '*' || token == '+' || token == '-' ;
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