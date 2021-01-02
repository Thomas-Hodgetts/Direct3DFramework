#pragma once
#include "PhysicalObject.h"
class Billboard : public PhysicalObject
{
public:
	Billboard();
	~Billboard();

	void Create(ID3D11Device* Device);
	void UpdateDirection(CameraData CD);
};

