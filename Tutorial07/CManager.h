#pragma once
#include "CDevice.h"
#include "CCamera.h"
#include "CDeviceContext.h"
#include "CSwapChain.h"
#include "CMesh.h"
#include "CBUFER.h"
#include "CTexture.h"
#include "CRenderTarget.h"
#include "CVertexShader.h"
#include "CPixelShader.h"
#include "CLayout.h"
class CManager
{
public:
	CManager();
	~CManager();
#ifndef DX
#else
#endif // !DX
	CDevice m_device;
	std::vector<CCamera>	m_camera;
	CCamera					m_camera1;
	CCamera					m_camera2;
	CDeviceContext			m_deviceContext;
	CSwapChain				m_swapChain;
	CBUFER					m_buffers;
	CTexture				m_texture;
	CRenderTarget			m_renderTargetV;
	CRenderTarget			m_renderTargetV2;
	CVertexShader			m_vs;
	CPixelShader			m_ps;
	CLayout					m_layout;


	//	CMesh				m_mesh;
	//	CMesh				m_hand;
	std::vector<CMesh>	m_meshes;
	//std::vector<CMesh>	m_meshes2;
	void drawnm_meshes();

	int m_cameraNum = 0;
	void cleanup();

	bool dvice_init = false;


#ifdef DX

	struct SimpleVertex
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
		XMFLOAT3 Norm;
	};

	ID3D11Texture2D* renderTargetTextureMap;
	ID3D11ShaderResourceView* shaderResourceViewMap;
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	void textureChange(ID3D11ShaderResourceView* &pTextureRV);
	void generateRenderTexture();
	_XMFLOAT4 color;


	XMFLOAT4 m_lightDir = { 1,0,0,0 };
	XMFLOAT4 SpecularColor = { 255,255,255,0 };
	XMFLOAT4 DifuseColor = { 0,255,0,0 };
	XMFLOAT4 SPpower = { 1,1,1,1 };
	XMFLOAT4 ambientColor = { 255,0,0,0 };
	XMFLOAT4 KDAS = { 1,0.001,1,1 };
#elif OPENGL
	vec4 color = vec4(1, 1, 1, 1);
	vec4 ligthDir = vec4(1, 0, -1, 0);
	vec4 SpecularColor = { 1,1,1,1 };
	vec4 ambientColor = { 1,0,0,0 };
	vec4 KDASL = { 1,0.001,1,10 };
	//vec4 DifuseColor = { 20,20,20,20 };
	vec4 DifuseColor = { 0,1,0,0 };
	//vec4 DifuseColor = { 0,0,0,0};
	//vec4 SpecularColor = { .4,.4,.4,.4 };
	vec4 SPpower = vec4(10);

	bool bRotationMesh = true;
	myRECT WindowSize;
	myRECT WindowSizeAnte;
	POINT windowPosition;
	GLenum DrawBuffers[1];
	GLuint m_LinearSampler;//linear sample ID
	GLuint programID;

	GLuint depthrenderbuffer;
	GLuint depthrenderbuffer2;
	GLuint renderedTexture;
	GLuint renderedTexture2;
	GLuint quad_VertexArrayID;
	GLuint quad_vertexbuffer;
	void initDevice();
	void initWindow(int argc, char* argv[]);
	//GLuint programID;
	GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
#endif // DX


	bool DInitDevice(HRESULT &hr, const HWND& hWnd);
	void DswapChain(const HWND& hWnd);
	bool DcreateRenderTargetV(HRESULT &hr);
	void DcreateDepthStencilTex(HRESULT &hr);
	void DcreateDepthStencilV(HRESULT &hr);
	void DsetupViewport();
	void DcreateVertexBuffer(int numvertex);
	void DcreateIndexBuffer(int numindice);
	void DcreateConstantBuffer();
	void DcreateSampleState();
	void DResize(float width, float height);
	//TEXTURE//////////////////////////////
	int m_numTexture = 3;
	void readTextureMesh(int i);
	void readNormTextureMesh(int i);
	//MESH/////////////////////////////////
	void changeToCube();
	void changeToTriangle();
	void changeToPantalla();
	void chargeMesh();
	//CAMERA//////////////////////////////
	//void initCamera(ID3D11DeviceContext* &pImmediateContext, ID3D11Buffer* &pCBNeverChanges, ID3D11Buffer* pCBChangeOnResize);
	void initCamera();
	void initCamera2();
	void resizeCamera(float width, float height);
	//void moveCamera(ID3D11DeviceContext* &pImmediateContext, ID3D11Buffer* &pCBNeverChanges, ID3D11Buffer* pCBChangeOnResize);
	void moveCamera(WPARAM wParam);
	void rotateCamera();
	void resetView();
	void changeCamera();
	void ReadPantalla();

#ifdef DX
	void changeSpeed(WPARAM wParam);
	//bool bEscalar = true;
#elif OPENGL
	void changeSpeed(unsigned char key);
#endif // DX

	void changeSpecularPower(int x);

	int m_Width;
	int m_Height;

	float spMaxi = 15;
	float spMin = 1;

	bool bMousePressed = false;
	bool bFiestaMode = false;
	float timeFiesta = 0;
	void fiesta();
};

