#include "KeyboardInput.h"

KeyboardInput::KeyboardInput()
{
	m_MsgHandled = false;
	m_SkipDelay = false;
}

KeyboardInput::~KeyboardInput()
{

}

void KeyboardInput::TranslateMsg(MSG* msg)
{
	if (!m_MsgHandled)
	{
		m_Translated = TranslateMessage(msg);
		DispatchMessage(msg);
	}
}

void KeyboardInput::UpdateMSG(MSG* msg)
{
	m_Translated = false;
	if (msg->message >= WM_KEYFIRST && msg->message <= WM_KEYLAST)
	{
		switch (msg->wParam)
		{
		case 0x41: //A
			m_CurrentKey = 0x41;
			m_MsgHandled = true;
			break;
		case 0x42: //B
			m_CurrentKey = 0x42;
			m_MsgHandled = true;
			break;
		case 0x43: //C
			m_CurrentKey = 0x43;
			m_MsgHandled = true;
			break;
		case 0x44: //D
			m_CurrentKey = 0x44;
			m_MsgHandled = true;
			break;
		case 0x45: //E
			m_CurrentKey = 0x45;
			m_MsgHandled = true;
			break;
		case 0x46: //F
			m_CurrentKey = 0x46;
			m_MsgHandled = true;
			break;
		case 0x47: //G
			m_CurrentKey = 0x47;
			m_MsgHandled = true;
			break;
		case 0x48: //H
			m_CurrentKey = 0x48;
			m_MsgHandled = true;
			break;
		case 0x49: //I
			m_CurrentKey = 0x49;
			m_MsgHandled = true;
			break;
		case 0x4A: //J
			m_CurrentKey = 0x4A;
			m_MsgHandled = true;
			break;
		case 0x4B: //K
			m_CurrentKey = 0x4B;
			m_MsgHandled = true;
			break;
		case 0x4C: //L
			m_CurrentKey = 0x4C;
			m_MsgHandled = true;
			break;
		case 0x4D: //M
			m_CurrentKey = 0x4D;
			m_MsgHandled = true;
			break;
		case 0x4E: //N
			m_CurrentKey = 0x4E;
			m_MsgHandled = true;
			break;
		case 0x4F: //O
			m_CurrentKey = 0x4F;
			m_MsgHandled = true;
			break;
		case 0x50: //P
			m_CurrentKey = 0x50;
			m_MsgHandled = true;
			break;
		case 0x51: //Q
			m_CurrentKey = 0x51;
			m_MsgHandled = true;
			break;
		case 0x52: //R
			m_CurrentKey = 0x52;
			m_MsgHandled = true;
			break;
		case 0x53: //S
			m_CurrentKey = 0x53;
			m_MsgHandled = true;
			break;
		case 0x54: //T
			m_CurrentKey = 0x54;
			m_MsgHandled = true;
			break;
		case 0x55: //U
			m_CurrentKey = 0x55;
			m_MsgHandled = true;
			break;
		case 0x56: //V
			m_CurrentKey = 0x56;
			m_MsgHandled = true;
			break;
		case 0x57: //W
			m_CurrentKey = 0x57;
			m_MsgHandled = true;
			break;
		case 0x58: //X
			m_CurrentKey = 0x58;
			m_MsgHandled = true;
			break;
		case 0x59: //Y
			m_CurrentKey = 0x59;
			m_MsgHandled = true;
			break;
		case 0x5A: //Z
			m_CurrentKey = 0x5A;
			m_MsgHandled = true;
			break;
		default:
			m_MsgHandled = false;
			break;
		}
	}
}



bool KeyboardInput::DoesKeyMatch(WPARAM KeyToCompare, float timeOfCall)
{
	if (m_SkipDelay || timeOfCall > m_PreviousCall + m_KeyDelay)
	{
		if (KeyToCompare == m_CurrentKey)
		{
			m_MsgHandled = false;
			m_CurrentKey = 0;
			m_PreviousCall = timeOfCall;
			m_CurrentKey = 0;
			return true;
		}
		else
		{
			return false;
		}
	}
	else 
	{
		return false;
	}


}