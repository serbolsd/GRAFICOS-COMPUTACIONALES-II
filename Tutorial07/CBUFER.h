#pragma once
#include "Header.h"
class CBUFER
{
public:
	CBUFER();
	~CBUFER();
#ifdef DX
	ID3D11Buffer*                       m_pVertexBuffer = NULL;

	ID3D11Buffer*                       m_pIndexBuffer = NULL;

	ID3D11Buffer*                       m_pCBNeverChanges = NULL;

	ID3D11Buffer*                       m_pCBChangeOnResize = NULL;

	ID3D11Buffer*                       m_pCBChangesEveryFrame = NULL;

#endif // DX

};

