#include "System.h"

System::System()
{
	_hInst = nullptr;
	_hWnd = nullptr;
	_driverType = D3D_DRIVER_TYPE_NULL;
	_featureLevel = D3D_FEATURE_LEVEL_11_0;
	_pd3dDevice = nullptr;
	_pImmediateContext = nullptr;
	_pSwapChain = nullptr;
	_pRenderTargetView = nullptr;
	_pVertexShader = nullptr;
	_pPixelShader = nullptr;
	_pVertexLayout = nullptr;
	_pVertexBuffer = nullptr;
	_pIndexBuffer = nullptr;
	_pConstantBuffer = nullptr;
	_depthStencilView = nullptr;
	_depthStencilBuffer = nullptr;
	_wireFrame = nullptr;
	_solidFrame = nullptr;
	_pSamplerLinear = nullptr;
	_pTransparency = nullptr;
}

System::~System()
{

}

void System::Clock()
{
	m_ProgramTime = clock();
}

void System::Update()
{
	for (int i = 0; i < m_Flottilas.size(); i++)
	{
		if (m_Flottilas[i].AutoUpdate)
		{
			m_Flottilas[i].Update();
		}
	}
}

void System::Draw(ID3D11DeviceContext* IC, ID3D11Buffer* _pCB, ConstantBuffer cb)
{
	for (int i = 0; i < m_Flottilas.size(); i++)
	{
		if (m_Flottilas[i].Drawable)
		{
			m_Flottilas[i].Draw(IC, _pCB, cb);
		}
	}
}

CameraBufferData System::ReturnCameraBufferData(int FlotillaPos, int SpecificCam)
{
	Camera* C = dynamic_cast <Camera*>(m_Flottilas[FlotillaPos].ReturnObject(SpecificCam));
	return C->ReturnViewPlusProjection();
	
}

void System::AddFlotillaToFlotillaList(Flotilla flotilla)
{
	m_Flottilas.push_back(flotilla);
}

void System::ClearFlotillaList(int StartPos, int EndPos)
{

	if (StartPos > 0 && EndPos == 0)
	{
		m_Flottilas.erase(m_Flottilas.begin() + StartPos);
	}
	else if (StartPos > 0 && EndPos > 0)
	{
		m_Flottilas.erase(m_Flottilas.begin() + StartPos, m_Flottilas.begin() + EndPos);
	}
	else if (StartPos == 0 && EndPos == 0)
	{
		m_Flottilas.clear();
	}
}

void System::InitSystems(SystemData* SD)
{
	_hInst = SD->hInst;
	_hWnd = SD->hWnd;
	_driverType = SD->driverType;
	_featureLevel = SD->featureLevel;
	_pd3dDevice = SD->d3dDevice;
	_pImmediateContext = SD->ImmediateContext;
	_pSwapChain = SD->SwapChain;
	_pRenderTargetView = SD->RenderTargetView;
	_pVertexShader = SD->VertexShader;
	_pPixelShader = SD->PixelShader;
	_pVertexLayout = SD->VertexLayout;
	_pVertexBuffer = SD->VertexBuffer;
	_pIndexBuffer = SD->IndexBuffer;
	_pConstantBuffer = SD->ConstantBuffer;
	_depthStencilView = SD->depthStencilView;
	_depthStencilBuffer = SD->depthStencilBuffer;
	_wireFrame = SD->wireFrame;
	_solidFrame = SD->solidFrame;
	_pSamplerLinear = SD->SamplerLinear;
	_pTransparency = SD->Transparency;
}

void System::Clean()
{
	if (_pImmediateContext) _pImmediateContext->ClearState();
	if (_pConstantBuffer) _pConstantBuffer->Release();
	if (_pVertexBuffer) _pVertexBuffer->Release();
	if (_pIndexBuffer) _pIndexBuffer->Release();
	if (_pVertexLayout) _pVertexLayout->Release();
	if (_pVertexShader) _pVertexShader->Release();
	if (_pPixelShader) _pPixelShader->Release();
	if (_pRenderTargetView) _pRenderTargetView->Release();
	if (_pSwapChain) _pSwapChain->Release();
	if (_pImmediateContext) _pImmediateContext->Release();
	if (_pd3dDevice) _pd3dDevice->Release();
	if (_depthStencilView) _depthStencilView->Release();
	if (_depthStencilBuffer) _depthStencilBuffer->Release();
	if (_wireFrame) _wireFrame->Release();
	if (_solidFrame) _solidFrame->Release();
}

Flotilla System::ReturnFlotilla(int Pos)
{
	return m_Flottilas[Pos];
}

clock_t System::ReturnClock()
{
	return m_ProgramTime / CLOCKS_PER_SEC;
}
