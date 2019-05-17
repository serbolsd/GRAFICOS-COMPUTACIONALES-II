#pragma once
#include "Header.h"

class CCamera
{
public:
	CCamera();
	~CCamera();
#ifdef DX
	struct CBNeverChanges
	{
		XMMATRIX mView = XMMatrixIdentity();
	};

	struct CBChangeOnResize
	{
		XMMATRIX mProjection;
	};


	XMMATRIX                            m_World = XMMatrixIdentity();
	XMMATRIX                            m_View;
	XMMATRIX                            m_originView;
	XMMATRIX                            m_Projection;

	XMMATRIX                            matRotation;
	XMMATRIX                            matPosition;
	XMMATRIX                            matTransform;
	
	LPPOINT A2;

	XMMATRIX rotationx = {
		(float)cos(0.1),0,(float)sin(0.1),0.0f,
		0.0f,1.0f,0.0f,0.0f,
		(float)-sin(0.1),0,(float)cos(0.1),0.0f,
		0.0f,0.0f,0.0f,0.0f
	};
	XMMATRIX rotationxn = {
		(float)-cos(0.1),0,(float)-sin(0.1),0.0f,
		0.0f,1.0f,0.0f,0.0f,
		(float)sin(0.1),0,(float)-cos(0.1),0.0f,
		0.0f,0.0f,0.0f,0.0f
	};
	XMVECTOR m_Eye;
	XMVECTOR originalEye;

	XMVECTOR m_At;
	XMVECTOR originalAt;

	XMVECTOR m_Up;
	XMVECTOR originalUp;

	XMVECTOR m_cameraFront;
	XMVECTOR m_cameraRight;
	XMVECTOR m_realUp;
	CBNeverChanges m_cbNeverChanges;
	CBChangeOnResize m_cbChangesOnResize;
	
	float m_apertura;
	float m_nearr;
	float m_farr;

	void createNeverChanges();
	void createProjection(float apertura, float width, float height, float nearr, float farr);
	void setProjection();
	void setTransform();
	XMVECTOR pCruss(XMVECTOR v1, XMVECTOR v2);

#elif OPENGL
	vec3 at;
	vec3 originalAt;
	vec3 RelativeUp;
	vec3 originalUp;
	vec3 eye;
	vec3 originaleye;
	mat4 m_view;
	mat4 m_matView;
	mat4 m_matProjection;

	vec3 cameraFront;
	vec3 cameraRight;
	vec3 cameraUp;

	mat4 camTransform;
	mat4 camMatRot;
	mat4 camMatPos;
	
	int frame = 1;
	void setProjection(float widht, float height);
	float farr = 1000;
	float nearr = 0.01;
	float ratio = 50;
#endif // DX
	void setView();
	float m_width;
	float m_height;

	float cameraSpeedRotation = .5f;
	float m_speed = 0.50f;
	POINT m_antePosCur;
	POINT m_ActualPosCur;
	bool isLClick = false;
	//void createView();
	void setEye(float x, float y, float z, float w);
	void setAt(float x, float y, float z, float w);
	void setUp(float x, float y, float z, float w);
	void zoom(unsigned char key);
};

