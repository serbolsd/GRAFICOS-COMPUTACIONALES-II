#pragma once
#include "Header.h"

class CSwapChain
{
public:
	CSwapChain();
	~CSwapChain();
#ifdef DX
	IDXGISwapChain*                     m_pSwapChain = NULL;

	//ID3D11RenderTargetView*             m_pRenderTargetView = NULL;
	ID3D11Texture2D* m_pBackBuffer = NULL;

#endif // !DX

};

