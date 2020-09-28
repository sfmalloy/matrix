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
using tokenstack_t = std::stack<std::string>;
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
  {'*', 2}
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
isOperator(const std::string& token);

bool
isOperator(char token);

tokenlist_t
tokenizeMath(tokenlist_t tokens);

bool
isNumber(const std::string& token);

bool
foundMatrix(const std::string& name);

template <typename T>
void
doOp(tokenstack_t& eval, tokenlist_t& results, const std::string& a, const std::string& b, const std::string& opStr);

/**********************************************************************/

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
  else if (g_matrices.find(tokens[0]) != g_matrices.end() || std::isdigit(tokens[0][0]) || tokens[0][0] == '(')
    return evaluate<T>(tokens);
  else
    printError("Command does not exist.");
  
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
  if (foundMatrix(name))
    std::cout << g_matrices.at(name);
  else
    printError("Matrix not found");
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
  if (foundMatrix(name))
    return mat::transpose(g_matrices.at(name));
  else
    return mat::matrix<T>();

}

template <typename T>
mat::matrix<T>
inverse(const tokenlist_t& tokens)
{
  if (tokens.size() != 2)
  {
    printUsage("inverse <name>");
    return mat::matrix<T>();
  }

  std::string name = tokens[1];
  if (foundMatrix(name))
    return mat::inverse(g_matrices.at(name));
  else
    printUsage(name + " does not exist");
  
  return mat::matrix<T>();
}

template <typename T>
mat::matrix<T>
rowEchelon(const tokenlist_t& tokens)
{
  if (tokens.size() != 2)
  {
    printUsage("row_echelon <name>");
    return mat::matrix<T>();
  }
  
  std::string name = tokens[1];
  if (foundMatrix(name))
    return mat::rowEchelon(g_matrices.at(name));
  return mat::matrix<T>();
}

template <typename T>
mat::matrix<T>
reducedRowEchelon(const tokenlist_t& tokens)
{
  if (tokens.size() != 2)
  {
    printUsage("reduced_row_echelon <name>");
    return mat::matrix<T>();
  }

  std::string name = tokens[1];
  if (foundMatrix(name))
    return mat::reducedRowEchelon(g_matrices.at(name));
  return mat::matrix<T>();
}

void
swapRows(const tokenlist_t& tokens)
{
  std::string name = tokens[1];
  size_t r1 = std::stoul(tokens[2]);
  size_t r2 = std::stoul(tokens[3]);
  if (foundMatrix(name))
    g_matrices.at(name).swapRows(r1, r2);
}

void
addRows(const tokenlist_t& tokens)
{
  std::string name = tokens[1];
  size_t r1 = std::stoul(tokens[2]);
  size_t r2 = std::stoul(tokens[3]);
  if (foundMatrix(name))
    g_matrices.at(name).addRows(r1, r2);
}

template <typename T>
void
multiplyRow(const tokenlist_t& tokens)
{
  std::string name = tokens[1];
  size_t row = std::stoul(tokens[2]);
  T scalar = std::stof(tokens[3]);
  if (foundMatrix(name))
    g_matrices.at(name).multiplyRow(row, scalar);
}


// FIXME check size of tokens to prevent seg faults when giving too few args
// FIXME if token after equal sign is only a number, must add to seperate elem_t map.
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
    size_t size = std::stoul(tokens[3]);
    g_matrices[name] = getIdentity<T>(size);
  }
  else if (keyword == "random")
  {
    std::cout << tokens.size() << '\n';
    if (tokens.size() != 7 && tokens.size() != 8)
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
      // Need to cast "Missing " to a std::string or else I can't use operator+ to concatenate "[" or "]"...
      printError((std::string) "Missing " + (openCount < closeCount ? "[" : "]"));
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
    || foundMatrix(keyword) || keyword[0] == '(' || isNumber(keyword))
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
  tokenstack_t eval;
  tokenlist_t results;

  for (auto it = postfix.begin(); it != postfix.end(); ++it)
  {
    if (!isOperator(*it))
      eval.push(*it);
    else
    {
      std::string b = eval.top();
      eval.pop();
      std::string a = eval.top();
      eval.pop();

      doOp<T>(eval, results, a, b, *it);
    }

    if (eval.top() == "__error")
    {
      printError("Evaluation error");
      return mat::matrix<T>();
    }
  }

  return g_matrices[eval.top()];
}

