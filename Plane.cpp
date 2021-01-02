#include "Plane.h"


void Plane::Create(ID3D11Device* Device)
{
	InitWorld();
	ObjectStruct Object = c_SystemLoad.LoadFromFile("/Objects/Hercules.txt", Device);
	CreateObject(Object.MD, Object.Tex, c_ObjectController.MoveObject(ReturnWorld(), Object.Pos, XMFLOAT3(1.f, 1.0f, 1.f), (0), Axis::Z));
}

void Plane::Transform(float t)
{
	XMFLOAT4X4 NewPosition;
	XMStoreFloat4x4(&NewPosition , XMLoadFloat4x4(&c_ObjectController.MoveObject(ReturnWorld(), XMFLOAT3(0.0, 0.0f, 0.0f), XMFLOAT3(1.f, 1.0f, 1.f), (0), Axis::Z)));
	UpdateWorld(NewPosition);
}

