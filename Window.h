#pragma once
#include "PhysicalObject.h"
class Window :	public PhysicalObject
{
public:

	Window();
	~Window();

	void Create(ID3D11Device* Device, ID3D11BlendState* Blendstate, BlendFactor* BlendFactor);

private:
};

