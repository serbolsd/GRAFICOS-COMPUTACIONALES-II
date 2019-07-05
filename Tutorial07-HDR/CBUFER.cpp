#include "CBUFER.h"



CBUFER::CBUFER()
{
}


CBUFER::~CBUFER()
{
}

void CBUFER::cleanUp()
{
#ifdef DX
	if (m_pVertexBuffer)m_pVertexBuffer->Release();
	if (m_pIndexBuffer)m_pIndexBuffer->Release();
	if (m_pCBNeverChanges)m_pCBNeverChanges->Release();
	if (m_pCBChangeOnResize)m_pCBChangeOnResize->Release();
	if (m_pCBChangesEveryFrame)m_pCBChangesEveryFrame->Release();
	if (m_pCBBright)m_pCBBright->Release();
	if (m_pCBlurH)m_pCBlurH->Release();
	if (m_pCBlurV)m_pCBlurV->Release();
	if (m_pCBAddBirght)m_pCBAddBirght->Release();
	if (m_pCToneMap)m_pCToneMap->Release();
#endif // DX

	
}
