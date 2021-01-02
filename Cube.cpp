#include "Cube.h"
#include <random>

void Cube::Create(ID3D11Device* Device)
{
	InitWorld();
	ObjectStruct Object = c_SystemLoad.LoadFromFile("/Objects/Cube.txt", Device);
	XMFLOAT4X4 NewPosition;
	XMStoreFloat4x4(&NewPosition, XMLoadFloat4x4((&c_ObjectController.MoveObject(ReturnWorld(), Object.Pos, XMFLOAT3(50.f, 50.0f, 50.f), 0, Axis::Z))));
	CreateObject(Object.MD, Object.Tex, NewPosition);
}

void Cube::Transform(float t,XMFLOAT4X4 planePos)
{
	XMFLOAT4X4 NewPosition;
	XMStoreFloat4x4(&NewPosition, XMMatrixMultiply(XMLoadFloat4x4(&planePos), XMLoadFloat4x4(&c_ObjectController.MoveObject(ReturnWorld(), XMFLOAT3(Distance * 4, 0.0f, t), XMFLOAT3(1.f, 1.0f, 1.f), t, Axis::Z))));
	UpdateWorld(NewPosition);
}
