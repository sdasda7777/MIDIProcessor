#pragma once

#include "Tokens_Token.hpp"

//! Represents indexation token.
class TokenIndex : public Token{
	Token * m_collection; //!< Pointers to the base collection
	Token * m_index; //!< Pointer to the index
	
public:

	/**
	 * Creates instance of an index token
 	 * @param[in]	lhs	left hand side of the operator
 	 * @param[in]	rhs	right hand side of the operator
	 */
	explicit TokenIndex(Token * collection, Token * index);
	
	/**
	 * Destructor - frees child tokens
	 */
	~TokenIndex(){
		delete m_collection;
		delete m_index;
	}
	
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
	 * Returns result of the operation as an array
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	Token * evaluateArraySub(TokenEvaluationContext & tec);
	
	/**
	 * Returns reference to the value index points at
	 * @param[in,out]	tec	evaluation context
 	 * @returns				result
	 */
	TokenWrapper * evaluateReferenceSub(TokenEvaluationContext & tec) const override;
	
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
	 * Returns stuff
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
