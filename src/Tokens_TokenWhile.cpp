#include "Tokens_TokenWhile.hpp"
#include "Tokens_TokenNumber.hpp"
#include "Tokens_TokenString.hpp"

TokenWhile::TokenWhile(Token * condition, Token * body): m_condition(condition), m_body(body){}

TokenWhile::~TokenWhile(){
	delete m_condition;
	delete m_body;
}

Token * TokenWhile::evaluateNumberSub(TokenEvaluationContext & tec) const{
	evaluateBoolSub(tec);
	return new TokenNumber(1);
}
Token * TokenWhile::evaluateStringSub(TokenEvaluationContext & tec) const{
	evaluateBoolSub(tec);
	return new TokenString("1", false);
}
Token * TokenWhile::evaluateVarnameSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: while loop evaluation cannot result in variable name");
}
Token * TokenWhile::evaluateInstanceSub(TokenEvaluationContext & tec) const{
	evaluateBoolSub(tec);
	return new TokenNumber(1);
}
bool TokenWhile::evaluateBoolSub(TokenEvaluationContext & tec) const{
	while(m_condition->evaluateBool(tec)){
		m_body->evaluateBool(tec);
	}
	return true;
}
std::string TokenWhile::printContentSub() const {
	return "while" + m_condition->printContent() + m_body->printContent();
}
std::string TokenWhile::toStringSub() const {
	return "{while}";
}