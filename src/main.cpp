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
#include "TokenEvaluationContext.hpp"

Token * tokenTree = NULL;
TokenEvaluationContext * tec = NULL;
std::map<std::string, RtMidiIn *> inputPorts;
std::string * defaultOutputPortName = NULL;

bool done;
static void finish(int ignore){ done = true; }

int main(int argc, char ** argv){
	if(argc == 1){
		//Creates instances used for reading numbers of ports
		RtMidiIn * midiIn = new RtMidiIn();
		RtMidiOut * midiOut = new RtMidiOut();
	
		//Prints usage
		std::cout << "Usage: " << argv[0] << " -i 'Input Port Name' -o 'Output Port Name' -f scriptName.midiproc" << std::endl;
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
						if(defaultOutputPortName != NULL)
							delete defaultOutputPortName;
						defaultOutputPortName = new std::string(argv[i]);
					}else if(lastFlag != -1 && argv[lastFlag][1] == 'f'){
		                //Taken from https://stackoverflow.com/a/116192
		                std::ifstream in( argv[i] );
		                std::getline(in, script, std::string::traits_type::to_char_type(std::string::traits_type::eof()));
					}
				}
				
				delete midiIn;
				
		        if(script == ""){
		            std::cout << "No script was specified, or specified script file was empty" << std::endl;
		            goto cleanup;
		        }
            }
			
			//Builds executable tree from loaded script
			std::vector<std::string> almostTokens = StaticMethods::LexicallySplitString(script);
			tokenTree = StaticMethods::BuildATokenTree(almostTokens);
			
			//std::cout << std::endl << "'" << tokenTree->printContent() << "'" << std::endl << std::endl;
			
			//Creates evaluation context
			tec = new TokenEvaluationContext(tokenTree, defaultOutputPortName);
			
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
						tec->setVariable("SOURCE_PORT", new TokenString(it->first, true));
						tec->setVariable("MESSAGE_TYPE", new TokenNumber((message[0] >> 4) & 15));
						tec->setVariable("CHANNEL", new TokenNumber(message[0] & 15));
						tec->setVariable("BYTE_1", new TokenNumber(message[1]));
						tec->setVariable("BYTE_2", new TokenNumber((message.size() > 2 ? message[2] : -1)));
						
						//Evaluates tokenTree in current context
						tokenTree->evaluateBool(*tec);
					}
				}
			}
		}catch(std::string excp){
			std::cout << std::endl << excp << std::endl;
			std::cout << "In '" << script << "'" << std::endl;
		}
	}
	
	//Cleans up variables
	cleanup:
	
	if(tokenTree != NULL){
		delete tokenTree;
		tokenTree = NULL;
	}
	if(tec != NULL){
		delete tec;
		tec = NULL;
	}
	
	for(std::map<std::string, RtMidiIn*>::iterator it = inputPorts.begin(); it != inputPorts.end(); ++it){
		delete it->second;
	}
	inputPorts.clear();
	
	if(defaultOutputPortName != NULL){
		delete defaultOutputPortName;
		defaultOutputPortName = NULL;
	}
	
	return 0;
}