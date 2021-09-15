#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <signal.h>

#if defined(__WINDOWS_MM__)
#include <time.h>
#endif

#include "../../rtmidi/RtMidi.h"
#include "Tokens_Token.hpp"
#include "Tokens_TokenNumber.hpp"
#include "Tokens_TokenString.hpp"
#include "Tokens_TokenArray.hpp"
#include "TokenEvaluationContext.hpp"
#include "TokenWrapper.hpp"

Token * tokenTree = nullptr;
TokenEvaluationContext * tec = nullptr;
std::map<std::string, RtMidiIn *> inputPorts;

bool done;
static void finish(int ignore){ done = true; }

int main(int argc, char ** argv){
	if(argc == 1){
		//Creates instances used for reading numbers of ports
		RtMidiIn * midiIn = new RtMidiIn();
		RtMidiOut * midiOut = new RtMidiOut();
	
		//Prints usage
		std::cout << "Usage: " << argv[0] << " -i 'Input Port Name' -o 'Output Port Name' -f 'scriptName.midiproc'" << std::endl;
		#if defined( __LINUX_ALSA__ ) or defined( __UNIX_JACK__ )
			std::cout << "(Nonexistent ports will be created)" << std::endl;
		#endif
		
		//Recovers numbers of ports
		unsigned int inPortsCnt = midiIn->getPortCount();
		unsigned int outPortsCnt = midiOut->getPortCount();
		
		//Prints names of ports to help user with operation
		std::cout << std::endl << "Available ports:" << std::endl;
		if( inPortsCnt == 0 ) {
			std::cout << "No input ports available!" << std::endl;
		}else{
			std::cout << "Input ports:" << std::endl;
		}
		for(unsigned int porti = 0; porti < inPortsCnt; ++porti){
			std::cout << "'" << midiIn->getPortName(porti) << "'" << std::endl;
		}
		if( outPortsCnt == 0 ) {
			std::cout << "No output ports available!" << std::endl;
		}else{
			std::cout << "Output ports:" << std::endl;
		}
		for(unsigned int porti = 0; porti < outPortsCnt; ++porti){
			std::cout << "'" << midiOut->getPortName(porti) << "'" << std::endl;
		}
		
		delete midiIn;
		delete midiOut;
	}else{
		srand (time(NULL));
		
		std::string script = "";
		std::vector<std::string> inputDevices;
		std::vector<std::string> outputDevices;
		try{
			{
				//Creates temporary instance used for creation of other input instances
				RtMidiIn * midiIn = new RtMidiIn();
				
				//Reads parameters
				int lastFlag = -1;
				for(int i = 1; i < argc; ++i){
		            //std::cout << "Parameter " << i << ": '" << argv[i] << "'" << std::endl;
					
					if(strlen(argv[i]) == 2 && argv[i][0] == '-'){
						lastFlag = i;
					}else if(lastFlag != -1 && argv[lastFlag][1] == 'i'){
						std::string portName(argv[i]);
						inputDevices.push_back(portName);
						std::map<std::string, RtMidiIn *>::iterator it = inputPorts.find(portName);
						
						if(it == inputPorts.end()){
							bool exists = false;
							unsigned int portNumber = 0;
							unsigned int inPortsCnt = midiIn->getPortCount();
							for(unsigned int porti = 0; porti < inPortsCnt; ++porti){
								if(midiIn->getPortName(porti) == portName){
									exists = true;
									portNumber = porti;
									break;
								}
							}
							
							if(!exists){
								#if defined( __LINUX_ALSA__ ) or defined( __UNIX_JACK__ )
									RtMidiIn * tmp = new RtMidiIn();
									tmp->openVirtualPort(portName);
									inputPorts[portName] = tmp;
									std::cout << "Input port '" << portName << "' was created" << std::endl;
								#else
									std::cout << "Input port '" << portName << "' does not exist" << std::endl;
								#endif
							}else{
								RtMidiIn * tmp = new RtMidiIn();
								tmp->openPort(portNumber);
								inputPorts[portName] = tmp;
								std::cout << "Input port '" << portName << "' was opened" << std::endl;
							}
						}
					}else if(lastFlag != -1 && argv[lastFlag][1] == 'o'){
						std::string portName(argv[i]);
						outputDevices.push_back(portName);
					}else if(lastFlag != -1 && argv[lastFlag][1] == 'f'){
		                //Taken from https://stackoverflow.com/a/116192
		                std::ifstream in( argv[i] );
		                std::getline(in, script, std::string::traits_type::to_char_type(std::string::traits_type::eof()));
					}
				}
				
				delete midiIn;
            }
			
			if(script == ""){
	            std::cout << "No script was specified, or specified script file was empty" << std::endl;
	        }else{
				//Builds executable tree from loaded script
				std::vector<std::string> almostTokens = StaticMethods::LexicallySplitString(script);
				tokenTree = StaticMethods::BuildATokenTree(almostTokens);
				
				//Prints content of generated tree (testing only)
				//std::cout << std::endl << "'" << tokenTree->printContent() << "'" << std::endl << std::endl;
				
				//Creates evaluation context, pushes input and  output port names as read-only variables
				tec = new TokenEvaluationContext(tokenTree, outputDevices);
				std::vector<Token *> inputPortNames;
				for(size_t i = 0; i < inputDevices.size(); ++i){
					inputPortNames.push_back(new TokenString(inputDevices[i], true));
				}
				tec->setVariable("INPUT_PORTS", new TokenArray(inputPortNames), true);
				std::vector<Token *> outputPortNames;
				for(size_t i = 0; i < outputDevices.size(); ++i){
					outputPortNames.push_back(new TokenString(outputDevices[i], true));
				}
				tec->setVariable("OUTPUT_PORTS", new TokenArray(outputPortNames), true);
				
				//Installs an interrupt handler function.
				done = false;
				(void) signal(SIGINT, finish);
				std::cout << "Processing in progress, quit with ctrl+c" << std::endl;
				
				
				std::vector<unsigned char> message;
				while( !done ) {
					for(std::map<std::string, RtMidiIn*>::iterator it = inputPorts.begin(); it != inputPorts.end(); ++it){					
						double stamp = it->second->getMessage( &message );
					
						if(message.size() > 1){
							//Stores message into variables
							tec->setVariable("DELTA", new TokenNumber(stamp));
							tec->setVariable("INPUT_PORT", new TokenString(it->first, true));
							tec->setVariable("MESSAGE_TYPE", new TokenNumber((message[0] >> 4) & 15));
							tec->setVariable("CHANNEL", new TokenNumber(message[0] & 15));
							tec->setVariable("BYTE_1", new TokenNumber(message[1]));
							tec->setVariable("BYTE_2", new TokenNumber((message.size() > 2 ? message[2] : -1)));
							
							//Evaluates tokenTree in current context
							tokenTree->evaluateBool(*tec);
						}
					}
				}
			}
		}catch(std::string excp){
			std::cout << std::endl << excp << std::endl;
			std::cout << "In '" << script << "'" << std::endl;
		}
	}
	
	//Cleans up variables	
	if(tokenTree != nullptr){
		delete tokenTree;
		tokenTree = nullptr;
	}
	if(tec != nullptr){
		delete tec;
		tec = nullptr;
	}
	
	for(std::map<std::string, RtMidiIn*>::iterator it = inputPorts.begin(); it != inputPorts.end(); ++it){
		delete it->second;
		it->second = nullptr;
	}
	inputPorts.clear();
	
	return 0;
}