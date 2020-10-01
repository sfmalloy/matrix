///\author Sean Malloy
///\name	 main.cpp
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
using tokenlist_t = std::vector<std::string>;
using tokenstack_t = std::stack<std::string>;
using matmap_t = std::unordered_map<std::string, mat::matrix>;

/**********************************************************************/
// Global variables
matmap_t g_matrices;

/**********************************************************************/
// Global constants

// List of matrix functions that return a new matrix. These can be used
// to store into a new matrix variable
const std::string g_newMatFunctions[4]
{
	"transpose",
	"inverse",
	"row_echelon",
	"reduced_row_echelon"
};

// Used when converting math expressions from infix to postfix for
// easy computation while keeping order of operations.
const std::unordered_map<char, int> g_operatorPrecedence
{
	{'+', 1},
	{'-', 1},
	{'*', 2}
};

/**********************************************************************/
// Command declarations

/// \brief Prints matrix in easy to read form
/// \param tokens contains expression with name of matrix to print
/// \note Error message printed if name specified in tokens does not exist.
///
/// print <name>
/// TODO expand to be able to print result of any command that returns a matrix.
void
printMatrix(const tokenlist_t& tokens);

mat::matrix
random(const tokenlist_t& tokens);

mat::matrix
identity(const tokenlist_t& tokens);

/// \brief Swaps ros and columns of given matrix
/// \param tokens contains name of matrix to transpose
/// \return Transposed matrix if matrix exists, otherwise empty matrix
///
/// transpose <name>
mat::matrix
transpose(const tokenlist_t& tokens);

/// \brief Find inverse of matrix if it exists
/// \param tokens contains name of matrix to invert
/// \return Inverse of matrix if matrix exists and can be inverted, 
///		otherwise empty matrix.
///
/// inverse <name>
mat::matrix
inverse(const tokenlist_t& tokens);

/// \brief Transform matrix to have leading ones and zeros
///		below the leading ones.
/// \param tokens contains name of matrix to transform
/// \return Matrix in row echelon form if it exists, otherwise empty matrix.
///
/// row_echelon <name>
mat::matrix
rowEchelon(const tokenlist_t& tokens);

/// \brief Transform matrix to have leading ones and zeros 
///		both below and above the leading ones.
/// \param tokens contains name of matrix to transform
/// \return Matrix in reduced row echelon form if it exists, otherwise empty matrix.
///
/// reduced_row_echelon <name>
mat::matrix
reducedRowEchelon(const tokenlist_t& tokens);

/// \brief Swap two rows in a matrix
/// \param tokens contains name of matrix to swap two rows in
/// 
/// swap_rows <name> <row1> <row2>
void
swapRows(const tokenlist_t& tokens);

mat::matrix
augment(const tokenlist_t& tokens);

void
addRows(const tokenlist_t& tokens);

void
multiplyRow(const tokenlist_t& tokens);

void
equalExpression(const tokenlist_t& tokens);

/**********************************************************************/
// Helper function declarations

mat::matrix
doCommand(const tokenlist_t& tokens);

mat::matrix
getIdentity(size_t size);

mat::matrix
getRandom(size_t rows, size_t cols, int lower, int upper);

mat::matrix
getRandom(size_t rows, size_t cols, int lower, int upper, unsigned long seed);

mat::matrix
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

		if (tokens.size() > 0 && tokens[0] != "exit")
			doCommand(tokens);
		else if (tokens.size() > 0 && tokens[0] == "exit")
			break;
		
		std::cout << "mat> ";
	}

	return 0;
}

mat::matrix
doCommand(const tokenlist_t& tokens)
{
	if (tokens[0] == "reset")
		g_matrices = matmap_t();
	else if (tokens[0] == "print")
		printMatrix(tokens);
	else if (tokens[0] == "transpose")
		return transpose(tokens);
	else if (tokens[0] == "inverse")
		return inverse(tokens);
	else if (tokens[0] == "row_echelon")
		return rowEchelon(tokens);
	else if (tokens[0] == "reduced_row_echelon")
		return reducedRowEchelon(tokens);
	else if (tokens[0] == "swap_rows")
		swapRows(tokens);
	else if (tokens[0] == "add_rows")
		addRows(tokens);
	else if (tokens[0] == "multiply_row")
		multiplyRow(tokens);
	else if (tokens[0] == "random")
		return random(tokens);
	else if (tokens[0] == "identity")
		return identity(tokens);
	else if (tokens[0] == "augment")
		return augment(tokens);
	else if (tokens.size() > 1 && tokens[1] == "=")
		equalExpression(tokens);
	else if (g_matrices.find(tokens[0]) != g_matrices.end() || std::isdigit(tokens[0][0]) || tokens[0][0] == '(')
		return evaluate(tokens);
	else
		printError("Command does not exist.");
	
	return mat::matrix();
}

void
printMatrix(const tokenlist_t& tokens)
{	
	std::cout << doCommand(tokenlist_t(tokens.begin() + 1, tokens.end()));
}

mat::matrix
transpose(const tokenlist_t& tokens)
{
	if (tokens.size() != 2)
	{
		printUsage("transpose <name>");
		return mat::matrix();
	}

	std::string name = tokens[1];
	if (foundMatrix(name))
		return mat::transpose(g_matrices.at(name));
	else
		return mat::matrix();
}

