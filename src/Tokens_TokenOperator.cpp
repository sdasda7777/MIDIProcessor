
#include <cmath>

#include "Tokens_TokenOperator.hpp"
#include "Tokens_TokenNumber.hpp"
#include "Tokens_TokenString.hpp"
#include "TokenWrapper.hpp"

/*
	Order taken from:
	https://en.cppreference.com/w/cpp/language/operator_precedence
	https://www.php.net/manual/en/language.operators.precedence.php
*/														
const std::vector<std::vector<std::string>> TokenOperator::operators = {
			std::vector<std::string>{"d++", "d--"},
			std::vector<std::string>{"++d", "--d", "-d", "+d", "!", "~"},
			std::vector<std::string>{"**"},
			std::vector<std::string>{"*", "/", "%"},
			std::vector<std::string>{"+", "-", "#"},
			std::vector<std::string>{"<", "<=", ">", ">="},
			std::vector<std::string>{"==", "!="},
			std::vector<std::string>{"&"},
			std::vector<std::string>{"^"},
			std::vector<std::string>{"|"},
			std::vector<std::string>{"&&"}, 
			std::vector<std::string>{"||"},
			std::vector<std::string>{"="}};

TokenOperator::TokenOperator(std::string val, Token * lhs, Token * rhs){	
	size_t i = 0;
	std::vector<std::string>::const_iterator res;
	for(;i < operators.size(); i++){
		res = std::find(operators[i].begin(), operators[i].end(), val);
		if(res != operators[i].end()){
			break;
		}
	}
	
	m_value = (i << 16) + (res - operators[i].begin());
	
	m_operands[0] = lhs;
	m_operands[1] = rhs;
}

