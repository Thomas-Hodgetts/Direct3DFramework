#pragma once
#include "PhysicalObject.h"

class Ground :public PhysicalObject
{
public:
	void Create(ID3D11Device* Device, ID3D11VertexShader* vs);
	void SetSubResources();

private:
	ID3D11VertexShader* VS;
};

