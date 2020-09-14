///\author Sean Malloy
///\name   main.cpp
///\brief  Matrix class interpreter for general use.
/**********************************************************************/
// System includes
#include <iostream>

/**********************************************************************/
// Local includes
#include "matrix.hpp"

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

int
main()
{
  
  return 0;
}