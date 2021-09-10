#pragma once

#include "Tokens_Token.hpp"

//! Represents condition.
class TokenIf : public Token{
	Token * m_condition; //!< Pointer to condition of the if
	Token * m_if; //!< Pointer to expression to be evaluated if true
	Token * m_else; //!< Pointer to expression to be evaluated otherwise
	
public:

	/**
	 * Creates instance of an if token
 	 * @param[in]	condition	pointer to condition
 	 * @param[in]	ifcode		pointer to if block
  	 * @param[in]	elsecode	pointer to else block
	 */
	explicit TokenIf(Token * condition, Token * ifcode, Token * elsecode);
	
	/**
	 * Destructor - frees child tokens
	 */
	~TokenIf();
	
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
	 * Returns result of the operation, type independent
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	Token * evaluateInstanceSub(TokenEvaluationContext & tec) const override;
	
	/**
	 * Returns whether result of operation is true, that is non-0 number or non-"" string
	 * @param[in,out]	tec	evaluation context
 	 * @returns				true or false
	 */
	bool evaluateBoolSub(TokenEvaluationContext & tec) const override;
	
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
	
	/**
	 * Returns into which category operator belongs
 	 * @returns	operator rank
	 */
	Operator_Rank getRank() const;
};
