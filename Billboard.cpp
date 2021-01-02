#include "Billboard.h"


Billboard::Billboard()
{

}

Billboard::~Billboard()
{

}

void Billboard::Create(ID3D11Device* Device)
{
	InitWorld();
	ObjectStruct Object = c_SystemLoad.LoadFromFile("/Objects/PineTreeBill.txt", Device);
	CreateObject(Object.MD, Object.Tex, ReturnWorld());
	std::random_device rd; 
	std::mt19937 RandomGenerator(rd()); 
	std::uniform_int_distribution<> distr(-3, 3);
	XMFLOAT4X4 NewPosition;
	Object.Pos.x = distr(RandomGenerator);
	Object.Pos.y = distr(RandomGenerator);
	Object.Pos.z = distr(RandomGenerator);

	XMStoreFloat4x4(&NewPosition, XMLoadFloat4x4(&c_ObjectController.MoveObject(ReturnWorld(), Object.Pos, XMFLOAT3(0.1f, 2.f, 2.f), (90), Axis::Y)));
	UpdateWorld(NewPosition);
}


void Billboard::UpdateDirection(CameraData CD)
{
	XMFLOAT4X4 NewPosition;
	XMStoreFloat4x4(&NewPosition, XMMatrixLookToLH(XMLoadFloat3(&CD.m_eye), XMLoadFloat3(&CD.m_at), XMLoadFloat3(&CD.m_up)));
	//UpdateWorld(NewPosition);
}