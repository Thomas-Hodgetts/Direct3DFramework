#include "Player.h"

Player::Player()
{
	Speed = 2;
}
Player::~Player()
{

}

XMFLOAT3 Player::PlayerPos()
{
	return pos;
}

void Player::Create(ID3D11Device* Device)
{
	InitWorld();
	ObjectStruct Object = c_SystemLoad.LoadFromFile("/Objects/Cube.txt", Device);
	Object.Pos.z += 5;
	pos = Object.Pos;
	XMFLOAT4X4 NewPosition;
	XMStoreFloat4x4(&NewPosition, XMLoadFloat4x4(&c_ObjectController.MoveObject(ReturnWorld(), Object.Pos, XMFLOAT3(1.f, 1.0f, 1.f), (0), Axis::Y)));
	CreateObject(Object.MD, Object.Tex, NewPosition);
}

void Player::Transform(float walk, float strafe, float rotate)
{
	Walk(walk);
	Strafe(strafe);
	Rotate(rotate);
}

void Player::Walk(float t)
{
	XMFLOAT4X4 NewPosition;
	pos.z += t * Speed;
	XMStoreFloat4x4(&NewPosition, XMLoadFloat4x4(&c_ObjectController.MoveObject(ReturnWorld(), pos, XMFLOAT3(1.f, 1.0f, 1.f), (0), Axis::Y)));
	UpdateWorld(NewPosition);
}

void Player::Strafe(float t)
{
	XMFLOAT4X4 NewPosition;
	pos.x += t * Speed;
	XMStoreFloat4x4(&NewPosition, XMLoadFloat4x4(&c_ObjectController.MoveObject(ReturnWorld(), pos, XMFLOAT3(1.f, 1.0f, 1.f), (0), Axis::Y)));
	UpdateWorld(NewPosition);
}

void Player::Rotate(float t)
{
	XMFLOAT4X4 NewPosition;
	pos.y += t * Speed;
	XMStoreFloat4x4(&NewPosition, XMLoadFloat4x4(&c_ObjectController.MoveObject(ReturnWorld(), pos, XMFLOAT3(1.f, 1.0f, 1.f), (t), Axis::Y)));
	UpdateWorld(NewPosition);
}

void Player::SetSpeed(int speed)
{
	Speed = speed;
}

int Player::ReturnSpeed()
{
	return Speed;
}