tokenlist_t
toPostfix(tokenlist_t tokens)
{
  tokens = tokenizeMath(tokens);
  // Setup for conversion algorithm
  tokenstack_t stack;
  stack.push("(");
  tokens.push_back(")");

  tokenlist_t expression;
  for (const auto& t : tokens)
  {
    if (isOperator(t))
    {
      int currPrecedence = g_operatorPrecedence.at(t[0]);
			while (isOperator(stack.top()) && currPrecedence <= g_operatorPrecedence.at(stack.top()[0]))
      {
        expression.push_back(stack.top());
        stack.pop();
      }

      stack.push(t);
    }
    else if (t == ")")
    {
      while (stack.top() != "(")
      {
        expression.push_back(stack.top());
        stack.pop();
      }

      stack.pop();
    }
    else if (t == "(")
      stack.push(t);
    else
      expression.push_back(t);
  }

  return expression;
}

tokenlist_t
tokenizeMath(tokenlist_t tokens)
{
  tokenlist_t newTokens;
  // Split parentheses
  for (const auto& t : tokens)
  {
    if (t[0] != '(' && t.back() != ')')
      newTokens.push_back(t);
    else if (t[0] == '(')
    {
      newTokens.push_back("(");
      newTokens.push_back(t.substr(1));
    }
    else
    {
      newTokens.push_back(t.substr(0, t.size() - 1));
      newTokens.push_back(")");
    }
  }

  return newTokens;
}

template <typename T>
void
doOp(tokenstack_t& eval, tokenlist_t& results, const std::string& a, const std::string& b, const std::string& opStr)
{
  char op = opStr[0];
  bool error = false;
  int resId = rand() % 10000;
  std::string resName = "__result" + std::to_string(resId);

  while (std::find(results.begin(), results.end(), resName) != results.end())
  {
    resId = rand() % 10000;
    resName = "__result" + std::to_string(resId);
  }

  if (foundMatrix(a) && foundMatrix(b))
  {
    if (op == '+')
      g_matrices[resName] = g_matrices[a] + g_matrices[b];
    else if (op == '-')
      g_matrices[resName] = g_matrices[a] - g_matrices[b];
    else if (op == '*')
      g_matrices[resName] = g_matrices[a] * g_matrices[b];
    else
      error = true;
    
    if (!error)
      eval.push(resName);
  }
  else if (foundMatrix(a) && isNumber(b))
  {
    if (op == '*')
      g_matrices[resName] = g_matrices[a] * std::stof(b);
    else
      error = true;
    
    if (!error)
      eval.push(resName);
  }
  else if (foundMatrix(b) && isNumber(a))
  {
    if (op == '*')
      g_matrices[resName] = g_matrices[b] * std::stof(a);
    else
      error = true;
    
    if (!error)
      eval.push(resName);
  }
  else
    error = true;

  if (error)
  {
    eval = tokenstack_t();
    eval.push("__error");
  }
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
foundMatrix(const std::string& name)
{
  return (name.substr(0, 2) != "__" || name.find("result") != std::string::npos) && (g_matrices.find(name) != g_matrices.end());
}

bool
isOperator(const std::string& token)
{
  return token.size() == 1 && 
    g_operatorPrecedence.find(token[0]) != g_operatorPrecedence.end();
}

bool
isOperator(char token)
{
  return g_operatorPrecedence.find(token) != g_operatorPrecedence.end();
}

bool
isNumber(const std::string& token)
{
  bool foundDecimal = false;
  for (char c : token)
  {
    if (foundDecimal && c == '.')
      return false;
    else if (!foundDecimal && c == '.')
      foundDecimal = true;
    else if (!std::isdigit(c))
      return false;
  }

  return true;
}
