#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>
#include <regex>
#include <string>
#include <vector>

#include <iostream>

#include "Tokens_Token.hpp"
#include "Tokens_TokenNumber.hpp"
#include "Tokens_TokenString.hpp"
#include "Tokens_TokenVariable.hpp"
#include "Tokens_TokenOperator.hpp"
#include "Tokens_TokenIndex.hpp"
#include "Tokens_TokenFunction.hpp"
#include "Tokens_TokenScope.hpp"
#include "Tokens_TokenIf.hpp"
#include "Tokens_TokenWhile.hpp"
#include "TokenWrapper.hpp"
#include "StaticMethods.hpp"
#include "TokenEvaluationContext.hpp"

std::string StaticMethods::ReplaceParams(std::string input, const std::vector<std::string> & params){
	std::string ret = input;
	
	for(size_t i = 0; i < ret.length(); ++i){
		if(ret[i] == '\\'){
			++i;
		}else if(ret[i] == '$' && i+1 < ret.length() && 
				ret[i+1] >= '0' && ret[i+1] <= '9'){
			
			size_t j = i+1;
			for(;j+1<ret.length(); ++j){
				if(ret[j+1] < '0' || ret[j+1] > '9')
					break;
			}
			
			std::stringstream str(ret.substr(i+1, j-i));
			size_t parno;
			str >> parno;
			
			if(parno < params.size()){
				size_t replen = params[parno].length();
				ret.replace(i, j-i+1, params[parno]);
				i += replen;
				--i;
			}
		}
	}
	
	return ret;		
}

std::vector<std::string> StaticMethods::LexicallySplitString(const std::string & code){
	std::vector<std::string> ret;

	bool escaped = false;
	bool comment = false;
	std::string tokenSoFar = "";
	
	size_t code_length = code.length();
	for (size_t i = 0; i < code_length; i++){
		if(comment){
			if(code[i] == '\n')
				comment = false;
			continue;
		}
		
		tokenSoFar += code[i];
		
		if(!escaped && code_length > i+1 && code[i] == '/' && code[i+1] == '/'){
			comment = true;
			tokenSoFar = "";
		}else if(i+1 == code.length() && !isspace(code[i])){
			ret.push_back(std::string(tokenSoFar));
			tokenSoFar = "";
		}else if(tokenSoFar.length() == 1){
			if(code[i] == '\"'){
				escaped = true;
			}else if(isspace(code[i])){
				tokenSoFar = "";
			}else if(	code[i] == ',' || code[i] == ';' || code[i] == '(' || code[i] == ')' || 
						code[i] == '{' || code[i] == '}' ||	code[i] == '[' || code[i] == ']' || 
						code[i] == '*' || code[i] == '/' ||	code[i] == '+' || code[i] == '-' ||
						code[i] == '=' || code[i] == '!' || code[i] == '>' || code[i] == '<' ||
						code[i] == '~' || code[i] == '&' ||	code[i] == '^' || code[i] == '|' ||
						code[i] == '%' || code[i] == '#'){
				ret.push_back(std::string(tokenSoFar));
				tokenSoFar = "";
			}else if(code_length > i+1 && (isspace(code[i+1]) || 
					code[i+1] == ',' || code[i+1] == ';' || code[i+1] == '(' || code[i+1] == ')' ||
					code[i+1] == '{' || code[i+1] == '}' ||	code[i+1] == '[' || code[i+1] == ']' ||
					code[i+1] == '*' || code[i+1] == '/' || code[i+1] == '+' || code[i+1] == '-' ||
					code[i+1] == '=' || code[i+1] == '!' ||	code[i+1] == '>' || code[i+1] == '<' ||
					code[i+1] == '~' || code[i+1] == '&' ||	code[i+1] == '^' || code[i+1] == '|' ||
					code[i+1] == '%' || code[i+1] == '#')){
				ret.push_back(std::string(tokenSoFar));
				tokenSoFar = "";
			}
		}else{
			if(!escaped){
				if(isspace(code[i+1]) || 
					code[i+1] == ',' || code[i+1] == ';' || code[i+1] == '(' || code[i+1] == ')' ||
					code[i+1] == '{' || code[i+1] == '}' || code[i+1] == '[' || code[i+1] == ']' ||
					code[i+1] == '*' || code[i+1] == '/' || code[i+1] == '+' || code[i+1] == '-' ||
					code[i+1] == '=' || code[i+1] == '!' || code[i+1] == '>' || code[i+1] == '<' ||
					code[i+1] == '~' || code[i+1] == '&' ||	code[i+1] == '^' || code[i+1] == '|' ||
					code[i+1] == '%' || code[i+1] == '#'){
						ret.push_back(std::string(tokenSoFar));
						tokenSoFar = "";
				}
			}else if(code[i] == '\\'){
				if(code[i+1] == '\\'){
					i++;
					tokenSoFar += code[i];
				}
				
				if(code[i+1] == '"'){
					i++;
					tokenSoFar += code[i];
				}
			}else if(code[i] == '"'){
				escaped = false;
				ret.push_back(std::string(tokenSoFar));
				tokenSoFar = "";
			}
		}
	}
	
	CombineMulticharOperators(ret);
	
	return ret;
}

