#pragma once
#include "Header.h";

#include "CTexture.h"
#include "CMaterial.h"
//#include "resource.h"

class CMesh
{
public:
	CMesh();
	~CMesh();
#ifdef DX
	typedef unsigned short indexBuf;
	struct Vertex
	{
		float pos[3];
		float tex[3];

	};
	typedef unsigned short world;
	struct SimpleVertex
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
		XMFLOAT3 Norm;
		XMFLOAT3 Tan;
	};

	SimpleVertex *m_vertex = nullptr;
	WORD *m_index = nullptr;
	std::vector <std::uint32_t> indicesss;
	XMMATRIX Esmatris = XMMatrixIdentity();
	XMMATRIX Escar = XMMatrixIdentity();
	XMMATRIX matrixRotation=XMMatrixIdentity();
	LPCWSTR direcTextur;
	float posx = 0;
	float posy = 0;
	float posz = 0;
	void loadText(ID3D11Device*                       m_pd3dDevice);
	void mesh_escalar(WPARAM wParam, XMMATRIX& Esmatris);
#elif OPENGL
	vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	vector< glm::vec3 > temp_vertices;
	vector< glm::vec2 > temp_uvs;
	vector< glm::vec3 > temp_normals;
	vector< glm::vec3 > temp_tangs;
	
	mat4 matModel;
	mat4 matPos;
	mat4 matRotationModel=mat4(1.0f);

	glm::vec3 vertex;
	glm::vec3 vertexx;
	glm::vec3 normalis;
	glm::vec2 uv;
	glm::vec3 index;
	vec3 tangg;

	struct Vertex
	{
		vec3 Position;												/*!< Vertex position */
		vec2 TexCoord;
		vec3 Normals;
		vec3 tangents;
	};

	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normales;
	std::vector< glm::vec3 > tangs;

	Vertex* buffer;

	int numTris;
	int numIndices;
	GLuint VertexArrayID;
	GLuint vertexbuffer;
	GLuint uvBuffer;
	float m_time = 0.0f;
	mat4 tralatemat = {
		1, 0, 0, 1,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
};
	float tras = -10;
	void escaleModel();
	void loadText(int width, int height);
	void loadNormalText(int width, int height);
	mat4 ecal = mat4(1.0f);

	float traslateX = 0;
	float traslateY = 0;
	float traslateZ = 0;

	float posx = 0;
	float posy = 0;
	float posz = 0;
#endif // DX
	
	float escalatime = 0;
	
	void voidescalar();
	bool bescalar = false;
	int m_numTries;
	int m_numIndex;

	void clean();
	void mesh_cube();
	void mesh_triange();
	void mesh_pantalla();

	void meshRead(int numVertices,int numIndices, aiVector3D*&  vertex, aiVector3D*& normals, aiVector3D*&  textcord,aiVector3D*&tang, std::vector <std::uint32_t>& indis);
	
	float escalar = 1.0f;
	CTexture m_texture;
	std::string texName= "Scene/";
	CMaterial textures;

	bool rotateOff=true;
	bool isquad = false;
	void rotation(float t);
};

