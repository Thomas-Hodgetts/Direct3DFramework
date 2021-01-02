#pragma once
#include "PhysicalObject.h"
class Player : public PhysicalObject
{
public: 
	//Constructor
	Player();
	//Destructor
	~Player();
	//Responsoble for populating Texture and mesh details
	void Create(ID3D11Device* Device);
	//Calls the individual transform functions
	void Transform(float Walk, float Strafe, float Rotate);
	//Returns the players position
	XMFLOAT3 PlayerPos();
	//Sets the objects speed
	void SetSpeed(int speed);
	//Returns Object Speed
	int ReturnSpeed();
private:
	//Controls Speed
	int Speed;
	//Players Position
	XMFLOAT3 pos;
	//Moves the Player along the Z vector
	void Walk(float t);
	//Moves the Player along the X vector
	void Strafe(float t);
	//Rotates the Player on the Y vector
	void Rotate(float t);

};

