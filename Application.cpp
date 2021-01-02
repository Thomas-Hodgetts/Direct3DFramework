#include "Application.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

Application::Application()
{
    m_System = nullptr;
    m_Camera = nullptr;
    m_Camera2 = nullptr;
    m_Camera3 = nullptr;
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
    _pyramidVertexBuffer = nullptr;
	_pyramidIndexBuffer = nullptr;
	_pConstantBuffer = nullptr;
}

void Application::TransferDataToSystem()
{
}

Application::~Application()
{
	Cleanup();
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
    if (FAILED(InitWindow(hInstance, nCmdShow)))
	{
        return E_FAIL;
	}

    RECT rc;
    GetClientRect(_hWnd, &rc);
    _WindowWidth = rc.right - rc.left;
    _WindowHeight = rc.bottom - rc.top;

    if (FAILED(InitDevice()))
    {
        Cleanup();

        return E_FAIL;
    }
    Loader L;

    m_System = new System();

	// Initialize the world matrix
	XMStoreFloat4x4(&_world, XMMatrixIdentity());
    XMStoreFloat4x4(&_world2, XMMatrixIdentity());
    XMStoreFloat4x4(&_world3, XMMatrixIdentity());
    XMStoreFloat4x4(&_world4, XMMatrixIdentity());
    XMStoreFloat4x4(&_world5, XMMatrixIdentity());
    XMStoreFloat4x4(&_world6, XMMatrixIdentity());
    XMStoreFloat4x4(&_world7, XMMatrixIdentity());
    XMStoreFloat4x4(&_world8, XMMatrixIdentity());

    // Init Mesh
    objPlane = L.LoadMesh("Hercules.obj", _pd3dDevice, false);

    //Set the mouse & keyboard input
    m_KeyboardInput = new KeyboardInput();
    m_MouseInput = new MouseInput(&_hWnd);


    //Create A list of Camera's
    Flotilla CameraFlotilla;
    CameraFlotilla.Drawable = false;

    // Initialize the camera view matrix
    XMVECTOR Eye = XMVectorSet(0.0f, 0.0f, -3.0f, 0.0f);
    XMVECTOR At = XMVectorSet(0.0f, 0.0f, 30.0f, 0.0f);
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMFLOAT3 e, a, u;
    XMStoreFloat3(&e, Eye);
    XMStoreFloat3(&a, At);
    XMStoreFloat3(&u, Up);
    m_Camera = new Camera(e, a, u, _WindowWidth, _WindowHeight, 0.01, 100.0f);
    CameraFlotilla.AddObject(m_Camera);

    Eye = XMVectorSet(0.0f, 5.0f, 3.0f, 0.0f);
    At = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMStoreFloat3(&e, Eye);
    XMStoreFloat3(&a, At);
    XMStoreFloat3(&u, Up);
    m_Camera2 = new Camera(e, a, u, _WindowWidth, _WindowHeight, 0.01, 100.0f);
    CameraFlotilla.AddObject(m_Camera2);


    Eye = XMVectorSet(0.0f, 0.0f, 6.f, 0.0f);
    At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMStoreFloat3(&e, Eye);
    XMStoreFloat3(&a, At);
    XMStoreFloat3(&u, Up);
    m_Camera3 = new Camera(e, a, u, _WindowWidth, _WindowHeight, 0.01, 100.0f);
    CameraFlotilla.AddObject(m_Camera3);


    Eye = XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f);
    At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMStoreFloat3(&e, Eye);
    XMStoreFloat3(&a, At);
    XMStoreFloat3(&u, Up);
    m_Camera4 = new Camera(e, a, u, _WindowWidth, _WindowHeight, 0.01, 100.0f);
    CameraFlotilla.AddObject(m_Camera4);

    m_System->AddFlotillaToFlotillaList(CameraFlotilla);



	return S_OK;
}

