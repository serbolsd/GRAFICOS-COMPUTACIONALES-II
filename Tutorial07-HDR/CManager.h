#pragma once
#include "CDevice.h"
#include "CCamera.h"
#include "CDeviceContext.h"
#include "CSwapChain.h"
#include "CMesh.h"
#include "CBUFER.h"
#include "CTexture.h"
#include "CRenderTarget.h"
#include "CProgramShader.h"
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
	CRenderTarget			m_lightPass;
	CRenderTarget			m_LuminancePass;
	CRenderTarget			m_BrightPass;
	CRenderTarget			m_BlurHPass;
	CRenderTarget			m_BlurVPass;
	CRenderTarget			m_AddBrightPass;
	CRenderTarget			m_ToneMapPass;
	CProgramShader			m_programSVertexBP;
	CProgramShader			m_programSVertexB;
	CProgramShader			m_programSPixelBP;
	CProgramShader			m_programSPixelB;
	CProgramShader			m_programSActual;
	CProgramShader			m_programSLuminance;
	CProgramShader			m_programSBright;
	CProgramShader			m_programSBlur_H;
	CProgramShader			m_programSBlur_V;
	CProgramShader			m_programSAddBirght;
	CProgramShader			m_programSToneMap;
	CProgramShader			m_programSRenderScreen;
	CLayout					m_layout;


	CMesh				m_SAQ;
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
		XMFLOAT3 tan;
	};

	ID3D11Texture2D* renderTextureLightPass;
	ID3D11ShaderResourceView* SResourceLightPass;
	ID3D11Texture2D* renderTextureLuminancePass;
	ID3D11ShaderResourceView* SResourceLuminancePass;
	ID3D11Texture2D* renderTextureBrightPass;
	ID3D11ShaderResourceView* SResourceBrightPass;

	ID3D11Texture2D* renderTextureBlurHPass;
	ID3D11ShaderResourceView* SResourceBlurHPass;

	ID3D11Texture2D* renderTextureBlurVPass;
	ID3D11ShaderResourceView* SResourceBlurVPass;

	ID3D11Texture2D* renderTextureAddBrightPass;
	ID3D11ShaderResourceView* SResourceAddBrightPass;

	ID3D11Texture2D* renderTextureToneMapPass;
	ID3D11ShaderResourceView* SResourceToneMapPass;
	ID3D11ShaderResourceView* SResourceActual;

	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	void textureChange(ID3D11ShaderResourceView* &pTextureRV);
	void generateRenderTexture();
	void generateRenderTextureWhitReferences(ID3D11RenderTargetView*&pRenderTargetView, ID3D11DepthStencilView* &pDepthStencilView, ID3D11Texture2D* &renderTexture, ID3D11ShaderResourceView* &ShaderResource);
	_XMFLOAT4 color;


	XMFLOAT4 m_lightDir = { 0,1,0,0 };			//Direction of the light direction
	XMFLOAT4 SpecularColorDir = { 1,1,1,1 };  	//specular color for the light direction
	XMFLOAT4 SpecularColorPoint = { 1,1,1,1 };	//specular color for the point light
	XMFLOAT4 SpecularColorSpot = { 1,1,1,1 };	//specular color for the spotlight
	XMFLOAT4 DifuseColorDir = { 0,1,0,0 };			// color of the difuse
	XMFLOAT4 DifuseColorPoint = { 0,1,0,0 };			// color of the difuse
	XMFLOAT4 DifuseColorSpot = { 0,1,0,0 };			// color of the difuse
	XMFLOAT4 SPpower = { 10,10,10,10 };			// Specular power
	XMFLOAT4 ambientColor = { 1,0,0,0 };		// ambiental color
	XMFLOAT4 KDASL = { 1,0.001,1,1 };			// constant of diufse, Ambiental, specular and point light
	XMFLOAT4 LPLQLD = { 4,4 * 4,10,0 };			//LIGHT POINT CONST LINEAR QUADRATIC max dist
	XMFLOAT4 ligthPosition = { 0,2,-5,0 };	//position of the point light
	XMFLOAT4 SLCOC = { -5,0,0,0 };				// spot light cutoof, other cut
	XMFLOAT4 SLDPS = { 1, 0, 0, 0 };			// SWITCH LIGHT DIRECTION, POINT, SPOT
	XMFLOAT4 ADPS = { 1, 1, 1, 1 };				// ATTENUATION LIGHT DIRECTION, POINT, SPOT

	void chargeVertexShadervVertexBLINNPHONG(const char * file_path, ID3D11VertexShader* &pVS);
	void chargePixelShadervVertexBLINNPHONG(const char * file_path, ID3D11PixelShader* &pPS);
	void chargeVertexShadervVertexBLINN(const char * file_path, ID3D11VertexShader* &pVS);
	void chargePixelShadervVertexBLINN(const char * file_path, ID3D11PixelShader* &pPS);
	void chargeVertexShadervPixelBLINNPHONG(const char * file_path, ID3D11VertexShader* &pVS);
	void chargePixelShadervPixelBLINNPHONG(const char * file_path, ID3D11PixelShader* &pPS);
	void chargeVertexShadervPixelBLINN(const char * file_path, ID3D11VertexShader* &pVS);
	void chargePixelShadervPixelBLINN(const char * file_path, ID3D11PixelShader* &pPS);
	void chargeVertexShader(const char * file_path, ID3D11VertexShader* &pVS);
	void chargePixelShader(const char * file_path, ID3D11PixelShader* &pPS);