Token * TokenOperator::evaluateNumberSub(TokenEvaluationContext & tec) const{	
	std::string operation = toString();
	Operator_Rank mor = getRank();
	Token * r1 = nullptr;
	Token * r2 = nullptr;
	Token * rr = nullptr;
	
	
	try{
		if(mor != EqNeq && mor != PostIncDec && operation != "++d" && operation != "--d" && operation != "!" && mor != Assignment && mor != LogAND && mor != LogOR)
			r1 = m_operands[0]->evaluateNumber(tec);
		if(m_operands[1] != nullptr && mor != EqNeq && mor != LogAND && mor != LogOR && mor != Assignment){
			r2 = m_operands[1]->evaluateNumber(tec);
		}
		
		if(mor == PostIncDec || operation == "++d" || operation == "--d"){
			TokenWrapper * tw = m_operands[0]->evaluateReference(tec);
			
			if(tw->getContentPointer() == nullptr)
				throw std::string("Runtime error: variable ") + (mor == PostIncDec? "left of postfix ": "right of prefix ") + operation + " holds no value\nIn '" + ((Token *)tec.getRootNode())->printContent() + "'";
			
			try{
				Token * tpsfty = tw->getContentPointer()->evaluateNumber(tec);
				delete tpsfty;
			}catch(std::string excp){
				throw excp;
			}
			
			if(operation == "d++"){
				rr = tw->getContentPointer()->evaluateNumber(tec);
				((TokenNumber*)tw->getContentPointer())->increment(1);
			}else if(operation == "++d"){
				((TokenNumber*)tw->getContentPointer())->increment(1);
				rr = tw->getContentPointer()->evaluateNumber(tec);
			}else if(operation == "d--"){
				rr = tw->getContentPointer()->evaluateNumber(tec);
				((TokenNumber*)tw->getContentPointer())->decrement(1);
			}else if(operation == "--d"){
				((TokenNumber*)tw->getContentPointer())->decrement(1);
				rr = tw->getContentPointer()->evaluateNumber(tec);
			}
		}else if(operation == "+d" || operation == "-d"){
			if(operation == "+d"){
				rr = new TokenNumber(((TokenNumber*)r1)->getValue());
			}else{
				rr = new TokenNumber(-((TokenNumber*)r1)->getValue());
			}
		}else if(operation == "!"){ //logical negation
			if(!m_operands[0]->evaluateBool(tec)){
				rr = new TokenNumber(1);
			}else{
				rr = new TokenNumber(0);
			}
		}else if(operation == "~"){ //bitwise negation
			int d = round(((TokenNumber*)r1)->getValue());
			rr = new TokenNumber((double)~d);
		}else if(operation == "+"){
			rr = new TokenNumber(((TokenNumber*)r1)->getValue() + ((TokenNumber*)r2)->getValue());
		}else if(operation == "-"){
			rr = new TokenNumber(((TokenNumber*)r1)->getValue() - ((TokenNumber*)r2)->getValue());
		}else if(mor == Power){
			rr = new TokenNumber(pow(((TokenNumber*)r1)->getValue(), ((TokenNumber*)r2)->getValue()));
		}else if(operation == "*"){ //multiplication
			rr = new TokenNumber(((TokenNumber*)r1)->getValue() * ((TokenNumber*)r2)->getValue());
		}else if(operation == "/"){ //division
			rr = new TokenNumber(((TokenNumber*)r1)->getValue() / ((TokenNumber*)r2)->getValue());
		}else if(operation == "%"){ //modulo
			double a = ((TokenNumber*)r1)->getValue();
			double b = ((TokenNumber*)r2)->getValue();
			
			//Formula taken from https://stackoverflow.com/a/53998265, author is https://stackoverflow.com/users/6368005/mystical
			rr = new TokenNumber(a - (int)(a/b) * b);
		}else if(mor == BitAND){
			rr = new TokenNumber((int)((TokenNumber*)r1)->getValue()&(int)((TokenNumber*)r2)->getValue());
		}else if(mor == BitXOR){
			rr = new TokenNumber((int)((TokenNumber*)r1)->getValue()^(int)((TokenNumber*)r2)->getValue());
		}else if(mor == BitOR){
			rr = new TokenNumber((int)((TokenNumber*)r1)->getValue()|(int)((TokenNumber*)r2)->getValue());
		}else if(operation == "<"){
			rr = new TokenNumber(((TokenNumber*)r1)->getValue() < ((TokenNumber*)r2)->getValue());
		}else if(operation == "<="){
			rr = new TokenNumber(((TokenNumber*)r1)->getValue() <= ((TokenNumber*)r2)->getValue());
		}else if(operation == ">"){
			rr = new TokenNumber(((TokenNumber*)r1)->getValue() > ((TokenNumber*)r2)->getValue());
		}else if(operation == ">="){
			rr = new TokenNumber(((TokenNumber*)r1)->getValue() >= ((TokenNumber*)r2)->getValue());
		}else if(mor == EqNeq){
			r1 = m_operands[0]->evaluateString(tec);
			r2 = m_operands[1]->evaluateString(tec);
			if(operation == "=="){
				rr = new TokenNumber(r1->toString() == r2->toString());
			}else if(operation == "!="){
				rr = new TokenNumber(r1->toString() != r2->toString());
			}
		}else if(mor == LogAND){	
			if(m_operands[0]->evaluateBool(tec) && m_operands[1]->evaluateBool(tec)){
				rr = new TokenNumber(1);
			}else{
				rr = new TokenNumber(0);
			}
		}else if(mor == LogOR){
			if(m_operands[0]->evaluateBool(tec) || m_operands[1]->evaluateBool(tec)){
				rr = new TokenNumber(1);
			}else{
				rr = new TokenNumber(0);
			}
		}else if(mor == Assignment){
			rr = m_operands[1]->evaluateInstance(tec);
			
			TokenWrapper * varref = m_operands[0]->evaluateReference(tec);
			varref->setContentPointer(rr->evaluateInstance(tec));
		}else{
			throw std::string("Syntax error: use of unimplemented operator ") + operation + " \nIn '" + ((Token *)tec.getRootNode())->printContent() + "'";
		}
		
		if(r1 != nullptr && mor != Assignment && mor != PostIncDec && operation != "++d" && operation != "--d"){
			delete r1;
			delete r2;
		}	
		return rr;
	}catch(std::string excp){
		if(mor != Assignment && mor != PostIncDec && operation != "++d" && operation != "--d"){
			if(r1 != nullptr) delete r1;
			if(r2 != nullptr) delete r2;
		}
		throw excp;
	}
}

