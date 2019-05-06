#include "CCamera.h"


CCamera::CCamera()
{
#ifdef DX
#elif OPENGL
	eye = { 0.0f,0.0f,-6.0f };
	at = { 0.0f,0.0f,0.0f };
	RelativeUp = { 0.0f,1.0f,0.0f };

	originaleye = { 0.0f,0.0f,-6.0f };
	originalAt = { 0.0f,0.0f,0.0f };
	originalUp = { 0.0f,1.0f,0.0f };
#endif // DX

}


CCamera::~CCamera()
{

}

void CCamera::setEye(float x, float y, float z, float w)
{
#ifdef DX
	m_Eye = XMVectorSet(x, y, z, w);
	originalEye = m_Eye;
#elif OPENGL
	eye.x = x;
	eye.y = y;
	eye.z = z;
	originaleye = eye;
#endif
}
void CCamera::setAt(float x, float y, float z, float w)
{
#ifdef DX
	m_At = XMVectorSet(x, y, z, w);
	originalAt = m_At;
#elif OPENGL
	at.x = x;
	at.y = y;
	at.z = z;
	originalAt = at;
#endif
}
void CCamera::setUp(float x, float y, float z, float w)
{
#ifdef DX
	m_Up = XMVectorSet(x, y, z, w);
	originalUp = m_Up;
#elif OPENGL
	RelativeUp.x = x;
	RelativeUp.y = y;
	RelativeUp.z = z;
	originalUp = RelativeUp;
#endif
}
void CCamera::setView()
{
#ifdef DX
	
	XMVECTOR NORMALICE= m_Eye - m_At;
	NORMALICE = {

		NORMALICE.x / sqrtf((NORMALICE.x*NORMALICE.x) + (NORMALICE.y*NORMALICE.y) + (NORMALICE.z*NORMALICE.z) + (NORMALICE.w*NORMALICE.w)),
		NORMALICE.y / sqrtf((NORMALICE.x*NORMALICE.x) + (NORMALICE.y*NORMALICE.y) + (NORMALICE.z*NORMALICE.z) + (NORMALICE.w*NORMALICE.w)),
		NORMALICE.z / sqrtf((NORMALICE.x*NORMALICE.x) + (NORMALICE.y*NORMALICE.y) + (NORMALICE.z*NORMALICE.z) + (NORMALICE.w*NORMALICE.w)),
		NORMALICE.w / sqrtf((NORMALICE.x*NORMALICE.x) + (NORMALICE.y*NORMALICE.y) + (NORMALICE.z*NORMALICE.z) + (NORMALICE.w*NORMALICE.w)),
	};
	m_cameraFront = NORMALICE;

	NORMALICE = pCruss (m_cameraFront,m_Up);
	NORMALICE = {

		NORMALICE.x / sqrtf((NORMALICE.x*NORMALICE.x) + (NORMALICE.y*NORMALICE.y) + (NORMALICE.z*NORMALICE.z) + (NORMALICE.w*NORMALICE.w)),
		NORMALICE.y / sqrtf((NORMALICE.x*NORMALICE.x) + (NORMALICE.y*NORMALICE.y) + (NORMALICE.z*NORMALICE.z) + (NORMALICE.w*NORMALICE.w)),
		NORMALICE.z / sqrtf((NORMALICE.x*NORMALICE.x) + (NORMALICE.y*NORMALICE.y) + (NORMALICE.z*NORMALICE.z) + (NORMALICE.w*NORMALICE.w)),
		NORMALICE.w / sqrtf((NORMALICE.x*NORMALICE.x) + (NORMALICE.y*NORMALICE.y) + (NORMALICE.z*NORMALICE.z) + (NORMALICE.w*NORMALICE.w)),
	};

	m_cameraRight = NORMALICE;
	m_realUp = pCruss(m_cameraFront , m_cameraRight);
	m_Up = m_realUp;


	matPosition = XMMATRIX(
		1, 0, 0, -m_Eye.x,
		0, 1, 0, -m_Eye.y,
		0, 0, 1, -m_Eye.z,
		0, 0, 0, 1
	);
	//matPosition = XMMatrixTranspose(matPosition);
	matRotation = XMMATRIX(
		m_cameraRight.x, m_cameraRight.y, m_cameraRight.z, 0,
		-m_Up.x, -m_Up.y, -m_Up.z, 0,
		-m_cameraFront.x, -m_cameraFront.y, -m_cameraFront.z, 0,
		0, 0, 0, 1
		);
	//matRotation = XMMatrixTranspose(matRotation);
	matTransform =  matPosition*matRotation ;
	//matTransform = XMMatrixTranspose(matTransform);
	m_View = XMMatrixLookAtLH(m_Eye, m_At, m_Up);
	createNeverChanges();
	//matTransform = matPosition*m_View;

	//matTransform = XMMatrixTranspose(matTransform);
#elif OPENGL
	cameraFront = normalize(eye - at);


	//cameraFront = { 0.0f,0.0f,-1.0f };
	cameraRight = normalize(cross(RelativeUp, cameraFront));
	cameraUp = cross(cameraFront, cameraRight);
	//vec3 result= substract
	RelativeUp = cameraUp;
	//camTransform = mat4(vec4(cameraRight,0), vec4(-cameraUp,0), vec4(cameraFront,0), vec4(eye-at,1));
	//camTransform = mat4(vec4(-cameraRight,0), vec4(cameraUp,0), vec4(-cameraFront,0), vec4(eye,1));
	camMatPos = mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, 0), vec4(-eye, 1));
	vec3 N;
	vec3 U;
	vec3 V;
	N = {
		cameraFront.x / sqrtf((cameraFront.x*cameraFront.x) + (cameraFront.y*cameraFront.y) + (cameraFront.z*cameraFront.z)),
		cameraFront.y / sqrtf((cameraFront.x*cameraFront.x) + (cameraFront.y*cameraFront.y) + (cameraFront.z*cameraFront.z)),
		cameraFront.z / sqrtf((cameraFront.x*cameraFront.x) + (cameraFront.y*cameraFront.y) + (cameraFront.z*cameraFront.z)),
	};
	U = cross(cameraUp, cameraFront);
	U = {
		U.x / sqrtf((U.x*U.x) + (U.y*U.y) + (U.z*U.z)),
		U.y / sqrtf((U.x*U.x) + (U.y*U.y) + (U.z*U.z)),
		U.z / sqrtf((U.x*U.x) + (U.y*U.y) + (U.z*U.z)),
	};
	V = cross(N, -cameraRight);
	//camTransform=inverse

	//camTransform = inverse(camTransform);
	camMatRot = mat4(vec4(cameraRight, 0), vec4(V, 0), vec4(N, 0), vec4(0, 0, 0, 1));
	//camMatRot = mat4(vec4(1,0,0,0), vec4(0,1,0,0), vec4(0,0,1,0), vec4(0,0,0,1));
	//NewAt = eye + cameraFront;
	//camMatRot= inverse(camMatRot);
	camTransform = camMatRot * camMatPos;
	//camTransform = mat4
	//{
	//	cameraRight.x,cameraRight.y,cameraRight.z,eye.x,
	//	cameraUp.x,cameraUp.y,cameraUp.z,eye.x,
	//	cameraFront.x,cameraFront.y,cameraFront.z,eye.x,
	//	0,0,0,1
	//};
	//camTransform = inverse(camTransform);
	//camer
	vec3 focusPos = at - eye;
	m_matView = lookAt(
		eye, // the position of your camera, in world space
		at,   // where you want to look at, in world space
		RelativeUp        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
	);