#elif OPENGL
	vec4 color = vec4(1, 1, 1, 1);
	vec4 ligthDir = vec4(0, 1, 0, 0);
	vec4 SpecularColorDir = { 1,1,1,1 };
	vec4 SpecularColorPoint = { 1,1,1,1 };
	vec4 SpecularColorSpot = { 1,1,1,1 };
	vec4 ambientColor = { 1,0,0,0 };
	vec4 KDASL = { 1,0.001,1,1 };
	vec4 SLCOC = vec4{ -.3,0,0,0 };
	vec4 ligthPosition = { 0,0.7,-2,0 };
	//bool rotateDirLight = true;
	//vec4 DifuseColor = { 20,20,20,20 };
	vec4 DifuseColorDir = { 0,1,0,0 };
	vec4 DifuseColorPoint = { 0,1,0,0 };
	vec4 DifuseColorSpot = { 0,1,0,0 };
	//vec4 DifuseColor = { 0,0,0,0};
	//vec4 SpecularColor = { .4,.4,.4,.4 };
	vec4 SPpower = vec4(10);
	vec4 LPLQLD = { 4,4 * 4,10,0 };//LIGHT POINT CONST LINEAR QUADRATIC

	vec4 SLDPS = { 1, 0, 0, 0 };// SWITCH LIGHT DIRECTION, POINT, SPOT
	vec4 ADPS = { 1, 1, 1, 1 };// ATTENUATION LIGHT DIRECTION, POINT, SPOT
	vec2 fViewportDimensions;
	bool bRotationMesh = true;
	myRECT WindowSize;
	myRECT WindowSizeAnte;
	POINT windowPosition;
	GLenum DrawBuffers[1];
	GLuint m_LinearSampler;//linear sample ID
	//GLuint programPBPID;//PROGRAM PIXEL BLINPHONG ID
	//GLuint programPBID;//PROGRAM PIXEL BLINN ID
	//GLuint programVBPID;//PROGRAM VERTES BLINNPHONG ID
	//GLuint programVBID;//PROGRAM VERTES BLINN ID
	//GLuint ActualProgramID;
	
	GLuint depthrenderbuffer;
	GLuint renderedTexture;

	GLuint depthrenderbuffer2;
	GLuint renderedTextureLight;

	GLuint depthrenderbufferLuminance;
	GLuint renderedTextureLuminance;

	GLuint depthrenderbufferBright;
	GLuint renderedTextureBright;

	GLuint depthrenderbufferBlur_H;
	GLuint renderedTextureBlur_H;

	GLuint depthrenderbufferBlur_V;
	GLuint renderedTextureBlur_V;

	GLuint depthrenderbufferAddBright;
	GLuint renderedTextureAddBright;

	GLuint depthrenderbufferToneMap;
	GLuint renderedTextureToneMap;

	GLuint renderedTextureToScreen;

	GLuint quad_VertexArrayID;
	GLuint quad_vertexbuffer;

	void initDevice();
	void initWindow(int argc, char* argv[]);
	//GLuint programID;
	GLuint LoadShadersLightBLINNPHONGv(const char * vertex_file_path, const char * fragment_file_path);
	GLuint LoadShadersLightBLINNv(const char * vertex_file_path, const char * fragment_file_path);
	GLuint LoadShadersVertexBLINNPHONGv(const char * vertex_file_path, const char * fragment_file_path);
	GLuint LoadShadersVertexBLINNv(const char * vertex_file_path, const char * fragment_file_path);
	GLuint LoadShadersSAQ(const char * vertex_file_path, const char * fragment_file_path);
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

	bool rotateDirLight = true;					// bool for rotate the ligth direction
	float BloomThreshold = 0;
	int BrightLod = 0;
	int blurHmip=0;
	int blurVmip = 0;
	int AddBmip = 0;
	float Exposure = 1;
	float BloomMuiltiplier = 1;

	void changeSpecularPower(int x);

	int m_Width;
	int m_Height;

	float spMaxi = 15;
	float spMin = 1;

	bool bMousePressed = false;
	bool bFiestaMode = false;
	float timeFiesta = 0;
	void fiesta();
	float time = 0;
	int passActual[7] = {0,0,0,0,0,0,1};
	void LightPass();
	void LuminancePass();
	void BrightPass();
	void Blur_HPass();
	void Blur_VPass();
	void AddBrightPass();
	void tonePass();
	void RenderToscreenPass();
	void createRenderTaregetLightPass();
	void createRenderTaregetLuminancePass();
	void createRenderTaregetBrightPass();
	void createRenderTaregetBlur_HPass();
	void createRenderTaregetBlur_VPass();
	void createRenderTaregetAddBrightPass();
	void createRenderTaregetTonePass();
};

