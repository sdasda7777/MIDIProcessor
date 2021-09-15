
#include "Tokens_TokenNumber.hpp"
#include "Tokens_TokenVariable.hpp"
#include "Tokens_TokenString.hpp"
#include "TokenWrapper.hpp"

TokenVariable::TokenVariable(std::string n){
	m_name = n;
}
Token * TokenVariable::evaluateNumberSub(TokenEvaluationContext & tec) const{	
	TokenWrapper * tw = tec.getVariable(m_name);
	if(tw->getContentPointer() == nullptr)
		throw std::string("Runtime error: variable " + m_name + " holds no value\nIn '") + ((Token *)tec.getRootNode())->printContent() +"'";
	
	return tw->getContentPointer()->evaluateNumber(tec);
}
Token * TokenVariable::evaluateStringSub(TokenEvaluationContext & tec) const{
	TokenWrapper * tw = tec.getVariable(m_name);
	if(tw->getContentPointer() == nullptr)
		throw std::string("Runtime error: variable " + m_name + " holds no value\nIn '") + ((Token *)tec.getRootNode())->printContent() +"'";
	
	return tw->getContentPointer()->evaluateString(tec);
}
Token * TokenVariable::evaluateArraySub(TokenEvaluationContext & tec){
	TokenWrapper * tw = tec.getVariable(m_name);
	if(tw->getContentPointer() == nullptr)
		throw std::string("Runtime error: variable " + m_name + " holds no value\nIn '") + ((Token *)tec.getRootNode())->printContent() +"'";
	
	return tw->getContentPointer()->evaluateArray(tec);
}
TokenWrapper * TokenVariable::evaluateReferenceSub(TokenEvaluationContext & tec) const{
	TokenWrapper * tw = tec.getVariable(m_name);
	if(tw != nullptr){
		return tw;
	}
	tec.setVariable(m_name, nullptr);
	return tec.getVariable(m_name);
}
Token * TokenVariable::evaluateInstanceSub(TokenEvaluationContext & tec) const{
	TokenWrapper * tw = tec.getVariable(m_name);
	if(tw->getContentPointer() == nullptr)
		throw std::string("Runtime error: variable " + m_name + " holds no value\nIn '") + ((Token *)tec.getRootNode())->printContent() +"'";
	
	return tw->getContentPointer()->evaluateInstanceSub(tec);
}
bool TokenVariable::evaluateBoolSub(TokenEvaluationContext & tec) const{
	Token * tmp = evaluateInstance(tec);
	bool ret = tmp->evaluateBool(tec);
	delete tmp;
	return ret;
}
TokenWrapper * TokenVariable::getReferenceAtIndexSub(Token * index, TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: expected indexable object, got variable name\nIn '") + ((Token *)tec.getRootNode())->printContent() + "'";
}
std::string TokenVariable::printContentSub() const {
	return m_name;
}
std::string TokenVariable::toStringSub() const {
	return m_name;
}
