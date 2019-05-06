#include "CMesh.h"



CMesh::CMesh()
{
}


CMesh::~CMesh()
{
	
}

void CMesh::mesh_cube()
{
#ifdef DX
	Esmatris = XMMatrixIdentity();
//	Esmatris = XMMatrixIdentity()*XMMatrixTranslation(0.0f, 2.0f, 0.0f);
	if (m_vertex !=nullptr)
	{
		delete []m_vertex;
	}
	if (m_index != nullptr)
	{
		delete[]m_index;
	}
	m_numTries = 24;
	m_numIndex = 36;
	
	m_vertex = new SimpleVertex[24] { { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, -0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, -0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, -1.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, -1.0f) },

	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, -0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, -0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, -1.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, -1.0f) },

	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, -0.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, -0.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, -1.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, -1.0f) },

	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, -0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, -0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, -1.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, -1.0f) },

	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, -0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, -0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, -1.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, -1.0f) },

	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, -0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, -0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, -1.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, -1.0f) }, };

	
	//vertex = (SimpleVertex*)cubever;
	m_index = new WORD[m_numIndex]{
		3,1,0,
		2,1,3,


		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	}; 

#endif // DX
	
}

void CMesh::mesh_triange()
{
#ifdef DX

	Esmatris = XMMatrixIdentity();
	//Esmatris = XMMatrixIdentity()*XMMatrixTranslation(0.0f, 1.0f, 0.0f);
	if (m_vertex != nullptr)
	{
		delete[]m_vertex;
	}
	if (m_index != nullptr)
	{
		delete[]m_index;
	}
	m_numTries = 3;
	m_numIndex = 3;
	m_vertex = new SimpleVertex[24]
	{
		{ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f,0.0f) },
	};

	m_index = new WORD[m_numIndex] 
	{
		2,0,1,

	};

#endif // DX
}

void CMesh::mesh_pantalla()
{
#ifdef DX
	Esmatris = XMMatrixIdentity();
	if (m_vertex != nullptr)
	{
		delete[]m_vertex;
	}
	if (m_index != nullptr)
	{
		delete[]m_index;
	}
	m_numTries = 4;
	m_numIndex = 6;

	m_vertex = new SimpleVertex[m_numTries]{ 

	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
	};


	//vertex = (SimpleVertex*)cubever;
	m_index = new WORD[m_numIndex]{
		3,1,0,
		2,1,3,
	};

#endif // DX
}

void CMesh::meshRead(int numVertices, int numIndices,  aiVector3D*& vertex, aiVector3D*& normals,  aiVector3D*&textcord, std::vector <std::uint32_t>& indis)
{
#ifdef DX


	Esmatris = XMMATRIX(escalar, 0.0f, 0.0f, 0.0f,
		0.0f, escalar, 0.0f, 0.0f,
		0.0f, 0.0f, escalar, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	m_numTries = numVertices;
	m_numIndex = numIndices;

	m_vertex = new SimpleVertex[m_numTries];
	for (int i = 0; i < m_numTries; i++)
	{
		m_vertex[i].Pos.x = vertex[i].x;
		m_vertex[i].Pos.y = vertex[i].y;
		m_vertex[i].Pos.z = vertex[i].z;
		m_vertex[i].Tex.x = textcord[i].x;
		m_vertex[i].Tex.y = -textcord[i].y;
		m_vertex[i].Norm.x = normals[i].x;
		m_vertex[i].Norm.y = normals[i].y;
		m_vertex[i].Norm.z = normals[i].z;
		//m_vertex[i].Norm.x = 0;
		//m_vertex[i].Norm.y = 0;
		//m_vertex[i].Norm.z = 0;
	}
	//indicesss = new unsigned int[numIndices];
	m_index = new WORD[m_numIndex];
	for (int i = 0; i < m_numIndex; i++)
	{
		m_index[i] = (WORD)indis[i];
	}
#elif OPENGL
	numTris = numVertices;
	buffer = new Vertex[numTris * 3];


	for (int i = 0; i < numTris; i++)
	{
		vertexx.x = vertex[i].x;
		vertexx.y = vertex[i].y;
		vertexx.z = vertex[i].z;
		temp_vertices.push_back(vertexx);
		//uv

		uv = { textcord[i].x,textcord[i].y };
		temp_uvs.push_back(uv);

		normalis.x = normals[i].x;
		normalis.y = normals[i].y;
		normalis.z = normals[i].z;
		temp_normals.push_back(normalis);

	}
	for (int i = 0; i < numIndices; i++)
	{
		vertexIndices.push_back(indis[i]);
	}

	//for (int i = 0; i < numTris; i++)
	//{
	//	
	//}

	for (int i = 0; i < vertexIndices.size(); i++)
	{

		unsigned int vertexIndex = vertexIndices[i];

		buffer[i].Position = temp_vertices[vertexIndex];
		//buffer[i].TexCoord = vec2(temp_uvs[vertexIndex].x, -temp_uvs[vertexIndex].y);
		//buffer[i].Position = temp_vertices[vertexIndex];
		if (isquad)
			buffer[i].TexCoord = vec2(temp_uvs[vertexIndex].x, temp_uvs[vertexIndex].y);
		else
			buffer[i].TexCoord = vec2(temp_uvs[vertexIndex].x, -temp_uvs[vertexIndex].y);
		buffer[i].Normals = temp_normals[vertexIndex];

	}
	//for (int i = 0; i < uvIndices.size(); i++)
	//{
	//	unsigned int uvxIndex = uvIndices[i];
	//	uvs.push_back(temp_uvs[uvxIndex]);
	//}
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, (vertexIndices.size() * sizeof(Vertex)), &buffer[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Specify the location and format of the position portion of the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // atributo 0. No hay razón particular para el 0, pero debe corresponder en el shader.
		3,                  // tamaño
		GL_FLOAT,           // tipo
		GL_FALSE,           // normalizado?
		sizeof(Vertex),                    // Paso
		0  // desfase del buffer
	);

	// Specify the location and format of the texture coordinate portion of the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		1,                  // atributo 0. No hay razón particular para el 0, pero debe corresponder en el shader.
		2,                  // tamaño
		GL_FLOAT,           // tipo
		GL_FALSE,           // normalizado?
		sizeof(Vertex),                    // Paso
		(unsigned char*)NULL + (3 * sizeof(float))            // desfase del buffer
	);

	glVertexAttribPointer(
		2,                  // atributo 0. No hay razón particular para el 0, pero debe corresponder en el shader.
		3,                  // tamaño
		GL_FLOAT,           // tipo
		GL_FALSE,           // normalizado?
		sizeof(Vertex),                    // Paso
		(unsigned char*)NULL + (5 * sizeof(float))            // desfase del buffer
	);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif // DX
}