#endif
}

#ifdef DX
void CCamera::setTransform()
{
	
	//matPosition=
	//{
	//	1,0,0,m_View._41,
	//	0,1,0,m_View._42,
	//	0,0,1,m_View._43,
	//	0,0,0,1,
	//
	//};
	//matTransform = 
	//
	//matTransform = XMMatrixTranspose(matTransform);
}

void CCamera::createNeverChanges()
{

	m_cbNeverChanges.mView = m_View;
	m_cbNeverChanges.mView = XMMatrixTranspose(m_cbNeverChanges.mView);

}
void CCamera::createProjection(float apertura, float width, float height, float nearr, float farr) 
{
	m_apertura = apertura;
	m_width = width;
	m_height = height;
	m_nearr = nearr;
	m_farr = farr;
	m_Projection = XMMatrixPerspectiveFovLH(apertura,width / height, nearr, farr);

	m_cbChangesOnResize.mProjection = m_Projection;
	m_cbChangesOnResize.mProjection = XMMatrixTranspose(m_cbChangesOnResize.mProjection);
	//m_Projection = m_cbChangesOnResize.mProjection;

}
void CCamera::setProjection()
{
	m_Projection = XMMatrixPerspectiveFovLH(m_apertura, m_width / m_height, m_nearr, m_farr);

	m_cbChangesOnResize.mProjection = m_Projection;
	m_cbChangesOnResize.mProjection = XMMatrixTranspose(m_cbChangesOnResize.mProjection);
}

XMVECTOR CCamera::pCruss(XMVECTOR v1, XMVECTOR v2)
{
	XMVECTOR pCrus = {(v1.y*v2.z-v1.z*v2.y),(v1.x*v2.z - v1.z*v2.x),(v1.x*v2.y - v1.y*v2.x)};
	return pCrus;
}

#elif OPENGL
void CCamera::setProjection(float widht, float height)
{
	m_width = widht;
	m_height = height;
	m_matProjection = perspective(
		radians(ratio), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
		widht / height,       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
		nearr,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
		farr             // Far clipping plane. Keep as little as possible.
	);
}
#endif // OPENGL

void CCamera::zoom(unsigned char key)
{
#ifdef DX
	if (key == 'K')
	{
		m_apertura += .1;
	}
	else
	{
		m_apertura -= .1;
	}
	if (m_apertura <= 0.1)
	{
		m_apertura = 0.1;
	}
	else if (m_apertura >= 2)
	{
		m_apertura = 2;
	}
	setProjection();
#elif OPENGL
	if (key == 'k' || key == 'K')
	{
		ratio += 1;
	}
	else
	{
		ratio -= 1;
	}
	if (ratio <= 0.1)
	{
		ratio = 0.1;
	}
	else if (ratio >= 120)
	{
		ratio = 120;
	}
	setProjection(m_width, m_height);
#endif // DX

}