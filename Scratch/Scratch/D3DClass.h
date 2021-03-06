#pragma once

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"DXErr.lib")

#include<windows.h>
#include<d3d11.h>
#include<d3dx11.h>
#include<d3dx10.h>
#include<xnamath.h>
#include<DXErr.h>

//#define _XM_NO_INTRINSICS_

class D3DClass
{
public:
	D3DClass();
	~D3DClass();
	/*ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();*/

	bool InitializeD3DApp(HINSTANCE,HWND);
	void ReleaseObjects();
	bool InitScene();
	void UpdateScene();
	void DrawScene();

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView* m_targetView;

	ID3D11RasterizerState* WireFrame;

	ID3D11ShaderResourceView* CubesTexture;
	ID3D11SamplerState* CubesTexSamplerState;

	ID3D11BlendState* Transparency;
	ID3D11RasterizerState* CCWcullMode;
	ID3D11RasterizerState* CWcullMode;

	ID3D11RasterizerState* noCull;

	ID3D11Buffer* squareIndexBuffer;
	//ID3D11Buffer* squareVertBuffer;
	ID3D11Buffer* cbPerObjectBuffer;

	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;

	ID3D11Buffer* triangleVertBuffer;
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D10Blob* VS_Buffer;
	ID3D10Blob* PS_Buffer;
	ID3D11InputLayout* vertLayout;

	XMMATRIX WVP;
	XMMATRIX World;
	XMMATRIX camView;
	XMMATRIX camProjection;

	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp;
	XMMATRIX cube1World;
	XMMATRIX cube2World;

	XMMATRIX Rotation;
	XMMATRIX Scale;
	XMMATRIX Translation;
	

	
	float rot;
	float red, green, blue;
	int colormodr, colormodg, colormodb;
};

struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z,
		float u, float v)
		: pos(x, y, z), texCoord(u, v){}

	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
};

struct cbPerObject
{
	XMMATRIX  WVP;
};