mat::matrix
inverse(const tokenlist_t& tokens)
{
	if (tokens.size() != 2)
	{
		printUsage("inverse <name>");
		return mat::matrix();
	}

	std::string name = tokens[1];
	if (foundMatrix(name))
		return mat::inverse(g_matrices.at(name));
	else
		printError("Matrix " + name + " not found");
	
	return mat::matrix();
}

mat::matrix
rowEchelon(const tokenlist_t& tokens)
{
	if (tokens.size() != 2)
	{
		printUsage("row_echelon <name>");
		return mat::matrix();
	}
	
	std::string name = tokens[1];
	if (foundMatrix(name))
		return mat::rowEchelon(g_matrices.at(name));
	else
		printError("Matrix " + name + " not found");

	return mat::matrix();
}

mat::matrix
reducedRowEchelon(const tokenlist_t& tokens)
{
	if (tokens.size() != 2)
	{
		printUsage("reduced_row_echelon <name>");
		return mat::matrix();
	}

	std::string name = tokens[1];
	if (foundMatrix(name))
		return mat::reducedRowEchelon(g_matrices.at(name));
	else
		printError("Matrix " + name + " not found");

	return mat::matrix();
}

mat::matrix
augment(const tokenlist_t& tokens)
{
	if (tokens.size() != 3)
	{
		printUsage("augment <name1> <name2>");
		return mat::matrix();
	}

	std::string name1 = tokens[1];
	std::string name2 = tokens[2];
	if (foundMatrix(name1) && foundMatrix(name2))
		return mat::augment(g_matrices.at(name1), g_matrices.at(name2));
	else
		printError("Not all matrices found");

	return mat::matrix();
}

void
swapRows(const tokenlist_t& tokens)
{
	if (tokens.size() != 3)
	{
		printUsage("swap_rows <name> <r1> <r2>");
		return;
	}

	std::string name = tokens[1];
	size_t r1 = std::stoul(tokens[2]);
	size_t r2 = std::stoul(tokens[3]);
	if (foundMatrix(name))
		g_matrices.at(name).swapRows(r1, r2);
	else
		printError("Matrix " + name + " not found");
}

void
addRows(const tokenlist_t& tokens)
{
	if (tokens.size() != 4)
	{
		printUsage("add_rows <name> <row1> <row2>");
		return;
	}

	std::string name = tokens[1];
	size_t r1 = std::stoul(tokens[2]);
	size_t r2 = std::stoul(tokens[3]);
	if (foundMatrix(name))
		g_matrices.at(name).addRows(r1, r2);
	else
		printError("Matrix " + name + " not found");
}

void
multiplyRow(const tokenlist_t& tokens)
{
	if (tokens.size() != 4)
	{
		printUsage("multiply_row <name> <scalar> <row>");
		return;
	}

	std::string name = tokens[1];
	size_t row = std::stoul(tokens[2]);
	T scalar = std::stof(tokens[3]);
	if (foundMatrix(name))
		g_matrices.at(name).multiplyRow(row, scalar);
	else
		printError("Matrix " + name + " not found");
}

mat::matrix
random(const tokenlist_t& tokens)
{
	if (tokens.size() != 5 && tokens.size() != 6)
	{
		printUsage("random <rows> <cols> <lower_bound> <upper_bound> [<seed>]");
		return mat::matrix();
	}

	size_t rows = std::stoul(tokens[1]);
	size_t cols = std::stoul(tokens[2]);
	size_t lowerBound = std::stoul(tokens[3]);
	size_t upperBound = std::stoul(tokens[4]);
	
	if (tokens.size() == 8)
	{
		unsigned long seed = std::stoul(tokens[5]);
		return getRandom(rows, cols, lowerBound, upperBound, seed);
	}
	else
		return getRandom(rows, cols, lowerBound, upperBound);
}

mat::matrix
identity(const tokenlist_t& tokens)
{
	if (tokens.size() != 2)
	{
		printUsage("identity <size>");
		return mat::matrix();
	}

	size_t size = std::stoul(tokens[1]);
	return getIdentity(size);
}

void
equalExpression(const tokenlist_t& tokens)
{
	std::string name = tokens[0];
	std::string keyword = tokens[2];
	if (keyword[0] == '[')
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
		mat::matrix A(numRows, numCols);
		auto it = A.begin();

		while (std::getline(tokenize, num, ','))
			*(it++) = std::stof(num);
		g_matrices[name] = A;
	}
	else
		g_matrices[name] = doCommand(tokenlist_t(tokens.begin() + 2, tokens.end()));
}

mat::matrix
getIdentity(size_t size)
{
	mat::matrix A(size, size);
	for (size_t i = 0; i < A.rows(); ++i)
		for (size_t j = 0; j < A.cols(); ++j)
			A(i, j) = i == j;

	return A;
}

mat::matrix
getRandom(size_t rows, size_t cols, int lower, int upper)
{
	std::random_device seed;
	return getRandom(rows, cols, lower, upper, seed());
}

mat::matrix
getRandom(size_t rows, size_t cols, int lower, int upper, unsigned long seed)
{
	std::mt19937 gen(seed);
	std::uniform_int_distribution<int> dist(lower, upper);
	
	mat::matrix A(rows, cols);
	for (T& elem : A)
		elem = dist(gen);

	return A;
}

mat::matrix
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

			doOp(eval, results, a, b, *it);
		}

		if (eval.top() == "__error")
		{
			printError("Evaluation error");
			return mat::matrix();
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
	return (name.substr(0, 2) != "__" || name.find("result") != std::string::npos) 
		&& (g_matrices.find(name) != g_matrices.end());
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
