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
	throw std::string("Syntax error: conditional evaluation cannot result in number");
}
Token * TokenIf::evaluateStringSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: conditional evaluation cannot result in string");
}
Token * TokenIf::evaluateArraySub(TokenEvaluationContext & tec){
	throw std::string("Syntax error: conditional evaluation cannot result in array");
}
TokenWrapper * TokenIf::evaluateReferenceSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: conditional evaluation cannot result in variable name");
}
Token * TokenIf::evaluateInstanceSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: conditional evaluation cannot result in instance");
}
bool TokenIf::evaluateBoolSub(TokenEvaluationContext & tec) const{
	if(m_condition->evaluateBool(tec)){
		m_if->evaluateBool(tec);
	}else if(m_else != nullptr){
		m_else->evaluateBool(tec);
	}
	return true;
}
TokenWrapper * TokenIf::getReferenceAtIndexSub(Token * index, TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: conditional evaluation cannot result in indexable object");
}
std::string TokenIf::printContentSub() const {
	return "if" + m_condition->printContent() + m_if->printContent() + (m_else != nullptr?"else " + m_else->printContent() : "");
}
std::string TokenIf::toStringSub() const {
	return "{if}";
}