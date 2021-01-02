#pragma once
#include "Structures.h"
#include "Flotilla.h"
#include "Camera.h"
#include <time.h>


//<summary> Contains Key Functions for the framework as well as managing the central data for the framework
class System
{
public:

	System();
	~System();

	void InitSystems(SystemData* SD);
	void Clean();


	//Adds a flotilla type to m_Flotillas
	void AddFlotillaToFlotillaList(Flotilla flotilla);
	//Clears m_Flotilla using specified values by the user using Erase(). put 0 for both to just call Clear() on the Vector.
	void ClearFlotillaList(int StartPos, int EndPos);
	//Returns One of the flotillas
	Flotilla ReturnFlotilla(int Pos);
	//Returns the Systems Camera data. The camera flotilla is usually 0
	CameraBufferData ReturnCameraBufferData(int FlotillaPos, int SpecificCam);
	//Calls the update on the objects in m_flotillas
	void Update();
	//Calls the update on the objects in m_flotillas
	void Draw(ID3D11DeviceContext* IC, ID3D11Buffer* _pCB, ConstantBuffer cb);

protected:

	clock_t ReturnClock();


private:

	HINSTANCE               _hInst;
	HWND                    _hWnd;
	D3D_DRIVER_TYPE         _driverType;
	D3D_FEATURE_LEVEL       _featureLevel;
	ID3D11Device*			_pd3dDevice;
	ID3D11DeviceContext*	_pImmediateContext;
	IDXGISwapChain*			_pSwapChain;
	ID3D11RenderTargetView* _pRenderTargetView;
	ID3D11VertexShader*		_pVertexShader;
	ID3D11PixelShader*		_pPixelShader;
	ID3D11InputLayout*		_pVertexLayout;
	ID3D11Buffer*			_pVertexBuffer;
	ID3D11Buffer*			_pIndexBuffer;
	ID3D11Buffer*			_pConstantBuffer;
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11Texture2D*		_depthStencilBuffer;
	ID3D11RasterizerState*	_wireFrame;
	ID3D11RasterizerState*	_solidFrame;
	ID3D11SamplerState*		_pSamplerLinear;
	ID3D11BlendState*		_pTransparency;

	//Stores all of the Folitllas the user generates. m_Flottilas[0] is usually taken by Cameras automatically.
	std::vector<Flotilla>	m_Flottilas;

	clock_t					m_ProgramTime;
	void Clock();
};

