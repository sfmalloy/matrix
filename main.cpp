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

/**********************************************************************/
// Local includes
#include "matrix.hpp"

/**********************************************************************/
// Using declarations
using matrix_t = float;
using tokenlist_t = std::vector<std::string>;

/**********************************************************************/
// Global variables
std::unordered_map<std::string, mat::matrix<matrix_t>> g_matrices;

/**********************************************************************/
// Forward declarations

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

int
main()
{
  g_matrices = std::unordered_map<std::string, mat::matrix<matrix_t>>();

  std::string line;
  while (std::getline(std::cin, line))
  {
    std::stringstream tokenize(line);
    std::string token;
    tokenlist_t tokens;

    while (std::getline(tokenize, token, ' '))
      tokens.push_back(token);

    if (tokens[0] == "exit")
      exit(0);
    else if (tokens[0] == "reset")
      g_matrices = std::unordered_map<std::string, mat::matrix<matrix_t>>();
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
    else
      equalExpression(tokens);
      
  }

  return 0;
}

void
printMatrix(tokenlist_t tokens)
{
  
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

}

/*
 * Command list
 * 
 * Equals operations
 * To print any result after doing an equals operation, use print command
 * <name> = identity <order>
 * <name> = random <rows> <cols> [<lower_bound>] [<upper_bound>]
 * <name> = custom <rows> <cols> EOL
 *    row0: <value00>,<value01>,...,<value0N>
 *    ...
 *    rowM: <valueM0>,<valueM1>,...,<valueMN>
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