HRESULT Application::InitShadersAndInputLayout()
{
	HRESULT hr;

    // Compile the vertex shader
    ID3DBlob* pVSBlob = nullptr;
    hr = CompileShaderFromFile(L"DX11 Framework.fx", "VS", "vs_4_0", &pVSBlob);

    if (FAILED(hr))
    {
        MessageBox(nullptr,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

	// Create the vertex shader
	hr = _pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_pVertexShader);

	if (FAILED(hr))
	{	
		pVSBlob->Release();
        return hr;
	}

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
    hr = CompileShaderFromFile(L"DX11 Framework.fx", "PS", "ps_4_0", &pPSBlob);

    if (FAILED(hr))
    {
        MessageBox(nullptr,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

	// Create the pixel shader
	hr = _pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_pPixelShader);
	pPSBlob->Release();

    if (FAILED(hr))
        return hr;

    // Compile the next vertex shader
    hr = CompileShaderFromFile(L"MeshManipulate.fx", "VS", "vs_4_0", &pVSBlob);

    if (FAILED(hr))
    {
        MessageBox(nullptr,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

    // Create the Next vertex shader
    hr = _pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_VertexShader);

    if (FAILED(hr))
    {
        pVSBlob->Release();
        return hr;
    }

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

    // Create the input layout
    UINT numElements = ARRAYSIZE(layout);
    hr = _pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &_pVertexLayout);


    //Adds a Texture
    Loader L;
    hr = CreateDDSTextureFromFile(_pd3dDevice, L"Crate_COLOR.dds", nullptr, &_pTextureRV);
    if (FAILED(hr))
        return hr;

    //Create Objects
    m_Plane.Create(_pd3dDevice);
    m_Player.Create(_pd3dDevice);
    m_Billboard1.Create(_pd3dDevice);
    m_Billboard2.Create(_pd3dDevice);
    m_Billboard3.Create(_pd3dDevice);
    m_Billboard4.Create(_pd3dDevice);
    m_Ground.Create(_pd3dDevice, m_VertexShader);
    m_SkyBox.Create(_pd3dDevice);

    //Set Pixel Shader
    m_Plane.SetPixelShader(_pPixelShader);
    m_Player.SetPixelShader(_pPixelShader);
    m_Billboard1.SetPixelShader(_pPixelShader);
    m_Billboard2.SetPixelShader(_pPixelShader);
    m_Billboard3.SetPixelShader(_pPixelShader);
    m_Billboard4.SetPixelShader(_pPixelShader);
    m_Ground.SetPixelShader(_pPixelShader);
    m_SkyBox.SetPixelShader(_pPixelShader);

    //Set Vertex Shader
    m_Plane.SetVertexShader(_pVertexShader);
    m_Player.SetVertexShader(_pVertexShader);
    m_Billboard1.SetVertexShader(_pVertexShader);
    m_Billboard2.SetVertexShader(_pVertexShader);
    m_Billboard3.SetVertexShader(_pVertexShader);
    m_Billboard4.SetVertexShader(_pVertexShader);
    m_Ground.SetVertexShader(m_VertexShader);
    m_SkyBox.SetVertexShader(_pVertexShader);

    //Create Window //SetUp blend settings
    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(blendDesc));

    D3D11_RENDER_TARGET_BLEND_DESC rtbd;
    ZeroMemory(&rtbd, sizeof(rtbd));

    rtbd.BlendEnable = true;
    rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
    rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR;
    rtbd.BlendOp = D3D11_BLEND_OP_ADD;
    rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
    rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
    rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.RenderTarget[0] = rtbd;

    _pd3dDevice->CreateBlendState(&blendDesc, &Transparency);

    BlendFactor BF;
    BF.R = 0.99;
    BF.G = 0.99;
    BF.B = 0.99;
    BF.A = 1.;
    m_Window.Create(_pd3dDevice, Transparency, &BF);
    m_Window.SetPixelShader(_pPixelShader);
    m_Window.SetVertexShader(_pVertexShader);



    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    _pd3dDevice->CreateSamplerState(&sampDesc, &_pSamplerLinear);
    _pImmediateContext->PSSetSamplers(0, 1, &_pSamplerLinear);

     
	pVSBlob->Release();

	if (FAILED(hr))
        return hr;

    // Set the input layout
    _pImmediateContext->IASetInputLayout(_pVertexLayout);

	return hr;
}

HRESULT Application::InitVertexAndIndexBuffer()
{
	HRESULT hr;
    // Create vertex buffer
    SimpleVertex vertices[] =
    {
        { XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT2(0,0) }, //Postion then colour
        { XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT2(1,0)},
        { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3( 0.f, 0.f, 0.f), XMFLOAT2(0,1) },
        { XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT2(1,1) },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.f,0.f,0.f), XMFLOAT2(0,0) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.f,0.f,0.f), XMFLOAT2(1,0) },
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.f,0.f,0.f), XMFLOAT2(0,1) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.f,0.f,0.f) , XMFLOAT2(1,1)},
    };
     

    D3D11_BUFFER_DESC bd;

    //Create Index Buffer
    WORD indices[] = // Chnage this to add vertexes 
    {
        0,1,2,    // side 1
        2,1,3,
        4,0,6,    // side 2
        6,0,2,
        7,5,6,    // side 3
        6,5,4,
        3,1,7,    // side 4
        7,1,5,
        4,5,0,    // side 5
        0,5,1,
        3,7,2,    // side 6
        2,7,6,
    };



    for (int i = 0; i < *(&vertices + 1) - vertices; i++)
    {
        vertices[i].Normal =  VertexNormalAveraging(vertices, i, indices);
    };

    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * 54;    
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = indices;
    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pIndexBuffer);

    if (FAILED(hr))
        return hr;

    //Create Vertex Buffer

	ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 8; // CHnage this to add vertexes 
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;

    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pVertexBuffer);

    if (FAILED(hr))
        return hr;

	return S_OK;
}

