#pragma once
#include "PhysicalObject.h"
class Plane : public PhysicalObject
{
public:
	void Create(ID3D11Device* Device);
	void Transform(float t);

private:

};

