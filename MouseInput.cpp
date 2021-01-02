#include "MouseInput.h"

MouseInput::MouseInput(HWND* hWnd)
{
	m_WindowToTrack = hWnd;
}
MouseInput::~MouseInput()
{
	delete m_WindowToTrack;
}

void MouseInput::UpdateMousePos(MSG* wMsg)
{

	switch (wMsg->message)
	{
		case WM_MOUSEMOVE:
		{
			PushBackPrevPos();
			POINT p;
			GetCursorPos(&p);
			m_CurrentData.xPos = p.x;
			m_CurrentData.yPos = p.y;			
			break;
		}
		case WM_LBUTTONDOWN:
		{

			break;
		}
	}
}

void MouseInput::PushBackPrevPos()
{
	m_CurrentData.PrevXPos = m_CurrentData.xPos;
	m_CurrentData.PrevYPos = m_CurrentData.yPos;
}
