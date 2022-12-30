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
#include "TokenEvaluationContext.hpp"

Token * tokenTree = NULL;
std::string * defaultInputPortName = NULL;
std::string * defaultOutputPortName = NULL;
std::map<std::string, Token*> variables;

bool done;
static void finish(int ignore){ done = true; }

/*
NOTE: ADBeta
I would suggest using an RtMidi Callback function, and use the RtMidi 
unsigned char array, instead of vector as it can be much faster. 

I have a program called MIDIPad which uses these methods in different places,
which may be useful if this project is still open for improvement.
*/

int main(int argc, char ** argv){
	srand (time(NULL));
	RtMidiIn * midiIn = new RtMidiIn();
	RtMidiOut * midiOut = new RtMidiOut();
	
	if(argc == 1){
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
                //std::cout << "Parameter " << i << ": '" << argv[i] << "'" << std::endl;

				if(argc > i+1 && strncmp(argv[i], "-i", std::min(strlen(argv[i]), (size_t)2)) == 0){
					if(defaultInputPortName != NULL)
						delete defaultInputPortName;
					defaultInputPortName = new std::string(argv[i+1]);
				}else if(argc > i+1 && strncmp(argv[i], "-o", std::min(strlen(argv[i]), (size_t)2)) == 0){
					if(defaultOutputPortName != NULL)
						delete defaultOutputPortName;
					defaultOutputPortName = new std::string(argv[i+1]);
				}else if(argc > i+1 && strncmp(argv[i], "-f", std::min(strlen(argv[i]), (size_t)2)) == 0){
                    //Taken from https://stackoverflow.com/a/116192
                    std::ifstream in( argv[i+1] );
                    std::getline( in, script, std::string::traits_type::to_char_type(std::string::traits_type::eof() ) );
				}
			}

            if(script == ""){
                std::cout << "No script was specified, or specified script file was empty" << std::endl;
                goto cleanup;
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
			}
		}catch(std::string excp){
			std::cout << std::endl << excp << std::endl;
			std::cout << "In '" << script << "'" << std::endl;
		}
	}
	cleanup:
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
