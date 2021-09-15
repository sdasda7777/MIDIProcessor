#pragma once

class RtMidiIn;
class RtMidiOut;
class Token;
class TokenWrapper;

//! Context for evaluating tokens
/*!
	Holds variables and handles all output.
*/
class TokenEvaluationContext{
	Token * m_rootNode; //!< root token of the evaluation
	std::map<std::string, std::pair<bool, TokenWrapper*>> m_variables; //!< variables
	std::vector<std::string> m_paramOutputPortNames;
	std::map<std::string, RtMidiOut*> m_openOutputPorts;
	
public:
	/**
	 * Creates brand new TokenEvaluationContext instance
	 * @param[in]		rootNode				root of the evaluation
 	 * @param[in]		paramOutputPortNames	names of output ports
	 */
	TokenEvaluationContext(Token * rootNode, std::vector<std::string> paramOutputPortNames);
	
	/**
	 * Deletes variables and RtMidiOut instances
	 */
	~TokenEvaluationContext();
	
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
	TokenWrapper * getVariable(std::string name);
	
	/**
	 * Sets a variable, deletes old if one existed
	 * @param[in]	name		variable name
	 * @param[in]	newval		value to be saved
	 * @param[in]	readOnly	whether variable should be read only (const, unmutable)
	 */
	void setVariable(std::string name, Token * newval, bool readOnly = false);
	
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
	
	/**
	 * Attempts to send message to specified out port
	 * @param[in]	portName	name of the output port
	 * @param[in]	messageType	type of the midi message (8-15)
	 * @param[in]	channel		number of the channel (0-15)
	 * @param[in]	byte1		value of the first byte (0-127)
	 * @param[in]	byte2		value of the second byte (0-127) or -1 if not applicable
	 * @returns					true if successfully sent
	 */
	bool send(const std::string & portName, unsigned char messageType, unsigned char channel, unsigned char byte1, unsigned char byte2);
private:
	
	/**
	 * Constructs well formed message
	 * @param[out]	message		output buffer
	 * @param[in]	messageType	type of the midi message (8-15)
	 * @param[in]	channel		number of the channel (0-15)
	 * @param[in]	byte1		value of the first byte (0-127)
	 * @param[in]	byte2		value of the second byte (0-127) or -1 if not applicable
	 */
	static void constructMessage(std::vector<unsigned char> & message, unsigned char messageType, unsigned char channel, unsigned char byte1, unsigned char byte2);
};
