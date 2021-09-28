#pragma once

#include "Tokens_Token.hpp"

//! Represents array.
class TokenArray : public Token{
	std::vector<TokenWrapper *> m_elements; //!< Pointers to elements of the array
	
public:

	/**
	 * Creates instance of an array token
 	 * @param[in]	elements	pointers to elements
	 */
	explicit TokenArray(std::vector<Token *> elements);
	
	/**
	 * Destructor - deletes child tokens
	 */
	~TokenArray();
	
	/**
	 * Throws exception
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	Token * evaluateNumberSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Throws exception
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	Token * evaluateStringSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Returns shallow copy
	 * @param[in,out]	tec	evaluation context
 	 * @returns				shallow copy
	 */
	Token * evaluateArraySub(TokenEvaluationContext & tec);
	
	/**
	 * Throws exception
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	TokenWrapper * evaluateReferenceSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Returns deep copy
	 * @param[in,out]	tec	evaluation context
 	 * @returns				deep copy
	 */
	Token * evaluateInstanceSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Returns whether result of operation is true, that is non-0 number or non-"" string
	 * @param[in,out]	tec	evaluation context
 	 * @returns				true or false
	 */
	bool evaluateBoolSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Returns reference to elememt with matching index, or throws exception
	 * @param[in]		index	index
	 * @param[in,out]	tec		evaluation context
 	 * @returns					result
	 */
	TokenWrapper * getReferenceAtIndexSub(Token * index, TokenEvaluationContext & tec) const override;
	
	/**
	 * Returns string with representation of operands and representation of operator
	 * @returns	approximation of original code
	 */
	std::string printContentSub() const override;
	
	/**
	 * Returns value in printable form
 	 * @returns	printable string
	 */
	std::string toStringSub() const override;
};