HRESULT Application::InitPyrmidBuffer()
{
    HRESULT hr;

    // Create vertex buffer
    SimpleVertex vertices[] =
    {
        { XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0,0) }, //Postion then colour
        { XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), XMFLOAT2(0,1) },
        { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), XMFLOAT2(1,0) },
        { XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3( 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(0.0f, 0.0f, 2.0f), XMFLOAT3( 1.f, 1.f, 1.f), XMFLOAT2(1,1)},

    };

    // Create index buffer
    WORD indices[] = // Chnage this to add vertexes 
    {
        0,1,2,    // side 1
        2,1,3,
        0,4,1,    // side 2
        1,4,3,    // side 3
        3,4,2,    // side 4
        2,4,0,    // side 5
    };

    for (int i = 0; i < *(&vertices + 1) - vertices; i++)
    {
        vertices[i].Normal = VertexNormalAveraging(vertices, i, indices);
    };


    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 5; // CHnage this to add vertexes 
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;

    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pyramidVertexBuffer);

    if (FAILED(hr))
        return hr;


    ZeroMemory(&bd, sizeof(bd));
    ZeroMemory(&InitData, sizeof(InitData));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * 18;    // CHnage this to add vertexes 
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = indices;
    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pyramidIndexBuffer);

    if (FAILED(hr))
        return hr;
    return S_OK;
}


HRESULT Application::InitFloorBuffer(int Length, int Width)
{
    HRESULT hr;
    std::vector<SimpleVertex> Vertices;

    int Corners[4] = { 0, Width * 1, Length * Width - Width + 1,  Length* Width};

    for (int i = 0; i < Length; i++)
    {
        for (int j = 0; j < Width; j++)
        {   
            XMFLOAT2 TexCord{};
            if(i == Corners[0])
            {
                TexCord = { 0,0 };
            }
            else if (i == Corners[1])
            {
                TexCord = { 1,0 };
            }
            else if (i == Corners[2])
            {
                TexCord = { 0,1 };
            }
            else if (i == Corners[3])
            {
                TexCord = { 1,1 };
            }

            Vertices.push_back({ XMFLOAT3(j, -4, i), XMFLOAT3(1.0f, 1.0f, 1.0f), TexCord });
        }
    }

    int amountOfIndicies = Length * Width;

    // Create index buffer
    std::vector<WORD> Indices;

    for (int i = 0; i < (Length * Width) - 2 ; i++)
    {
        Indices.push_back(i);
        Indices.push_back(i + 1);
        Indices.push_back(i + 2);
    }



    SimpleVertex* vertices = &Vertices[0];
    WORD* indices = &Indices[0];

    for (int i = 0; i < Vertices.size(); i++)
    {
        Vertices[i].Normal = VertexNormalAveragingWithVectors(Vertices, i, Indices);
    };

    // Create vertex buffer

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * Vertices.size(); // CHnage this to add vertexes 
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;

    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_floorVertexBuffer);

    if (FAILED(hr))
        return hr;

    ZeroMemory(&bd, sizeof(bd));
    ZeroMemory(&InitData, sizeof(InitData));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * Indices.size();    // Chanage this to add vertexes 
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = indices;
    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_floorIndexBuffer);

    if (FAILED(hr))
        return hr;


    return S_OK;
}

HRESULT Application::InitFloorIndexBuffer(int Length, int Width)
{
    HRESULT hr;

    int amountOfIndicies = Length * Width;

    // Create index buffer
    std::vector<WORD> Indices;

    for (int i = 0; i < (Length*Width) * 3; i++)
    {
        Indices.push_back(i);
        Indices.push_back(i + 1);
        Indices.push_back(i + 2);
    }

    WORD* indices = &Indices[0];

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * Indices.size();    // Chanage this to add vertexes 
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = indices;
    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_floorIndexBuffer);

    if (FAILED(hr))
        return hr;

    return S_OK;
}

