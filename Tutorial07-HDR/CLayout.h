#pragma once
#include "Header.h";
class CLayout
{
public:
	CLayout();
	~CLayout();
#ifdef DX
	ID3D11InputLayout*                  m_pVertexLayout = NULL;

#endif // DX

};