void StaticMethods::CombineMulticharOperators(std::vector<std::string> & target){
	for(size_t i = 1; i < target.size(); i++){		
		if( (target[i-1] == "+" && target[i] == "+") ||
			(target[i-1] == "-" && target[i] == "-") ||
			(target[i-1] == "*" && target[i] == "*") ||
			(target[i-1] == "<" && target[i] == "=") ||
			(target[i-1] == ">" && target[i] == "=") ||
			(target[i-1] == "=" && target[i] == "=") ||
			(target[i-1] == "!" && target[i] == "=") ||
			(target[i-1] == "&" && target[i] == "&") ||
			(target[i-1] == "|" && target[i] == "|") ){
			target[i-1] += target[i];
			target.erase(target.begin()+i);
			i--;
		}
	}
}

std::string StaticMethods::JoinsStrings(const std::string & glue, const std::vector<std::string> & strings){
	std::string ret = "";
	
	if(strings.size() > 0){
		ret += strings[0];
	}
	for(size_t i = 1; i < strings.size(); ++i){
		ret += glue;
		ret += strings[i];
	}
	return ret;
}

std::string StaticMethods::SmudgeString(const std::string & source){
	std::string ret = "\"";
	
	for(size_t i = 0; i < source.length(); i++){
		if(source[i] == '\\'){
			ret += "\\\\";
		}else if(source[i] == '\n'){
			ret += "\\n";
		}else if(source[i] == '\"'){
			ret += "\\\"";
		}else{
			ret += source[i];
		}
	}
	
	ret += "\"";
	
	return ret;
}

std::string StaticMethods::PrettyPrintDouble(double d){
	//Inspired by: https://stackoverflow.com/a/554078, author is https://stackoverflow.com/users/14356/paul-beckingham
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(15) << d;
	std::string ret = oss.str();
	
	if( ret.find('.') != std::string::npos ){
		size_t lpos = ret.length() - 1;
		while(ret[lpos] == '0'){
			ret.pop_back();
			--lpos;
		}
		if(ret[lpos] == '.')
			ret.pop_back();
	}
	
	return ret;
}

std::vector<std::pair<bool,void*>> StaticMethods::TestAndTransformArguments(TokenEvaluationContext & tec, const std::string & name, const std::vector<Token*> & tokens, const std::function <bool(size_t)>& paramCountComparator, std::vector<Token_Type> expectedParamTypes){
	if(!paramCountComparator(tokens.size()))
		throw std::string("Syntax error: Function ") + name + " called with invalid amount of arguments\nIn '" + ((Token *)tec.getRootNode())->printContent() + "'";
	
	std::vector<std::pair<bool, void*>> ret;
	try{
		for(size_t i = 0; i < tokens.size(); ++i){
			Token_Type exptype = expectedParamTypes[i % expectedParamTypes.size()];
		
			if(exptype == TokNumber){
				ret.push_back(std::make_pair(true, tokens[i]->evaluateNumber(tec)));
			}else if(exptype == TokString){
				ret.push_back(std::make_pair(true, tokens[i]->evaluateString(tec)));
			}else if(exptype == TokArray){
				ret.push_back(std::make_pair(true, tokens[i]->evaluateArray(tec)));
			}else if(exptype == TokVariable){
				ret.push_back(std::make_pair(false, tokens[i]->evaluateReference(tec)));
			}else{
				throw std::string("Syntax error: Function ") + name + " called with invalid argument number " + std::to_string(i+1) + "\nIn '" + ((Token *)tec.getRootNode())->printContent() + "'";
			}
		}
		
		return ret;
	}catch(std::string excstr){
		for(size_t i = 0; i < ret.size(); ++i){
			if(ret[i].first){
				delete (Token*)ret[i].second;
			}else{
				delete (TokenWrapper*)ret[i].second;
			}
		}
		throw excstr;
	}
}

