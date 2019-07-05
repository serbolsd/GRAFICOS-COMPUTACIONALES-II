#pragma once

#include "Header.h"
class CTexture
{
public:
	CTexture();
	~CTexture();
#ifdef DX
	ID3D11Texture2D*                    m_pDepthStencil = NULL;
	ID3D11ShaderResourceView*           m_pTextureRV = NULL;
	ID3D11SamplerState*                 m_pSamplerLinear = NULL;
#elif OPENGL
	GLuint m_textureID;
#endif // DX
};

