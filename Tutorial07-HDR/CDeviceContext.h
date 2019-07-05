#pragma once
#include "Header.h"
class CDeviceContext
{
public:
	CDeviceContext();
	~CDeviceContext();
#ifdef DX

	D3D_DRIVER_TYPE                     m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL                   m_featureLevel = D3D_FEATURE_LEVEL_11_0;

	ID3D11DeviceContext*                m_pImmediateContext = NULL;
#endif // DEBUG

};