Token * StaticMethods::BuildATokenTree(const std::vector<std::string> & lexedExpression){

	/**
	Programmed based on pseudocode on https://en.wikipedia.org/wiki/Shunting-yard_algorithm,
			 modifications inpired by https://blog.kallisti.net.nz/2008/02/extension-to-the-shunting-yard-algorithm-to-allow-variable-numbers-of-arguments-to-functions/
	*/
	
	bool wasOperandLast = false;
	
	const std::string * strref;
	
	Operator_Rank tmpor;
	Operator_Rank topor;
	
	std::regex regexNumber("^-?[0-9]+(\\.[0-9]+)?$");
	std::regex regexString("^\"[\\s\\S]*\"$");
	std::regex regexVariable("^[a-zA-Z][a-zA-Z0-9_]*$");
		
	std::stack<Token *> operands;
	std::stack<long int> operators; //{--d, ++d, -d, +d} == {-4, -3, -2, -1}
	std::stack<int> argumentCounts;
	argumentCounts.push(0);
	
	try{	
		for(size_t i = 0; i < lexedExpression.size(); ++i){
			if(lexedExpression[i] == "if"){
				if(wasOperandLast)
					throw std::string( "Syntax error: expected operator, got '") + lexedExpression[i] + "'";
				
				if(lexedExpression.size() <= i+1 || lexedExpression[i+1] != "("){
					throw std::string( "Syntax error: expected '(' after 'if'");
				}
				
				argumentCounts.push(0);
				operators.push(i);
				wasOperandLast = false;
			}else if(lexedExpression[i] == "else"){
				if(wasOperandLast)
					throw std::string( "Syntax error: expected operator, got '") + lexedExpression[i] + "'";
				
				operators.push(i);
				wasOperandLast = false;
			}else if(lexedExpression[i] == "while"){
				if(wasOperandLast)
					throw std::string( "Syntax error: expected operator, got '") + lexedExpression[i] + "'";
				
				if(lexedExpression.size() <= i+1 || lexedExpression[i+1] != "("){
					throw std::string( "Syntax error: expected '(' after 'while'");
				}
				
				argumentCounts.push(0);
				operators.push(i);
				wasOperandLast = false;
			}else if(regex_match(lexedExpression[i], regexNumber)){
				if(wasOperandLast)
					throw std::string( "Syntax error: expected operator, got '") + lexedExpression[i] + "'";
				
				operands.push(new TokenNumber(stod(lexedExpression[i])));
				++argumentCounts.top();
				wasOperandLast = true;
			}else if(regex_match(lexedExpression[i], regexString)){
				if(wasOperandLast)
					throw std::string( "Syntax error: expected operator, got '") + lexedExpression[i] + "'";
				
				operands.push(new TokenString(lexedExpression[i]));
				++argumentCounts.top();
				wasOperandLast = true;
			}else if(regex_match(lexedExpression[i], regexVariable)){
				if(wasOperandLast)
					throw std::string( "Syntax error: expected operator, got '") + lexedExpression[i] + "'";
				
				if(lexedExpression.size() > i+1 && lexedExpression[i+1] == "("){				
					argumentCounts.push(0);
					operators.push(i);
					wasOperandLast = false;
				}else{
					operands.push(new TokenVariable(lexedExpression[i]));
					++argumentCounts.top();
					wasOperandLast = true;
				}
			}else if((lexedExpression[i] == "--" || lexedExpression[i] == "++" ) && wasOperandLast){
				Token * tmp = operands.top();
				operands.pop();
				Token * tmp2 = new TokenOperator("d"+lexedExpression[i], tmp, nullptr);
				operands.push( tmp2 );
				wasOperandLast = true;
			}else if((lexedExpression[i] == "--" || lexedExpression[i] == "++" ) && !wasOperandLast){
				if(lexedExpression[i] == "--"){
					operators.push(-4);
				}else{
					operators.push(-3);
				}
				wasOperandLast = false;
			}else if((lexedExpression[i] == "-" || lexedExpression[i] == "+" ) && !wasOperandLast){
				if(lexedExpression[i] == "-"){
					operators.push(-2);
				}else{
					operators.push(-1);
				}
				wasOperandLast = false;
			}else if(lexedExpression[i] == "!" || lexedExpression[i] == "~"){
				if(wasOperandLast)
					throw std::string( "Syntax error: expected binary operator, got '") + lexedExpression[i] + "'";
				
				operators.push( i );
				wasOperandLast = false;
			}else if(TokenOperator::isValidOperator(lexedExpression[i], tmpor)){
				if(!wasOperandLast)
					throw std::string( "Syntax error: expected operand, got '") + lexedExpression[i] + "'";
				
				while(operators.size() > 0 && 
					((strref = OperatorToString(operators.top(), lexedExpression)) || true) && 
					(TokenOperator::isValidOperator(*strref, topor)) &&
					((topor < tmpor) ||
						(topor == tmpor && tmpor != PrefIncDecNegations && tmpor != Power && tmpor != Assignment))){
					EmplaceOperator(operands, *strref, topor, argumentCounts.top());
					operators.pop();
				}
				
				operators.push( i );
				wasOperandLast = false;
			}else if(lexedExpression[i] == "[" || lexedExpression[i] == "(" || lexedExpression[i] == "{"){
				if((!wasOperandLast && lexedExpression[i] == "[") || (wasOperandLast && lexedExpression[i] != "["))
					throw std::string( "Syntax error: expected operator, got '") + lexedExpression[i] + "'";
				if(lexedExpression[i] != "(")
					argumentCounts.push(0);
				operators.push( i );
				wasOperandLast = false;
			}else if(lexedExpression[i] == "]"){
				if(!wasOperandLast && ( i < 1 || lexedExpression[i-1] != "["))
					throw std::string( "Syntax error: expected operand, got '") + lexedExpression[i] + "'";
				
				while(operators.size() > 0 && 
					((strref = OperatorToString(operators.top(), lexedExpression)) || true) && 
					(TokenOperator::isValidOperator(*strref, topor))){
					EmplaceOperator(operands, *strref, topor, argumentCounts.top());
					operators.pop();
				}
				
				if(operators.size() == 0)
					throw std::string( "Syntax error: unexpected '") + lexedExpression[i] + "'";
					
				if( operators.size() > 0 && operators.top() >= 0 && lexedExpression[operators.top()] == "["){					
					if(argumentCounts.size() == 0 || argumentCounts.top() != 1)
						throw std::string("Syntax error: multiple indexes supplied for operator []");
					
					Token * rhs = operands.top();
					operands.pop();
					--argumentCounts.top();
					Token * lhs = operands.top();
					operands.pop();
					--argumentCounts.top();
					
					operands.push(new TokenIndex(lhs, rhs));
					operators.pop();
					argumentCounts.pop();
				}
				wasOperandLast = true;
			}else if(lexedExpression[i] == ")"){
				if(!wasOperandLast && ( i < 1 || lexedExpression[i-1] != "("))
					throw std::string( "Syntax error: expected operand, got '") + lexedExpression[i] + "'";
					
				while(operators.size() > 0 && 
					((strref = OperatorToString(operators.top(), lexedExpression)) || true) && 
					(TokenOperator::isValidOperator(*strref, topor))){
					EmplaceOperator(operands, *strref, topor, argumentCounts.top());
					operators.pop();
				}
				
				if(operators.size() == 0)
					throw std::string( "Syntax error: unexpected '") + lexedExpression[i] + "'";
				
				wasOperandLast = true;
				if( operators.size() > 0 && operators.top() >= 0 && lexedExpression[operators.top()] == "("){
					operators.pop();
					
					if(operators.size() > 0 && operators.top() >= 0){
						if(lexedExpression[operators.top()] == "if" || lexedExpression[operators.top()] == "while"){
							if(argumentCounts.size() == 0 || argumentCounts.top() != 1)
								throw std::string("Syntax error: multiple expressions supplied as condition for control structure '") + lexedExpression[operators.top()] + "'";
							
							argumentCounts.pop();
							++argumentCounts.top();
							wasOperandLast = false;
						}else if(regex_match(lexedExpression[operators.top()], regexVariable)){
							std::vector<Token*> args;
							
							while(argumentCounts.top() > 0){
								args.push_back(operands.top());
								operands.pop();
								--argumentCounts.top();
							}
							
							reverse(args.begin(), args.end());
							
							operands.push(new TokenFunction(lexedExpression[operators.top()], args));
							operators.pop();
							argumentCounts.pop();
							++argumentCounts.top();
						}
					}	
				}
			}else if(lexedExpression[i] == "}"){
				if(wasOperandLast)
					throw std::string( "Syntax error: unexpected '}', you might be missing a ';'");
				
				while(operators.size() > 0 && 
					((strref = OperatorToString(operators.top(), lexedExpression)) || true) && 
					(TokenOperator::isValidOperator(*strref, topor))){
					EmplaceOperator(operands, *strref, topor, argumentCounts.top());
					operators.pop();
				}
				
				if(operators.size() == 0)
					throw std::string( "Syntax error: unexpected '") + lexedExpression[i] + "'";
				
				if( operators.size() > 0 && operators.top() >= 0 && lexedExpression[operators.top()] == "{"){
					operators.pop();
					
					
					std::vector<Token*> args;
					
					while(argumentCounts.top() > 0){
						args.push_back(operands.top());
						operands.pop();
						--argumentCounts.top();
					}
					
					reverse(args.begin(), args.end());
					
					operands.push(new TokenScope(args));
					//operators.pop();
					argumentCounts.pop();
					++argumentCounts.top();
				}
				
				//We've reached '}', so we have to check whether we can construct any control structures
				if((lexedExpression.size() <= i+1 || lexedExpression[i+1] != "else") ||
					(operators.size() > 0 && lexedExpression[operators.top()] == "while")){
					TryEmplaceControlStructures(operands, operators, argumentCounts, lexedExpression);
				}
				
				wasOperandLast = false;
			}else if(lexedExpression[i] == ","){
				if(!wasOperandLast)
					throw std::string( "Syntax error: expected operand, got '") + lexedExpression[i] + "'";
			
				while(operators.size() > 0 && 
					((strref = OperatorToString(operators.top(), lexedExpression)) || true) && 
					(TokenOperator::isValidOperator(*strref, topor))){
					EmplaceOperator(operands, *strref, topor, argumentCounts.top());
					operators.pop();
				}
				
				if(operators.size() == 0)
					throw std::string( "Syntax error: unexpected '") + lexedExpression[i] + "'";
					
				wasOperandLast = false;
			}else if(lexedExpression[i] == ";"){
				if(!wasOperandLast)
					throw std::string( "Syntax error: expected operand, got '") + lexedExpression[i] + "'";
			
				while(operators.size() > 0 && 
					((strref = OperatorToString(operators.top(), lexedExpression)) || true) && 
					(TokenOperator::isValidOperator(*strref, topor))){
					EmplaceOperator(operands, *strref, topor, argumentCounts.top());
					operators.pop();
				}
				
				//We've reached ';', so we have to check whether we can construct any control structures
				if((lexedExpression.size() <= i+1 || lexedExpression[i+1] != "else") ||
					(operators.size() > 0 && lexedExpression[operators.top()] == "while")){
					TryEmplaceControlStructures(operands, operators, argumentCounts, lexedExpression);
				}
				
				wasOperandLast = false;
			}else{
				throw std::string( "Syntax error: unexpected symbol '") + lexedExpression[i] + "'";
			}
		}
		
		while(operators.size() > 0 && 
			((strref = OperatorToString(operators.top(), lexedExpression)) || true) && 
			(TokenOperator::isValidOperator(*strref, topor))){
			EmplaceOperator(operands, *strref, topor, argumentCounts.top());
			operators.pop();
		}
		
		if(operators.size() != 0)
			throw std::string( "Syntax error: unexpected '") + *OperatorToString(operators.top(), lexedExpression) + "'";
		
		if(operands.size() > 1){
			std::vector<Token*> args;
			
			while(operands.size() > 0){
				args.push_back(operands.top());
				operands.pop();
			}
			reverse(args.begin(), args.end());
			
			return new TokenScope(args);
		}
		
		return operands.top();
	}catch(std::string exc){
		while(operands.size() > 0){
			delete operands.top();
			operands.pop();
		}
		throw exc + "\nIn '" + JoinsStrings(" ", lexedExpression)+ "'";
	}
}

