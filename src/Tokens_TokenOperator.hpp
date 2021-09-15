#pragma once

#include "Tokens_Token.hpp"


enum Operator_Rank : unsigned int { PostIncDec, PrefIncDecNegations, Power, MulDivMod, AddSubCon, LtGt, EqNeq, BitAND, BitXOR, BitOR, LogAND, LogOR, Assignment};

//! Represents operator token.
class TokenOperator : public Token{
	unsigned int m_value; //!< Operator type
	Token * m_operands[2] = {nullptr, nullptr}; //!< Pointers to the operands
	
public:

	/**
	 * Creates instance of an operator token
 	 * @param[in]	val	value
 	 * @param[in]	lhs	left hand side of the operator
 	 * @param[in]	rhs	right hand side of the operator
	 */
	explicit TokenOperator(std::string val, Token * lhs, Token * rhs);
	
	/**
	 * Destructor - frees child tokens
	 */
	~TokenOperator(){
		if(m_operands[0] != nullptr)
			delete m_operands[0];
		if(m_operands[1] != nullptr)
			delete m_operands[1];
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
	 * Returns result of the operation as a reference if applicable
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
	
	/**
	 * Returns whether operator token could be constructed from given string
 	 * @param[in]	val	value
  	 * @returns			true if it could, false if it could not
	 */
	static bool isValidOperator(const std::string & val);
	
	/**
	 * Returns whether operator token could be constructed from given string
 	 * @param[in]	val			value
 	 * @param[out]	outputrank	rank the operator would have
  	 * @returns					true if it could, false if it could not
	 */
	static bool isValidOperator(const std::string & val, Operator_Rank & outputrank);
	
	static const std::vector<std::vector<std::string>> operators; //!< Holds all valid operators sorted into their ranks
};
