#pragma once
#include "PhysicalObject.h"
class Cube : public PhysicalObject
{
public:
	void Create(ID3D11Device* Device);
	void Transform(float t, XMFLOAT4X4 planePos);
private:
	float Distance;
};

