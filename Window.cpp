#include "Window.h"


Window::Window()
{
}

Window::~Window()
{

}

void Window::Create(ID3D11Device* Device, ID3D11BlendState* Blendstate, BlendFactor* BlendFactor)
{
	InitWorld();
	ObjectStruct Object = c_SystemLoad.LoadFromFile("/Objects/Window.txt", Device);
	XMFLOAT4X4 NewPosition;
	SetTransparency(Blendstate, BlendFactor);
	XMStoreFloat4x4(&NewPosition, XMLoadFloat4x4(&c_ObjectController.MoveObject(ReturnWorld(), Object.Pos, XMFLOAT3(1.f, 1.0f, 1.f), (0.), Axis::Z)));
	CreateObject(Object.MD, Object.Tex, NewPosition);

}