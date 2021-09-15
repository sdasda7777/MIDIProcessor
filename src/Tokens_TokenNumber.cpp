
#include "Tokens_TokenNumber.hpp"
#include "Tokens_TokenString.hpp"

TokenNumber::TokenNumber(double d){
	m_value = d;
}
Token * TokenNumber::evaluateNumberSub(TokenEvaluationContext & tec) const{
	return new TokenNumber(m_value);
}
Token * TokenNumber::evaluateStringSub(TokenEvaluationContext & tec) const{
	return new TokenString(toStringSub(), true);
}
Token * TokenNumber::evaluateArraySub(TokenEvaluationContext & tec){
	throw std::string("Syntax error: expected array, got number");
}
TokenWrapper * TokenNumber::evaluateReferenceSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: expected reference, got number");
}
Token * TokenNumber::evaluateInstanceSub(TokenEvaluationContext & tec) const{
	return new TokenNumber(m_value);
}
bool TokenNumber::evaluateBoolSub(TokenEvaluationContext & tec) const{
	return m_value != 0;
}
TokenWrapper * TokenNumber::getReferenceAtIndexSub(Token * index, TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: expected indexable object, got number\nIn '") + ((Token *)tec.getRootNode())->printContent() + "'";
}
std::string TokenNumber::printContentSub() const {
	return toString();
}
std::string TokenNumber::toStringSub() const {
	return StaticMethods::PrettyPrintDouble(m_value);
}
double TokenNumber::getValue() const {
	return m_value;
}
void TokenNumber::setValue(double d){
	m_value = d;
}
void TokenNumber::increment(double d){
	m_value += d;
}
void TokenNumber::decrement(double d){
	m_value -= d;
}
