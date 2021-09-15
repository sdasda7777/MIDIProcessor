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
	throw std::string("Syntax error: scope evaluation cannot result in number");
}
Token * TokenScope::evaluateStringSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: scope evaluation cannot result in string");
}
Token * TokenScope::evaluateArraySub(TokenEvaluationContext & tec){
	throw std::string("Syntax error: scope evaluation cannot result in reference");
}
TokenWrapper * TokenScope::evaluateReferenceSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: scope evaluation cannot result in reference");
}
Token * TokenScope::evaluateInstanceSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: scope evaluation cannot result in instance");
}
bool TokenScope::evaluateBoolSub(TokenEvaluationContext & tec) const{
	for(size_t i = 0; i < m_content.size(); ++i){
		m_content[i]->evaluateBool(tec);
	}
	return true;
}
TokenWrapper * TokenScope::getReferenceAtIndexSub(Token * index, TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: scope evaluation cannot result in indexable object\nIn '") + ((Token *)tec.getRootNode())->printContent() + "'";
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