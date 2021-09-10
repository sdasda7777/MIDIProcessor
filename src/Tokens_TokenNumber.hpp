#pragma once

#include "Tokens_Token.hpp"


//! Represents number token.
class TokenNumber : public Token{
	double m_value; //!< Number value
public:
	/**
	 * Creates instance of a number token
 	 * @param[in]	d	value
	 */
	explicit TokenNumber(double d);
	
	/**
	 * Returns result of the operation as a number
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	Token * evaluateNumberSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Returns result of the operation as a string
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	Token * evaluateStringSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Returns result of the operation as a variable name
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	Token * evaluateVarnameSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Returns value, same as evaluateNumberSub
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	Token * evaluateInstanceSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Returns true if value is not 0
	 * @param[in,out]	tec	evaluation context
 	 * @returns				true or false
	 */
	bool evaluateBoolSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Returns number
	 * @returns	number
	 */
	std::string printContentSub() const override;
	
	/**
	 * Returns value in printable form
 	 * @returns	printable string
	 */
	std::string toStringSub() const override;
	
	/**
	 * Returns token value
 	 * @returns	token value
	 */
	double getValue() const;
	
	/**
	 * Sets token value to new value
 	 * @param[in]	d	new value
	 */
	void setValue(double d);
	
	/**
	 * Increments token value by given number
 	 * @param[in]	d	increment
	 */
	void increment(double d);
	
	/**
	 * Decrements token value by given number
 	 * @param[in]	d	decrement
	 */
	void decrement(double d);
};
