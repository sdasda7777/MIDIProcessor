#pragma once

#include <vector>

#include "StaticMethods.hpp"
#include "TokenEvaluationContext.hpp"

class TokenEvaluationContext;
class TokenWrapper;

enum Token_Type : unsigned int
{
 	TokAny = 1,
	 			
	TokOperator = 2,
	TokSeparator = 4,

	TokVariable = 8,
	TokFunction = 16,

	TokInstance = 32,
	TokNumber = 64,
	TokString = 128,
	TokArray = 256/*,
	TokObject = 512*/
}; //!< used purely for specifying expected parameter types

//! Parent to all token types.
class Token{	
public:
	/**
	 * Ensures every instantiable derived class will have a destructor
	 */
	virtual ~Token(){};
	
	/**
	 * Attempts to resolve tree evaluation to get a number
	 * @param[in,out]	tec	evaluation context
	 * @returns				TokenNumber
	 */
	Token * evaluateNumber(TokenEvaluationContext & tec) const {
		return this->evaluateNumberSub(tec);
	}
	
	/**
	 * Declares that every instantiable derived class shall have a method that either:
	 *	a) returns its value (for TokenNumber instances)
	 *	b) returns value it references (for TokenVariable instances that reference a number)
	 *	c) returns its result (for operators and functions that return a number)
	 *	d) throws an exception (if function parameters are not correct or operators are used incorrectly)
	 * @param[in,out]	tec	evaluation context
 	 * @returns	result
	 */
	virtual Token * evaluateNumberSub(TokenEvaluationContext & tec) const = 0;
	
	/**
	 * Attempts to resolve tree evaluation to get a string
	 * @param[in,out]	tec	evaluation context
	 * @returns				TokenString
	 */
	Token * evaluateString(TokenEvaluationContext & tec) const {
		return this->evaluateStringSub(tec);
	}
	
	/**
	 * Declares that every instantiable derived class shall have a method that either:
	 *	a) returns its value (for TokenString instances)
	 *	b) returns value it references (for TokenVariable instances that reference a string)
	 *	c) returns its result (for operators and functions that return a string)
	 *	d) throws an exception (if function parameters are not correct or operators are used incorrectly)
	 * @param[in,out]	tec	evaluation context
 	 * @returns	result
	 */
	virtual Token * evaluateStringSub(TokenEvaluationContext & tec) const = 0;
	
	/**
	 * Attempts to resolve tree evaluation to get an array
	 * @param[in,out]	tec	evaluation context
	 * @returns				TokenArray
	 */
	Token * evaluateArray(TokenEvaluationContext & tec){
		return this->evaluateArraySub(tec);
	}
	
	/**
	 * Declares that every instantiable derived class shall have a method that either:
	 *	a) returns its value (for TokenArray instances)
	 *	b) returns value it references (for TokenVariable instances that reference an array)
	 *	c) returns its result (for operators and functions that return an array)
	 *	d) throws an exception (other cases)
	 * @param[in,out]	tec	evaluation context
 	 * @returns	result
	 */
	virtual Token * evaluateArraySub(TokenEvaluationContext & tec) = 0;
	
	/**
	 * Attempts to resolve tree evaluation to get a reference
	 * @param[in,out]	tec	evaluation context
	 * @returns				TokenString
	 */
	TokenWrapper * evaluateReference(TokenEvaluationContext & tec) const {
		return this->evaluateReferenceSub(tec);
	}
	
	/**
	 * Declares that every instantiable derived class shall have a method that either:
	 *	a) returns reference to a token (for TokenVariable instances and operators --d and ++d)
	 *	b) throws an exception (other cases)
	 * @param[in,out]	tec	evaluation context
 	 * @returns	result
	 */
	virtual TokenWrapper * evaluateReferenceSub(TokenEvaluationContext & tec) const = 0;
	
	/**
	 * Attempts to resolve tree evaluation to get a value to assign
	 * @param[in,out]	tec	evaluation context
	 * @returns				TokenString
	 */
	Token * evaluateInstance(TokenEvaluationContext & tec) const {
		return this->evaluateInstanceSub(tec);
	}
	
	/**
	 * Declares that every instantiable derived class shall have a method that either:
	 *	a) returns its copy (for TokenString and TokenNumber)
	 *  b) returns copy of value it references (for TokenVariable instances)
	 *	c) returns the result (for TokenOperator and TokenFunction instances)
	 *	d) throws an exception (if function parameters are not correct or operators are used incorrectly)
	 * @param[in,out]	tec	evaluation context
 	 * @returns	result
	 */
	virtual Token * evaluateInstanceSub(TokenEvaluationContext & tec) const = 0;
	
	/**
	 * Attempts to resolve a tree to get a boolean value
	 * @param[in,out]	tec	evaluation context
	 * @returns				boolean value
	 */
	bool evaluateBool(TokenEvaluationContext & tec) const {
		return this->evaluateBoolSub(tec);
	}
	
	/**
	 * Declares that every instantiable derived class shall have a method that either:
	 *	a) returns false (for TokenString that is empty or TokenNumber that is 0)
	 *  b) returns true (for TokenString that is not empty or TokenNumber that is not 0)
	 *  c) evaluateBool() of referenced token (for TokenVariable instances)
	 *	d) evaluateBool() of the result (for TokenOperator and TokenFunction instances)
	 *	e) throws an exception (if function parameters are not correct or operators are used incorrectly)
	 * @param[in,out]	tec	evaluation context
 	 * @returns	result
	 */
	virtual bool evaluateBoolSub(TokenEvaluationContext & tec) const = 0;
	
	/**
	 * Attempts to use index to get a reference to child note
	 * @param[in]		index	index
	 * @param[in,out]	tec		evaluation context
	 * @returns					TokenWrapper
	 */
	TokenWrapper * getReferenceAtIndex(Token * index, TokenEvaluationContext & tec) const {
		return this->getReferenceAtIndexSub(index, tec);
	}
	
	/**
	 * Declares that every instantiable derived class shall have a method that either:
	 *	a) returns reference to matching Token with matching index
	 *	b) throws an exception (other cases)
	 * @param[in]		index	evaluation context
	 * @param[in,out]	tec		evaluation context
	 * @returns					TokenWrapper
	 */
	virtual TokenWrapper * getReferenceAtIndexSub(Token * index, TokenEvaluationContext & tec) const = 0;
	
	/**
	 * Returns string approximating original code
	 * @returns	string
	 */
	std::string printContent() const {
		return this->printContentSub();
	}
	
	/**
	 * Declares that every instantiable derived class shall have a method that returns approximation of oringinal code
 	 * @returns	string
	 */
	virtual std::string printContentSub() const = 0;
	
	/**
	 * Returns value converted to printable string
 	 * @returns	printable string
	 */
	std::string toString() const {
		return this->toStringSub();
	};
	
	/**
	 * Declares that every instantiable derived class shall have a method that converts its value to printable string
 	 * @returns	printable string
	 */
	virtual std::string toStringSub() const = 0;
};
