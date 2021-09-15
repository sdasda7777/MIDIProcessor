#include <iostream>

#include "Tokens_Token.hpp"
#include "TokenWrapper.hpp"

TokenWrapper::TokenWrapper(Token * content){
	m_content = content;
}

TokenWrapper::~TokenWrapper(){
	if(m_content != nullptr){
		delete m_content;
	}
}

Token * TokenWrapper::getContentPointer(){
	return m_content;
}

void TokenWrapper::setContentPointer(Token * newPointer){	
	if(m_content != nullptr){
		delete m_content;
	}
	m_content = newPointer;
}