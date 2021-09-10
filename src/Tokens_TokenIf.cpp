#include "Tokens_TokenIf.hpp"
#include "Tokens_TokenNumber.hpp"
#include "Tokens_TokenString.hpp"

TokenIf::TokenIf(Token * condition, Token * ifcode, Token * elsecode): m_condition(condition), m_if(ifcode), m_else(elsecode){}

TokenIf::~TokenIf(){
	delete m_condition;
	delete m_if;
	if(m_else)
		delete m_else;
}

Token * TokenIf::evaluateNumberSub(TokenEvaluationContext & tec) const{
	evaluateBoolSub(tec);
	return new TokenNumber(1);
}
Token * TokenIf::evaluateStringSub(TokenEvaluationContext & tec) const{
	evaluateBoolSub(tec);
	return new TokenString("1", false);
}
Token * TokenIf::evaluateVarnameSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: condition evaluation cannot result in variable name");
}
Token * TokenIf::evaluateInstanceSub(TokenEvaluationContext & tec) const{
	evaluateBoolSub(tec);
	return new TokenNumber(1);
}
bool TokenIf::evaluateBoolSub(TokenEvaluationContext & tec) const{
	if(m_condition->evaluateBool(tec)){
		m_if->evaluateBool(tec);
	}else if(m_else != NULL){
		m_else->evaluateBool(tec);
	}
	return true;
}
std::string TokenIf::printContentSub() const {
	return "if" + m_condition->printContent() + m_if->printContent() + (m_else != NULL?"else " + m_else->printContent() : "");
}
std::string TokenIf::toStringSub() const {
	return "{if}";
}