XMFLOAT3 Application::VertexNormalAveragingWithVectors(std::vector<SimpleVertex> VertexList, int VertexPosition, std::vector<WORD> IndicesList)
{
    int trianglePointPos = 1;
    std::vector<int> TruePositions;
    //Figure out what the Vertex is attached to
    for (int j = 0; j < IndicesList.size(); j++)
    {
        if (VertexPosition == IndicesList[j])
        {
            switch (trianglePointPos)
            {
            case 1:
                TruePositions.push_back(1);
                break;
            case 2:
                TruePositions.push_back(2);
                break;
            case 3:
                TruePositions.push_back(3);
                break;
            }

        }
        if (trianglePointPos == 3)
        {
            trianglePointPos = 0;
        }
        trianglePointPos++;
    }

    //calucalate normal average
    NormalsCalculations NewNormals;
    std::vector<XMFLOAT3> CrossProducts;


    for (int i = 0; i < TruePositions.size(); i++)
    {
        if (i > TruePositions.size())
        {
            return { 0,0,0 };
        }
        else
        {
            switch (TruePositions[i])
            {
            case 1:
                NewNormals.Original = VertexList[VertexPosition].Pos;
                NewNormals.NextPoint = VertexList[VertexPosition + 1].Pos;
                NewNormals.NextPoint2 = VertexList[VertexPosition + 2].Pos;
                CrossProducts.push_back(NewNormals.CrossProductCalculation());
                break;
            case 2:
                NewNormals.Original = VertexList[VertexPosition].Pos;
                NewNormals.NextPoint = VertexList[VertexPosition - 1].Pos;
                NewNormals.NextPoint2 = VertexList[VertexPosition + 1].Pos;
                CrossProducts.push_back(NewNormals.CrossProductCalculation());
                break;
            case 3:
                NewNormals.Original = VertexList[VertexPosition].Pos;
                NewNormals.NextPoint = VertexList[VertexPosition - 2].Pos;
                NewNormals.NextPoint2 = VertexList[VertexPosition - 1].Pos;
                CrossProducts.push_back(NewNormals.CrossProductCalculation());
                break;
            }

            return NewNormals.CalculateAverageWithVector(CrossProducts);

        }
    }

}

XMFLOAT3 Application::VertexNormalAveraging(SimpleVertex VertexList[], int VertexPosition, WORD IndicesList[])
{

    int trianglePointPos = 1;
    std::vector<int> TruePositions;
    //Figure out what the Vertex is attached to
    for (int j = 0; j < *(&IndicesList + 1) - IndicesList; j++)
    {
        if (VertexPosition == IndicesList[j])
        {
            switch (trianglePointPos)
            {
            case 1:
                TruePositions.push_back(1);
                break;                
            case 2:
                TruePositions.push_back(2);
                break;                
            case 3:
                TruePositions.push_back(3);
                break;
            }

        }
        if (trianglePointPos == 3)
        {
            trianglePointPos = 0;
        }
        trianglePointPos++;
    }

    //calucalate normal average
    NormalsCalculations NewNormals;
    std::vector<XMFLOAT3> CrossProducts;


    for (int i = 0; i < TruePositions.size(); i++)
    {
        if (i > TruePositions.size())
        {
            return { 0,0,0 };
        }
        else
        { 
            switch (TruePositions[i])
            {
            case 1:
                NewNormals.Original = VertexList[VertexPosition].Pos;
                NewNormals.NextPoint = VertexList[VertexPosition + 1].Pos;
                NewNormals.NextPoint2 = VertexList[VertexPosition + 2].Pos;
                CrossProducts.push_back(NewNormals.CrossProductCalculation());
                break;
            case 2:
                NewNormals.Original = VertexList[VertexPosition].Pos;
                NewNormals.NextPoint = VertexList[VertexPosition - 1].Pos;
                NewNormals.NextPoint2 = VertexList[VertexPosition + 1].Pos;
                CrossProducts.push_back(NewNormals.CrossProductCalculation());
                break;
            case 3:
                NewNormals.Original = VertexList[VertexPosition].Pos;
                NewNormals.NextPoint = VertexList[VertexPosition - 2].Pos;
                NewNormals.NextPoint2 = VertexList[VertexPosition - 1].Pos;
                CrossProducts.push_back(NewNormals.CrossProductCalculation());
                break;
            }

            return NewNormals.CalculateAverageWithVector(CrossProducts);

        }
    }


}