void StaticMethods::TryEmplaceControlStructures(std::stack<Token *> & operands, std::stack<long int> & operators, 
												std::stack<int> & argumentCounts, const std::vector<std::string> & lexedExpression){
	if(operators.size() > 0 && lexedExpression[operators.top()] == "while"){
		Token * body = operands.top();
		operands.pop();
		--argumentCounts.top();
		Token * condition = operands.top();
		operands.pop();
		--argumentCounts.top();
		
		operands.push(new TokenWhile(condition, body));
		operators.pop();
		++argumentCounts.top();
	}else{
		while(operators.size() > 0 && (lexedExpression[operators.top()] == "if" || lexedExpression[operators.top()] == "else")){
			if(lexedExpression[operators.top()] == "else"){
				operators.pop();
				if(operators.size() == 0 || lexedExpression[operators.top()] != "if")
					throw std::string( "Syntax error: found 'else' without preceding 'if'");
				if(operands.size() < 3)
					throw std::string( "Syntax error: expected scope, found 'else'");
				
				Token * elsebody = operands.top();
				operands.pop();
				--argumentCounts.top();
				Token * ifbody = operands.top();
				operands.pop();
				--argumentCounts.top();
				Token * condition = operands.top();
				operands.pop();
				--argumentCounts.top();
				
				operands.push(new TokenIf(condition, ifbody, elsebody));
				operators.pop();
				++argumentCounts.top();
			}else if(lexedExpression[operators.top()] == "if"){
				Token * ifbody = operands.top();
				operands.pop();
				--argumentCounts.top();
				Token * condition = operands.top();
				operands.pop();
				--argumentCounts.top();
				
				operands.push(new TokenIf(condition, ifbody, nullptr));
				operators.pop();
				++argumentCounts.top();
			}
			
			if(operators.size() == 0 || lexedExpression[operators.top()] != "else") break;
		}
	}
}

