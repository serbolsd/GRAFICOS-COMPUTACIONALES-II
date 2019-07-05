#pragma once
#include "Header.h"
class CRenderTarget
{
public:
	CRenderTarget();
	~CRenderTarget();
#ifdef DX
	ID3D11DepthStencilView*             m_pDepthStencilView = NULL;
	ID3D11RenderTargetView*             m_pRenderTargetView = NULL;
	ID3D11Texture2D* m_pBackBuffer = NULL;
#elif OPENGL
	GLuint m_RendeTargetID;
#endif // DX
};

