#pragma once

#include "Tokens_Token.hpp"

//! Represents string token.
class TokenString : public Token{
	std::string m_value; //!< String value (clean)
public:
	/**
	 * Creates instance of a string token
 	 * @param[in]	v				string value
 	 * @param[in]	already_clean	whether given value is already clean, therefore should not be cleaned
	 */
	explicit TokenString(std::string v, bool already_clean = false);
	
	/**
	 * Throws exception
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	Token * evaluateNumberSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Returns held value
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	Token * evaluateStringSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Throws exception
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	Token * evaluateArraySub(TokenEvaluationContext & tec);
	
	/**
	 * Throws exception
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	TokenWrapper * evaluateReferenceSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Returns value, same as evaluateStringSub
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	Token * evaluateInstanceSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Returns true if value is not ""
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
	 * Returns string
	 * @returns	string
	 */
	std::string printContentSub() const override;
	
	/**
	 * Returns value in printable form
 	 * @returns	printable string
	 */
	std::string toStringSub() const override;
	
	/**
	 * Returns clean value
 	 * @returns	clean string
	 */
	std::string getCleanString() const;
};
