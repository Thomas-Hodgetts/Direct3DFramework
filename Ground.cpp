#include "Ground.h"


void Ground::Create(ID3D11Device* Device, ID3D11VertexShader* vs)
{
	InitWorld();
	ObjectStruct Object = c_SystemLoad.LoadFromFile("/Objects/Ground.txt", Device);
	CreateObject(Object.MD, Object.Tex, ReturnWorld());
	XMFLOAT4X4 NewPosition;
	XMStoreFloat4x4(&NewPosition, XMLoadFloat4x4(&c_ObjectController.MoveObject(ReturnWorld(), Object.Pos, XMFLOAT3(30.f, 0.1f, 30.f), (90), Axis::Y)));
	UpdateWorld(NewPosition);
	VS = vs;
}

void Ground::SetSubResources()
{
}