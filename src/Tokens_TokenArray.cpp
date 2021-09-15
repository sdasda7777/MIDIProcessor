#include <math.h>

#include "Tokens_TokenArray.hpp"
#include "Tokens_TokenNumber.hpp"
#include "Tokens_TokenString.hpp"
#include "TokenWrapper.hpp"

TokenArray::TokenArray(std::vector<Token *> elements){
	for(size_t i = 0; i < elements.size(); ++i){
		m_elements.push_back(new TokenWrapper(elements[i]));
	}
}

TokenArray::~TokenArray(){
	for(size_t i = 0; i < m_elements.size(); ++i){
		delete m_elements[i];
	}
}

Token * TokenArray::evaluateNumberSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: array evaluation cannot result in number");
}
Token * TokenArray::evaluateStringSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: array evaluation cannot result in string");
}
Token * TokenArray::evaluateArraySub(TokenEvaluationContext & tec){
	return this;
}
TokenWrapper * TokenArray::evaluateReferenceSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: array evaluation cannot result in reference");
}
Token * TokenArray::evaluateInstanceSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: array evaluation cannot result in instance");
}
bool TokenArray::evaluateBoolSub(TokenEvaluationContext & tec) const{
	return true;
}
TokenWrapper * TokenArray::getReferenceAtIndexSub(Token * index, TokenEvaluationContext & tec) const{
	Token * i2 = index->evaluateNumber(tec);
	double d = ((TokenNumber*)i2)->getValue();
	delete i2;
	if(floor(d) < 0 || floor(d) >= m_elements.size())		
		throw std::string("Runtime error: index out of range (has ") + StaticMethods::PrettyPrintDouble(m_elements.size()) + 
			" elements, requested index " + StaticMethods::PrettyPrintDouble(floor(d)) + ")";
	return m_elements[floor(d)];
}
std::string TokenArray::printContentSub() const {
	std::string ret = "{";
	
	for(size_t i = 0; i < m_elements.size(); ++i){
		ret += m_elements[i]->getContentPointer()->printContent() + ", ";
	}

	return ret + "}";
}
std::string TokenArray::toStringSub() const {
	return "{array}";
}