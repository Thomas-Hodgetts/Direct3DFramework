#pragma once
#include "System.h"
#include <vector>
#include <windowsx.h>


class MouseInput
{
public:
	MouseInput(HWND* hWnd);
	~MouseInput();

	//Updates Mouse Data
	void UpdateMousePos(MSG* wMsg);

	//Returns the mouse position. Mouse position is updated when this is called
	MouseData ReturnMouseData() { return m_CurrentData; };

private:
	//Sets the current position of the mouse to the previous mouse position in MouseData
	void PushBackPrevPos();
	HWND* m_WindowToTrack;
	MouseData m_CurrentData;
	HDC m_HandleDeviceContext;

};

