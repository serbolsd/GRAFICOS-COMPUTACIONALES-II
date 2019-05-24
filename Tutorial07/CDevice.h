#pragma once

#include"Header.h"
class CDevice
{
public:
	CDevice();
	~CDevice();

#ifdef DX
	ID3D11Device*                       m_pd3dDevice = NULL;
	ID3D11Texture2D* m_pBackBuffer = NULL;

	UINT createDeviceFlags = 0;
	UINT width;
	UINT height;


	D3D_DRIVER_TYPE driverTypes[3] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	D3D_FEATURE_LEVEL featureLevels[3] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numDriverTypes;
	UINT numFeatureLevels;

	DXGI_SWAP_CHAIN_DESC sd;



	D3D11_TEXTURE2D_DESC descDepth;
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;

	D3D11_VIEWPORT vp;
	ID3DBlob* pVSBlob = NULL;

	D3D11_INPUT_ELEMENT_DESC layout[4] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32_FLOAT, 0,20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32_FLOAT, 0,28, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
	UINT numElements;

	struct SimpleVertex
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
		XMFLOAT3 Norm;
		XMFLOAT3 tan;
	};

	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	struct CBNeverChanges
	{
		XMMATRIX mView;
	};

	struct CBChangeOnResize
	{
		XMMATRIX mProjection;
	};

	struct CBChangesEveryFrame
	{
		XMMATRIX mWorld;
		XMFLOAT4 vMeshColor;
		XMFLOAT4 lightDir;
		XMFLOAT4 vViewPosition;
		//float SpecularPower;                // Specular pow factor
		XMFLOAT4 SpecularColor;               // Specular color (sRGBA)
		XMFLOAT4 DifuseColor;                 // Difuse color (sRGBA)
		XMFLOAT4 AmbientalColor;
		XMFLOAT4 SPpower;
		XMFLOAT4 KDASL;
		XMFLOAT4 LPLQ;
		XMFLOAT4 lightPosition;
		XMFLOAT4 SLCOC;
	
	};
	//struct CBChangesEveryFrameMarix
	//{
	//	XMMATRIX mWorld;
	//};
	//struct CBChangesEveryFrameFloat
	//{
	//	XMFLOAT4 vMeshColor;
	//
	//};

	D3D11_SAMPLER_DESC sampDesc;
#endif // DX

	const aiScene * model;
	//const aiScene * model2;
	

};

