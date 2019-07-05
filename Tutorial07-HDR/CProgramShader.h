#pragma once
#include "Header.h"
#include "CPixelShader.h"
#include "CVertexShader.h"
class CProgramShader
{
public:
	CProgramShader();
	~CProgramShader();
#ifdef DX
	CVertexShader m_VertexS;
	CPixelShader m_PixelS;
#elif OPENGL
	GLuint ID;
#endif // DX

};