Token * TokenOperator::evaluateStringSub(TokenEvaluationContext & tec) const{
	std::string operation = toString();
	Token * rr;
	
	if(operation == "#"){
		Token * r1 = nullptr;
	
		try{
			r1 = m_operands[0]->evaluateString(tec);
			Token * r2 = m_operands[1]->evaluateString(tec);
			
			rr = new TokenString(((TokenString*)r1)->getCleanString()+((TokenString*)r2)->getCleanString(), true);
			
			delete r1;
			delete r2;
		}catch(std::string excp){
			if(r1 != nullptr)
				delete r1;
			throw excp;
		}
	}else{
		Token * r1 = evaluateNumber(tec);
		
		rr = new TokenString(StaticMethods::PrettyPrintDouble(((TokenNumber*)r1)->getValue()), true);
		
		delete r1;
	}
		
	return rr;
}
Token * TokenOperator::evaluateArraySub(TokenEvaluationContext & tec){
	throw std::string("Syntax error: prefix, infix nor postfix operator can return array") + " \nIn '" + ((Token *)tec.getRootNode())->printContent() + "'";
}
TokenWrapper * TokenOperator::evaluateReferenceSub(TokenEvaluationContext & tec) const{
	std::string operation = toString();
	
	if(operation != "--d" && operation != "++d")
		throw std::string("Syntax error: value used as a reference near") + operation + " \nIn '" + ((Token *)tec.getRootNode())->printContent() + "'";
	
	TokenWrapper * tw = m_operands[0]->evaluateReference(tec);
	
	try{
		Token * tpsfty = tw->getContentPointer()->evaluateNumber(tec);
		delete tpsfty;
	}catch(std::string excp){
		throw excp;
	}
	
	if(operation == "--d"){
		((TokenNumber*)tw->getContentPointer())->decrement(1);
	}else if(operation == "++d"){
		((TokenNumber*)tw->getContentPointer())->increment(1);
	}
	
	return tw;
}
Token * TokenOperator::evaluateInstanceSub(TokenEvaluationContext & tec) const{
	if(toString() == "#"){
		return evaluateString(tec);
	}else{
		return evaluateNumber(tec);
	}
}
bool TokenOperator::evaluateBoolSub(TokenEvaluationContext & tec) const{
	Token * tmp = evaluateInstance(tec);
	bool ret = tmp->evaluateBool(tec);
	delete tmp;
	return ret;
}
TokenWrapper * TokenOperator::getReferenceAtIndexSub(Token * index, TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: prefix, infix nor postfix operator can return array");
}
std::string TokenOperator::printContentSub() const {
	std::string operation = toString();
	std::string r1, r2, rr;
	
	Operator_Rank mor = getRank();
	
	r1 = m_operands[0]->printContent();
	if(m_operands[1] != nullptr)
		r2 = m_operands[1]->printContent();
	
	if(operation == "d++"){
		rr = "(" + r1 + "++)";
	}else if(operation == "d--"){
		rr = "(" + r1 + "--)";
	}else if(operation == "++d"){
		rr = "(++" + r1 + ")";
	}else if(operation == "--d"){
		rr = "(--" + r1 + ")";
	}else if(operation == "+d"){
		rr = "(+" + r1 + ")";
	}else if(operation == "-d"){
		rr = "(-" + r1 + ")";
	}else if(operation == "!" || operation == "~"){
		rr = "(" + operation + r1 + ")" ;
	}else if(mor == Power || mor == MulDivMod || mor == AddSubCon || mor == LtGt || mor == EqNeq || mor == BitAND || mor == BitXOR || mor == BitOR || mor == LogAND || mor == LogOR || mor == Assignment){
		rr = "(" + r1 + " " + operation + " " + r2 + ")";
	}
	
	return rr;
}
std::string TokenOperator::toStringSub() const {
	return operators[(m_value >> 16)][(m_value & 0xFFFF)];
}
Operator_Rank TokenOperator::getRank() const {	
	return Operator_Rank( m_value >> 16 );
}
bool TokenOperator::isValidOperator(const std::string & val){
	for(size_t i = 0; i < operators.size(); i++){
		std::vector<std::string>::const_iterator res = std::find(operators[i].begin(), operators[i].end(), val);
		if(res != operators[i].end()){
			return true;
		}
	}

	return false;
}
bool TokenOperator::isValidOperator(const std::string & val, Operator_Rank & outputrank){
	for(size_t i = 0; i < operators.size(); i++){
		std::vector<std::string>::const_iterator res = std::find(operators[i].begin(), operators[i].end(), val);
		if(res != operators[i].end()){
			outputrank = Operator_Rank(i);
			return true;
		}
	}

	return false;
}