void StaticMethods::EmplaceOperator(std::stack<Token*> & destination, const std::string & opName, Operator_Rank type, int & argCnt){
	if(type == PrefIncDecNegations){
		if(destination.size() < 1)
			throw std::string("Syntax Error: not enough operands near '") + opName + "'";
		
		Token * tmp = destination.top();
		destination.pop();
		Token * tmpr = new TokenOperator(opName, tmp, nullptr);
		destination.push(tmpr);
	}else{
		if(destination.size() < 2)
			throw std::string("Syntax Error: not enough operands near '") + opName + "'";
	
		Token * tmp2 = destination.top();
		destination.pop();
		Token * tmp1 = destination.top();
		destination.pop();
		Token * tmpr = new TokenOperator(opName, tmp1, tmp2);
		--argCnt;
		destination.push(tmpr);
	}
}

const std::string * StaticMethods::OperatorToString(long int operatorIndex, const std::vector<std::string> & source){
	if(operatorIndex < 0){
		return &(StaticMethods::CustomOperators[operatorIndex]);
	}else{
		return &(source[operatorIndex]);
	}
}

std::map<long int, std::string> StaticMethods::CustomOperators = {
	{-4, "--d"},
	{-3, "++d"},
	{-2, "-d"},
	{-1, "+d"}
};
