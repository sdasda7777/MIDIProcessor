#pragma once

class Token;

class TokenWrapper{
	Token * m_content = nullptr;
	
public:
	/**
	 * Creates brand new TokenWrapper instance
	 * @param[in]	content	Token being wrapped
	 */
	TokenWrapper(Token * content);
	
	/**
	 * Deletes variables and RtMidiOut instances
	 */
	~TokenWrapper();
	
	/**
	 * Returns pointer to wrapped content
	 * @returns	pointer to content
	 */
	Token * getContentPointer();
	
	/**
	 * Changes inner pointer (deletes old if applicable)
	 * @param[in]	newPointer	pointer to new content
	 */
	void setContentPointer(Token * newPointer);
};