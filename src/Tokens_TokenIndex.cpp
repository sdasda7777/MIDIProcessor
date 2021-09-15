
#include <cmath>

#include "Tokens_TokenIndex.hpp"
#include "Tokens_TokenNumber.hpp"
#include "Tokens_TokenString.hpp"
#include "TokenWrapper.hpp"

TokenIndex::TokenIndex(Token * lhs, Token * rhs) : m_collection(lhs), m_index(rhs){}

Token * TokenIndex::evaluateNumberSub(TokenEvaluationContext & tec) const{	
	Token * arr = m_collection->evaluateArray(tec);
	TokenWrapper * ref = arr->getReferenceAtIndex(m_index, tec);
	Token * rr = ref->getContentPointer()->evaluateNumber(tec);
	return rr;
}
Token * TokenIndex::evaluateStringSub(TokenEvaluationContext & tec) const{
	Token * arr = m_collection->evaluateArray(tec);
	TokenWrapper * ref = arr->getReferenceAtIndex(m_index, tec);
	Token * rr = ref->getContentPointer()->evaluateString(tec);
	return rr;
}
Token * TokenIndex::evaluateArraySub(TokenEvaluationContext & tec){
	Token * arr = m_collection->evaluateArray(tec);
	TokenWrapper * ref = arr->getReferenceAtIndex(m_index, tec);
	Token * rr = ref->getContentPointer()->evaluateArray(tec);
	return rr;
}
TokenWrapper * TokenIndex::evaluateReferenceSub(TokenEvaluationContext & tec) const{
	//TODO if(m_index == nullptr){
	
	//}else{
		Token * arr = m_collection->evaluateArray(tec);
		return arr->getReferenceAtIndex(m_index, tec);
	//}
}
Token * TokenIndex::evaluateInstanceSub(TokenEvaluationContext & tec) const{
	Token * arr = m_collection->evaluateArray(tec);
	TokenWrapper * ref = arr->getReferenceAtIndex(m_index, tec);
	Token * rr = ref->getContentPointer()->evaluateInstance(tec);
	return rr;
}
bool TokenIndex::evaluateBoolSub(TokenEvaluationContext & tec) const{
	Token * arr = m_collection->evaluateArray(tec);
	TokenWrapper * ref = arr->getReferenceAtIndex(m_index, tec);
	return ref->getContentPointer()->evaluateBool(tec);
}
TokenWrapper * TokenIndex::getReferenceAtIndexSub(Token * index, TokenEvaluationContext & tec) const{
	throw std::string("Not implemented TokenIndex::getReferenceAtIndexSub()");
}
std::string TokenIndex::printContentSub() const {
	return m_collection->printContent() + "[" + m_index->printContent() + "]";
}
std::string TokenIndex::toStringSub() const {
	return "[index]";
}