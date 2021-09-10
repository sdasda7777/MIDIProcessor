

#include "Tokens_TokenVariable.hpp"
#include "Tokens_TokenString.hpp"

TokenVariable::TokenVariable(std::string n){
	m_name = n;
}
Token * TokenVariable::evaluateNumberSub(TokenEvaluationContext & tec) const{	
	Token * val = tec.getVariable(m_name);
	if(val == nullptr)
		throw std::string("Runtime error: variable " + m_name + " holds no value\nIn '") + ((Token *)tec.getRootNode())->printContent() +"'";
	
	return val->evaluateNumber(tec);
}
Token * TokenVariable::evaluateStringSub(TokenEvaluationContext & tec) const{
	Token * val = tec.getVariable(m_name);
	if(val == nullptr)
		throw std::string("Runtime error: variable " + m_name + " holds no value\nIn '") + ((Token *)tec.getRootNode())->printContent() +"'";
	
	return val->evaluateString(tec);
}
Token * TokenVariable::evaluateVarnameSub(TokenEvaluationContext & tec) const{
	return new TokenString(m_name, true);
}
Token * TokenVariable::evaluateInstanceSub(TokenEvaluationContext & tec) const{
	Token * val = tec.getVariable(m_name);
	if(val == nullptr)
		throw std::string("Runtime error: variable " + m_name + " holds no value\nIn '") + ((Token *)tec.getRootNode())->printContent() +"'";
	
	return val->evaluateInstanceSub(tec);
}
bool TokenVariable::evaluateBoolSub(TokenEvaluationContext & tec) const{
	Token * tmp = evaluateInstance(tec);
	bool ret = tmp->evaluateBool(tec);
	delete tmp;
	return ret;
}
std::string TokenVariable::printContentSub() const {
	return m_name;
}
std::string TokenVariable::toStringSub() const {
	return m_name;
}
