#include <map>
#include <string>

#include "../../rtmidi/RtMidi.h"
#include "TokenEvaluationContext.hpp"
#include "Tokens_Token.hpp"
#include "TokenWrapper.hpp"

TokenEvaluationContext::TokenEvaluationContext(Token * rootNode, std::vector<std::string> paramOutputPortNames) : 
	m_rootNode(rootNode), m_paramOutputPortNames(paramOutputPortNames){
	
	RtMidiOut * tmpOut = new RtMidiOut();
	unsigned int outPortsCnt = tmpOut->getPortCount();
	
	//Opens specified ports or creates virtual one (if applicable)
	if(paramOutputPortNames.size() == 0){
		std::cout << "No output port specified" << std::endl;
	}
	for(size_t i = 0; i < paramOutputPortNames.size(); ++i){
		bool exists = false;
		unsigned int portNumber = 0;
		for(unsigned int porti = 0; porti < outPortsCnt; ++porti){
			if(tmpOut->getPortName(porti) == paramOutputPortNames[i]){
				exists = true;
				portNumber = porti;
				break;
			}
		}
		
		if(!exists){
			#if defined( __LINUX_ALSA__ ) or defined( __UNIX_JACK__ )
				RtMidiOut * tmp = new RtMidiOut();
				tmp->openVirtualPort(paramOutputPortNames[i]);
				m_openOutputPorts[paramOutputPortNames[i]] = tmp;
				std::cout << "Output port '" << paramOutputPortNames[i] << "' was created" << std::endl;
			#else
				std::cout << "Output port '" << paramOutputPortNames[i] << "' not found" << std::endl;
			#endif
		}else{
			RtMidiOut * tmp = new RtMidiOut();
			tmp->openPort(portNumber);
			m_openOutputPorts[paramOutputPortNames[i]] = tmp;
			std::cout << "Output port '" << paramOutputPortNames[i] << "' was opened" << std::endl;
		}
	}
	
	delete tmpOut;
}

TokenEvaluationContext::~TokenEvaluationContext(){
	for(std::map<std::string, RtMidiOut*>::iterator it = m_openOutputPorts.begin(); it != m_openOutputPorts.end(); ++it){
		it->second->closePort();
		delete it->second;
	}
	m_openOutputPorts.clear();
	
	for(std::map<std::string, TokenWrapper*>::iterator it = m_variables.begin(); it != m_variables.end(); ++it){
		delete it->second;
	}
	m_variables.clear();
}

Token * TokenEvaluationContext::getRootNode(){
	return m_rootNode;
}

void TokenEvaluationContext::print(std::string text) const {
	std::cout << text;
}

void TokenEvaluationContext::printLn(std::string text) const {
	std::cout << text << std::endl;
}

TokenWrapper * TokenEvaluationContext::getVariable(std::string name){	
	std::map<std::string, TokenWrapper*>::iterator varit = m_variables.find(name);
	if(varit != m_variables.end()){
		return varit->second;
	}
	return nullptr;
}

void TokenEvaluationContext::setVariable(std::string name, Token * newval){
	std::map<std::string, TokenWrapper*>::iterator varit = m_variables.find(name);
	if(varit != m_variables.end()){
		delete varit->second;
	}
	m_variables[name] = new TokenWrapper(newval);
}

bool TokenEvaluationContext::eraseVariable(std::string name){
	std::map<std::string, TokenWrapper*>::iterator varit = m_variables.find(name);
	if(varit != m_variables.end()){
		delete varit->second;
		m_variables.erase(varit);
		return true;
	}
	return false;
}

bool TokenEvaluationContext::sendToDefault(unsigned char messageType, unsigned char channel, unsigned char byte1, unsigned char byte2){
	if(m_paramOutputPortNames.size() > 0){
		std::map<std::string, RtMidiOut*>::iterator it = m_openOutputPorts.find(m_paramOutputPortNames[0]);
		
		if(it != m_openOutputPorts.end()){
			std::vector<unsigned char> message;
			constructMessage(message, messageType, channel, byte1, byte2);
			it->second->sendMessage(&message);
			
			return true;
		}
	}
	return false;
}

bool TokenEvaluationContext::send(const std::string & portName, unsigned char messageType, unsigned char channel, unsigned char byte1, unsigned char byte2){
	std::map<std::string, RtMidiOut*>::iterator it = m_openOutputPorts.find(portName);
	
	//If not open yet, tries to open new existin port
	if(it == m_openOutputPorts.end()){
		RtMidiOut * tmp = new RtMidiOut();
		unsigned int outPortsCnt = tmp->getPortCount();
		for(unsigned int porti = 0; porti < outPortsCnt; ++porti){
			if(tmp->getPortName(porti) == portName){
				RtMidiOut * tmprt = new RtMidiOut();
				tmprt->openPort(porti);
				m_openOutputPorts[portName] = tmprt;
				break;
			}
		}
		delete tmp;
		it = m_openOutputPorts.find(portName);
	}
	
	//If not open yet, either returns false or opens virtual
	if(it == m_openOutputPorts.end()){
		#if defined( __LINUX_ALSA__ ) or defined( __UNIX_JACK__ )
			RtMidiOut * tmprt = new RtMidiOut();
			tmprt->openVirtualPort(portName);
			m_openOutputPorts[portName] = tmprt;
			it = m_openOutputPorts.find(portName);
		#else
			return false;
		#endif	
	}
	
	std::vector<unsigned char> message;
	constructMessage(message, messageType, channel, byte1, byte2);
	(*it).second->sendMessage(&message);
	
	return true;
}

void TokenEvaluationContext::constructMessage(std::vector<unsigned char> & message, unsigned char messageType, unsigned char channel, unsigned char byte1, unsigned char byte2){
	if(messageType < 8)
		messageType = 8;
	else if(messageType > 15)
		messageType = 15;
	if(channel > 15)
		channel = 15;
	if(byte1 > 127)
		byte1 = 127;
	if(byte2 > 127 && byte2 != (unsigned char) -1)
		byte2 = 127;
	
	message.push_back((messageType << 4) | channel);
	message.push_back(byte1);
	if(byte2 != (unsigned char) -1){
		message.push_back(byte2);
	}
}