#pragma once

#include "Tokens_Token.hpp"


//! Represents variable token.
class TokenVariable : public Token{
	std::string m_name; //!< Variable name
public:
	/**
	 * Creates instance of a variable token
 	 * @param[in]	n	variable name
	 */
	explicit TokenVariable(std::string n);
	
	/**
	 * Returns value of referenced token as a number
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	Token * evaluateNumberSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Returns value of referenced token as a string
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	Token * evaluateStringSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Returns value of referenced token as an array
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	Token * evaluateArraySub(TokenEvaluationContext & tec);
	
	/**
	 * Returns reference
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	TokenWrapper * evaluateReferenceSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Returns value of referenced token, type independent
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	Token * evaluateInstanceSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Returns whether referenced value is true, that is non-0 number or non-"" string
	 * @param[in,out]	tec	evaluation context
 	 * @returns				true or false
	 */
	bool evaluateBoolSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Throws exception
	 * @param[in]		index	index
	 * @param[in,out]	tec		evaluation context
 	 * @returns					result
	 */
	TokenWrapper * getReferenceAtIndexSub(Token * index, TokenEvaluationContext & tec) const override;
	
	/**
	 * Returns variable name
	 * @returns	variable name
	 */
	std::string printContentSub() const override;
	
	/**
	 * Returns value in printable form
 	 * @returns	printable string
	 */
	std::string toStringSub() const override;
};
