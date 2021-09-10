#include <cstdlib>
#include <cstring>
#include <iostream>
#include <signal.h>

#include "../../rtmidi/RtMidi.h"
#include "Tokens_Token.hpp"
#include "Tokens_TokenNumber.hpp"
#include "TokenEvaluationContext.hpp"

Token * tokenTree = NULL;
std::string * defaultInputPortName = NULL;
std::string * defaultOutputPortName = NULL;
std::map<std::string, Token*> variables;

bool done;
static void finish(int ignore){ done = true; }

int main(int argc, char ** argv){
	srand (time(NULL));
	RtMidiIn * midiIn = new RtMidiIn();
	RtMidiOut * midiOut = new RtMidiOut();
	
	if(argc == 1){
		//Prints usage
		std::cout << "Usage: " << argv[0] << " -i 'Input Port Name' -o 'Output Port Name' < scriptName.midiproc" << std::endl;
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
		}
		for(unsigned int porti = 0; porti < inPortsCnt; ++porti){
			std::cout << "Input port " << porti << ": '" << midiIn->getPortName(porti) << "'" << std::endl;
		}
		if( outPortsCnt == 0 ) {
			std::cout << "No output ports available!" << std::endl;
		}
		for(unsigned int porti = 0; porti < outPortsCnt; ++porti){
			std::cout << "Output port " << porti << ": '" << midiOut->getPortName(porti) << "'" << std::endl;
		}
	}else{
		std::string script = "";
		try{
			//Reads parameters
			for(int i = 1; i < argc; ++i){
				if(argc > i+1 && strncmp(argv[i], "-i", std::min(strlen(argv[i]), (size_t)2)) == 0){
					if(defaultInputPortName != NULL)
						delete defaultInputPortName;
					defaultInputPortName = new std::string(argv[i+1]);
				}else if(argc > i+1 && strncmp(argv[i], "-o", std::min(strlen(argv[i]), (size_t)2)) == 0){
					if(defaultOutputPortName != NULL)
						delete defaultOutputPortName;
					defaultOutputPortName = new std::string(argv[i+1]);
				}
			}
			
			//Reads script from stdin
			std::cin >> std::noskipws;
			while(std::cin){
				std::string tmp;
				getline(std::cin, tmp);
				script += tmp + "\n";
			}
			
			//Builds executable tree from loaded script
			std::vector<std::string> almostTokens = StaticMethods::LexicallySplitString(script);
			tokenTree = StaticMethods::BuildATokenTree(almostTokens);
			
			//std::cout << std::endl << "'" << tokenTree->printContent() << "'" << std::endl << std::endl;
			
			//Creates evaluation context
			TokenEvaluationContext tec(tokenTree, &variables, midiIn, midiOut, defaultInputPortName, defaultOutputPortName);
			
			// Install an interrupt handler function.
			done = false;
			(void) signal(SIGINT, finish);
			std::cout << "Processing in progress, quit with ctrl+c" << std::endl;
			
			std::vector<unsigned char> message;
			while ( !done ) {
				double stamp = midiIn->getMessage( &message );
				
				if(message.size() > 1){
					//Stores message into variables
					tec.setVariable("DELTA", new TokenNumber(stamp));
					tec.setVariable("MESSAGE_TYPE", new TokenNumber((message[0] >> 4) & 15));
					tec.setVariable("CHANNEL", new TokenNumber(message[0] & 15));
					tec.setVariable("BYTE_1", new TokenNumber(message[1]));
					tec.setVariable("BYTE_2", new TokenNumber((message.size() > 2 ? message[2] : -1)));
					
					//Evaluates tokenTree in current context
					tokenTree->evaluateBool(tec);
				}
				
				/*int nBytes = message.size();
				for (int i = 0; i<nBytes; ++i )
					std::cout << "Byte " << i << " = " << std::hex << (int)message[i] << ", ";
				if ( nBytes > 0 )
					std::cout << "stamp = " << std::dec << stamp << std::endl;*/
			}
		}catch(std::string excp){
			std::cout << std::endl << excp << std::endl;
			std::cout << "In '" << script << "'" << std::endl;
		}
	}
	
	//Cleans up variables
	midiIn->closePort();
	delete midiIn;
	midiOut->closePort();
	delete midiOut;
	if(tokenTree != NULL) delete tokenTree;
	if(defaultInputPortName != NULL) delete defaultInputPortName;
	if(defaultOutputPortName != NULL) delete defaultOutputPortName;
	
	std::map<std::string, Token*>::iterator it;
	for(it = variables.begin(); it != variables.end(); ++it){
		delete it->second;
	}
	variables.clear();
	
	return 0;
}