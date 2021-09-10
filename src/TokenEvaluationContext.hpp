#pragma once

class RtMidiIn;
class RtMidiOut;
class Token;

//! Context for evaluating tokens
/*!
	Provides information about level of recursion, root token and reference to the core.
*/
class TokenEvaluationContext{
	Token * m_rootNode; //!< root token of the evaluation
	std::map<std::string, Token*> * m_variables; //!< variables
	RtMidiIn * m_midiIn;
	RtMidiOut * m_midiOut;
	std::string * m_defaultInputPortName;
	std::string * m_defaultOutputPortName;
	
public:
	/**
	 * Creates brand new TokenEvaluationContext instance
	 * @param[in]		rootNode				root of the evaluation
	 * @param[in,out]	variables				variables
	 * @param[in,out]	midiIn					RtMidiIn instance
 	 * @param[in,out]	midiOut					RtMidiOut instance
	 * @param[in]		defaultInputPortName	name of default input port
 	 * @param[in]		defaultOutputPortName	name of default output port
	 */
	TokenEvaluationContext(Token * rootNode, std::map<std::string, Token*> * variables, 
							RtMidiIn * midiIn, RtMidiOut * midiOut,
							std::string * defaultInputPortName, std::string * defaultOutputPortName);
	
	/**
	 * Returns pointer to root token
	 * @returns	pointer to root token
	 */
	Token * getRootNode();
	
	/**
	 * Calls the parent to print text
	 * @param[in]	text	text that is to be printed
	 */
	void print(std::string text) const;
	
	/**
	 * Calls the parent to print text, then new line
	 * @param[in]	text	text that is to be printed
	 */
	void printLn(std::string text) const;

	
	/**
	 * Attempts to retrieve a variable
	 * @param[in]	name	variable name
	 * @returns				pointer to variable value or nullptr
	 */
	Token * getVariable(std::string name);
	
	/**
	 * Sets a variable, deletes old if one existed
	 * @param[in]	name	variable name
	 * @param[in]	newval	value to be saved
	 */
	void setVariable(std::string name, Token * newval);
	
	/**
	 * Attempts to erase a variable
	 * @param[in]	name	variable name
	 * @returns				true if one existed, false otherwise
	 */
	bool eraseVariable(std::string name);
	
	/**
	 * Attempts to send message to default out port
	 * @param[in]	messageType	type of the midi message (8-15)
	 * @param[in]	channel		number of the channel (0-15)
	 * @param[in]	byte1		value of the first byte (0-127)
	 * @param[in]	byte2		value of the second byte (0-127) or -1 if not applicable
	 * @returns					true if successfully sent
	 */
	bool sendToDefault(unsigned char messageType, unsigned char channel, unsigned char byte1, unsigned char byte2);
};
