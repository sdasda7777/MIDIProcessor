#include <filesystem>
#include <regex>
#include <iomanip>

#include "Tokens_TokenFunction.hpp"
#include "Tokens_TokenNumber.hpp"
#include "Tokens_TokenString.hpp"

TokenFunction::TokenFunction(std::string n, std::vector<Token *> args){
	m_name = n;
	m_arguments = args;
}

TokenFunction::~TokenFunction(){
	for(size_t i = 0; i < m_arguments.size(); ++i){
		delete m_arguments[i];
	}
}

Token * TokenFunction::evaluateNumberSub(TokenEvaluationContext & tec) const{
	if(m_name == "nStr" || m_name == "strSub" || m_name == "noteNameFromNumber")
		throw std::string("Syntax error: caller of function ") + m_name + " expected return type number, got string";

	return evaluateInstanceSub(tec);
}
Token * TokenFunction::evaluateStringSub(TokenEvaluationContext & tec) const{
	Token * rr;
	
	if(m_name == "nStr" || m_name == "strSub" || m_name == "noteNameFromNumber"){
		rr = evaluateInstanceSub(tec);
	}else{
		Token * tmp = evaluateInstanceSub(tec);
		rr = new TokenString(tmp->toString(), true);
		delete tmp;
	}
	
	return rr;
}
Token * TokenFunction::evaluateVarnameSub(TokenEvaluationContext & tec) const{
	throw std::string("Syntax error: expected variable, got function call");
}
Token * TokenFunction::evaluateInstanceSub(TokenEvaluationContext & tec) const{
	std::vector<Token *> inputs;
	Token * rr;
	
	try{	
		if(m_name == "nStr"){
			inputs = StaticMethods::TestAndTransformArguments(tec, m_name, m_arguments, [](size_t s)->bool{return s == 2;}, {TokNumber});
			
			std::ostringstream oss;
			
			oss << std::fixed << std::setprecision(((TokenNumber*)inputs[1])->getValue()) << ((TokenNumber*)inputs[0])->getValue();
			
			rr = new TokenString(oss.str(), true);
		}else if(m_name == "sNum"){
			inputs = StaticMethods::TestAndTransformArguments(tec, m_name, m_arguments, [](size_t s)->bool{return s == 2;}, {TokString, TokVariable});
			
			// Taken from https://stackoverflow.com/a/5932665, author is https://stackoverflow.com/users/410767/tony-delroy
			std::istringstream iss( ((TokenString*)inputs[0])->getCleanString() );
			double d;
			bool ok = iss >> d >> std::ws && iss.eof();
			
			if(ok){
				rr = new TokenNumber(1);
				
				std::string varname = ((TokenString*)inputs[1])->getCleanString();
				
				tec.setVariable(varname, new TokenNumber(d));
			}else{
				rr = new TokenNumber(0);
			}
		}else if(m_name == "strLen"){
			inputs = StaticMethods::TestAndTransformArguments(tec, m_name, m_arguments, [](size_t s)->bool{return s == 1;}, {TokString});
			
			rr = new TokenNumber(((TokenString*)inputs[0])->getCleanString().length());
		}else if(m_name == "strSub"){
			inputs = StaticMethods::TestAndTransformArguments(tec, m_name, m_arguments, [](size_t s)->bool{return s == 2 || s == 3;}, {TokString, TokNumber, TokNumber});	
			
			if(inputs.size() == 3){
				rr = new TokenString(((TokenString*)inputs[0])->getCleanString().substr((size_t)((TokenNumber*)inputs[1])->getValue(), (size_t)((TokenNumber*)inputs[2])->getValue()), true);
			}else{
				rr = new TokenString(((TokenString*)inputs[0])->getCleanString().substr((size_t)((TokenNumber*)inputs[1])->getValue()), true);
			}
		}else if(m_name == "strLike"){
			inputs = StaticMethods::TestAndTransformArguments(tec, m_name, m_arguments, [](size_t s)->bool{return s == 2;}, {TokString});
			
			std::regex rex(((TokenString*)inputs[1])->getCleanString());
			
			if(regex_match(((TokenString*)inputs[0])->getCleanString(), rex)){
				rr = new TokenNumber(1);
			}else{
				rr = new TokenNumber(0);
			}
		}else if(m_name == "isSet"){
			inputs = StaticMethods::TestAndTransformArguments(tec, m_name, m_arguments, [](size_t s)->bool{return s == 1;}, {TokVariable});
			
			rr = new TokenNumber(tec.getVariable(((TokenString*)inputs[0])->getCleanString()) != nullptr);
		}else if(m_name == "unset"){
			inputs = StaticMethods::TestAndTransformArguments(tec, m_name, m_arguments, [](size_t s)->bool{return s == 1;}, {TokVariable});
			
			rr = new TokenNumber(tec.eraseVariable(((TokenString*)inputs[0])->getCleanString()));
		}else if(m_name == "random"){
			inputs = StaticMethods::TestAndTransformArguments(tec, m_name, m_arguments, [](size_t s)->bool{return s == 0;}, {});
			
			rr = new TokenNumber(((double)rand())/RAND_MAX);
		}else if(m_name == "print"){
			inputs = StaticMethods::TestAndTransformArguments(tec, m_name, m_arguments, [](size_t s)->bool{return s == 1;}, {TokString});
			
			rr = new TokenNumber(1);
			
			tec.print(((TokenString*)inputs[0])->getCleanString());
		}else if(m_name == "printLn"){
			inputs = StaticMethods::TestAndTransformArguments(tec, m_name, m_arguments, [](size_t s)->bool{return s == 1;}, {TokString});
			
			rr = new TokenNumber(1);
			
			tec.printLn(((TokenString*)inputs[0])->getCleanString());
		}else if(m_name == "noteNameFromNumber"){
			inputs = StaticMethods::TestAndTransformArguments(tec, m_name, m_arguments, [](size_t s)->bool{return s == 1;}, {TokNumber});
			
			//Taken from https://stackoverflow.com/a/64408826
			std::vector<std::string> noteNames = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
			int noteNumber = (int)((TokenNumber*)inputs[0])->getValue();
			
			std::stringstream str;
			str << ((noteNumber / 12) - 1);
			rr = new TokenString(noteNames[noteNumber % 12] + str.str(), true);
		}else if(m_name == "sendToDefault"){
			inputs = StaticMethods::TestAndTransformArguments(tec, m_name, m_arguments, [](size_t s)->bool{return s == 4;}, {TokNumber});
			
			rr = new TokenNumber(tec.sendToDefault((unsigned char)((TokenNumber*)inputs[0])->getValue(), 
												(unsigned char)((TokenNumber*)inputs[1])->getValue(), 
												(unsigned char)((TokenNumber*)inputs[2])->getValue(),
												(unsigned char)((TokenNumber*)inputs[3])->getValue()));
		}else if(m_name == "send"){
			inputs = StaticMethods::TestAndTransformArguments(tec, m_name, m_arguments, [](size_t s)->bool{return s == 5;}, {TokString, TokNumber, TokNumber, TokNumber, TokNumber});
			
			rr = new TokenNumber(tec.send(((TokenString*)inputs[0])->getCleanString(),
												(unsigned char)((TokenNumber*)inputs[1])->getValue(),
												(unsigned char)((TokenNumber*)inputs[2])->getValue(),
												(unsigned char)((TokenNumber*)inputs[3])->getValue(),
												(unsigned char)((TokenNumber*)inputs[4])->getValue()));
		}else{
			throw std::string("Syntax error: function call of unrecognized function ") + m_name + " \nIn '" + ((Token *)tec.getRootNode())->printContent() + "'";
		}
		
		for(size_t i = 0; i < inputs.size(); ++i){
			delete inputs[i];
		}
		
		if(rr == nullptr)
			throw std::string("Generic error: call of function") + m_name + " did not result in any result \nIn '" + ((Token *)tec.getRootNode())->printContent() + "'";
		
		return rr;
	
	}catch(std::string excp){
		while(inputs.size() > 0){
			delete inputs.back();
			inputs.pop_back();
		}
		throw excp;
	}
}
bool TokenFunction::evaluateBoolSub(TokenEvaluationContext & tec) const{
	Token * tmp = evaluateInstance(tec);
	bool ret = tmp->evaluateBool(tec);
	delete tmp;
	return ret;
}
std::string TokenFunction::printContentSub() const {
	std::string ret = toString()+"(";
	
	for(size_t i = 0; i < m_arguments.size(); ++i){
		ret += m_arguments[i]->printContent();
		
		if(i+1 < m_arguments.size()){
			ret += ", ";
		}
	}
	
	return ret+")";
}
std::string TokenFunction::toStringSub() const {
	return m_name;
}
