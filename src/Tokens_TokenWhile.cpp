#include "Tokens_TokenWhile.hpp"
#include "Tokens_TokenNumber.hpp"
#include "Tokens_TokenString.hpp"

TokenWhile::TokenWhile(Token * condition, Token * body): m_condition(condition), m_body(body){}

TokenWhile::~TokenWhile(){
	delete m_condition;
	delete m_body;
}

Token * TokenWhile::evaluateNumberSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: while loop evaluation cannot result in number");
}
Token * TokenWhile::evaluateStringSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: while loop evaluation cannot result in string");
}
Token * TokenWhile::evaluateArraySub(TokenEvaluationContext & tec){
	throw std::string("Syntax error: while loop evaluation cannot result in array");
}
TokenWrapper * TokenWhile::evaluateReferenceSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: while loop evaluation cannot result in variable name");
}
Token * TokenWhile::evaluateInstanceSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: while loop evaluation cannot result in instance");
}
bool TokenWhile::evaluateBoolSub(TokenEvaluationContext & tec) const{
	while(m_condition->evaluateBool(tec)){
		m_body->evaluateBool(tec);
	}
	return true;
}
TokenWrapper * TokenWhile::getReferenceAtIndexSub(Token * index, TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: while loop evaluation cannot result in indexable object");
}
std::string TokenWhile::printContentSub() const {
	return "while" + m_condition->printContent() + m_body->printContent();
}
std::string TokenWhile::toStringSub() const {
	return "{while}";
}