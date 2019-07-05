#pragma once
#include "Header.h"
#include "CTexture.h"
class CMaterial
{
public:
	bool hasdifuce;
	bool hasNormals;
#ifdef  DX
	CTexture m_Difuse;
	CTexture m_Normal;
#elif OPENGL
	GLuint textureID;
	GLuint normalID;
	

#endif //  DX

	CMaterial();
	~CMaterial();
};

