#include <map>
#include <string>

#include "../../rtmidi/RtMidi.h"
#include "TokenEvaluationContext.hpp"
#include "Tokens_Token.hpp"

TokenEvaluationContext::TokenEvaluationContext(Token * rootNode, std::map<std::string, Token*> * variables, 
							RtMidiIn * midiIn, RtMidiOut * midiOut,
							std::string * defaultInputPortName, std::string * defaultOutputPortName) : 
	m_rootNode(rootNode), m_variables(variables), m_midiIn(midiIn), m_midiOut(midiOut),
	m_defaultInputPortName(defaultInputPortName), m_defaultOutputPortName(defaultOutputPortName){
	
	unsigned int inPortsCnt = midiIn->getPortCount();
	unsigned int outPortsCnt = midiOut->getPortCount();
	
	//Opens specified ports or creates virtual ones (if applicable)
	if(defaultInputPortName != NULL){
		for(unsigned int porti = 0; porti < inPortsCnt; ++porti){
			if(midiIn->getPortName(porti) == *defaultInputPortName){
				midiIn->openPort(porti);
				midiIn->ignoreTypes(false, false, false);
			}
		}
		if(!midiIn->isPortOpen()){
			#if defined( __LINUX_ALSA__ ) or defined( __UNIX_JACK__ )
				midiIn->openVirtualPort(*defaultInputPortName);
				std::cout << "Input port '" << *defaultInputPortName << "' was created" << std::endl;
			#else
				std::cout << "Input port '" << *defaultInputPortName << "' not found" << std::endl;
			#endif
		}
	}else{
		std::cout << "No input port specified" << std::endl;
	}
	
	if(defaultOutputPortName != NULL){
		for(unsigned int porti = 0; porti < outPortsCnt; ++porti){
			if(midiOut->getPortName(porti) == *defaultOutputPortName){
				midiOut->openPort(porti);	
			}
		}
		if(!midiOut->isPortOpen()){
			#if defined( __LINUX_ALSA__ ) or defined( __UNIX_JACK__ )
				midiOut->openVirtualPort(*defaultOutputPortName);
				std::cout << "Output port '" << *defaultOutputPortName << "' was created" << std::endl;
			#else
				std::cout << "Output port '" << *defaultOutputPortName << "' not found" << std::endl;
			#endif
		}
		if(midiOut->isPortOpen()){
			m_openOutputPorts[*defaultOutputPortName] = midiOut;
		}
	}else{
		std::cout << "No output port specified" << std::endl;
	}
}

TokenEvaluationContext::~TokenEvaluationContext(){
	for(std::map<std::string, RtMidiOut*>::iterator it = m_openOutputPorts.begin(); it != m_openOutputPorts.begin(); ++it){
		if((*it).first != *m_defaultOutputPortName){
			delete (*it).second;
		}
	}
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

Token * TokenEvaluationContext::getVariable(std::string name){	
	std::map<std::string, Token*>::iterator varit = m_variables->find(name);
	if(varit != m_variables->end()){
		return (*varit).second;
	}
	return nullptr;
}

void TokenEvaluationContext::setVariable(std::string name, Token * newval){
	std::map<std::string, Token*>::iterator varit = m_variables->find(name);
	if(varit != m_variables->end()){
		delete (*varit).second;
	}
	(*m_variables)[name] = newval;
}

bool TokenEvaluationContext::eraseVariable(std::string name){
	std::map<std::string, Token*>::iterator varit = m_variables->find(name);
	if(varit != m_variables->end()){
		delete (*varit).second;
		m_variables->erase(varit);
		return true;
	}
	return false;
}

bool TokenEvaluationContext::sendToDefault(unsigned char messageType, unsigned char channel, unsigned char byte1, unsigned char byte2){
	std::vector<unsigned char> message;
	constructMessage(message, messageType, channel, byte1, byte2);
	m_midiOut->sendMessage(&message);
	
	return true;
}

bool TokenEvaluationContext::send(const std::string & portName, unsigned char messageType, unsigned char channel, unsigned char byte1, unsigned char byte2){
	std::map<std::string, RtMidiOut*>::iterator it = m_openOutputPorts.find(portName);
	
	//If not open yet, tries to open new existin port
	if(it == m_openOutputPorts.end()){
		unsigned int outPortsCnt = m_midiOut->getPortCount();
		for(unsigned int porti = 0; porti < outPortsCnt; ++porti){
			if(m_midiOut->getPortName(porti) == portName){
				RtMidiOut * tmprt = new RtMidiOut();
				tmprt->openPort(porti);
				m_openOutputPorts[portName] = tmprt;
				break;
			}
		}
		it = m_openOutputPorts.find(portName);
	}
	
	//If not open yet, either returns false or opens virtual
	if(it == m_openOutputPorts.end()){
		#ifdef __WINDOWS_MM__
			return false;
		#else
			RtMidiOut * tmprt = new RtMidiOut();
			tmprt->openVirtualPort(portName);
			m_openOutputPorts[portName] = tmprt;
			it = m_openOutputPorts.find(portName);
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