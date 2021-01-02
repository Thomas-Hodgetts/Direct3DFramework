#pragma once
#include "System.h"
class KeyboardInput
{
public:
	KeyboardInput();
	~KeyboardInput();

	//Resets the current key to 0
	void ResetCurrentKey() { m_CurrentKey = 0; };
	//If the user wishes to recieve inputs as fast as possible they set this to false. Set to false by default
	void SetSkipDelay(bool Choice) { m_SkipDelay = Choice; };
	//Allows the user to control the delay until the next key is read. set to 0.3 as default
	void SetKeyDelay(int Delay) { m_KeyDelay = Delay; };
	//If no key is found it will translate new message
	void TranslateMsg(MSG* msg);
	//Gets a new message from the WinMain
	void UpdateMSG(MSG* msg);
	//Sees if the keyboard input mathces the passed Char. Need time to be inputed for key delay
	bool DoesKeyMatch(WPARAM KeyToCompare, float timeOfCall);
	//Returns the WPARAM from the windows message loop
	WPARAM ReturnCurrentKey() { return m_CurrentKey; };
	
private:

	WPARAM m_CurrentKey;
	bool m_MsgHandled;
	bool m_Translated;
	bool m_SkipDelay;
	float m_KeyDelay = 0.3f;
	float m_PreviousCall;
};

