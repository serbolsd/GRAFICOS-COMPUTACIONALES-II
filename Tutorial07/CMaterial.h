#pragma once
#include "Header.h"
class CMaterial
{
public:
#ifdef  DX
#elif OPENGL
	GLuint textureID;
	GLuint normalID;
	bool hasdifuce;
	bool hasNormals;

#endif //  DX

	CMaterial();
	~CMaterial();
};

