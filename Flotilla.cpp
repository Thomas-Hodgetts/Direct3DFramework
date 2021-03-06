#include "Flotilla.h"

Flotilla::Flotilla()
{
	
}

Flotilla::~Flotilla()
{

}

void Flotilla::AddObject(Object* newObject)
{

	m_Objects.push_back(newObject);
}

void Flotilla::ClearObjects()
{
	for (int i = 0; i < m_Objects.size(); i++)
	{
		delete m_Objects[i];
	}
	m_Objects.clear();
}

void Flotilla::Update()
{
	for (int i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->Update();
	}
}

void Flotilla::Draw(ID3D11DeviceContext* IC, ID3D11Buffer* _pCB, ConstantBuffer cb)
{
	for (int i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->SetSubResources();
		m_Objects[i]->Draw(IC, _pCB, cb);
	}
}

Object* Flotilla::ReturnObject(int Placement)
{
	return m_Objects[Placement];
}

CameraBufferData* Flotilla::ReturnCameraBufferData()
{
	return NULL;
}

int Flotilla::ReturnVectorSize()
{
	return m_Objects.size();
}