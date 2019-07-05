#pragma once
#pragma once
#include "Header.h";
class CVertexShader
{
public:
	CVertexShader();
	~CVertexShader();
#ifdef DX
	ID3D11VertexShader*                 m_pVertexShader = NULL;

#endif // DX
};