void CMesh::clean()
{
#ifdef DX


	if (m_vertex != nullptr)
	{
		delete[]m_vertex;
	}
	if (m_index != nullptr)
	{
		delete[]m_index;
	}

	if (m_texture.m_pTextureRV) m_texture.m_pTextureRV->Release();
#endif // DX
}

#ifdef DX
void CMesh::mesh_escalar(WPARAM wParam, XMMATRIX& Esmatris)
{


	if (wParam=='O')
	{
		escalar += 0.01f;
	}
	else
	{
		escalar -= 0.01f;
	}

	Esmatris = XMMATRIX(escalar, 0.0f, 0.0f, posx,
		0.0f, escalar, 0.0f, posy,
		0.0f, 0.0f, escalar, posz,
		0.0f, 0.0f, 0.0f, 1.0f);
}
#endif // DX

#ifdef DX
void CMesh::loadText(ID3D11Device* m_pd3dDevice)
{


	wchar_t wtext[50];
	////LPWSTR ptr = wtext;
	////LPCWSTR direcTextur
	//std::string tex;
	char *token = NULL;
	char *nextToken = NULL;
	//token = strtok_s((char *)texName.c_str(), ".", &nextToken);
	token = (char *)texName.c_str();
	////texName = tex;
	//tex += token;
	//tex += ".dds";
	//token = (char *)tex.c_str();
	//std::string neww = token;
	mbstowcs(wtext, token, strlen(token) + 1);//Plus null
	////int i = 0;

	direcTextur = (LPCWSTR)wtext;
	
	D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, direcTextur, NULL, NULL, &m_texture.m_pTextureRV, NULL);

	//D3DX11CreateShaderResourceViewFromFile(m_device.m_pd3dDevice, L"miku.dds", NULL, NULL, &m_texture.m_pTextureRV, NULL);
	//if (FAILED(hr))
	//	return hr;
}
#elif OPENGL
void CMesh::loadText(int width, int height)
{

	char *token = NULL;
	char *nextToken = NULL;
	token = strtok_s((char *)texName.c_str(), ".", &nextToken);
	//token = (char *)texName.c_str();
	//token +=(char *) pngextencion.c_str();
	string pngextencion = token;
	pngextencion += ".png";

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_texture.m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_texture.m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//unsigned char* data = SOIL_load_image("imagen.png", &m_tex.width, &m_tex.height, 0, SOIL_LOAD_RGBA);
	//unsigned char* data = SOIL_load_image("axe.png", &m_tex.width, &m_tex.height, 0, SOIL_LOAD_RGBA);
	//unsigned char* data = SOIL_load_image(pngextencion.c_str(), &m_tex.width, &m_tex.height, 0, SOIL_LOAD_RGBA);
	int m_width, m_height;
	unsigned char* data = SOIL_load_image(pngextencion.c_str(), &m_width, &m_height, 0, SOIL_LOAD_RGBA);
	if (!data)
	{
		cout << "can*t load texture";
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_tex.height, 768, 0, GL_DEPTH_COMPONENT, GL_FLOAT, data);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, width, height);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(data);

	//	m_tex.m_textureID = LoadGLTexture((char*)texName.c_str(), width, height);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void CMesh::escaleModel()
{
	matPos = {
		escalar,0.0,0.0,posx,
		0.0,escalar,0.0,posy,
		0.0,0.0,escalar,posz,
		0.0,0.0,0.0,1.0,
	};
	matModel = matPos * glm::translate(vec3(traslateX, traslateY, traslateZ));
	matModel *= matRotationModel;
}


#endif // DX

void CMesh::rotation(float t)
{
#ifdef DX
if (!rotateOff)
	{
		matrixRotation = XMMatrixRotationY(t);
	}
#elif OPENGL
	m_time += 0.5;
	matRotationModel = rotate(radians(m_time), vec3(0.0, 1.0, 0.0));
#endif // DX
}

void CMesh::voidescalar()
{
#ifdef DX
	Esmatris = XMMATRIX(escalar, 0.0f, 0.0f, posx,
		0.0f, escalar, 0.0f, posy,
		0.0f, 0.0f, escalar, posz,
		0.0f, 0.0f, 0.0f, 1.0f);
	if (bescalar)
	{
		escalatime += .005;
		Escar = XMMatrixScaling(sin(escalatime), sin(escalatime), sin(escalatime));
	}
#elif OPENGL
	if (bescalar)
	{
		escalatime += .005;
		ecal = {
			sin(escalatime),0.0,0.0,0.0,
			0.0,sin(escalatime),0.0,0.0,
			0.0,0.0,sin(escalatime),0.0,
			0.0,0.0,0.0,1.0,
		};
	}

	matModel *= ecal;
#endif // DX

}