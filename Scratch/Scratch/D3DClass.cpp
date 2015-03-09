#include "D3DClass.h"
#include "FrameWork.h"

D3DClass::D3DClass()
{
	red = 0;
	green = 0;
	blue = 0;
	colormodr = 1;
	colormodg = 1;
	colormodb = 1;
}


D3DClass::~D3DClass()
{
}

bool D3DClass::InitializeD3DApp(HINSTANCE hInstance,HWND hwnd)
{
	HRESULT hr;

	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = width;
	bufferDesc.Height = height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);

	if (FAILED(hr))
	{
		MessageBox(0, DXGetErrorDescription(hr),TEXT("Create Device and Swap Chain"), MB_OK);
		return false;
	}

	ID3D11Texture2D* backBuffer;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, DXGetErrorDescription(hr), TEXT("Create Back Buffer"), MB_OK);
		return false;
	}

	hr = m_device->CreateRenderTargetView(backBuffer, NULL, &m_targetView);
	if (FAILED(hr))
	{
		MessageBox(0, DXGetErrorDescription(hr), TEXT("Create render target view"), MB_OK);
		return false;
	}

	backBuffer->Release();


	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	m_device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	m_device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

	m_deviceContext->OMSetRenderTargets(1, &m_targetView, depthStencilView);

	return true;

}

void D3DClass::ReleaseObjects()
{
	m_swapChain->Release();
	m_device->Release();
	m_deviceContext->Release();
	m_targetView->Release();
	squareIndexBuffer->Release();
	triangleVertBuffer->Release();
	VS->Release();
	PS->Release();
	VS_Buffer->Release();
	PS_Buffer->Release();
	vertLayout->Release();
	depthStencilView->Release();
	depthStencilBuffer->Release();
}

bool D3DClass::InitScene()
{
	HRESULT hr;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	UINT numElements = ARRAYSIZE(layout);
	//Compile Shaders from shader file
	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "VS", "vs_4_0", 0, 0, 0, &VS_Buffer, 0, 0);
	/*if (FAILED(hr))
	{
		MessageBox(0, DXGetErrorDescription(hr), TEXT("WTF1"), MB_OK);
		return false;
	}*/

	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS_Buffer, 0, 0);

	/*if (FAILED(hr))
	{
		MessageBox(0, DXGetErrorDescription(hr), TEXT("WTF2"), MB_OK);
		return false;
	}*/
	//Create the Shader Objects
	hr = m_device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	hr = m_device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);

	//Set Vertex and Pixel Shaders
	m_deviceContext->VSSetShader(VS, 0, 0);
	m_deviceContext->PSSetShader(PS, 0, 0);

	//Create the vertex buffer
	Vertex v[] =
	{
		Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f),
		Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
	};

	DWORD indices[] = { 0, 1, 2,
		0, 2, 3 };

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 2 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = indices;

	m_device->CreateBuffer(&indexBufferDesc,&indexBufferData,&squareIndexBuffer);
	m_deviceContext->IASetIndexBuffer(squareIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	hr = m_device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &triangleVertBuffer);

	//Set the vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &triangleVertBuffer, &stride, &offset);

	//Create the Input Layout
	m_device->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(),
		VS_Buffer->GetBufferSize(), &vertLayout);

	//Set the Input Layout
	m_deviceContext->IASetInputLayout(vertLayout);

	//Set Primitive Topology
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//Set the Viewport
	m_deviceContext->RSSetViewports(1, &viewport);
	return true;
}

void D3DClass::UpdateScene()
{
	red += colormodr*0.00005f;
	green += colormodg*0.00002f;
	blue += colormodb*0.00001f;

	if (red >= 1.0f || red <= 0.0f)
		colormodr *= -1;
	if (green >= 1.0f || green <= 0.0f)
		colormodg *= -1;
	if (blue >= 1.0f || blue <= 0.0f)
		colormodb *= -1;
}

void D3DClass::DrawScene()
{
	float color[4] = { red, green, blue, 1.0f };
	m_deviceContext->ClearRenderTargetView(m_targetView, color);

	m_deviceContext->ClearDepthStencilView(depthStencilView,D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f,0);
	//Draw the triangle
	//m_deviceContext->Draw(3, 0);
	m_deviceContext->DrawIndexed(6, 0, 0);

	m_swapChain->Present(0, 0);
}