HRESULT Application::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW );
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
    if (!RegisterClassEx(&wcex))
        return E_FAIL;

    // Create window
    _hInst = hInstance;
    RECT rc = {0, 0, 640, 480};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    _hWnd = CreateWindow(L"TutorialWindowClass", L"DX11 Framework", WS_OVERLAPPEDWINDOW,
                         CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
                         nullptr);
    if (!_hWnd)
		return E_FAIL;

    ShowWindow(_hWnd, nCmdShow);

    return S_OK;
}

HRESULT Application::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

    if (FAILED(hr))
    {
        if (pErrorBlob != nullptr)
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

        if (pErrorBlob) pErrorBlob->Release();

        return hr;
    }

    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}

HRESULT Application::InitDevice()
{
    HRESULT hr = S_OK;

    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };

    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = _WindowWidth;
    sd.BufferDesc.Height = _WindowHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = _hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        _driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
                                           D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pd3dDevice, &_featureLevel, &_pImmediateContext);
        if (SUCCEEDED(hr))
            break;
    }

    if (FAILED(hr))
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    if (FAILED(hr))
        return hr;

    hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
    pBackBuffer->Release();

    if (FAILED(hr))
        return hr;
    
    D3D11_TEXTURE2D_DESC depthStencilDesc;

    depthStencilDesc.Width = _WindowWidth;
    depthStencilDesc.Height = _WindowHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    _pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencilBuffer);
    _pd3dDevice->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthStencilView);
    
    D3D11_RASTERIZER_DESC wfdesc;
    ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
    wfdesc.FillMode = D3D11_FILL_WIREFRAME;
    wfdesc.CullMode = D3D11_CULL_NONE;
    hr = _pd3dDevice->CreateRasterizerState(&wfdesc, &_wireFrame);

    ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
    wfdesc.FillMode = D3D11_FILL_SOLID;
    wfdesc.CullMode = D3D11_CULL_NONE;
    hr = _pd3dDevice->CreateRasterizerState(&wfdesc, &_solidFrame);

    _pImmediateContext->RSSetState(_solidFrame);
    _WireFrameOn = false;
    _pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _depthStencilView);
    


    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)_WindowWidth;
    vp.Height = (FLOAT)_WindowHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    _pImmediateContext->RSSetViewports(1, &vp);

	InitShadersAndInputLayout();



    InitPyrmidBuffer();
    InitVertexAndIndexBuffer();
    InitFloorBuffer(4,4);


    // Set primitive topology
    _pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
    hr = _pd3dDevice->CreateBuffer(&bd, nullptr, &_pConstantBuffer);


    if (FAILED(hr))
        return hr;

    return S_OK;
}


void Application::Cleanup()
{
    if (_pImmediateContext) _pImmediateContext->ClearState();
    if (_pConstantBuffer) _pConstantBuffer->Release();
    if (_pVertexBuffer) _pVertexBuffer->Release();
    if (_pIndexBuffer) _pIndexBuffer->Release();
    if (_pyramidVertexBuffer) _pyramidVertexBuffer->Release();
    if (_pyramidIndexBuffer) _pyramidIndexBuffer->Release();
    if (_floorVertexBuffer) _floorVertexBuffer->Release();
    if (_floorIndexBuffer) _floorIndexBuffer->Release();
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
    if (Transparency) Transparency->Release();
    delete m_Camera;
    delete m_Camera2;
    delete m_Camera3;
    delete m_Camera4;
    delete m_System;
    delete m_KeyboardInput;
    delete m_MouseInput;
}

void Application::KeyboardCheck(MSG* message)
{
    m_KeyboardInput->UpdateMSG(message);
    m_MouseInput->UpdateMousePos(message);
    m_KeyboardInput->TranslateMsg(message);
}

void Application::MouseCheck(MSG* message)
{

}

