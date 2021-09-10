#pragma once

#include <functional>
#include <map>
#include <stack>
#include <string>
#include <vector>

class Token;
enum Token_Type : unsigned int;
enum Operator_Rank : unsigned int;
class StringEvaluationContext;
class TokenEvaluationContext;

//! Contains various static methods for work with tokens
class StaticMethods{
public:
	/**
	 * Replaces replacement markers ("$n" where n is a whole number larger than or equal to 0)
	 *  with corresponding string in supplied vector
	 * @param[in]	input	string to replace in
	 * @param[in]	params	vector that replacements are picked from
	 * @returns				string where all replacement markers with numbers smaller than size of params are replaced
	 */
	static std::string ReplaceParams(std::string input, const std::vector<std::string> & params);

	/**
	 * Goes through string, splits it into what will later become tokens
	 * @param[in]	code	string to be lexed
	 * @returns				resulting strings
	 */
	static std::vector<std::string> LexicallySplitString(const std::string & code);
	
	/**
	 * Goes through lexed strings, combines operators that form multicharacter operator (++, ||, <=, etc.)
	 * @param[in,out]	target	strings to go through
	 */
	static void CombineMulticharOperators(std::vector<std::string> & target);
	
	/**
	 * Joins vector of strings
	 * @param[in]	glue	string that will be put between every two neighboring strings from 
	 * @param[in]	strings	strings to join
	 * @returns				resulting string
	 */
	static std::string JoinsStrings(const std::string & glue, const std::vector<std::string> & strings);
	
	/**
	 * Encodes string
	 * @param[in]	source	clean, original string
	 * @returns				smudged string
	 */
	static std::string SmudgeString(const std::string & source);
	
	/**
	 * Converts double to string with all valid digits but without unnecessary decimal zeroes
	 * @param[in]	d	double to be converted
	 * @returns			string representing the number
	 */
	static std::string PrettyPrintDouble(double d);
	
	/**
	 * Checks whether function was called with correct argument types
	 * @param[in,out]	tec						variables for use in ReplaceVariable
	 * @param[in]		name					vector of tokens
	 * @param[in]		tokens					expected amount of parameters
	 * @param[in]		paramCountComparator	comparator for testng param count
	 * @param[in]		expectedParamTypes		expected parameter types
	 * @returns									evaluated arguments
	 */
	static std::vector<Token*> TestAndTransformArguments(TokenEvaluationContext & tec, const std::string & name, const std::vector<Token*> & tokens, const std::function <bool(size_t)>& paramCountComparator, std::vector<Token_Type> expectedParamTypes);
	
	/**
	 * Uses shunting-yard algorithm to build tree of tokens
	 * @param[in]	lexedExpression	lexed expression to build from
	 * @returns						pointer to root node
	 */
	static Token * BuildATokenTree(const std::vector<std::string> & lexedExpression);
	
	/**
	 * Attempts to construct control structures
	 * @param[in,out]	operands		stack to take from and put result onto
	 * @param[in,out]	operators		stack to take keywords from
	 * @param[in,out]	argumentCounts	stack with counts of arguments
	 * @param[in]		lexedExpression	original expression
	 */
	static void TryEmplaceControlStructures(std::stack<Token *> & operands, std::stack<long int> & operators, std::stack<int> & argumentCounts, const std::vector<std::string> & lexedExpression);
	
	/**
	 * Takes necessary amount of tokens from stack, creates operator token, places back
	 * @param[in,out]	destination	stack to take from and put onto
	 * @param[in]		opName		"name" of operator
	 * @param[in]		type		type of operator
	 * @param[out]		argCnt		is decremented if operator took two operands
	 */
	static void EmplaceOperator(std::stack<Token*> & destination, const std::string & opName, Operator_Rank type, int & argCnt);
	
	/**
	 * Returns either pointer to native operator in source, or custom operator {"--d", "++d", "-d", "+d"}
	 * @param[in]	operatorIndex	index to look for
	 * @param[in]	source			source to look in if index is not negative
	 * @returns						pointer string that unambiguously describes the operator 
	 */
	static const std::string * OperatorToString(long int operatorIndex, const std::vector<std::string> & source);
	
	/**
	 * Stores custom operators established for removing ambiguity {"--d", "++d", "-d", "+d"}
	 */
	static std::map<long int, std::string> CustomOperators;
};
