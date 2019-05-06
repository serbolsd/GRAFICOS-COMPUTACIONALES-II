#pragma once
#include "Header.h";
class CPixelShader
{
public:
	CPixelShader();
	~CPixelShader();
#ifdef DX
	ID3D11PixelShader*                  m_pPixelShader = NULL;

#endif // DX
};