void Application::Update()
{


    // Update our time
    static float t = 0.0f;

    if (_driverType == D3D_DRIVER_TYPE_REFERENCE)
    {
        t += (float)XM_PI * 0.0125f;
    }
    else
    {
        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount();

        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;

        t = (dwTimeCur - dwTimeStart) / 1000.0f;
    }




    m_Camera3->ManageMouseMovement(m_MouseInput->ReturnMouseData());

    if (m_KeyboardInput->DoesKeyMatch('P', t))
    {
        SetWireFrame();
    }

    if (m_KeyboardInput->DoesKeyMatch('R', t))
    {
        XMFLOAT3 Playerpos = m_Player.PlayerPos();
        XMVECTOR cam3 = { Playerpos.x, Playerpos.y, Playerpos.z += 1, 1.f };
        XMVECTOR cam4 = { Playerpos.x, Playerpos.y += 3, Playerpos.z += 1, 1.f };
        m_Camera3->SetLookTo(cam3);
        m_Camera4->SetLookTo(cam4);
    }

    if (m_KeyboardInput->DoesKeyMatch('H', t))
    {
        switch (CurrentCam)
        {
        case 3:
        {
            m_Player.Transform(0, 0, 0.5);
            break;
        }
        }

    }
    if (m_KeyboardInput->DoesKeyMatch('G', t))
    {
        switch (CurrentCam)
        {
        case 3:
        {
            m_Player.Transform(0, 0, -0.5);
            break;
        }
        }

    }
    if (m_KeyboardInput->DoesKeyMatch('J', t))
    {
        switch (m_Player.ReturnSpeed())
        {
        case 2:
        {
            m_Player.SetSpeed(4);
            break;
        }
        case 4:
        {
            m_Player.SetSpeed(2);
            break;
        }
        }
    }

    if (m_KeyboardInput->DoesKeyMatch('W', t))
    {
        switch (CurrentCam)
        {
            case 2:
            {
                m_Camera3->Walk(1.f);
                m_Camera4->Walk(1.f);
                m_Player.Transform(0.5, 0, 0);
                break;
            }
            case 3:
            {
                m_Camera3->Walk(1.f);
                m_Camera4->Walk(1.f);
                m_Player.Transform(0.5, 0, 0);
                break;
            }
        }
    }
    if (m_KeyboardInput->DoesKeyMatch('S', t))
    {
        switch (CurrentCam)
        {
            case 2:
            {
                m_Camera3->Walk(-1.f);
                m_Camera4->Walk(-1.f);
                m_Player.Transform(-0.5, 0, 0);
                break;
            }
            case 3:
            {
                m_Camera3->Walk(-1.f);
                m_Camera4->Walk(-1.f);
                m_Player.Transform(-0.5, 0, 0);
                break;
            }
        }
    }
    if (m_KeyboardInput->DoesKeyMatch('A', t))
    {
        switch (CurrentCam)
        {
            case 2:
            {
                m_Camera3->Strafe(-1.f);
                m_Camera4->Strafe(-1.f);
                m_Player.Transform(0, -0.5, 0);
                break;
            }
            case 3:
            {
                m_Camera3->Strafe(-1.f);
                m_Camera4->Strafe(-1.f);
                m_Player.Transform(0, -0.5, 0);
                break;
            }
        }
    }
    if (m_KeyboardInput->DoesKeyMatch('D', t))
    {

        switch (CurrentCam)
        {
            case 2:
            {
                m_Camera3->Strafe(1.f);
                m_Camera4->Strafe(1.f);
                m_Player.Transform(0, 0.5, 0);
                break;
            }
            case 3:
            {
                m_Camera3->Strafe(1.f);
                m_Camera4->Strafe(1.f);
                m_Player.Transform(0, 0.5, 0);
                break;
            }
        }
    }


    if (m_KeyboardInput->DoesKeyMatch('O', t))
    {
        switch (m_HideBill)
        {
        case true:
            m_HideBill = false;
            break;
        case false:
            m_HideBill = true;
            break;
        }
    }
    if (m_KeyboardInput->DoesKeyMatch('I', t))
    {
        switch (m_IsNight)
        {
        case true:
            m_IsNight = false;
            break;
        case false:
            m_IsNight = true;
            break;
        }
    }

    if (m_KeyboardInput->DoesKeyMatch('C', t))
    {
        switch (CurrentCam)
        {
            case 0:
            {
                CurrentCam = 1;
                break;
            }
            case 1:
            {
                CurrentCam = 2;
                break;
            }
            case 2:
            {
                CurrentCam = 3;
                break;
            }
            case 3:
            {
                CurrentCam = 0;
                break;
            }
        }
    }

    if (m_KeyboardInput->DoesKeyMatch('Y',t))
    {
        switch (m_Hidetranslucent)
        {
        case true:
            m_Hidetranslucent = false;
            break;
        case false:
            m_Hidetranslucent = true;
            break;
        }
    }

    m_KeyboardInput->ResetCurrentKey();
    Time = t;



    //
    // Animate the cube
    //


    if (!HideOriginal)
    {
        //cube 1
        XMStoreFloat4x4(&_world, XMMatrixTranslation(8, 0, 0) * XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationZ(t));
        //cube 2 Centre
        XMStoreFloat4x4(&_world2, XMMatrixTranslation(0.0f, 0.0f, 0.0f) * XMMatrixScaling(0.4f, 0.4f, 0.4f) * XMMatrixRotationZ(t * 2)); XMStoreFloat4x4(&_world2, XMMatrixTranslation(0.0f, 0.0f, 0.0f) * XMMatrixScaling(0.4f, 0.4f, 0.4f) * XMMatrixRotationZ(t * 2));
        //cube 3
        XMStoreFloat4x4(&_world3, XMMatrixTranslation(15, 0, 0) * XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationZ(t * 0.67));
        //cube 4
        XMStoreFloat4x4(&_world4, XMMatrixMultiply(XMLoadFloat4x4(&_world), (XMMatrixTranslation(15, 0, 0) * XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationZ(t * 0.67))));
        //cube 5
        XMStoreFloat4x4(&_world5, XMMatrixMultiply(XMLoadFloat4x4(&_world3), (XMMatrixTranslation(8, 0, 0) * XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationZ(t))));

        XMStoreFloat4x4(&_world6, XMMatrixTranslation(0.0f, 0.0f, 0.0f) * XMMatrixScaling(1.f, 1.0f, 1.f));
    }
   
    

    m_Camera3->Update();
    m_Camera4->Update();

    
    switch (CurrentCam)
    {
        case 0:
        {
            m_Billboard1.UpdateDirection(m_Camera->ReturnCamData());
            m_Billboard2.UpdateDirection(m_Camera->ReturnCamData());
            m_Billboard3.UpdateDirection(m_Camera->ReturnCamData());
            m_Billboard4.UpdateDirection(m_Camera->ReturnCamData());
            break;
        }
        case 1:
        {
            m_Billboard1.UpdateDirection(m_Camera2->ReturnCamData());
            m_Billboard2.UpdateDirection(m_Camera2->ReturnCamData());
            m_Billboard3.UpdateDirection(m_Camera2->ReturnCamData());
            m_Billboard4.UpdateDirection(m_Camera2->ReturnCamData());
            break;
        }
        case 2:
        {

            m_Billboard1.UpdateDirection(m_Camera3->ReturnCamData());
            m_Billboard2.UpdateDirection(m_Camera3->ReturnCamData());
            m_Billboard3.UpdateDirection(m_Camera3->ReturnCamData());
            m_Billboard4.UpdateDirection(m_Camera3->ReturnCamData());
            break;
        }
        case 3:
        {
            m_Billboard1.UpdateDirection(m_Camera4->ReturnCamData());
            m_Billboard2.UpdateDirection(m_Camera4->ReturnCamData());
            m_Billboard3.UpdateDirection(m_Camera4->ReturnCamData());
            m_Billboard4.UpdateDirection(m_Camera4->ReturnCamData());
            break;
        }
    }
    
    Flotilla newFlotilla;
    newFlotilla.AddObject(&m_Plane);

    newFlotilla.AddObject(&m_Ground);
    newFlotilla.AddObject(&m_Player);
    if (!m_HideBill)
    {
        newFlotilla.AddObject(&m_Billboard1);
        newFlotilla.AddObject(&m_Billboard2);
        newFlotilla.AddObject(&m_Billboard3);
        newFlotilla.AddObject(&m_Billboard4);
    }
    if (!m_Hidetranslucent)
    {
        newFlotilla.AddObject(&m_Window);
    }
    newFlotilla.AddObject(&m_SkyBox);
    m_System->AddFlotillaToFlotillaList(newFlotilla);

    XMStoreFloat4x4(&_world7, XMMatrixTranslation(t, t, t * 2) * XMMatrixScaling(1.f, 1.0f, 1.f));
}

