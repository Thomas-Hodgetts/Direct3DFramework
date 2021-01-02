#pragma once

#include <conio.h>
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <vector>
#include "resource.h"
#include "Loader.h"
#include "Structures.h"
#include "MouseInput.h"
#include "KeyboardInput.h"
#include "Plane.h"
#include "Cube.h"
#include "Camera.h"
#include "Window.h"
#include "Billboard.h"
#include "Player.h"
#include "Ground.h"

using namespace DirectX;


class Application
{
private:
	HINSTANCE               _hInst;
	HWND                    _hWnd;
	D3D_DRIVER_TYPE         _driverType;
	D3D_FEATURE_LEVEL       _featureLevel;
	ID3D11Device*           _pd3dDevice;
	ID3D11DeviceContext*    _pImmediateContext;
	IDXGISwapChain*         _pSwapChain;
	ID3D11RenderTargetView* _pRenderTargetView;
	ID3D11VertexShader*     _pVertexShader;
	ID3D11PixelShader*      _pPixelShader;
	ID3D11InputLayout*      _pVertexLayout;
	ID3D11Buffer*           _pVertexBuffer;
	ID3D11Buffer*           _pIndexBuffer;
	ID3D11Buffer*           _pConstantBuffer;
	XMFLOAT4X4              _world, _world2, _world3, _world4, _world5, _world6, _world7, _world8;
	XMFLOAT4X4              _view;
	XMFLOAT4X4              _projection;
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11Texture2D*		_depthStencilBuffer;
	ID3D11RasterizerState*	_wireFrame;
	ID3D11RasterizerState*	_solidFrame;
	ID3D11Buffer*			_pyramidVertexBuffer;
	ID3D11Buffer*			_pyramidIndexBuffer;
	ID3D11Buffer*			_floorVertexBuffer;
	ID3D11Buffer*			_floorIndexBuffer;
	ID3D11ShaderResourceView* _pTextureRV = nullptr;
	ID3D11SamplerState*		_pSamplerLinear = nullptr;
	ID3D11BlendState*		Transparency;


private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	void Cleanup();
	void SetWireFrame();
	XMFLOAT3 VertexNormalAveraging(SimpleVertex VertexList[], int VertexPosition, WORD IndicesList[]);
	XMFLOAT3 VertexNormalAveragingWithVectors(std::vector<SimpleVertex> VertexList, int VertexPosition, std::vector<WORD> IndicesList);
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitShadersAndInputLayout();
	HRESULT InitVertexAndIndexBuffer();
	HRESULT InitPyrmidBuffer();
	HRESULT InitFloorBuffer(int Length, int Width);
	HRESULT InitFloorIndexBuffer(int Length, int Width);
	void TransferDataToSystem();

	UINT _WindowHeight;
	UINT _WindowWidth;
	
	MSG msg;
	float Time;
	System* m_System;

	//Objects
	MeshData objPlane;
	Plane m_Plane;
	Cube m_SkyBox;
	Window m_Window;
	Billboard m_Billboard1, m_Billboard2, m_Billboard3, m_Billboard4;
	Player m_Player;
	Ground m_Ground;

	//Shaders
	ID3D11VertexShader* m_VertexShader;

	//Cameras
	Camera* m_Camera;
	Camera* m_Camera2;
	Camera* m_Camera3;
	Camera* m_Camera4;
	int CurrentCam = 0;
	
	//Input
	MouseInput* m_MouseInput;
	KeyboardInput* m_KeyboardInput;

	//Optional Scene Controllers
	bool HideOriginal = false;
	bool _WireFrameOn;
	bool m_IsNight = false;
	bool m_HideBill = true;
	bool m_Hidetranslucent = false;


public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void KeyboardCheck(MSG* message);
	void MouseCheck(MSG* message);

	void Update();
	void Draw();
};

