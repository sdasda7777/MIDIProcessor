

#include "Tokens_TokenString.hpp"

TokenString::TokenString(std::string v, bool already_clean){
	if(already_clean){
		m_value = v;
	}else{
		m_value = "";
		
		for(size_t i = 1; i + 1 < v.length(); i++){
			if(v[i] == '\\'){
				if(i + 1 < v.length()){
					if(v[i+1] == '\\'){
						m_value += '\\';
					}else if(v[i+1] == 'n'){
						m_value += '\n';
					}else if(v[i+1] == '"'){
						m_value += '\"';
					}
					i++;
				}
			}else{
				m_value += v[i];
			}
		}
	}
}
Token * TokenString::evaluateNumberSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: expected number, got string\nIn '") + ((Token *)tec.getRootNode())->printContent() + "'";
}
Token * TokenString::evaluateStringSub(TokenEvaluationContext & tec) const{
	return new TokenString(m_value, true);
}
Token * TokenString::evaluateArraySub(TokenEvaluationContext & tec){
	throw std::string("Syntax error: expected array, got string\nIn '") + ((Token *)tec.getRootNode())->printContent() + "'";
}
TokenWrapper * TokenString::evaluateReferenceSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: expected reference, got string\nIn '") + ((Token *)tec.getRootNode())->printContent() + "'";
}
Token * TokenString::evaluateInstanceSub(TokenEvaluationContext & tec) const{
	return new TokenString(m_value, true);
}
bool TokenString::evaluateBoolSub(TokenEvaluationContext & tec) const{
	return m_value != "";
}
TokenWrapper * TokenString::getReferenceAtIndexSub(Token * index, TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: expected indexable object, got string\nIn '") + ((Token *)tec.getRootNode())->printContent() + "'";
}
std::string TokenString::printContentSub() const {
	return toStringSub();
}
std::string TokenString::toStringSub() const {
	return StaticMethods::SmudgeString(m_value);
}
std::string TokenString::getCleanString() const {
	return m_value;
}