void Application::SetWireFrame()
{

    switch (_WireFrameOn)
    {
    case true:
        _pImmediateContext->RSSetState(_solidFrame);
        _WireFrameOn = false;
        break;
    case false:
        _pImmediateContext->RSSetState(_wireFrame);
        _WireFrameOn = true;
    }

}


void Application::Draw()
{
    //
    // Clear the back buffer
    //
    float ClearColor[4] = {0.0f, 0.125f, 0.3f, 1.0f}; // red,green,blue,alpha
    _pImmediateContext->ClearRenderTargetView(_pRenderTargetView, ClearColor);
    _pImmediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


    //Flotilla 0 is the camera flotilla
    CameraBufferData CBD;

    
    if (Time < 10)
    {
        CBD = m_System->ReturnCameraBufferData(0, CurrentCam);
    }
    else if (Time < 15)
    {
        CBD = m_System->ReturnCameraBufferData(0, CurrentCam);
    }
    else
    {
        CBD = m_System->ReturnCameraBufferData(0, CurrentCam);
    }
    

    XMMATRIX world = XMLoadFloat4x4(&_world);
    XMMATRIX view = XMLoadFloat4x4(&CBD.m_view);
    XMMATRIX projection = XMLoadFloat4x4(&CBD.m_projection);


   



    //
    // Update variables
    //

    //CUBE 1

    ConstantBuffer cb;
   
    // "fine-tune" the blending equation
    float blendFactor[] = { 0.75f, 0.75f, 0.75f, 1.0f };

    cb.gTime = Time;

    if (!m_IsNight)
    {
        cb.DiffuseLight = XMFLOAT4(0.1f, 0.1f, 1.0f, 1.0f);
        cb.DiffuseMtrl = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
        cb.LightVecW = XMFLOAT3(0.25f, 0.5f, -1.0f);
        cb.AmbientMtrl = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.f);
        cb.AmbientLight = XMFLOAT4(0.2f, 0.2f, 0.2f, 0.2f);
        cb.SpecularLight = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.f);
        cb.SpecularPower = 2.f;
        cb.SpecularMtrl = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.f);
    }
    else
    {
        cb.DiffuseLight = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.1f);
        cb.DiffuseMtrl = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.1f);
        cb.LightVecW = XMFLOAT3(0.25f, 0.5f, -1.0f);
        cb.AmbientMtrl = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.1f);
        cb.AmbientLight = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.1f);
        cb.SpecularLight = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.1f);
        cb.SpecularPower = 0.1f;
        cb.SpecularMtrl = XMFLOAT4(0.2f, 0.2f, 0.2f, 0.1f);
    }

    _pImmediateContext->PSSetShader(_pPixelShader, nullptr, 0);
    _pImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);
    _pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
    _pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
    _pImmediateContext->PSSetShaderResources(0, 1, &_pTextureRV);
    
	cb.mWorld = XMMatrixTranspose(world);
	cb.mView = XMMatrixTranspose(view);
	cb.mProjection = XMMatrixTranspose(projection);
	_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

    // Set vertex buffer
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;

    //Original Code
    if (!HideOriginal)
    {
        //CUBE1
        _pImmediateContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
        _pImmediateContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
        _pImmediateContext->DrawIndexed(54, 0, 0);


        //CUBE 2

        cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&_world2));
        _pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
        _pImmediateContext->IASetVertexBuffers(0, 1, &_pyramidVertexBuffer, &stride, &offset);
        _pImmediateContext->IASetIndexBuffer(_pyramidIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
        _pImmediateContext->DrawIndexed(18, 0, 0);



        //CUBE 3

        cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&_world3));
        _pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
        _pImmediateContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
        _pImmediateContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
        _pImmediateContext->DrawIndexed(54, 0, 0);



        //CUBE 4

        cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&_world4));
        _pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
        _pImmediateContext->IASetVertexBuffers(0, 1, &_pyramidVertexBuffer, &stride, &offset);
        _pImmediateContext->IASetIndexBuffer(_pyramidIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
        _pImmediateContext->DrawIndexed(18, 0, 0);




        //CUBE 5

        cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&_world5));
        _pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
        _pImmediateContext->IASetVertexBuffers(0, 1, &_pyramidVertexBuffer, &stride, &offset);
        _pImmediateContext->IASetIndexBuffer(_pyramidIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
        _pImmediateContext->DrawIndexed(18, 0, 0);

        //Pyrmid 1

        cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&_world6));
        _pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
        _pImmediateContext->IASetVertexBuffers(0, 1, &_floorVertexBuffer, &stride, &offset);
        _pImmediateContext->IASetIndexBuffer(_floorIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
        _pImmediateContext->DrawIndexed(42, 0, 0);
    }

    m_System->Draw(_pImmediateContext, _pConstantBuffer, cb);

    //
    // Present our back buffer to our front buffer
    //
    _pSwapChain->Present(0, 0);
    m_System->ClearFlotillaList(1, 0);
}
