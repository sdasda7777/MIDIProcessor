#pragma once

#include "Tokens_Token.hpp"

//! Represents scope token.
class TokenScope : public Token{
	std::vector<Token *> m_content; //!< Pointers to expressions within the scope
	
public:

	/**
	 * Creates instance of a scope token
 	 * @param[in]	cntnt	expressions in the scope
	 */
	explicit TokenScope(std::vector<Token *> cntnt);
	
	/**
	 * Destructor - frees child tokens
	 */
	~TokenScope();
	
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
	 * Throws exception
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
	 * Throws exception
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
	
	/**
	 * Returns into which category operator belongs
 	 * @returns	operator rank
	 */
	Operator_Rank getRank() const;
};
