#include "Tokens_TokenScope.hpp"
#include "Tokens_TokenNumber.hpp"
#include "Tokens_TokenString.hpp"

TokenScope::TokenScope(std::vector<Token *> cntnt){
	m_content = cntnt;
}

TokenScope::~TokenScope(){
	for(size_t i = 0; i < m_content.size(); ++i){
		delete m_content[i];
	}
}

Token * TokenScope::evaluateNumberSub(TokenEvaluationContext & tec) const{
	evaluateBoolSub(tec);
	return new TokenNumber(1);
}
Token * TokenScope::evaluateStringSub(TokenEvaluationContext & tec) const{
	evaluateBoolSub(tec);
	return new TokenString("1", false);
}
Token * TokenScope::evaluateVarnameSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: scope evaluation cannot result in variable name");
}
Token * TokenScope::evaluateInstanceSub(TokenEvaluationContext & tec) const{
	evaluateBoolSub(tec);
	return new TokenNumber(1);
}
bool TokenScope::evaluateBoolSub(TokenEvaluationContext & tec) const{
	for(size_t i = 0; i < m_content.size(); ++i){
		m_content[i]->evaluateBool(tec);
	}
	return true;
}
std::string TokenScope::printContentSub() const {
	std::string ret = "{\n";
	
	for(size_t i = 0; i < m_content.size(); ++i){
		ret += m_content[i]->printContent();
		ret += ";\n";
	}
	
	return ret+"}";
}
std::string TokenScope::toStringSub() const {
	return "{scope}";
}