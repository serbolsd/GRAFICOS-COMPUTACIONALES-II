#include "CManager.h"




CManager::CManager()
{
}


CManager::~CManager()
{
}
#ifdef DX



HRESULT CManager::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		SIZE_T msgSize = pErrorBlob->GetBufferSize();
		string msg;
		msg.resize(msgSize);
		memcpy(&msg[0], pErrorBlob->GetBufferPointer(), msgSize);

		//pErroMsg->Release();
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}
#endif // DX

void CManager::cleanup()
{
#ifdef DX
	//CleanUp the models
	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].clean();
	}
	m_SAQ.clean();
	if (m_deviceContext.m_pImmediateContext) m_deviceContext.m_pImmediateContext->ClearState();
	//
	//CleanUp the samplers
	if (m_texture.m_pSamplerLinear) m_texture.m_pSamplerLinear->Release();
	if (m_texture.m_pTextureRV) m_texture.m_pTextureRV->Release();
	if (m_texture.m_pDepthStencil) m_texture.m_pDepthStencil->Release();
	//
	//CleanUp the buffers
	m_buffers.cleanUp();
	//
	//CleanUp the Layout
	if (m_layout.m_pVertexLayout) m_layout.m_pVertexLayout->Release();
	//CleaUp the programs shaders
	if (m_programSVertexBP.m_VertexS.m_pVertexShader) m_programSVertexBP.m_VertexS.m_pVertexShader->Release();
	if (m_programSVertexBP.m_PixelS.m_pPixelShader) m_programSVertexBP.m_PixelS.m_pPixelShader->Release();

	if (m_programSVertexB.m_VertexS.m_pVertexShader) m_programSVertexB.m_VertexS.m_pVertexShader->Release();
	if (m_programSVertexB.m_PixelS.m_pPixelShader) m_programSVertexB.m_PixelS.m_pPixelShader->Release();

	if (m_programSPixelBP.m_VertexS.m_pVertexShader) m_programSPixelBP.m_VertexS.m_pVertexShader->Release();
	if (m_programSPixelBP.m_PixelS.m_pPixelShader) m_programSPixelBP.m_PixelS.m_pPixelShader->Release();

	if (m_programSPixelB.m_VertexS.m_pVertexShader) m_programSPixelB.m_VertexS.m_pVertexShader->Release();
	if (m_programSPixelB.m_PixelS.m_pPixelShader) m_programSPixelB.m_PixelS.m_pPixelShader->Release();
	if (m_programSLuminance.m_VertexS.m_pVertexShader)m_programSLuminance.m_VertexS.m_pVertexShader->Release();
	if (m_programSLuminance.m_PixelS.m_pPixelShader)m_programSLuminance.m_PixelS.m_pPixelShader->Release();
	if (m_programSToneMap.m_VertexS.m_pVertexShader)m_programSToneMap.m_VertexS.m_pVertexShader->Release();
	if (m_programSToneMap.m_PixelS.m_pPixelShader)m_programSToneMap.m_PixelS.m_pPixelShader->Release();
	//
	//CleanUp the renderTargets and RenderTarget's DephStencil
	if (m_renderTargetV.m_pDepthStencilView) m_renderTargetV.m_pDepthStencilView->Release();
	if (m_renderTargetV.m_pRenderTargetView) m_renderTargetV.m_pRenderTargetView->Release();
	
	if (m_lightPass.m_pDepthStencilView) m_lightPass.m_pDepthStencilView->Release();
	if (m_lightPass.m_pRenderTargetView) m_lightPass.m_pRenderTargetView->Release();
	if (m_LuminancePass.m_pDepthStencilView) m_LuminancePass.m_pDepthStencilView->Release();
	if (m_LuminancePass.m_pRenderTargetView) m_LuminancePass.m_pRenderTargetView->Release();
	if (m_BrightPass.m_pDepthStencilView) m_BrightPass.m_pDepthStencilView->Release();
	if (m_BrightPass.m_pRenderTargetView) m_BrightPass.m_pRenderTargetView->Release();
	if (m_BlurHPass.m_pDepthStencilView) m_BlurHPass.m_pDepthStencilView->Release();
	if (m_BlurHPass.m_pRenderTargetView) m_BlurHPass.m_pRenderTargetView->Release();
	if (m_BlurVPass.m_pDepthStencilView) m_BlurVPass.m_pDepthStencilView->Release();
	if (m_BlurVPass.m_pRenderTargetView) m_BlurVPass.m_pRenderTargetView->Release();
	if (m_AddBrightPass.m_pDepthStencilView) m_AddBrightPass.m_pDepthStencilView->Release();
	if (m_AddBrightPass.m_pRenderTargetView) m_AddBrightPass.m_pRenderTargetView->Release();
	if (m_ToneMapPass.m_pDepthStencilView) m_ToneMapPass.m_pDepthStencilView->Release();
	if (m_ToneMapPass.m_pRenderTargetView) m_ToneMapPass.m_pRenderTargetView->Release();
	//
	//cleanUp the backBuffer and deviceContext
	if (m_swapChain.m_pSwapChain) m_swapChain.m_pSwapChain->Release();
	if (m_device.m_pd3dDevice) m_device.m_pd3dDevice->Release();
	//
	//CleanUp the Resources sahders
	if (renderTextureLightPass) renderTextureLightPass->Release();
	if (SResourceLightPass)SResourceLightPass->Release();
	if (renderTextureBrightPass)renderTextureBrightPass->Release();
	if (SResourceBrightPass) SResourceBrightPass->Release();
	if (renderTextureBlurHPass)renderTextureBlurHPass->Release();
	if (SResourceBlurHPass)SResourceBlurHPass->Release();
	if (renderTextureBlurVPass)renderTextureBlurVPass->Release();
	if (SResourceBlurVPass)SResourceBlurVPass->Release();
	if (renderTextureAddBrightPass)renderTextureAddBrightPass->Release();
	if (SResourceAddBrightPass)SResourceAddBrightPass->Release();
	//
	
#else
#endif // !DX
}
////////////////////////////////////////
//MESH
////////////////////////////////////////
void CManager::changeToCube()
{
#ifdef DX
	CMesh newmesh;
	m_meshes.push_back(newmesh);
	m_meshes.back().mesh_cube();
#else
#endif // !DX

}
void CManager::changeToTriangle()
{
#ifdef DX
	CMesh newmesh;
	m_meshes.push_back(newmesh);
	m_meshes.back().mesh_triange();
#else
#endif // !DX
}
void CManager::changeToPantalla()
{
#ifdef DX
	CMesh newmesh;
	m_meshes.push_back(newmesh);
	m_meshes.back().mesh_pantalla();
#else
#endif // !DX
}

void CManager::chargeMesh()
{
#ifdef DX
	//m_device.model = aiImportFile("Dwarf/dwarf.x", aiProcessPreset_TargetRealtime_MaxQuality);
	m_device.model = aiImportFile("Scene/Scene.fbx", aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	//m_device.model = aiImportFile("dwarf/dwarfWithEffectInstance.x", aiProcessPreset_TargetRealtime_MaxQuality);

	for (unsigned int i = 0; i < m_device.model->mNumMeshes; i += (unsigned int)1)
	{
		CMesh newmesh;
		m_meshes.push_back(newmesh);
		std::vector <std::uint32_t> indis;
		indis.reserve(m_device.model->mMeshes[i]->mNumFaces * 3);
		for (std::uint32_t faceIdx = 0u; faceIdx < m_device.model->mMeshes[i]->mNumFaces; faceIdx++)
		{
			//assert(m_device.model->mMeshes[faceIdx]->mFaces->mNumIndices == 3u);
			indis.push_back(m_device.model->mMeshes[i]->mFaces[faceIdx].mIndices[0u]);
			indis.push_back(m_device.model->mMeshes[i]->mFaces[faceIdx].mIndices[1u]);
			indis.push_back(m_device.model->mMeshes[i]->mFaces[faceIdx].mIndices[2u]);
		}
		m_meshes[i].meshRead(m_device.model->mMeshes[i]->mNumVertices, (m_device.model->mMeshes[i]->mNumFaces) * 3, m_device.model->mMeshes[i]->mVertices, m_device.model->mMeshes[i]->mNormals, m_device.model->mMeshes[i]->mTextureCoords[0], m_device.model->mMeshes[i]->mTangents, indis);
		//m_meshes[i].indicesss = m_device.model->mMeshes[i]->mFaces->mIndices;
		readTextureMesh(i);
		readNormTextureMesh(i);
	}
#elif OPENGL
	//load model data
	//m_device.model = aiImportFile("Wolf_One_x.x", aiProcessPreset_TargetRealtime_MaxQuality);
	//m_device.model = aiImportFile("ninjaHead.obj", aiProcessPreset_TargetRealtime_MaxQuality);
	//m_device.model = aiImportFile("Dwarf/DwarfWithEffectInstance.x", aiProcessPreset_TargetRealtime_MaxQuality);
	m_device.model = aiImportFile("Scene/Scene.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	//m_device.model=aiImportFileEx("Dwarf/dwarf.x",0, 0);
	for (unsigned int i = 0; i < m_device.model->mNumMeshes; i += (unsigned int)1)
	{
		// for each mesh, i create a new mesh object
		CMesh newmesh;
		//newmesh.traslateX = 30;
		m_meshes.push_back(newmesh);
		//i gen de vertex array and bind it
		glGenVertexArrays(1, &m_meshes[i].VertexArrayID);
		glBindVertexArray(m_meshes[i].VertexArrayID);


		std::vector <std::uint32_t> indis;
		indis.reserve(m_device.model->mMeshes[i]->mNumFaces * 3);

		for (std::uint32_t faceIdx = 0u; faceIdx < m_device.model->mMeshes[i]->mNumFaces; faceIdx++)
		{
			//get the indexes of the vertices
			uint32_t ind1 = (m_device.model->mMeshes[i]->mFaces[faceIdx].mIndices[0u]);
			uint32_t ind2 = (m_device.model->mMeshes[i]->mFaces[faceIdx].mIndices[1u]);
			uint32_t ind3 = (m_device.model->mMeshes[i]->mFaces[faceIdx].mIndices[2u]);

			//I organize the indexes of the vertices
			indis.push_back(ind1);
			indis.push_back(ind2);
			indis.push_back(ind3);
		}
		//this i create the buffer vertex and index
		m_meshes[i].meshRead(m_device.model->mMeshes[i]->mNumVertices, (m_device.model->mMeshes[i]->mNumFaces) * 3, m_device.model->mMeshes[i]->mVertices, m_device.model->mMeshes[i]->mNormals, m_device.model->mMeshes[i]->mTextureCoords[0], m_device.model->mMeshes[i]->mTangents, indis);
		glBindVertexArray(0);
		readTextureMesh(i);
		readNormTextureMesh(i);
	}
	//m_meshes[0].texName = "Toad_n.tga";
	//m_meshes[0].loadNormalText(640, 480);
#endif // !DX
}
void CManager::readTextureMesh(int i)
{
	
	const aiMaterial* pMaterial = m_device.model->mMaterials[m_device.model->mMeshes[i]->mMaterialIndex];

	//m_Textures[i] = NULL;
	if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		aiString Path;

		if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
			m_meshes[i].texName += (std::string) Path.data;
		}
#ifdef DX
		m_meshes[i].loadText(m_device.m_pd3dDevice);
#elif OPENGL
		//create the texture
		m_meshes[i].loadText(640, 480);
#endif // !DX
	}


	
}
void CManager::readNormTextureMesh(int i)
{

	const aiMaterial* pMaterial = m_device.model->mMaterials[m_device.model->mMeshes[i]->mMaterialIndex];

	//m_Textures[i] = NULL;
	//if (pMaterial->GetTextureCount(aiTextureType_NORMALS) > 0) {
	aiString Path;

	if (aiReturn::aiReturn_SUCCESS == pMaterial->GetTexture(aiTextureType_AMBIENT, 0, &Path))
	{
		m_meshes[i].norName += (std::string) Path.data;
#ifdef DX
		m_meshes[i].loadNormalText(m_device.m_pd3dDevice);
#elif OPENGL
		m_meshes[i].loadNormalText(640, 480);
		//create the texture
#endif // !DX<
	}
	//m_meshes[i].texName ="Toad_n.tga";
//}


}

void CManager::ReadPantalla()
{
	//m_device.model = aiImportFile("ScreenAlignedQuad.3ds", aiProcessPreset_TargetRealtime_MaxQuality|aiProcess_ConvertToLeftHanded);
	m_device.model = aiImportFile("SAQ.obj", aiProcessPreset_TargetRealtime_MaxQuality|aiProcess_ConvertToLeftHanded);

	std::vector <std::uint32_t> indis;
	indis.reserve(m_device.model->mMeshes[0]->mNumFaces * 3);

	for (std::uint32_t faceIdx = 0u; faceIdx < m_device.model->mMeshes[0]->mNumFaces; faceIdx++)
	{
		//get the indexes of the vertices
		uint32_t ind1 = (m_device.model->mMeshes[0]->mFaces[faceIdx].mIndices[0u]);
		uint32_t ind2 = (m_device.model->mMeshes[0]->mFaces[faceIdx].mIndices[1u]);
		uint32_t ind3 = (m_device.model->mMeshes[0]->mFaces[faceIdx].mIndices[2u]);

		//I organize the indexes of the vertices
		indis.push_back(ind1);
		indis.push_back(ind2);
		indis.push_back(ind3);
	}
	//this i create the buffer vertex and index
	m_SAQ.meshRead(m_device.model->mMeshes[0]->mNumVertices, (m_device.model->mMeshes[0]->mNumFaces) * 3, m_device.model->mMeshes[0]->mVertices, m_device.model->mMeshes[0]->mNormals, m_device.model->mMeshes[0]->mTextureCoords[0], m_device.model->mMeshes[0]->mTangents, indis);
	//glBindVertexArray(0);

}

void CManager::drawnm_meshes()
{
	for (int i = 0; i < m_meshes.size(); i++)
	{
		//if (m_buffers.m_pVertexBuffer) m_buffers.m_pVertexBuffer->Release();
		//if (m_buffers.m_pIndexBuffer) m_buffers.m_pIndexBuffer->Release();
		//
		//m_mesh.mesh_pantalla();
		//DcreateVertexBuffer(m_mesh.m_numTries);
		////	DcreateVertexBuffer(3);
		//m_device.InitData.pSysMem = m_mesh.m_vertex;
		////m_device.InitData.pSysMem = verticestri;
		//m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pVertexBuffer);
		//
		//// Set vertex buffer
		//UINT stride = sizeof(SimpleVertex);
		//UINT offset = 0;
		//m_deviceContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &m_buffers.m_pVertexBuffer, &stride, &offset);
		//
		//// Create index buffer
		//// Create vertex buffer
		//DcreateIndexBuffer(m_mesh.m_numIndex);
		////DcreateIndexBuffer(3);
		//m_device.InitData.pSysMem = m_mesh.m_index;
		//// m_device.InitData.pSysMem = indicesTri;
		//m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pIndexBuffer);
		//
		//
		//// Set index buffer
		//m_deviceContext.m_pImmediateContext->IASetIndexBuffer(m_buffers.m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		//cb.mWorld = move;
		//
		//m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
		//m_deviceContext.m_pImmediateContext->DrawIndexed(m_meshes[0].m_numIndex, 0, 0);
	}	
}
////////////////////////////////////////
//DEVICE
////////////////////////////////////////
bool CManager::DInitDevice(HRESULT &hr, const HWND& hWnd)
{
#ifdef DX
	//changeToCube();
	//changeToPantalla();
	//changeToTriangle();
	//HRESULT hr = S_OK;
	RECT rc;

	GetClientRect(hWnd, &rc);
	m_device.width = rc.right - rc.left;
	m_device.height = rc.bottom - rc.top;
	m_Width = m_device.width;
	m_Height = m_device.height;

#ifdef _DEBUG
	m_device.createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


	m_device.numDriverTypes = ARRAYSIZE(m_device.driverTypes);


	m_device.numFeatureLevels = ARRAYSIZE(m_device.featureLevels);

	DswapChain(hWnd);
	for (UINT driverTypeIndex = 0; driverTypeIndex < m_device.numDriverTypes; driverTypeIndex++)
	{
		m_deviceContext.m_driverType = m_device.driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, m_deviceContext.m_driverType, NULL, m_device.createDeviceFlags, m_device.featureLevels, m_device.numFeatureLevels,
			D3D11_SDK_VERSION, &m_device.sd, &m_swapChain.m_pSwapChain, &m_device.m_pd3dDevice, &m_deviceContext.m_featureLevel, &m_deviceContext.m_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	if (!(DcreateRenderTargetV(hr)))
		return hr;
	//generateRenderTexture
	DcreateDepthStencilTex(hr);
	if (FAILED(hr))
		return hr;

	DcreateDepthStencilV(hr);
	if (FAILED(hr))
		return hr;
	//generateRenderTexture();
	generateRenderTextureWhitReferences(m_lightPass.m_pRenderTargetView, m_lightPass.m_pDepthStencilView, renderTextureLightPass, SResourceLightPass);
	generateRenderTextureWhitReferences(m_LuminancePass.m_pRenderTargetView, m_LuminancePass.m_pDepthStencilView,renderTextureLuminancePass,SResourceLuminancePass);
	generateRenderTextureWhitReferences(m_BrightPass.m_pRenderTargetView,m_BrightPass.m_pDepthStencilView,renderTextureBrightPass,SResourceBrightPass);
	generateRenderTextureWhitReferences(m_BlurHPass.m_pRenderTargetView, m_BlurHPass.m_pDepthStencilView,renderTextureBlurHPass,SResourceBlurHPass);
	generateRenderTextureWhitReferences(m_BlurVPass.m_pRenderTargetView, m_BlurVPass.m_pDepthStencilView,renderTextureBlurVPass,SResourceBlurVPass);
	generateRenderTextureWhitReferences(m_AddBrightPass.m_pRenderTargetView, m_AddBrightPass.m_pDepthStencilView,renderTextureAddBrightPass,SResourceAddBrightPass);
	generateRenderTextureWhitReferences(m_ToneMapPass.m_pRenderTargetView, m_ToneMapPass.m_pDepthStencilView,renderTextureToneMapPass,SResourceToneMapPass);
	SResourceActual = SResourceToneMapPass;
	//m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &m_lightPass.m_pRenderTargetView, m_lightPass.m_pDepthStencilView);
	m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &m_renderTargetV.m_pRenderTargetView, m_renderTargetV.m_pDepthStencilView);

	DsetupViewport();
	m_deviceContext.m_pImmediateContext->RSSetViewports(1, &m_device.vp);
	// Compile the vertex shader////////////////
	hr = CompileShaderFromFile(L"Tutorial07.fx", "VS", "vs_4_0", &m_device.pVSBlob);

	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}
	// Create the vertex shader////////////////
	hr = m_device.m_pd3dDevice->CreateVertexShader(m_device.pVSBlob->GetBufferPointer(), m_device.pVSBlob->GetBufferSize(), NULL, &m_programSActual.m_VertexS.m_pVertexShader);
	
	
	if (FAILED(hr))
	{
		m_device.pVSBlob->Release();
		return hr;
	}
	// Define the input layout/////////////////////
	m_device.numElements = ARRAYSIZE(m_device.layout);
	// Create the input layout///////////////////
	hr = m_device.m_pd3dDevice->CreateInputLayout(m_device.layout, m_device.numElements, m_device.pVSBlob->GetBufferPointer(),
		m_device.pVSBlob->GetBufferSize(), &m_layout.m_pVertexLayout);
	m_device.pVSBlob->Release();
	if (FAILED(hr))
		return hr;
	// Set the input layout////////////////
	m_deviceContext.m_pImmediateContext->IASetInputLayout(m_layout.m_pVertexLayout);
	//chargeVertexShadervPixelBLINNPHONG("Tutorial07.fx", m_programSPixelBP.m_VertexS.m_pVertexShader);
	//chargePixelShadervPixelBLINNPHONG("Tutorial07.fx", m_programSPixelBP.m_PixelS.m_pPixelShader);

	//m_programSActual = m_programSPixelBP;
	//chargeVertexShadervPixelBLINN("Tutorial07.fx", m_programSPixelB.m_VertexS.m_pVertexShader);
	//chargePixelShadervPixelBLINN("Tutorial07.fx", m_programSPixelB.m_PixelS.m_pPixelShader);
	//
	//chargeVertexShadervVertexBLINNPHONG("Tutorial07.fx", m_programSVertexBP.m_VertexS.m_pVertexShader);
	//chargePixelShadervVertexBLINNPHONG("Tutorial07.fx", m_programSVertexBP.m_PixelS.m_pPixelShader);
	//
	//chargeVertexShadervVertexBLINN("Tutorial07.fx", m_programSVertexB.m_VertexS.m_pVertexShader);
	//chargePixelShadervVertexBLINN("Tutorial07.fx", m_programSVertexB.m_PixelS.m_pPixelShader);
	//
	//chargeVertexShadervLuminance("Tutorial07-Luminance.fx", m_programSLuminance.m_VertexS.m_pVertexShader);
	//chargePixelShadervLuminance("Tutorial07-Luminance.fx", m_programSLuminance.m_PixelS.m_pPixelShader);

	// Compile the pixel shader////////////////
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile(L"Tutorial07.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}
	// Create the pixel shader///////////////////
	hr = m_device.m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_programSActual.m_PixelS.m_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;
	chargeVertexShadervPixelBLINNPHONG("Tutorial07.fx", m_programSPixelBP.m_VertexS.m_pVertexShader);
	chargePixelShadervPixelBLINNPHONG("Tutorial07.fx", m_programSPixelBP.m_PixelS.m_pPixelShader);

	chargeVertexShadervPixelBLINN("Tutorial07.fx", m_programSPixelB.m_VertexS.m_pVertexShader);
	chargePixelShadervPixelBLINN("Tutorial07.fx", m_programSPixelB.m_PixelS.m_pPixelShader);

	chargeVertexShadervVertexBLINNPHONG("Tutorial07.fx", m_programSVertexBP.m_VertexS.m_pVertexShader);
	chargePixelShadervVertexBLINNPHONG("Tutorial07.fx", m_programSVertexBP.m_PixelS.m_pPixelShader);

	chargeVertexShadervVertexBLINN("Tutorial07.fx", m_programSVertexB.m_VertexS.m_pVertexShader);
	chargePixelShadervVertexBLINN("Tutorial07.fx", m_programSVertexB.m_PixelS.m_pPixelShader);

	chargeVertexShader("Tutorial07-LUMINANCE.fx",m_programSLuminance.m_VertexS.m_pVertexShader);
	chargePixelShader("Tutorial07-LUMINANCE.fx",m_programSLuminance.m_PixelS.m_pPixelShader);
	chargeVertexShader("Tutorial07-Bright.fx", m_programSBright.m_VertexS.m_pVertexShader);
	chargePixelShader("Tutorial07-Bright.fx", m_programSBright.m_PixelS.m_pPixelShader);
	chargeVertexShader("Tutorial07-BLURH.fx", m_programSBlur_H.m_VertexS.m_pVertexShader);
	chargePixelShader("Tutorial07-BLURH.fx", m_programSBlur_H.m_PixelS.m_pPixelShader);
	chargeVertexShader("Tutorial07-BLURV.fx", m_programSBlur_V.m_VertexS.m_pVertexShader);
	chargePixelShader("Tutorial07-BLURV.fx", m_programSBlur_V.m_PixelS.m_pPixelShader);
	chargeVertexShader("Tutorial07-AddBright.fx", m_programSAddBirght.m_VertexS.m_pVertexShader);
	chargePixelShader("Tutorial07-AddBright.fx", m_programSAddBirght.m_PixelS.m_pPixelShader);
	chargeVertexShader("Tutorial07-ToneMap.fx", m_programSToneMap.m_VertexS.m_pVertexShader);
	chargePixelShader("Tutorial07-ToneMap.fx", m_programSToneMap.m_PixelS.m_pPixelShader);
	chargeVertexShader("Tutorial07-RenderScreem.fx", m_programSRenderScreen.m_VertexS.m_pVertexShader);
	chargePixelShader("Tutorial07-RenderScreem.fx", m_programSRenderScreen.m_PixelS.m_pPixelShader);


	m_programSActual = m_programSPixelBP;
	//Create vertex buffer
	chargeMesh();
	ReadPantalla();
	//ReadPantalla();
	//m_hand.mesh_cube();
	
	
	//DcreateVertexBuffer(m_mesh.m_numTries);
	DcreateVertexBuffer(m_meshes[0].m_numTries);
	//	DcreateVertexBuffer(3);
	//m_device.InitData.pSysMem = m_mesh.m_vertex;
	m_device.InitData.pSysMem = m_meshes[0].m_vertex;
	//m_device.InitData.pSysMem = verticestri;
	hr = m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pVertexBuffer);
	if (FAILED(hr))
		return hr;
	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_deviceContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &m_buffers.m_pVertexBuffer, &stride, &offset);

	// Create index buffer
	//DcreateIndexBuffer(m_mesh.m_numIndex);
	DcreateIndexBuffer(m_meshes[0].m_numIndex);
	//DcreateIndexBuffer(3);
	//m_device.InitData.pSysMem = m_mesh.m_index;
	m_device.InitData.pSysMem = m_meshes[0].m_index;
	// m_device.InitData.pSysMem = indicesTri;
	hr = m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pIndexBuffer);
	if (FAILED(hr))
		return hr;

	// Set index buffer
	m_deviceContext.m_pImmediateContext->IASetIndexBuffer(m_buffers.m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	// Set primitive topology
	m_deviceContext.m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//DcreateIndexBuffer(24);
	// Create the constant buffers//////////////////////
	DcreateConstantBuffer();


	//hr = m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, NULL, &m_buffers.m_pCBNeverChanges);
	//if (FAILED(hr))
	//	return hr;
	//hr = m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, NULL, &m_buffers.m_pCBChangeOnResize);
	//if (FAILED(hr))
	//	return hr;
	//hr = m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, NULL, &m_buffers.m_pCBChangesEveryFrame);
	//if (FAILED(hr))
	//	return hr;
	//hr = m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, NULL, &m_buffers.m_pCBBright);
	//if (FAILED(hr))
	//	return hr;
	
	// Load the Texture//////
	LPCWSTR TEX = L"miku.dds";
	hr = D3DX11CreateShaderResourceViewFromFile(m_device.m_pd3dDevice, TEX, NULL, NULL, &m_texture.m_pTextureRV, NULL);
	if (FAILED(hr))
		return hr;
	
	// Create the sample state//////////////////////////////////////////////////////////////////////////
	DcreateSampleState();
	hr = m_device.m_pd3dDevice->CreateSamplerState(&m_device.sampDesc, &m_texture.m_pSamplerLinear);
	if (FAILED(hr))
		return hr;
	dvice_init = true;
#else
return false;
#endif // !DX	
	
	initCamera();
	initCamera2();
}
void CManager::DswapChain(const HWND& hWnd)
{
#ifdef DX
	//DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&m_device.sd, sizeof(m_device.sd));
	m_device.sd.BufferCount = 1;
	m_device.sd.BufferDesc.Width = m_device.width;
	m_device.sd.BufferDesc.Height = m_device.height;
	m_device.sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//    sd.back().BufferDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	m_device.sd.BufferDesc.RefreshRate.Numerator = 60;
	m_device.sd.BufferDesc.RefreshRate.Denominator = 1;
	m_device.sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_device.sd.OutputWindow = hWnd;
	m_device.sd.SampleDesc.Count = 1;
	m_device.sd.SampleDesc.Quality = 0;
	m_device.sd.Windowed = TRUE;
#else
#endif // !DX
}
bool CManager::DcreateRenderTargetV(HRESULT &hr)
{
#ifdef DX
	hr = m_swapChain.m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_device.m_pBackBuffer);
	if (FAILED(hr))
		return false;

	hr = m_device.m_pd3dDevice->CreateRenderTargetView(m_device.m_pBackBuffer, NULL, &m_renderTargetV.m_pRenderTargetView);
//	hr = m_device.m_pd3dDevice->CreateRenderTargetView(m_renderTargetV2.m_pBackBuffer, NULL, &m_renderTargetV2.m_pRenderTargetView);
	m_renderTargetV.m_pBackBuffer = m_device.m_pBackBuffer;
	m_device.m_pBackBuffer->Release();
	if (FAILED(hr))
		return false;
#else
	return false;
#endif // !DX
}
void CManager::DcreateDepthStencilTex(HRESULT &hr)
{
#ifdef DX
	ZeroMemory(&m_device.descDepth, sizeof(m_device.descDepth));
	m_device.descDepth.Width = m_device.width;
	m_device.descDepth.Height =m_device.height;
	m_device.descDepth.MipLevels = 1;
	m_device.descDepth.ArraySize = 1;
	m_device.descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_device.descDepth.SampleDesc.Count = 1;
	m_device.descDepth.SampleDesc.Quality = 0;
	m_device.descDepth.Usage = D3D11_USAGE_DEFAULT;
	m_device.descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	m_device.descDepth.CPUAccessFlags = 0;
	m_device.descDepth.MiscFlags = 0;

	hr = m_device.m_pd3dDevice->CreateTexture2D(&m_device.descDepth, NULL, &m_texture.m_pDepthStencil);
#else
#endif // !DX
	
}
void CManager::DcreateDepthStencilV(HRESULT &hr)
{
#ifdef DX
	ZeroMemory(&m_device.descDSV, sizeof(m_device.descDSV));
	m_device.descDSV.Format = m_device.descDepth.Format;
	m_device.descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	m_device.descDSV.Texture2D.MipSlice = 0;
	hr = m_device.m_pd3dDevice->CreateDepthStencilView(m_texture.m_pDepthStencil, &m_device.descDSV, &m_renderTargetV.m_pDepthStencilView);
	hr = m_device.m_pd3dDevice->CreateDepthStencilView(m_texture.m_pDepthStencil, &m_device.descDSV, &m_lightPass.m_pDepthStencilView);
	hr = m_device.m_pd3dDevice->CreateDepthStencilView(m_texture.m_pDepthStencil, &m_device.descDSV, &m_LuminancePass.m_pDepthStencilView);
	hr = m_device.m_pd3dDevice->CreateDepthStencilView(m_texture.m_pDepthStencil, &m_device.descDSV, &m_BrightPass.m_pDepthStencilView);
	hr = m_device.m_pd3dDevice->CreateDepthStencilView(m_texture.m_pDepthStencil, &m_device.descDSV, &m_BlurHPass.m_pDepthStencilView);
	hr = m_device.m_pd3dDevice->CreateDepthStencilView(m_texture.m_pDepthStencil, &m_device.descDSV, &m_BlurVPass.m_pDepthStencilView);
	hr = m_device.m_pd3dDevice->CreateDepthStencilView(m_texture.m_pDepthStencil, &m_device.descDSV, &m_AddBrightPass.m_pDepthStencilView);
	hr = m_device.m_pd3dDevice->CreateDepthStencilView(m_texture.m_pDepthStencil, &m_device.descDSV, &m_ToneMapPass.m_pDepthStencilView);
#else
#endif // !DX

}
void CManager::DsetupViewport()
{
#ifdef DX
	//m_device.vp.Width = (FLOAT)m_device.width*.5f;
	m_device.vp.Width = (FLOAT)m_device.width;

	//m_devi._Mylastce.vp.Height = (FLOAT)m_device.height*.5f;
	m_device.vp.Height = (FLOAT)m_device.height;
	m_device.vp.MinDepth = 0.0f;
	m_device.vp.MaxDepth = 1.0f;
	m_device.vp.TopLeftX = 0.0f;
	m_device.vp.TopLeftY = 0.0f;
#else
#endif // !DX
}
void CManager::DcreateVertexBuffer(int numvertex)
{
#ifdef DX
	ZeroMemory(&m_device.bd, sizeof(m_device.bd));
	m_device.bd.Usage = D3D11_USAGE_DEFAULT;
//	m_device.bd.ByteWidth = sizeof(CDevice::SimpleVertex) * 12;
	m_device.bd.ByteWidth = sizeof(CDevice::SimpleVertex) * numvertex;
	m_device.bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_device.bd.CPUAccessFlags = 0;

	ZeroMemory(&m_device.InitData, sizeof(m_device.InitData));
	//m_device.InitData.pSysMem = vertices;
#else
#endif // !DX
}
void CManager::DcreateIndexBuffer(int numindices)
{
#ifdef DX
	m_device.bd.Usage = D3D11_USAGE_DEFAULT;
//	m_device.bd.ByteWidth = sizeof(WORD) * 6;
	m_device.bd.ByteWidth = sizeof(WORD) * numindices;
	m_device.bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_device.bd.CPUAccessFlags = 0;
#else
#endif // !DX
}
void CManager::DcreateConstantBuffer()
{
#ifdef DX
	HRESULT hr;
	m_device.bd.Usage = D3D11_USAGE_DEFAULT;
	m_device.bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_device.bd.CPUAccessFlags = 0;
	
	m_device.bd.ByteWidth = sizeof(CDevice::CBNeverChanges);
	hr = m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, NULL, &m_buffers.m_pCBNeverChanges);
	if (FAILED(hr))
		return;
	m_device.bd.ByteWidth = sizeof(CDevice::CBChangeOnResize);
	hr = m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, NULL, &m_buffers.m_pCBChangeOnResize);
	if (FAILED(hr))
		return ;
	m_device.bd.ByteWidth = sizeof(CDevice::CBChangesEveryFrame);
	hr = m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, NULL, &m_buffers.m_pCBChangesEveryFrame);
	if (FAILED(hr))
		return;
	m_device.bd.ByteWidth = sizeof(CDevice::CBBright);
	hr = m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, NULL, &m_buffers.m_pCBBright);
	if (FAILED(hr))
		return ;
	m_device.bd.ByteWidth = sizeof(CDevice::CBBlur);
	hr = m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, NULL, &m_buffers.m_pCBlurH);
	if (FAILED(hr))
		return;
	m_device.bd.ByteWidth = sizeof(CDevice::CBAddBright);
	hr = m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, NULL, &m_buffers.m_pCBAddBirght);
	if (FAILED(hr))
		return;
	m_device.bd.ByteWidth = sizeof(CDevice::CBToneMap);
	hr = m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, NULL, &m_buffers.m_pCToneMap);
	if (FAILED(hr))
		return;
#else
#endif // !DX
}
void CManager::DcreateSampleState()
{
#ifdef DX
	ZeroMemory(&m_device.sampDesc, sizeof(m_device.sampDesc));
	m_device.sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	m_device.sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	m_device.sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	m_device.sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_device.sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	m_device.sampDesc.MinLOD = 0;
	m_device.sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	m_device.sampDesc.MipLODBias = 0;
#else
#endif // !DX
}
void CManager::DResize(float width, float height)
{
#ifdef DX
	if (dvice_init)
	{
		m_deviceContext.m_pImmediateContext->VSSetShader(m_programSActual.m_VertexS.m_pVertexShader, NULL, 0);
		m_deviceContext.m_pImmediateContext->PSSetShader(m_programSActual.m_PixelS.m_pPixelShader, NULL, 0);

		m_Width = width;
		m_Height = height;
		HRESULT hr;
		if (m_texture.m_pDepthStencil)
			m_texture.m_pDepthStencil->Release();
		if (m_renderTargetV.m_pRenderTargetView)
			m_renderTargetV.m_pRenderTargetView->Release();
		if (m_renderTargetV.m_pDepthStencilView)
			m_renderTargetV.m_pDepthStencilView->Release();
		if (m_lightPass.m_pRenderTargetView)
			m_lightPass.m_pRenderTargetView->Release();
		if (m_lightPass.m_pDepthStencilView)
			m_lightPass.m_pDepthStencilView->Release();
		if (m_LuminancePass.m_pRenderTargetView) 
			m_LuminancePass.m_pRenderTargetView->Release();
		if (m_LuminancePass.m_pDepthStencilView)
			m_LuminancePass.m_pDepthStencilView->Release();
		if (m_BrightPass.m_pDepthStencilView) m_BrightPass.m_pDepthStencilView->Release();
		if (m_BrightPass.m_pRenderTargetView) m_BrightPass.m_pRenderTargetView->Release();
		if (m_BlurHPass.m_pDepthStencilView) m_BlurHPass.m_pDepthStencilView->Release();
		if (m_BlurHPass.m_pRenderTargetView) m_BlurHPass.m_pRenderTargetView->Release();
		if (m_BlurVPass.m_pDepthStencilView) m_BlurVPass.m_pDepthStencilView->Release();
		if (m_BlurVPass.m_pRenderTargetView) m_BlurVPass.m_pRenderTargetView->Release();
		if (m_AddBrightPass.m_pDepthStencilView) m_AddBrightPass.m_pDepthStencilView->Release();
		if (m_AddBrightPass.m_pRenderTargetView) m_AddBrightPass.m_pRenderTargetView->Release();

		m_device.descDepth.Width = width;
		m_device.descDepth.Height = height;
		m_device.sd.BufferDesc.Width = width;
		m_device.sd.BufferDesc.Height = height;
		m_swapChain.m_pSwapChain->ResizeBuffers(0, (UINT)width, (UINT)height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);


		hr = m_swapChain.m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_device.m_pBackBuffer);

		hr = m_device.m_pd3dDevice->CreateRenderTargetView(m_device.m_pBackBuffer, NULL, &m_renderTargetV.m_pRenderTargetView);
		m_renderTargetV.m_pBackBuffer = m_device.m_pBackBuffer;
		m_device.m_pBackBuffer->Release();
		//generateRenderTexture();
		generateRenderTextureWhitReferences(m_lightPass.m_pRenderTargetView, m_lightPass.m_pDepthStencilView, renderTextureLightPass, SResourceLightPass);
		generateRenderTextureWhitReferences(m_LuminancePass.m_pRenderTargetView, m_LuminancePass.m_pDepthStencilView, renderTextureLuminancePass, SResourceLuminancePass);
		generateRenderTextureWhitReferences(m_BrightPass.m_pRenderTargetView, m_BrightPass.m_pDepthStencilView, renderTextureBrightPass, SResourceBrightPass);
		generateRenderTextureWhitReferences(m_BlurHPass.m_pRenderTargetView, m_BlurHPass.m_pDepthStencilView, renderTextureBlurHPass, SResourceBlurHPass);
		generateRenderTextureWhitReferences(m_BlurVPass.m_pRenderTargetView, m_BlurVPass.m_pDepthStencilView, renderTextureBlurVPass, SResourceBlurVPass);
		generateRenderTextureWhitReferences(m_AddBrightPass.m_pRenderTargetView, m_AddBrightPass.m_pDepthStencilView, renderTextureAddBrightPass, SResourceAddBrightPass);
		generateRenderTextureWhitReferences(m_ToneMapPass.m_pRenderTargetView, m_ToneMapPass.m_pDepthStencilView, renderTextureToneMapPass, SResourceToneMapPass);
		if (passActual[0] == 1)
			SResourceActual = SResourceLightPass;
		else if (passActual[1] == 1)
			SResourceActual = SResourceLuminancePass;
		else if (passActual[2] == 1)
			SResourceActual = SResourceBrightPass;
		else if (passActual[3] == 1)
			SResourceActual = SResourceBlurHPass;
		else if (passActual[4] == 1)
			SResourceActual = SResourceBlurVPass;
		else if (passActual[5] == 1)
			SResourceActual = SResourceAddBrightPass;
		else if (passActual[6] == 1)
			SResourceActual = SResourceToneMapPass;


		ZeroMemory(&m_device.descDepth, sizeof(m_device.descDepth));
		m_device.descDepth.Width = width;
		m_device.descDepth.Height = height;
		m_device.descDepth.MipLevels = 1;
		m_device.descDepth.ArraySize = 1;
		m_device.descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		m_device.descDepth.SampleDesc.Count = 1;
		m_device.descDepth.SampleDesc.Quality = 0;
		m_device.descDepth.Usage = D3D11_USAGE_DEFAULT;
		m_device.descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		m_device.descDepth.CPUAccessFlags = 0;
		m_device.descDepth.MiscFlags = 0;

		hr = m_device.m_pd3dDevice->CreateTexture2D(&m_device.descDepth, NULL, &m_texture.m_pDepthStencil);


		ZeroMemory(&m_device.descDSV, sizeof(m_device.descDSV));
		m_device.descDSV.Format = m_device.descDepth.Format;
		m_device.descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		m_device.descDSV.Texture2D.MipSlice = 0;
		hr = m_device.m_pd3dDevice->CreateDepthStencilView(m_texture.m_pDepthStencil, &m_device.descDSV, &m_renderTargetV.m_pDepthStencilView);
		hr = m_device.m_pd3dDevice->CreateDepthStencilView(m_texture.m_pDepthStencil, &m_device.descDSV, &m_lightPass.m_pDepthStencilView);
		hr = m_device.m_pd3dDevice->CreateDepthStencilView(m_texture.m_pDepthStencil, &m_device.descDSV, &m_LuminancePass.m_pDepthStencilView);
		hr = m_device.m_pd3dDevice->CreateDepthStencilView(m_texture.m_pDepthStencil, &m_device.descDSV, &m_BrightPass.m_pDepthStencilView);
		hr = m_device.m_pd3dDevice->CreateDepthStencilView(m_texture.m_pDepthStencil, &m_device.descDSV, &m_BlurHPass.m_pDepthStencilView);
		hr = m_device.m_pd3dDevice->CreateDepthStencilView(m_texture.m_pDepthStencil, &m_device.descDSV, &m_BlurVPass.m_pDepthStencilView);
		hr = m_device.m_pd3dDevice->CreateDepthStencilView(m_texture.m_pDepthStencil, &m_device.descDSV, &m_AddBrightPass.m_pDepthStencilView);
		hr = m_device.m_pd3dDevice->CreateDepthStencilView(m_texture.m_pDepthStencil, &m_device.descDSV, &m_ToneMapPass.m_pDepthStencilView);


		
		//m_device.vp.Width = (FLOAT)m_device.width*.5f;
		m_device.vp.Width = (FLOAT)width;

		//m_devi._Mylastce.vp.Height = (FLOAT)m_device.height*.5f;
		m_device.vp.Height = (FLOAT)height;
		m_device.vp.MinDepth = 0.0f;
		m_device.vp.MaxDepth = 1.0f;
		m_device.vp.TopLeftX = 0.0f;
		m_device.vp.TopLeftY = 0.0f;

		m_deviceContext.m_pImmediateContext->RSSetViewports(1, &m_device.vp);

	}
#elif OPENGL

	createRenderTaregetLightPass();
	createRenderTaregetLuminancePass();
	createRenderTaregetBrightPass();
	createRenderTaregetBlur_HPass();
	createRenderTaregetBlur_VPass();
	createRenderTaregetAddBrightPass();
	createRenderTaregetTonePass();
	if (passActual[0]==1)
		renderedTextureToScreen = renderedTextureLight;
	else if(passActual[1] == 1)
		renderedTextureToScreen = renderedTextureLuminance;
	else if(passActual[2] == 1)
		renderedTextureToScreen = renderedTextureBright;
	else if(passActual[3] == 1)
		renderedTextureToScreen = renderedTextureBlur_H;
	else if(passActual[4] == 1)
		renderedTextureToScreen = renderedTextureBlur_V;
	else if(passActual[5] == 1)
		renderedTextureToScreen = renderedTextureAddBright;
	else if(passActual[6] == 1)
		renderedTextureToScreen = renderedTextureToneMap;
#endif // !DX
}
/////////////////////////////////////////
//Shaders DX
/////////////////////////////////////////
#ifdef DX
void CManager::chargeVertexShadervVertexBLINNPHONG(const char * file_path, ID3D11VertexShader * &pVS)
{
	ID3D10Blob * pErroMsg = nullptr;
	if (m_device.pVSBlob != nullptr)
	{
		m_device.pVSBlob->Release();

	}
	// Create the shaders
// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	ifstream VertexShaderStream(file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream ss;
		ss << VertexShaderStream.rdbuf();
		VertexShaderCode = ss.str();
		char *token = NULL;
		char *nextToken = NULL;
		//token = strtok_s((char *)VertexShaderCode.c_str(), "?", &nextToken);
		//string addDef = token + '\n';
		token = strtok_s((char *)VertexShaderCode.c_str(), "¿", &nextToken);
		string addDef = token;
		addDef += "\n";
		//token = (char *)texName.c_str();
		//token +=(char *) pngextencion.c_str();

		addDef += "\n#define VER_LIGTH\n";

#ifdef BLINN
		addDef += "\n#define BLINN\n";
#endif // DEBUG
#ifdef DIR_LIGHT
		addDef += "\n#define DIR_LIGHT\n";
#endif // DEBUG
#ifdef CONE_LIGHT
		addDef += "\n#define CONE_LIGHT\n";
#endif // DEBUG
#ifdef POINT_LIGHT
		addDef += "\n#define POINT_LIGHT\n";
#endif // DEBUG

		addDef += nextToken;
		VertexShaderCode = addDef;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", file_path);
		getchar();
		return;
	}
	auto vertexShader = VertexShaderCode;

	HRESULT hr = D3DCompile(
		vertexShader.c_str(),
		vertexShader.length(),
		nullptr,
		nullptr,
		nullptr,
		"VS", "vs_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0,
		&m_device.pVSBlob,
		&pErroMsg);
	//compilefrom
	//reader
	if (pErroMsg)
	{
		SIZE_T msgSize = pErroMsg->GetBufferSize();
		string msg;
		msg.resize(msgSize);
		memcpy(&msg[0], pErroMsg->GetBufferPointer(), msgSize);

		pErroMsg->Release();
	}
	if (FAILED(hr))
	{
		cout << "can't compile pixel shader\n";
	}
	m_device.m_pd3dDevice->CreateVertexShader(m_device.pVSBlob->GetBufferPointer(), m_device.pVSBlob->GetBufferSize(), NULL, &pVS);


	return;
}
void CManager::chargePixelShadervVertexBLINNPHONG(const char * file_path, ID3D11PixelShader *& pPS)
{
	ID3D10Blob * pErroMsg = nullptr;
	if (m_device.pVSBlob != nullptr)
	{
		m_device.pVSBlob->Release();

	}
	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		char *token = NULL;
		char *nextToken = NULL;
		token = strtok_s((char *)FragmentShaderCode.c_str(), "¿", &nextToken);
		//token = (char *)texName.c_str();
		//token +=(char *) pngextencion.c_str();
		string addDef = token;
		addDef += '\n';

		addDef += "\n#define VER_LIGTH\n";

#ifdef BLINN
		addDef += "\n#define BLINN\n";
#endif // DEBUG
#ifdef DIR_LIGHT
		addDef += "\n#define DIR_LIGHT\n";
#endif // DEBUG
#ifdef CONE_LIGHT
		addDef += "\n#define CONE_LIGHT\n";
#endif // DEBUG
#ifdef POINT_LIGHT
		addDef += "\n#define POINT_LIGHT\n";
#endif // DEBUG
		addDef += nextToken;
		FragmentShaderCode = addDef;
		FragmentShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", file_path);
		getchar();
		return;
	}
	auto FrameShader = FragmentShaderCode;
	//Microsoft::WRL::ComPtr<ID3DBlob> blob;
	HRESULT hr = D3DCompile(
		FragmentShaderCode.c_str(),
		FragmentShaderCode.length(),
		nullptr,
		nullptr,
		nullptr,
		"PS", "ps_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0,
		&m_device.pVSBlob,
		&pErroMsg);
	if (pErroMsg)
	{
		SIZE_T msgSize = pErroMsg->GetBufferSize();
		string msg;
		msg.resize(msgSize);
		memcpy(&msg[0], pErroMsg->GetBufferPointer(), msgSize);

		pErroMsg->Release();
	}
	if (!hr)
	{
		cout << "can't compile pixel shader\n";
	}
	m_device.m_pd3dDevice->CreatePixelShader(m_device.pVSBlob->GetBufferPointer(), m_device.pVSBlob->GetBufferSize(), NULL, &pPS);
	return;
}

void CManager::chargeVertexShadervVertexBLINN(const char * file_path, ID3D11VertexShader * &pVS)
{
	ID3D10Blob * pErroMsg = nullptr;
	if (m_device.pVSBlob != nullptr)
	{
		m_device.pVSBlob->Release();

	}
	// Create the shaders
// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	ifstream VertexShaderStream(file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream ss;
		ss << VertexShaderStream.rdbuf();
		VertexShaderCode = ss.str();
		char *token = NULL;
		char *nextToken = NULL;
		//token = strtok_s((char *)VertexShaderCode.c_str(), "?", &nextToken);
		//string addDef = token + '\n';
		token = strtok_s((char *)VertexShaderCode.c_str(), "¿", &nextToken);
		string addDef = token;
		addDef += "\n";
		//token = (char *)texName.c_str();
		//token +=(char *) pngextencion.c_str();

		addDef += "\n#define VER_LIGTH\n";

		addDef += "\n#define BLINN\n";

#ifdef DIR_LIGHT
		addDef += "\n#define DIR_LIGHT\n";
#endif // DEBUG
#ifdef CONE_LIGHT
		addDef += "\n#define CONE_LIGHT\n";
#endif // DEBUG
#ifdef POINT_LIGHT
		addDef += "\n#define POINT_LIGHT\n";
#endif // DEBUG

		addDef += nextToken;
		VertexShaderCode = addDef;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", file_path);
		getchar();
		return;
	}
	auto vertexShader = VertexShaderCode;

	HRESULT hr = D3DCompile(
		vertexShader.c_str(),
		vertexShader.length(),
		nullptr,
		nullptr,
		nullptr,
		"VS", "vs_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0,
		&m_device.pVSBlob,
		&pErroMsg);
	//compilefrom
	//reader
	if (pErroMsg)
	{
		SIZE_T msgSize = pErroMsg->GetBufferSize();
		string msg;
		msg.resize(msgSize);
		memcpy(&msg[0], pErroMsg->GetBufferPointer(), msgSize);

		pErroMsg->Release();
	}
	if (FAILED(hr))
	{
		cout << "can't compile pixel shader\n";
	}
	m_device.m_pd3dDevice->CreateVertexShader(m_device.pVSBlob->GetBufferPointer(), m_device.pVSBlob->GetBufferSize(), NULL, &pVS);

	return;
}
void CManager::chargePixelShadervVertexBLINN(const char * file_path, ID3D11PixelShader * &pPS)
{
	ID3D10Blob * pErroMsg = nullptr;
	if (m_device.pVSBlob != nullptr)
	{
		m_device.pVSBlob->Release();

	}
	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		char *token = NULL;
		char *nextToken = NULL;
		token = strtok_s((char *)FragmentShaderCode.c_str(), "¿", &nextToken);
		//token = (char *)texName.c_str();
		//token +=(char *) pngextencion.c_str();
		string addDef = token;
		addDef += '\n';

		addDef += "\n#define VER_LIGTH\n";

		addDef += "\n#define BLINN\n";

#ifdef DIR_LIGHT
		addDef += "\n#define DIR_LIGHT\n";
#endif // DEBUG
#ifdef CONE_LIGHT
		addDef += "\n#define CONE_LIGHT\n";
#endif // DEBUG
#ifdef POINT_LIGHT
		addDef += "\n#define POINT_LIGHT\n";
#endif // DEBUG
		addDef += nextToken;
		FragmentShaderCode = addDef;
		FragmentShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", file_path);
		getchar();
		return;
	}
	auto FrameShader = FragmentShaderCode;
	//Microsoft::WRL::ComPtr<ID3DBlob> blob;
	HRESULT hr = D3DCompile(
		FragmentShaderCode.c_str(),
		FragmentShaderCode.length(),
		nullptr,
		nullptr,
		nullptr,
		"PS", "ps_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0,
		&m_device.pVSBlob,
		&pErroMsg);
	if (pErroMsg)
	{
		SIZE_T msgSize = pErroMsg->GetBufferSize();
		string msg;
		msg.resize(msgSize);
		memcpy(&msg[0], pErroMsg->GetBufferPointer(), msgSize);

		pErroMsg->Release();
	}
	if (!hr)
	{
		cout << "can't compile pixel shader\n";
	}
	m_device.m_pd3dDevice->CreatePixelShader(m_device.pVSBlob->GetBufferPointer(), m_device.pVSBlob->GetBufferSize(), NULL, &pPS);
	return;
}

void CManager::chargeVertexShadervPixelBLINNPHONG(const char * file_path, ID3D11VertexShader * &pVS)
{
	ID3D10Blob * pErroMsg = nullptr;
	if (m_device.pVSBlob != nullptr)
	{
		//m_device.pVSBlob->Release();

	}
	// Create the shaders
// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	ifstream VertexShaderStream(file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream ss;
		ss << VertexShaderStream.rdbuf();
		VertexShaderCode = ss.str();
		char *token = NULL;
		char *nextToken = NULL;
		//token = strtok_s((char *)VertexShaderCode.c_str(), "?", &nextToken);
		//string addDef = token + '\n';
		token = strtok_s((char *)VertexShaderCode.c_str(), "¿", &nextToken);
		string addDef = token;
		addDef += "\n";
		//token = (char *)texName.c_str();
		//token +=(char *) pngextencion.c_str();
#ifdef VER_LIGTH
		addDef += "\n#define VER_LIGTH\n";
#endif // DEBUG
#ifdef BLINN
		addDef += "\n#define BLINN\n";
#endif // DEBUG
#ifdef DIR_LIGHT
		addDef += "\n#define DIR_LIGHT\n";
#endif // DEBUG
#ifdef CONE_LIGHT
		addDef += "\n#define CONE_LIGHT\n";
#endif // DEBUG
#ifdef POINT_LIGHT
		addDef += "\n#define POINT_LIGHT\n";
#endif // DEBUG

		addDef += nextToken;
		VertexShaderCode = addDef;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", file_path);
		getchar();
		return;
	}
	auto vertexShader = VertexShaderCode;

	HRESULT hr = D3DCompile(
		vertexShader.c_str(),
		vertexShader.length(),
		nullptr,
		nullptr,
		nullptr,
		"VS", "vs_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0,
		&m_device.pVSBlob,
		&pErroMsg);
	//compilefrom
	//reader
	if (pErroMsg)
	{
		SIZE_T msgSize = pErroMsg->GetBufferSize();
		string msg;
		msg.resize(msgSize);
		memcpy(&msg[0], pErroMsg->GetBufferPointer(), msgSize);

		pErroMsg->Release();
	}
	if (FAILED(hr))
	{
		cout << "can't compile pixel shader\n";
	}
	m_device.m_pd3dDevice->CreateVertexShader(m_device.pVSBlob->GetBufferPointer(), m_device.pVSBlob->GetBufferSize(), NULL, &pVS);


	return;
}
void CManager::chargePixelShadervPixelBLINNPHONG(const char * file_path, ID3D11PixelShader * &pPS)
{
	ID3D10Blob * pErroMsg = nullptr;
	if (m_device.pVSBlob != nullptr)
	{
		m_device.pVSBlob->Release();

	}
	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		char *token = NULL;
		char *nextToken = NULL;
		token = strtok_s((char *)FragmentShaderCode.c_str(), "¿", &nextToken);
		//token = (char *)texName.c_str();
		//token +=(char *) pngextencion.c_str();
		string addDef = token;
		addDef += '\n';
#ifdef VER_LIGTH
		addDef += "\n#define VER_LIGTH\n";
#endif // DEBUG
#ifdef BLINN
		addDef += "\n#define BLINN\n";
#endif // DEBUG
#ifdef DIR_LIGHT
		addDef += "\n#define DIR_LIGHT\n";
#endif // DEBUG
#ifdef CONE_LIGHT
		addDef += "\n#define CONE_LIGHT\n";
#endif // DEBUG
#ifdef POINT_LIGHT
		addDef += "\n#define POINT_LIGHT\n";
#endif // DEBUG
		addDef += nextToken;
		FragmentShaderCode = addDef;
		FragmentShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", file_path);
		getchar();
		return;
	}
	auto FrameShader = FragmentShaderCode;
	//Microsoft::WRL::ComPtr<ID3DBlob> blob;
	HRESULT hr = D3DCompile(
		FragmentShaderCode.c_str(),
		FragmentShaderCode.length(),
		nullptr,
		nullptr,
		nullptr,
		"PS", "ps_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0,
		&m_device.pVSBlob,
		&pErroMsg);
	if (pErroMsg)
	{
		SIZE_T msgSize = pErroMsg->GetBufferSize();
		string msg;
		msg.resize(msgSize);
		memcpy(&msg[0], pErroMsg->GetBufferPointer(), msgSize);

		pErroMsg->Release();
	}
	if (!hr)
	{
		cout << "can't compile pixel shader\n";
	}
	m_device.m_pd3dDevice->CreatePixelShader(m_device.pVSBlob->GetBufferPointer(), m_device.pVSBlob->GetBufferSize(), NULL, &pPS);
	return;
}

void CManager::chargeVertexShadervPixelBLINN(const char * file_path, ID3D11VertexShader * &pVS)
{
	ID3D10Blob * pErroMsg = nullptr;
	if (m_device.pVSBlob != nullptr)
	{
		m_device.pVSBlob->Release();

	}
	// Create the shaders
// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	ifstream VertexShaderStream(file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream ss;
		ss << VertexShaderStream.rdbuf();
		VertexShaderCode = ss.str();
		char *token = NULL;
		char *nextToken = NULL;
		//token = strtok_s((char *)VertexShaderCode.c_str(), "?", &nextToken);
		//string addDef = token + '\n';
		token = strtok_s((char *)VertexShaderCode.c_str(), "¿", &nextToken);
		string addDef = token;
		addDef += "\n";
		//token = (char *)texName.c_str();
		//token +=(char *) pngextencion.c_str();
#ifdef VER_LIGTH
		addDef += "\n#define VER_LIGTH\n";
#endif // DEBUG

		addDef += "\n#define BLINN\n";

#ifdef DIR_LIGHT
		addDef += "\n#define DIR_LIGHT\n";
#endif // DEBUG
#ifdef CONE_LIGHT
		addDef += "\n#define CONE_LIGHT\n";
#endif // DEBUG
#ifdef POINT_LIGHT
		addDef += "\n#define POINT_LIGHT\n";
#endif // DEBUG

		addDef += nextToken;
		VertexShaderCode = addDef;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", file_path);
		getchar();
		return;
	}
	auto vertexShader = VertexShaderCode;

	HRESULT hr = D3DCompile(
		vertexShader.c_str(),
		vertexShader.length(),
		nullptr,
		nullptr,
		nullptr,
		"VS", "vs_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0,
		&m_device.pVSBlob,
		&pErroMsg);
	//compilefrom
	//reader
	if (pErroMsg)
	{
		SIZE_T msgSize = pErroMsg->GetBufferSize();
		string msg;
		msg.resize(msgSize);
		memcpy(&msg[0], pErroMsg->GetBufferPointer(), msgSize);

		pErroMsg->Release();
	}
	if (FAILED(hr))
	{
		cout << "can't compile pixel shader\n";
	}
	m_device.m_pd3dDevice->CreateVertexShader(m_device.pVSBlob->GetBufferPointer(), m_device.pVSBlob->GetBufferSize(), NULL, &pVS);


	return;
}
void CManager::chargePixelShadervPixelBLINN(const char * file_path, ID3D11PixelShader * &pPS)
{
	ID3D10Blob * pErroMsg = nullptr;
	if (m_device.pVSBlob != nullptr)
	{
		m_device.pVSBlob->Release();

	}
	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		char *token = NULL;
		char *nextToken = NULL;
		token = strtok_s((char *)FragmentShaderCode.c_str(), "¿", &nextToken);
		//token = (char *)texName.c_str();
		//token +=(char *) pngextencion.c_str();
		string addDef = token;
		addDef += '\n';
#ifdef VER_LIGTH
		addDef += "\n#define VER_LIGTH\n";
#endif // DEBUG
		addDef += "\n#define BLINN\n";

#ifdef DIR_LIGHT
		addDef += "\n#define DIR_LIGHT\n";
#endif // DEBUG
#ifdef CONE_LIGHT
		addDef += "\n#define CONE_LIGHT\n";
#endif // DEBUG
#ifdef POINT_LIGHT
		addDef += "\n#define POINT_LIGHT\n";
#endif // DEBUG
		addDef += nextToken;
		FragmentShaderCode = addDef;
		FragmentShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", file_path);
		getchar();
		return;
	}
	auto FrameShader = FragmentShaderCode;
	//Microsoft::WRL::ComPtr<ID3DBlob> blob;
	HRESULT hr = D3DCompile(
		FragmentShaderCode.c_str(),
		FragmentShaderCode.length(),
		nullptr,
		nullptr,
		nullptr,
		"PS", "ps_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0,
		&m_device.pVSBlob,
		&pErroMsg);
	if (pErroMsg)
	{
		SIZE_T msgSize = pErroMsg->GetBufferSize();
		string msg;
		msg.resize(msgSize);
		memcpy(&msg[0], pErroMsg->GetBufferPointer(), msgSize);

		pErroMsg->Release();
	}
	if (!hr)
	{
		cout << "can't compile pixel shader\n";
	}
	m_device.m_pd3dDevice->CreatePixelShader(m_device.pVSBlob->GetBufferPointer(), m_device.pVSBlob->GetBufferSize(), NULL, &pPS);
	return;
}

void CManager::chargeVertexShader(const char * file_path, ID3D11VertexShader * &pVS)
{
	ID3D10Blob * pErroMsg = nullptr;
	if (m_device.pVSBlob != nullptr)
	{
		m_device.pVSBlob->Release();

	}
	// Create the shaders
// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	ifstream VertexShaderStream(file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream ss;
		ss << VertexShaderStream.rdbuf();
		VertexShaderCode = ss.str();
		cout << VertexShaderCode << "\n";
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", file_path);
		getchar();
		return;
	}
	auto vertexShader = VertexShaderCode;

	HRESULT hr = D3DCompile(
		vertexShader.c_str(),
		vertexShader.length(),
		nullptr,
		nullptr,
		nullptr,
		"VS", "vs_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0,
		&m_device.pVSBlob,
		&pErroMsg);
	if (pErroMsg)
	{
		SIZE_T msgSize = pErroMsg->GetBufferSize();
		string msg;
		msg.resize(msgSize);
		memcpy(&msg[0], pErroMsg->GetBufferPointer(), msgSize);

		pErroMsg->Release();
	}

	//compilefrom
	//reader
	if (FAILED(hr))
	{
		cout << "can't compile pixel shader\n";
	}
	m_device.m_pd3dDevice->CreateVertexShader(m_device.pVSBlob->GetBufferPointer(), m_device.pVSBlob->GetBufferSize(), NULL, &pVS);


	return;
}
void CManager::chargePixelShader(const char * file_path, ID3D11PixelShader * &pPS)
{
	ID3D10Blob * pErroMsg = nullptr;
	if (m_device.pVSBlob != nullptr)
	{
		m_device.pVSBlob->Release();

	}
	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		cout << FragmentShaderCode << "\n";
		FragmentShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", file_path);
		getchar();
		return;
	}
	auto FrameShader = FragmentShaderCode;
	//Microsoft::WRL::ComPtr<ID3DBlob> blob;
	HRESULT hr = D3DCompile(
		FragmentShaderCode.c_str(),
		FragmentShaderCode.length(),
		nullptr,
		nullptr,
		nullptr,
		"PS", "ps_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0,
		&m_device.pVSBlob,
		&pErroMsg);
	if (pErroMsg)
	{
		SIZE_T msgSize = pErroMsg->GetBufferSize();
		string msg;
		msg.resize(msgSize);
		memcpy(&msg[0], pErroMsg->GetBufferPointer(), msgSize);

		pErroMsg->Release();
	}
	if (!hr)
	{
		cout << "can't compile pixel shader\n";
	}
	m_device.m_pd3dDevice->CreatePixelShader(m_device.pVSBlob->GetBufferPointer(), m_device.pVSBlob->GetBufferSize(), NULL, &pPS);
	return;
}
#endif
/////////////////////////////////////////
//Texture
/////////////////////////////////////////
#ifdef DX
void CManager::textureChange(ID3D11ShaderResourceView* &pTextureRV)
{
	m_numTexture++;
	if (m_numTexture > 3)
		m_numTexture = 0;

	switch (m_numTexture)
	{
	case 0:
		D3DX11CreateShaderResourceViewFromFile(m_device.m_pd3dDevice, L"seafloor.dds", NULL, NULL, &pTextureRV, NULL);
		break;
	case 1:
		D3DX11CreateShaderResourceViewFromFile(m_device.m_pd3dDevice, L"buho.dds", NULL, NULL, &pTextureRV, NULL);
		break;
	case 2:
		D3DX11CreateShaderResourceViewFromFile(m_device.m_pd3dDevice, L"textura4.dds", NULL, NULL, &pTextureRV, NULL);
		break;
	case 3:
		D3DX11CreateShaderResourceViewFromFile(m_device.m_pd3dDevice, L"miku.dds", NULL, NULL, &pTextureRV, NULL);
		break;
	}
}

void CManager::generateRenderTexture()
{
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	///////////////////////// Map's Texture
	// Initialize the  texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the texture description.
	// We will have our map be a square
	// We will need to have this texture bound as a render target AND a shader resource
	textureDesc.Width = m_Width;
	textureDesc.Height = m_Height;
	textureDesc.MipLevels = 10;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the texture
	m_device.m_pd3dDevice->CreateTexture2D(&textureDesc, NULL, &renderTextureLightPass);

	/////////////////////// Map's Render Target
	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	m_device.m_pd3dDevice->CreateRenderTargetView(renderTextureLightPass, &renderTargetViewDesc, &m_lightPass.m_pRenderTargetView);
	
	/////////////////////// Map's Shader Resource View
	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 10;

	// Create the shader resource view.
	m_device.m_pd3dDevice->CreateShaderResourceView(renderTextureLightPass, &shaderResourceViewDesc, &SResourceLightPass);
}
void CManager::generateRenderTextureWhitReferences(ID3D11RenderTargetView*&pRenderTargetView, ID3D11DepthStencilView* &pDepthStencilView, ID3D11Texture2D* &renderTexture, ID3D11ShaderResourceView* &ShaderResource)
{
	if (renderTexture)renderTexture->Release();
	if (ShaderResource)ShaderResource->Release();
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	///////////////////////// Map's Texture
	// Initialize the  texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the texture description.
	// We will have our map be a square
	// We will need to have this texture bound as a render target AND a shader resource
	textureDesc.Width = m_Width;
	textureDesc.Height = m_Height;
	textureDesc.MipLevels = 10;
	textureDesc.ArraySize = 1;
	//textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the texture
	m_device.m_pd3dDevice->CreateTexture2D(&textureDesc, NULL, &renderTexture);

	/////////////////////// Map's Render Target
	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	m_device.m_pd3dDevice->CreateRenderTargetView(renderTexture, &renderTargetViewDesc, &pRenderTargetView);

	/////////////////////// Map's Shader Resource View
	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 10;

	// Create the shader resource view.
	m_device.m_pd3dDevice->CreateShaderResourceView(renderTexture, &shaderResourceViewDesc, &ShaderResource);

}
#else
#endif // !DX

////////////////////////////////////////
//CAMERA
////////////////////////////////////////

void CManager::initCamera()
{
#ifdef DX
//	Sleep(10);
	//m_camera1.setEye(-9.03572083f, -0.626059771f, 7.06155205f,0.0f);
	//m_camera1.setAt(-7.03588009f, -0.364167482f, 2.44950199f,0.0f);
	//m_camera1.setUp(0.0242675878f, 0.998638034f, -0.0461842977f,0.0f);
	m_camera1.setEye(0, 2, -5, 0);
	m_camera1.setAt(0.f, 2.0f, 0.0f, 0);
	m_camera1.setUp(0.0f, 1.0f, 0.0f, 0);
	m_camera1.setView();
	m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBNeverChanges, 0, NULL, &m_camera1.m_cbNeverChanges, 0, 0);

	// Initialize the projection matrix
//El angulo de apertura, Ar, near y far
	m_camera1.createProjection(XM_PIDIV4, m_device.width, m_device.height, 0.01f, 100.0f);
	m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBChangeOnResize, 0, NULL, &m_camera1.m_cbChangesOnResize, 0, 0);
	m_camera1.m_originView = m_camera1.m_View;
	m_camera.push_back(m_camera1);
	
#elif OPENGL
	//set the elemets of the matriz view
	CCamera camera;
	//camera.setEye(0, 0, -6,0);
	//camera.setAt(0.f, 0.0f, 0.0f,0);
	//camera.setUp(0.0f, 1.0f, 0.0f,0);
	camera.setEye(0, 2, 5, 0);
	camera.setAt(0.f, 2.0f, 0.0f, 0);
	camera.setUp(0.0f, 1.0f, 0.0f, 0);
	camera.cameraSpeedRotation = .2;
	//with the elementes i create the matriz
	camera.setView();
	//with the size of the window, i create de mat projection 
	camera.setProjection(WindowSize.m_Width, WindowSize.m_Height);
	m_camera.push_back(camera);
	//numCamera++;
#endif // !DX
}

void CManager::initCamera2()
{
#ifdef DX
	//CCamera newcamera;
	//Sleep(10);
	//m_camera2.setEye(0.0f, 6.0f, 1.0f,0.0f);
	//m_camera2.setAt(0.0f, -2.0f, 0.0f,0.0f);
	//m_camera2.setUp(0.0f, 1.0f, 0.0f,0.0f);
	m_camera2.setEye(.5, 2, -8.5,0);
	m_camera2.setAt(0.5f, 2.0f, -2.5f,0);
	m_camera2.setUp(0.0f, 1.0f, 0.0f,0);
	m_camera2.setView();
	//m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBNeverChanges, 0, NULL, &m_camera2.m_cbNeverChanges, 0, 0);

	// Initialize the projection matrix
	m_camera2.createProjection(XM_PIDIV4, m_device.width, m_device.height, 0.01f, 100.0f);
	//m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBChangeOnResize, 0, NULL, &m_camera2.m_cbChangesOnResize, 0, 0);
	m_camera2.m_originView = m_camera2.m_View;
	m_camera.push_back(m_camera2);
#elif OPENGL
	//set the elemets of the matriz view
	CCamera camera;
	//camera.setEye(0, 6.0f, 1,0);
	//camera.setAt(0.f, -2.0f, 0.0f,0);
	//camera.setUp(0.0f, 1.0f, 0.0f,0);

	camera.setEye(64.5108490, 87.5670395, 166.076706, 0);
	camera.setAt(60.8535347, 86.4711990, 158.200012, 0);
	camera.setUp(-0.120152861, 0.989374995, -0.0818567798, 0);

	//with the elementes i create the matriz
	camera.setView();
	//with the size of the window, i create de mat projection 
	camera.setProjection(WindowSize.m_Width, WindowSize.m_Height);
	m_camera.push_back(camera);
	//numCamera++;
#endif // !DX
}

void CManager::resizeCamera(float width, float height)
{
#ifdef DX
	if (dvice_init)
	{
		m_deviceContext.m_pImmediateContext->VSSetShader(m_programSActual.m_VertexS.m_pVertexShader, NULL, 0);
		m_deviceContext.m_pImmediateContext->PSSetShader(m_programSActual.m_PixelS.m_pPixelShader, NULL, 0);
		
		for (int i = 0; i < m_camera.size(); i++)
		{
			m_camera[i].m_width = width;
			m_camera[i].m_height = height;

			m_camera[i].m_Projection = XMMatrixPerspectiveFovLH(m_camera[i].m_apertura, width / height, m_camera[i].m_nearr, m_camera[i].m_farr);
			m_camera[i].m_cbChangesOnResize.mProjection = XMMatrixTranspose(m_camera[i].m_Projection);

		}
		m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBChangeOnResize, 0, NULL, &m_camera[m_cameraNum].m_cbChangesOnResize, 0, 0);
		//m_swapChain.m_pSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	}
#elif OPENGL
	WindowSize.m_Width = width;
	WindowSize.m_Height = height;
	//resize the matriz projection
	for (int i = 0; i < m_camera.size(); i++)
	{
		m_camera[i].m_width = width;
		m_camera[i].m_height = height;
		m_camera[i].setProjection(WindowSize.m_Width, WindowSize.m_Height);
	}
	//resize the ViewPort
	glViewport(0, 0, WindowSize.m_Width, WindowSize.m_Height);

#endif // !DX
}

#ifdef DX
void CManager::moveCamera(WPARAM wParam)
{
	if (m_cameraNum!=0)
	{
		return;
	}
	switch (wParam)
	{
	case 'A':
		m_camera[m_cameraNum].m_View *= XMMatrixTranslation(m_camera[m_cameraNum].m_speed, 0.0f, 0.0f);
		//m_camera[m_cameraNum].m_Eye -= m_camera[m_cameraNum].m_cameraRight*m_camera[m_cameraNum].m_speed;
		//m_camera[m_cameraNum].m_At -= m_camera[m_cameraNum].m_cameraRight*m_camera[m_cameraNum].m_speed;
		break;
	case 'D':
		m_camera[m_cameraNum].m_View *= XMMatrixTranslation(-m_camera[m_cameraNum].m_speed, 0.0f, 0.0f);
		//m_camera[m_cameraNum].m_Eye += m_camera[m_cameraNum].m_cameraRight*m_camera[m_cameraNum].m_speed;
		//m_camera[m_cameraNum].m_At += m_camera[m_cameraNum].m_cameraRight*m_camera[m_cameraNum].m_speed;

		break;
	case 'W':
		m_camera[m_cameraNum].m_View *= XMMatrixTranslation(0.0f, 0.0f, -m_camera[m_cameraNum].m_speed);
		//m_camera.m_View *= XMMatrixTranslation(0.0f, -0.10f, 0.0f);
		//m_camera[m_cameraNum].m_Eye -= m_camera[m_cameraNum].m_cameraFront*m_camera[m_cameraNum].m_speed;
		//m_camera[m_cameraNum].m_At -= m_camera[m_cameraNum].m_cameraFront*m_camera[m_cameraNum].m_speed;

		break;
	case 'S':
		m_camera[m_cameraNum].m_View *= XMMatrixTranslation(0.0f, 0.0f, m_camera[m_cameraNum].m_speed);
		//m_camera[m_cameraNum].m_Eye += m_camera[m_cameraNum].m_cameraFront*m_camera[m_cameraNum].m_speed;
		//m_camera[m_cameraNum].m_At += m_camera[m_cameraNum].m_cameraFront*m_camera[m_cameraNum].m_speed;
		//m_camera[m_cameraNum].m_View *= XMMatrixTranslation(0.0f, 0.10f, 0.0f);
		break;
	case 37:
		//m_camera[m_cameraNum].m_View *= XMMatrixTranslation(0.15f, 0.0f, 0.0f);
		//m_camera[m_cameraNum].m_View *= XMMatrixRotationY(0.10f);
		m_camera[m_cameraNum].m_View *= XMMatrixTranslation(m_camera[m_cameraNum].m_speed, 0.0f, 0.0f);

		//m_camera[m_cameraNum].m_Eye -= m_camera[m_cameraNum].m_cameraRight*m_camera[m_cameraNum].m_speed;
		//m_camera[m_cameraNum].m_At -= m_camera[m_cameraNum].m_cameraRight*m_camera[m_cameraNum].m_speed;
		break;
	case 39:
		//m_camera[m_cameraNum].m_View *= XMMatrixTranslation(-0.15f, 0.0f, 0.0f);
		//m_camera[m_cameraNum].m_View *= XMMatrixRotationY(-0.10f);
		m_camera[m_cameraNum].m_View *= XMMatrixTranslation(-m_camera[m_cameraNum].m_speed, 0.0f, 0.0f);

		//m_camera[m_cameraNum].m_Eye += m_camera[m_cameraNum].m_cameraRight*m_camera[m_cameraNum].m_speed;
		//m_camera[m_cameraNum].m_At += m_camera[m_cameraNum].m_cameraRight*m_camera[m_cameraNum].m_speed;
		break;
	case 38:
		//m_camera[m_cameraNum].m_View *= XMMatrixTranslation(0.0f, 0.0f, -0.15f);
		//m_camera[m_cameraNum].m_View *= XMMatrixRotationX(0.10f);
		m_camera[m_cameraNum].m_View *= XMMatrixTranslation(0.0F, -m_camera[m_cameraNum].m_speed, 0.0f);

		//m_camera[m_cameraNum].m_Eye += m_camera[m_cameraNum].m_Up*m_camera[m_cameraNum].m_speed;
		//m_camera[m_cameraNum].m_At += m_camera[m_cameraNum].m_Up*m_camera[m_cameraNum].m_speed;
		break;
	case 40:
		//m_camera[m_cameraNum].m_View *= XMMatrixTranslation(0.0f, 0.0f, 0.15f);
		//m_camera[m_cameraNum].m_View *= XMMatrixRotationX(-0.10f);
		m_camera[m_cameraNum].m_View *= XMMatrixTranslation(0.0F, m_camera[m_cameraNum].m_speed, 0.0f);
		//m_camera[m_cameraNum].m_Eye -= m_camera[m_cameraNum].m_Up*m_camera[m_cameraNum].m_speed;
		//m_camera[m_cameraNum].m_At  -= m_camera[m_cameraNum].m_Up*m_camera[m_cameraNum].m_speed;
		break;
	}
	m_camera[m_cameraNum].m_cbNeverChanges.mView = XMMatrixTranspose(m_camera[m_cameraNum].m_View);
	m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBNeverChanges, 0, NULL, &m_camera[m_cameraNum].m_cbNeverChanges, 0, 0);
}
#else
#endif // !DX

void CManager::rotateCamera()
{
#ifdef DX
	if (m_cameraNum != 0)
	{
		return;
	}
	if (dvice_init)
	{
		GetCursorPos(&m_camera[m_cameraNum].m_ActualPosCur);

		if (!m_camera[m_cameraNum].isLClick)
		{
			m_camera[m_cameraNum].isLClick = true;
			m_camera[m_cameraNum].m_antePosCur= m_camera[m_cameraNum].m_ActualPosCur;
		}

		if ((m_camera[m_cameraNum].m_antePosCur.x) < (m_camera[m_cameraNum].m_ActualPosCur.x))
		{
			m_camera[m_cameraNum].m_View *= XMMatrixRotationY(.01);
//			m_camera[m_cameraNum].m_At -= m_camera[m_cameraNum].m_cameraRight*m_camera[m_cameraNum].cameraSpeedRotation;
		}
		else if ((m_camera[m_cameraNum].m_antePosCur.x) > (m_camera[m_cameraNum].m_ActualPosCur.x))
		{
			m_camera[m_cameraNum].m_View *= XMMatrixRotationY(-0.01);
			//m_camera[m_cameraNum].m_At += m_camera[m_cameraNum].m_cameraRight*m_camera[m_cameraNum].cameraSpeedRotation;
			
		}

		if ((m_camera[m_cameraNum].m_antePosCur.y) < (m_camera[m_cameraNum].m_ActualPosCur.y))
		{
			m_camera[m_cameraNum].m_View *= XMMatrixRotationX(.01);
			//m_camera[m_cameraNum].m_At += m_camera[m_cameraNum].m_Up*m_camera[m_cameraNum].cameraSpeedRotation;
		}
		else if ((m_camera[m_cameraNum].m_antePosCur.y) > (m_camera[m_cameraNum].m_ActualPosCur.y))
		{
			m_camera[m_cameraNum].m_View *= XMMatrixRotationX(-0.01);
			//m_camera[m_cameraNum].m_At -= m_camera[m_cameraNum].m_Up*m_camera[m_cameraNum].cameraSpeedRotation;

		}
		m_camera[m_cameraNum].m_cbNeverChanges.mView = XMMatrixTranspose(m_camera[m_cameraNum].m_View);
		m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBNeverChanges, 0, NULL, &m_camera[m_cameraNum].m_cbNeverChanges, 0, 0);
		SetCursorPos(m_camera[m_cameraNum].m_antePosCur.x, m_camera[m_cameraNum].m_antePosCur.y);

	}
#elif OPENGL
	//i get the cursor`s position for know where is move
	GetCursorPos(&m_camera[m_cameraNum].m_ActualPosCur);

	//////////////////////////////////////////////////////////////
	//MOVE EDGE X
	/////////////////////////////////////////////////////////////
	if ((m_camera[m_cameraNum].m_antePosCur.x) < (m_camera[m_cameraNum].m_ActualPosCur.x))
	{
		//Move at position to the lefht if the befotre position is menor
		m_camera[m_cameraNum].at -= m_camera[m_cameraNum].cameraRight*m_camera[m_cameraNum].cameraSpeedRotation;
	}
	else if ((m_camera[m_cameraNum].m_antePosCur.x) > (m_camera[m_cameraNum].m_ActualPosCur.x))
	{
		//Move at position to the right if the befotre position is mayor
		m_camera[m_cameraNum].at += m_camera[m_cameraNum].cameraRight*m_camera[m_cameraNum].cameraSpeedRotation;

	}
	//////////////////////////////////////////////////////////////
	//MOVE EDGE Y
	/////////////////////////////////////////////////////////////
	if ((m_camera[m_cameraNum].m_antePosCur.y) < (m_camera[m_cameraNum].m_ActualPosCur.y))
	{
		//Move at position to the up if the befotre position is menor
		m_camera[m_cameraNum].at += m_camera[m_cameraNum].RelativeUp*m_camera[m_cameraNum].cameraSpeedRotation;
	}
	else if ((m_camera[m_cameraNum].m_antePosCur.y) > (m_camera[m_cameraNum].m_ActualPosCur.y))
	{
		//Move at position to the down if the befotre position is menor
		m_camera[m_cameraNum].at -= m_camera[m_cameraNum].RelativeUp*m_camera[m_cameraNum].cameraSpeedRotation;
	}

	//set cursor to the original position
	SetCursorPos(m_camera[m_cameraNum].m_antePosCur.x, m_camera[m_cameraNum].m_antePosCur.y);

#endif // !DX
}

void CManager::resetView()
{
#ifdef DX
	if (dvice_init)
	{
		m_camera[m_cameraNum].m_Eye = m_camera[m_cameraNum].originalEye;
		m_camera[m_cameraNum].m_At = m_camera[m_cameraNum].originalAt;
		m_camera[m_cameraNum].m_Up = m_camera[m_cameraNum].originalUp;

		m_camera[m_cameraNum].setView();

		m_camera[m_cameraNum].m_cbNeverChanges.mView = XMMatrixTranspose(m_camera[m_cameraNum].m_View);
		m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBNeverChanges, 0, NULL, &m_camera[m_cameraNum].m_cbNeverChanges, 0, 0);
	}
#elif OPENGL 
	//m_camera[numCamera].resetCamera();
	m_camera[m_cameraNum].eye = m_camera[m_cameraNum].originaleye;
	m_camera[m_cameraNum].at = m_camera[m_cameraNum].originalAt;
	m_camera[m_cameraNum].RelativeUp = m_camera[m_cameraNum].originalUp;
	m_camera[m_cameraNum].setView();
#endif // !DX
}

void CManager::changeCamera()
{
#ifdef DX
	m_cameraNum++;
	if (m_cameraNum > m_camera.size() - 1)
		m_cameraNum = 0;
	m_camera[m_cameraNum].m_cbNeverChanges.mView = XMMatrixTranspose(m_camera[m_cameraNum].m_View);

	m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBNeverChanges, 0, NULL, &m_camera[m_cameraNum].m_cbNeverChanges, 0, 0);
	//m_deviceContextTri.m_pImmediateContext->UpdateSubresource(m_deviceContextTri.m_pCBNeverChanges, 0, NULL, &m_camera[m_cameraNum].m_cbNeverChanges, 0, 0);

	// Initialize the projection matrix
	//    g_Projection = XMMatrixPerspectiveFovLH( XM_PIDIV4, m_device.width / (FLOAT)m_device.height, 0.01f, 100.0f );
	m_camera[m_cameraNum].m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, m_device.width / (FLOAT)m_device.height, 0.01f, 100.0f);

	//m_camera.back().m_cbChangesOnResize;
	m_camera[m_cameraNum].m_cbChangesOnResize.mProjection = XMMatrixTranspose(m_camera[m_cameraNum].m_Projection);
	m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBChangeOnResize, 0, NULL, &m_camera[m_cameraNum].m_cbChangesOnResize, 0, 0);
	//m_deviceContextTri.m_pImmediateContext->UpdateSubresource(m_deviceContextTri.m_pCBChangeOnResize, 0, NULL, &m_camera[m_cameraNum].m_cbChangesOnResize, 0, 0);
#elif OPENGL
	m_cameraNum++;

	if (m_cameraNum >=m_camera.size())
	{
		m_cameraNum = 0;
	}
#endif // !DX

}

#ifdef DX
void CManager::changeSpeed(WPARAM wParam)
{
	for (int i = 0; i < m_camera.size(); i++)
	{
		if (wParam == 'M')
			m_camera[i].m_speed += 0.1;
		else
			m_camera[i].m_speed -= 0.1;
		
	}
}
#elif OPENGL
void CManager::changeSpeed(unsigned char key)
{
	if (key == 'm' || key == 'M')
	{
		for (size_t i = 0; i < m_camera.size(); i++)
		{
			m_camera[i].m_speed += 0.1;
		}
	}
	else
	{
		for (size_t i = 0; i < m_camera.size(); i++)
		{
			m_camera[i].m_speed -= 0.1;
		}
	}
}
#endif // !DX
void CManager::changeSpecularPower(int x)
{
	float vel;
#ifdef DX
	vel = .1;
#elif OPENGL
	vel = 1.f;
#endif // DX
	
	if (x==1)
	{
		SPpower.x+=vel;
		SPpower.y+=vel;
		SPpower.z+=vel;
		SPpower.w+=vel;
	}
	else
	{
		SPpower.x -= vel;
		SPpower.y -= vel;
		SPpower.z -= vel;
		SPpower.w -= vel;
	}
	if (SPpower.x<=spMin)
	{
		SPpower = { spMin,spMin,spMin,spMin };
	}
	if (SPpower.x >= spMaxi)
	{
		SPpower = { spMaxi,spMaxi,spMaxi,spMaxi };
	}

}


#ifdef OPENGL
void CManager::initWindow(int argc, char* argv[])
{
	//Set the window position
	glutInitWindowPosition(windowPosition.x, windowPosition.y);

	//Set the window size
	//glutInitWindowSize(WindowSize.m_Width, 480);
	glutInitWindowSize(WindowSize.m_Width, WindowSize.m_Height);

	//Set Display Mode
	// Set Active buffer
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);
	//Create the window
	glutCreateWindow("OPENGL");

}

void CManager::initDevice()
{
	//load my program shader
	m_programSPixelBP.ID = LoadShadersLightBLINNPHONGv("GBufferVS.glsl", "GBufferPS.glsl");
	m_programSPixelB.ID = LoadShadersLightBLINNv("GBufferVS.glsl", "GBufferPS.glsl");
	m_programSVertexBP.ID = LoadShadersVertexBLINNPHONGv("GBufferVS.glsl", "GBufferPS.glsl");
	m_programSVertexB.ID = LoadShadersVertexBLINNv("GBufferVS.glsl", "GBufferPS.glsl");
	m_programSLuminance.ID = LoadShadersSAQ("GBufferVS-SAO.glsl", "GBufferPS-Luminance.glsl");
	m_programSBright.ID = LoadShadersSAQ("GBufferVS-SAO.glsl", "GBufferPS-Bright.glsl");
	m_programSBlur_H.ID = LoadShadersSAQ("GBufferVS-SAO.glsl", "GBufferPS-Blur_H.glsl");
	m_programSBlur_V.ID = LoadShadersSAQ("GBufferVS-SAO.glsl", "GBufferPS-Blur_V.glsl");
	m_programSAddBirght.ID = LoadShadersSAQ("GBufferVS-SAO.glsl", "GBufferPS-AddBright.glsl");
	m_programSRenderScreen.ID = LoadShadersSAQ("GBufferVS-SAO.glsl", "GBufferPS-RenderScreen.glsl");
	m_programSToneMap.ID = LoadShadersSAQ("GBufferVS-SAO.glsl", "GBufferPS-ToneMap.glsl");
	m_programSActual.ID = m_programSPixelBP.ID;
	//use my program shader
	glUseProgram(m_programSActual.ID);

	//init de viewport
	//glViewport(0, 0, WindowSize.m_Width, WindowSize.m_Height);

	//charge mesh with assimp	
	chargeMesh();
	//in the funtion i gen a bind vertexArray
	glBindVertexArray(0);
	//load the texture
	glBindVertexArray(0);

	//m_device.model = aiImportFile("cube.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	////m_device.model = aiImportFile("cube.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	////i gen de vertex array and bind it
	//glGenVertexArrays(1, &m_SAQ.VertexArrayID);
	//glBindVertexArray(m_SAQ.VertexArrayID);
	//
	//std::vector <std::uint32_t> indis;
	//indis.reserve(m_device.model->mMeshes[0]->mNumFaces * 3);
	//
	//for (std::uint32_t faceIdx = 0u; faceIdx < m_device.model->mMeshes[0]->mNumFaces; faceIdx++)
	//{
	//	//get the indexes of the vertices
	//	uint32_t ind1 = (m_device.model->mMeshes[0]->mFaces[faceIdx].mIndices[0u]);
	//	uint32_t ind2 = (m_device.model->mMeshes[0]->mFaces[faceIdx].mIndices[1u]);
	//	uint32_t ind3 = (m_device.model->mMeshes[0]->mFaces[faceIdx].mIndices[2u]);
	//
	//	//I organize the indexes of the vertices
	//	indis.push_back(ind1);
	//	indis.push_back(ind2);
	//	indis.push_back(ind3);
	//}
	////this i create the buffer vertex and index
	//m_SAQ.meshRead(m_device.model->mMeshes[0]->mNumVertices, (m_device.model->mMeshes[0]->mNumFaces) * 3, m_device.model->mMeshes[0]->mVertices, m_device.model->mMeshes[0]->mNormals, m_device.model->mMeshes[0]->mTextureCoords[0], m_device.model->mMeshes[0]->mTangents, indis);
	//m_SAQ.isquad = true;
	//m_device.model = aiImportFile("cube.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	//m_device.model = aiImportFile("SAQ.obj", aiProcessPreset_TargetRealtime_MaxQuality);
	m_device.model = aiImportFile("ScreenAlignedQuad.3ds", aiProcessPreset_TargetRealtime_MaxQuality);
	//i gen de vertex array and bind it
	glGenVertexArrays(1, &m_SAQ.VertexArrayID);
	glBindVertexArray(m_SAQ.VertexArrayID);

	//indis.clear();
	std::vector <std::uint32_t> indis;
	indis.reserve(m_device.model->mMeshes[0]->mNumFaces * 3);

	for (std::uint32_t faceIdx = 0u; faceIdx < m_device.model->mMeshes[0]->mNumFaces; faceIdx++)
	{
		//get the indexes of the vertices
		uint32_t ind1 = (m_device.model->mMeshes[0]->mFaces[faceIdx].mIndices[0u]);
		uint32_t ind2 = (m_device.model->mMeshes[0]->mFaces[faceIdx].mIndices[1u]);
		uint32_t ind3 = (m_device.model->mMeshes[0]->mFaces[faceIdx].mIndices[2u]);

		//I organize the indexes of the vertices
		indis.push_back(ind1);
		indis.push_back(ind2);
		indis.push_back(ind3);
	}
	//this i create the buffer vertex and index
	m_SAQ.meshRead(m_device.model->mMeshes[0]->mNumVertices, (m_device.model->mMeshes[0]->mNumFaces) * 3, m_device.model->mMeshes[0]->mVertices, m_device.model->mMeshes[0]->mNormals, m_device.model->mMeshes[0]->mTextureCoords[0], m_device.model->mMeshes[0]->mTangents, indis);

	//m_SAQ.chargeSAQ();

	glBindVertexArray(0);

	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WindowSize.m_Width, WindowSize.m_Height, 0, GL_RGBA, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//gen a frame buffer for de texture
	glGenFramebuffers(1, &m_renderTargetV.m_RendeTargetID);
	//bind the frame buffer
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowSize.m_Width, WindowSize.m_Height);

	glBindFramebuffer(GL_FRAMEBUFFER, m_renderTargetV.m_RendeTargetID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR CREATE RENDER TARGET";
	glBindTexture(GL_TEXTURE_2D, 0);

	createRenderTaregetLightPass();
	createRenderTaregetLuminancePass();
	createRenderTaregetBrightPass();
	createRenderTaregetBlur_HPass();
	createRenderTaregetBlur_VPass();
	createRenderTaregetAddBrightPass();
	createRenderTaregetTonePass();
	fViewportDimensions = vec2(WindowSize.m_Width, WindowSize.m_Height);
	//renderedTextureToScreen = renderedTextureLight;
	//renderedTextureToScreen = renderedTextureLuminance;
	renderedTextureToScreen = renderedTextureToneMap;
	//renderedTextureToScreen = renderedTextureBlur_H;
	//renderedTextureToScreen = renderedTextureBlur_V;
	//renderedTextureToScreen = renderedTextureAddBright;
	initCamera();
	initCamera2();
}

GLuint CManager::LoadShadersLightBLINNPHONGv(const char * vertex_file_path, const char * fragment_file_path)
{

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		stringstream ss;
		ss << VertexShaderStream.rdbuf();
		VertexShaderCode = ss.str();
		char *token = NULL;
		char *nextToken = NULL;
		token = strtok_s((char *)VertexShaderCode.c_str(), "¿", &nextToken);
		//token = (char *)texName.c_str();
		//token +=(char *) pngextencion.c_str();
		string addDef = token + '\n';
#ifdef VER_LIGTH
		addDef += "\n#define VER_LIGTH\n";
#endif // DEBUG
#ifdef BLINN
		addDef += "\n#define BLINN\n";
#endif // DEBUG
#ifdef DIR_LIGHT
		addDef += "\n#define DIR_LIGHT\n";
#endif // DEBUG
#ifdef CONE_LIGHT
		addDef += "\n#define CONE_LIGHT\n";
#endif // DEBUG
#ifdef POINT_LIGHT
		addDef += "\n#define POINT_LIGHT\n";
#endif // DEBUG

		addDef += nextToken;
		VertexShaderCode = addDef;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		char *token = NULL;
		char *nextToken = NULL;
		token = strtok_s((char *)FragmentShaderCode.c_str(), "¿", &nextToken);
		//token = (char *)texName.c_str();
		//token +=(char *) pngextencion.c_str();
		string addDef = token + '\n';
#ifdef VER_LIGTH
		addDef += "\n#define VER_LIGTH\n";
#endif // DEBUG
#ifdef BLINN
		addDef += "\n#define BLINN\n";
#endif // DEBUG
#ifdef DIR_LIGHT
		addDef += "\n#define DIR_LIGHT\n";
#endif // DEBUG
#ifdef CONE_LIGHT
		addDef += "\n#define CONE_LIGHT\n";
#endif // DEBUG
#ifdef POINT_LIGHT
		addDef += "\n#define POINT_LIGHT\n";
#endif // DEBUG
		addDef += nextToken;
		FragmentShaderCode = addDef;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
GLuint CManager::LoadShadersLightBLINNv(const char * vertex_file_path, const char * fragment_file_path)
{

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		stringstream ss;
		ss << VertexShaderStream.rdbuf();
		VertexShaderCode = ss.str();
		char *token = NULL;
		char *nextToken = NULL;
		token = strtok_s((char *)VertexShaderCode.c_str(), "¿", &nextToken);
		//token = (char *)texName.c_str();
		//token +=(char *) pngextencion.c_str();
		string addDef = token + '\n';
#ifdef VER_LIGTH
		addDef += "\n#define VER_LIGTH\n";
#endif // DEBUG
		addDef += "\n#define BLINN\n";
#ifdef DIR_LIGHT
		addDef += "\n#define DIR_LIGHT\n";
#endif // DEBUG
#ifdef CONE_LIGHT
		addDef += "\n#define CONE_LIGHT\n";
#endif // DEBUG
#ifdef POINT_LIGHT
		addDef += "\n#define POINT_LIGHT\n";
#endif // DEBUG

		addDef += nextToken;
		VertexShaderCode = addDef;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		char *token = NULL;
		char *nextToken = NULL;
		token = strtok_s((char *)FragmentShaderCode.c_str(), "¿", &nextToken);
		//token = (char *)texName.c_str();
		//token +=(char *) pngextencion.c_str();
		string addDef = token + '\n';
#ifdef VER_LIGTH
		addDef += "\n#define VER_LIGTH\n";
#endif // DEBUG
		addDef += "\n#define BLINN\n";
#ifdef DIR_LIGHT
		addDef += "\n#define DIR_LIGHT\n";
#endif // DEBUG
#ifdef CONE_LIGHT
		addDef += "\n#define CONE_LIGHT\n";
#endif // DEBUG
#ifdef POINT_LIGHT
		addDef += "\n#define POINT_LIGHT\n";
#endif // DEBUG
		addDef += nextToken;
		FragmentShaderCode = addDef;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
GLuint CManager::LoadShadersVertexBLINNPHONGv(const char * vertex_file_path, const char * fragment_file_path)
{

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		stringstream ss;
		ss << VertexShaderStream.rdbuf();
		VertexShaderCode = ss.str();
		char *token = NULL;
		char *nextToken = NULL;
		token = strtok_s((char *)VertexShaderCode.c_str(), "¿", &nextToken);
		//token = (char *)texName.c_str();
		//token +=(char *) pngextencion.c_str();
		string addDef = token + '\n';
		addDef += "\n#define VER_LIGTH\n";
#ifdef BLINN
		addDef += "\n#define BLINN\n";
#endif // DEBUG
#ifdef DIR_LIGHT
		addDef += "\n#define DIR_LIGHT\n";
#endif // DEBUG
#ifdef CONE_LIGHT
		addDef += "\n#define CONE_LIGHT\n";
#endif // DEBUG
#ifdef POINT_LIGHT
		addDef += "\n#define POINT_LIGHT\n";
#endif // DEBUG

		addDef += nextToken;
		VertexShaderCode = addDef;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		char *token = NULL;
		char *nextToken = NULL;
		token = strtok_s((char *)FragmentShaderCode.c_str(), "¿", &nextToken);
		//token = (char *)texName.c_str();
		//token +=(char *) pngextencion.c_str();
		string addDef = token + '\n';
		addDef += "\n#define VER_LIGTH\n";
#ifdef BLINN
		addDef += "\n#define BLINN\n";
#endif // DEBUG
#ifdef DIR_LIGHT
		addDef += "\n#define DIR_LIGHT\n";
#endif // DEBUG
#ifdef CONE_LIGHT
		addDef += "\n#define CONE_LIGHT\n";
#endif // DEBUG
#ifdef POINT_LIGHT
		addDef += "\n#define POINT_LIGHT\n";
#endif // DEBUG
		addDef += nextToken;
		FragmentShaderCode = addDef;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
GLuint CManager::LoadShadersVertexBLINNv(const char * vertex_file_path, const char * fragment_file_path)
{

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		stringstream ss;
		ss << VertexShaderStream.rdbuf();
		VertexShaderCode = ss.str();
		char *token = NULL;
		char *nextToken = NULL;
		token = strtok_s((char *)VertexShaderCode.c_str(), "¿", &nextToken);
		//token = (char *)texName.c_str();
		//token +=(char *) pngextencion.c_str();
		string addDef = token + '\n';
		addDef += "\n#define VER_LIGTH\n";
		addDef += "\n#define BLINN\n";
#ifdef DIR_LIGHT
		addDef += "\n#define DIR_LIGHT\n";
#endif // DEBUG
#ifdef CONE_LIGHT
		addDef += "\n#define CONE_LIGHT\n";
#endif // DEBUG
#ifdef POINT_LIGHT
		addDef += "\n#define POINT_LIGHT\n";
#endif // DEBUG

		addDef += nextToken;
		VertexShaderCode = addDef;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		char *token = NULL;
		char *nextToken = NULL;
		token = strtok_s((char *)FragmentShaderCode.c_str(), "¿", &nextToken);
		//token = (char *)texName.c_str();
		//token +=(char *) pngextencion.c_str();
		string addDef = token + '\n';
		addDef += "\n#define VER_LIGTH\n";
		addDef += "\n#define BLINN\n";
#ifdef DIR_LIGHT
		addDef += "\n#define DIR_LIGHT\n";
#endif // DEBUG
#ifdef CONE_LIGHT
		addDef += "\n#define CONE_LIGHT\n";
#endif // DEBUG
#ifdef POINT_LIGHT
		addDef += "\n#define POINT_LIGHT\n";
#endif // DEBUG
		addDef += nextToken;
		FragmentShaderCode = addDef;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
GLuint CManager::LoadShadersSAQ(const char * vertex_file_path, const char * fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		stringstream ss;
		ss << VertexShaderStream.rdbuf();
		VertexShaderCode = ss.str();

		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
#endif // OPENGL



void CManager::fiesta()
{
#ifdef DX
	timeFiesta += .2;
	if (bFiestaMode&&timeFiesta>1)
	{
		float c1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float c2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float c3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		color = _XMFLOAT4(c1, c2, c3, 1.0f);
	}
	else if (!bFiestaMode)
	{
		color = _XMFLOAT4(1, 1, 1, 1);
	}
#elif OPENGL
	timeFiesta += .2;
	if (bFiestaMode&&timeFiesta>1)
	{
		float c1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float c2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float c3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		color = vec4(c1, c2, c3, 1.0f);
	}
	else if (!bFiestaMode)
	{
		color = vec4(1, 1, 1, 1);
	}
	GLuint colorID = glGetUniformLocation(m_programSActual.ID, "fiesta");
	//glUniform4i(colorID, color.x, color.y, color.z, color.w);
	glUniform4fv(colorID, 1, &color[0]);
#endif // DX

}

void CManager::LightPass()
{
#ifdef DX
	m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &m_lightPass.m_pRenderTargetView, m_lightPass.m_pDepthStencilView);

	m_deviceContext.m_pImmediateContext->VSSetShader(m_programSActual.m_VertexS.m_pVertexShader, NULL, 0);
	m_deviceContext.m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_buffers.m_pCBNeverChanges);
	m_deviceContext.m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_buffers.m_pCBChangeOnResize);
	m_deviceContext.m_pImmediateContext->VSSetConstantBuffers(2, 1, &m_buffers.m_pCBChangesEveryFrame);
	m_deviceContext.m_pImmediateContext->VSSetSamplers(0, 1, &m_texture.m_pSamplerLinear);
	m_deviceContext.m_pImmediateContext->PSSetShader(m_programSActual.m_PixelS.m_pPixelShader, NULL, 0);
	m_deviceContext.m_pImmediateContext->PSSetConstantBuffers(0, 1, &m_buffers.m_pCBNeverChanges);
	m_deviceContext.m_pImmediateContext->PSSetConstantBuffers(1, 1, &m_buffers.m_pCBChangeOnResize);
	m_deviceContext.m_pImmediateContext->PSSetConstantBuffers(2, 1, &m_buffers.m_pCBChangesEveryFrame);
	m_deviceContext.m_pImmediateContext->PSSetSamplers(0, 1, &m_texture.m_pSamplerLinear);

	m_camera[m_cameraNum].setTransform();

	float ClearColor[4] = { 0.50f, 0.0f, 0.60f, 1.0f }; // red, green, blue, alpha
	//m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &m_renderTargetV.m_pRenderTargetView, m_renderTargetV.m_pDepthStencilView);
	//m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBChangeOnResize, 0, NULL, &m_camera[m_cameraNum].m_cbChangesOnResize, 0, 0);
	m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBNeverChanges, 0, NULL, &m_camera[m_cameraNum].m_cbNeverChanges, 0, 0);
	m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBChangeOnResize, 0, NULL, &m_camera[m_cameraNum].m_cbChangesOnResize, 0, 0);

	m_deviceContext.m_pImmediateContext->ClearRenderTargetView(m_lightPass.m_pRenderTargetView, ClearColor);
	m_deviceContext.m_pImmediateContext->ClearDepthStencilView(m_lightPass.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//time += (float)XM_PI * 0.005f;
	if (rotateDirLight)
	{
		time += 0.005f;
	}
	m_lightDir = { cos(time),0, sin(time),0 };
	//m_lightDir = {0,0, -5,0 };
	//m_lightDir = {0,0,-1,0 };
	for (int i = 0; i < m_meshes.size(); i++)
	{
		static float t = 0.0f;

		if (m_buffers.m_pVertexBuffer)m_buffers.m_pVertexBuffer->Release();
		if (m_buffers.m_pIndexBuffer)m_buffers.m_pIndexBuffer->Release();
		
		CDevice::CBChangesEveryFrame m_meshe;
		//CBLightEveryFrame lights;
		m_deviceContext.m_pImmediateContext->PSSetShaderResources(0, 1, &m_meshes[i].textures.m_Difuse.m_pTextureRV);
		m_deviceContext.m_pImmediateContext->VSSetShaderResources(0, 1, &m_meshes[i].textures.m_Difuse.m_pTextureRV);
		m_deviceContext.m_pImmediateContext->PSSetShaderResources(1, 1, &m_meshes[i].textures.m_Normal.m_pTextureRV);
		m_deviceContext.m_pImmediateContext->VSSetShaderResources(1, 1, &m_meshes[i].textures.m_Normal.m_pTextureRV);

		//m_mesh.mesh_pantalla();
		DcreateVertexBuffer(m_meshes[i].m_numTries);
		//	DcreateVertexBuffer(3);
		m_device.InitData.pSysMem = m_meshes[i].m_vertex;
		//m_device.InitData.pSysMem = verticestri;
		m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pVertexBuffer);

		// Set vertex buffer
		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;
		m_deviceContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &m_buffers.m_pVertexBuffer, &stride, &offset);

		// Create index buffer
		// Create vertex buffer
		DcreateIndexBuffer(m_meshes[i].m_numIndex);
		//DcreateIndexBuffer(3);
		m_device.InitData.pSysMem = m_meshes[i].m_index;
		// m_device.InitData.pSysMem = indicesTri;
		m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pIndexBuffer);
		// Set index buffer
		m_deviceContext.m_pImmediateContext->IASetIndexBuffer(m_buffers.m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		
		
		m_meshe.mWorld = XMMatrixIdentity();
		fiesta();
		m_meshe.vMeshColor = color;
		m_meshe.lightDir = m_lightDir;

		m_meshe.vViewPosition = { m_camera[m_cameraNum].m_View._41,-m_camera[m_cameraNum].m_View._42,m_camera[m_cameraNum].m_View._43,m_camera[m_cameraNum].m_View._44 };

		m_meshe.DifuseColor = DifuseColor;
		m_meshe.SpecularColorDir = SpecularColorDir;
		m_meshe.SpecularColorPoint = SpecularColorPoint;
		m_meshe.SpecularColorSpot = SpecularColorSpot;
		m_meshe.SPpower = SPpower;
		m_meshe.AmbientalColor = ambientColor;
		m_meshe.KDASL = KDASL;
		m_meshe.lightPosition = ligthPosition;
		m_meshe.SLCOC = SLCOC;
		m_meshe.LPLQLD = LPLQLD;
		m_meshe.SLDPS = SLDPS;
		m_meshe.ADPS = ADPS;



		m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBChangesEveryFrame, 0, NULL, &m_meshe, 0, 0);
		//m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBLightEveryFrame, 0, NULL, &lights, 0, 0);
		m_deviceContext.m_pImmediateContext->DrawIndexed(m_meshes[i].m_numIndex, 0, 0);
	}



#elif OPENGL
	glViewport(0, 0, WindowSize.m_Width, WindowSize.m_Height);
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightPass.m_RendeTargetID);
	//glBindFramebuffer(GL_FRAMEBUFFER,0);
	//glBindFramebuffer(GL_FRAMEBUFFER, m_renderTargetLight.m_RendeTargetID);
	bRotationMesh = false;
	m_camera[m_cameraNum].setView();
	//glViewport(0, 0, WindowSize.m_Width, WindowSize.m_Width);

	mat4 Model = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};
	mat4 worldMatrix = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};
	// Our ModelViewProjection : multiplication of our 3 matrices
	//mvp = m_camera[numCamera].m_matProjection * m_camera[numCamera].m_matView * Model;




	GLuint MatrixID = glGetUniformLocation(m_programSActual.ID, "WM");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &worldMatrix[0][0]);

	MatrixID = glGetUniformLocation(m_programSActual.ID, "VM");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &m_camera[m_cameraNum].m_matView[0][0]);

	MatrixID = glGetUniformLocation(m_programSActual.ID, "PM");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &m_camera[0].m_matProjection[0][0]);


	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].escaleModel();

		m_meshes[i].voidescalar();
		MatrixID = glGetUniformLocation(m_programSActual.ID, "MM");
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &m_meshes[i].matModel[0][0]);
	}

	fiesta();

	if (bMousePressed)
	{
		rotateCamera();
	}

	glClearColor(0.0f, 1.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	////glLoadIdentity();
	//
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	bRotationMesh = false;

	//m_mesh.escaleModel();

	//m_mesh.voidescalar();
	
	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].escaleModel();

		m_meshes[i].voidescalar();
		MatrixID = glGetUniformLocation(m_programSActual.ID, "MM");
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &m_meshes[i].matModel[0][0]);
		//lBindTexture(GL_TEXTURE_2D, m_TextureArray.at(i)->m_GLHandleID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, m_meshes[i].vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), (void*)0);


		GLuint texID = glGetUniformLocation(m_programSActual.ID, "renderedTexture");
		glUniform1i(texID, m_meshes[i].textures.textureID);
		glActiveTexture(GL_TEXTURE0 + m_meshes[i].textures.textureID);
		glBindTexture(GL_TEXTURE_2D, m_meshes[i].textures.textureID);
		GLuint normID = glGetUniformLocation(m_programSActual.ID, "normalTexture");
		glUniform1i(normID, m_meshes[i].textures.normalID);
		glActiveTexture(GL_TEXTURE0 + m_meshes[i].textures.normalID);
		glBindTexture(GL_TEXTURE_2D, m_meshes[i].textures.normalID);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), (unsigned char*)NULL + (3 * sizeof(float)));

		glVertexAttribPointer(
			2,                  // atributo 0. No hay razón particular para el 0, pero debe corresponder en el shader.
			3,                  // tamaño
			GL_FLOAT,           // tipo
			GL_FALSE,           // normalizado?
			sizeof(CMesh::Vertex),                    // Paso
			(unsigned char*)NULL + (5 * sizeof(float))            // desfase del buffer
		);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), (unsigned char*)NULL + (8 * sizeof(float)));

		glDrawArrays(GL_TRIANGLES, 0, m_meshes[i].numTris * 3 * 3); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo


		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);

		glDisableVertexAttribArray(0);

	}
#endif // DX
}

void CManager::LuminancePass()
{
#ifdef DX

	float ClearColor[4] = { 0.0f, 1.0f, 0.60f, 1.0f }; // red, green, blue, alpha
	m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &m_LuminancePass.m_pRenderTargetView, m_LuminancePass.m_pDepthStencilView);
	m_deviceContext.m_pImmediateContext->ClearRenderTargetView(m_LuminancePass.m_pRenderTargetView, ClearColor);
	m_deviceContext.m_pImmediateContext->ClearDepthStencilView(m_LuminancePass.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	//m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &m_renderTargetV.m_pRenderTargetView, m_renderTargetV.m_pDepthStencilView);
	//m_deviceContext.m_pImmediateContext->ClearRenderTargetView(m_renderTargetV.m_pRenderTargetView, ClearColor);
	//m_deviceContext.m_pImmediateContext->ClearDepthStencilView(m_renderTargetV.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


	m_deviceContext.m_pImmediateContext->VSSetShader(m_programSLuminance.m_VertexS.m_pVertexShader, NULL, 0);
	m_deviceContext.m_pImmediateContext->PSSetShader(m_programSLuminance.m_PixelS.m_pPixelShader, NULL, 0);
	m_deviceContext.m_pImmediateContext->PSSetSamplers(0, 1, &m_texture.m_pSamplerLinear);
	//¡m_deviceContext.m_pImmediateContext->PSSetSamplers(0, 1, &g_manager.m_texture.m_pSamplerLinear);



	if (m_buffers.m_pVertexBuffer) m_buffers.m_pVertexBuffer->Release();
	if (m_buffers.m_pIndexBuffer)  m_buffers.m_pIndexBuffer->Release();

	m_deviceContext.m_pImmediateContext->PSSetShaderResources(0, 1, &SResourceLightPass);

	DcreateVertexBuffer(m_SAQ.m_numTries);
	m_device.InitData.pSysMem = m_SAQ.m_vertex;
	m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pVertexBuffer);

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_deviceContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &m_buffers.m_pVertexBuffer, &stride, &offset);

	// Create index buffer
	DcreateIndexBuffer(m_SAQ.m_numIndex);
	m_device.InitData.pSysMem = m_SAQ.m_index;
	m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pIndexBuffer);
	// Set index buffer
	m_deviceContext.m_pImmediateContext->IASetIndexBuffer(m_buffers.m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	m_deviceContext.m_pImmediateContext->DrawIndexed(m_SAQ.m_numIndex, 0, 0);
	m_deviceContext.m_pImmediateContext->GenerateMips(SResourceLuminancePass);
	/////////////////////////////////////////

#elif OPENGL
	glViewport(0, 0, WindowSize.m_Width, WindowSize.m_Height);
	//glUseProgram(programSAOLuminance);
	glUseProgram(m_programSLuminance.ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_LuminancePass.m_RendeTargetID);
	//glBindFramebuffer(GL_FRAMEBUFFER,0);

	glClearColor(1.0f, 1.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	////glLoadIdentity();
	//
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	bRotationMesh = false;
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_SAQ.vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), (void*)0);

	GLuint texID = glGetUniformLocation(m_programSLuminance.ID, "renderedTexture");
	//glGenerateMipmap(GL_TEXTURE_2D);
	glUniform1i(texID, renderedTextureLight);
	glActiveTexture(GL_TEXTURE0 + renderedTextureLight);
	glBindTexture(GL_TEXTURE_2D, renderedTextureLight);
	glGenerateMipmap(GL_TEXTURE_2D);
	//glGenerateMipmap(GL_TEXTURE_2D);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), (unsigned char*)NULL + (3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, m_SAQ.numTris * 3 * 2); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

#endif // DX

}
void CManager::BrightPass()
{
#ifdef DX 
	float ClearColor[4] = { 0.0f, 1.0f, 0.60f, 1.0f }; // red, green, blue, alpha
	m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &m_BrightPass.m_pRenderTargetView, m_BrightPass.m_pDepthStencilView);
	m_deviceContext.m_pImmediateContext->ClearRenderTargetView(m_BrightPass.m_pRenderTargetView, ClearColor);
	m_deviceContext.m_pImmediateContext->ClearDepthStencilView(m_BrightPass.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_deviceContext.m_pImmediateContext->VSSetShader(m_programSBright.m_VertexS.m_pVertexShader, NULL, 0);
	m_deviceContext.m_pImmediateContext->PSSetShader(m_programSBright.m_PixelS.m_pPixelShader, NULL, 0);
	m_deviceContext.m_pImmediateContext->PSSetSamplers(0, 1, &m_texture.m_pSamplerLinear);
	m_deviceContext.m_pImmediateContext->PSSetConstantBuffers(3, 1, &m_buffers.m_pCBBright);

	CDevice::CBBright m_cbBright;
	m_cbBright.BrightLod_BloomThreshold.y = BloomThreshold;
	m_cbBright.BrightLod_BloomThreshold.x = BrightLod;


	if (m_buffers.m_pVertexBuffer) m_buffers.m_pVertexBuffer->Release();
	if (m_buffers.m_pIndexBuffer)  m_buffers.m_pIndexBuffer->Release();

	m_deviceContext.m_pImmediateContext->PSSetShaderResources(0, 1, &SResourceLightPass);
	m_deviceContext.m_pImmediateContext->PSSetShaderResources(1, 1, &SResourceLuminancePass);
	m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBBright, 0, NULL, &m_cbBright, 0, 0);

	DcreateVertexBuffer(m_SAQ.m_numTries);
	m_device.InitData.pSysMem = m_SAQ.m_vertex;
	m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pVertexBuffer);

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_deviceContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &m_buffers.m_pVertexBuffer, &stride, &offset);

	// Create index buffer
	DcreateIndexBuffer(m_SAQ.m_numIndex);
	m_device.InitData.pSysMem = m_SAQ.m_index;
	m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pIndexBuffer);
	// Set index buffer
	m_deviceContext.m_pImmediateContext->IASetIndexBuffer(m_buffers.m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	m_deviceContext.m_pImmediateContext->DrawIndexed(m_SAQ.m_numIndex, 0, 0);

	m_deviceContext.m_pImmediateContext->GenerateMips(SResourceBrightPass);
	/////////////////////////////////////////
#elif OPENGL
	glViewport(0, 0, WindowSize.m_Width, WindowSize.m_Height);
	glUseProgram(m_programSBright.ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_BrightPass.m_RendeTargetID);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLint BrightLodID = glGetUniformLocation(m_programSBright.ID, "BrightLod");
	glUniform1f(BrightLodID, BrightLod);
	GLint BloomThresholdID = glGetUniformLocation(m_programSBright.ID, "BloomThreshold");
	glUniform1f(BloomThresholdID, BloomThreshold);

	glClearColor(0.0f, 1.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_SAQ.vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), (void*)0);

	GLuint texID = glGetUniformLocation(m_programSBright.ID, "lightTexture");
	glUniform1i(texID, renderedTextureLight);
	glActiveTexture(GL_TEXTURE0 + renderedTextureLight);
	glBindTexture(GL_TEXTURE_2D, renderedTextureLight);

	GLuint normID = glGetUniformLocation(m_programSBright.ID, "LuminanceTexture");
	glUniform1i(normID, renderedTextureLuminance);
	glActiveTexture(GL_TEXTURE0 + renderedTextureLuminance);
	glBindTexture(GL_TEXTURE_2D, renderedTextureLuminance);
	glGenerateMipmap(GL_TEXTURE_2D);



	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), (unsigned char*)NULL + (3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, m_SAQ.numTris * 3 * 2); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo


	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
#endif // DX
}

void CManager::Blur_HPass()
{
#ifdef DX 
	float ClearColor[4] = { 0.0f, 1.0f, 0.60f, 1.0f }; // red, green, blue, alpha
	m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &m_BlurHPass.m_pRenderTargetView, m_BlurHPass.m_pDepthStencilView);
	m_deviceContext.m_pImmediateContext->ClearRenderTargetView(m_BlurHPass.m_pRenderTargetView, ClearColor);
	m_deviceContext.m_pImmediateContext->ClearDepthStencilView(m_BlurHPass.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_deviceContext.m_pImmediateContext->VSSetShader(m_programSBlur_H.m_VertexS.m_pVertexShader, NULL, 0);
	m_deviceContext.m_pImmediateContext->PSSetShader(m_programSBlur_H.m_PixelS.m_pPixelShader, NULL, 0);
	m_deviceContext.m_pImmediateContext->PSSetSamplers(0, 1, &m_texture.m_pSamplerLinear);
	m_deviceContext.m_pImmediateContext->PSSetConstantBuffers(4, 1, &m_buffers.m_pCBlurH);

	CDevice::CBBlur m_cbBlurH;
	m_cbBlurH.f2ViewportDimensions_blurMip.y = m_Height;
	m_cbBlurH.f2ViewportDimensions_blurMip.x = m_Width;
	m_cbBlurH.f2ViewportDimensions_blurMip.z = blurHmip;


	if (m_buffers.m_pVertexBuffer) m_buffers.m_pVertexBuffer->Release();
	if (m_buffers.m_pIndexBuffer)  m_buffers.m_pIndexBuffer->Release();

	m_deviceContext.m_pImmediateContext->PSSetShaderResources(0, 1, &SResourceBrightPass);
	m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBlurH, 0, NULL, &m_cbBlurH, 0, 0);

	DcreateVertexBuffer(m_SAQ.m_numTries);
	m_device.InitData.pSysMem = m_SAQ.m_vertex;
	m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pVertexBuffer);

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_deviceContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &m_buffers.m_pVertexBuffer, &stride, &offset);

	// Create index buffer
	DcreateIndexBuffer(m_SAQ.m_numIndex);
	m_device.InitData.pSysMem = m_SAQ.m_index;
	m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pIndexBuffer);
	// Set index buffer
	m_deviceContext.m_pImmediateContext->IASetIndexBuffer(m_buffers.m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	m_deviceContext.m_pImmediateContext->DrawIndexed(m_SAQ.m_numIndex, 0, 0);
	m_deviceContext.m_pImmediateContext->GenerateMips(SResourceBlurHPass);
	/////////////////////////////////////////
#elif OPENGL
	glViewport(0, 0, WindowSize.m_Width, WindowSize.m_Height);
	glUseProgram(m_programSBlur_H.ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_BlurHPass.m_RendeTargetID);

	GLint blurMipID = glGetUniformLocation(m_programSBlur_H.ID, "blurMip");
	glUniform1i(blurMipID, blurHmip);
	GLint fViewportDimensionsID = glGetUniformLocation(m_programSBlur_H.ID, "fViewportDimensions");
	glUniform2f(fViewportDimensionsID, 1, fViewportDimensions[0]);

	glClearColor(0.0f, 1.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_SAQ.vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), (void*)0);

	GLuint texID = glGetUniformLocation(m_programSBlur_H.ID, "BrightTexture");
	glUniform1i(texID, renderedTextureBright);
	glActiveTexture(GL_TEXTURE0 + renderedTextureBright);
	glBindTexture(GL_TEXTURE_2D, renderedTextureBright);
	glGenerateMipmap(GL_TEXTURE_2D);




	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), (unsigned char*)NULL + (3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, m_SAQ.numTris * 3 * 2); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo


	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
#endif // DX
}

void CManager::Blur_VPass()
{
#ifdef DX 
	float ClearColor[4] = { 0.0f, 1.0f, 0.60f, 1.0f }; // red, green, blue, alpha
	m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &m_BlurVPass.m_pRenderTargetView, m_BlurVPass.m_pDepthStencilView);
	m_deviceContext.m_pImmediateContext->ClearRenderTargetView(m_BlurVPass.m_pRenderTargetView, ClearColor);
	m_deviceContext.m_pImmediateContext->ClearDepthStencilView(m_BlurVPass.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_deviceContext.m_pImmediateContext->VSSetShader(m_programSBlur_V.m_VertexS.m_pVertexShader, NULL, 0);
	m_deviceContext.m_pImmediateContext->PSSetShader(m_programSBlur_V.m_PixelS.m_pPixelShader, NULL, 0);
	m_deviceContext.m_pImmediateContext->PSSetSamplers(0, 1, &m_texture.m_pSamplerLinear);
	m_deviceContext.m_pImmediateContext->PSSetConstantBuffers(4, 1, &m_buffers.m_pCBlurH);

	CDevice::CBBlur m_cbBlurH;
	m_cbBlurH.f2ViewportDimensions_blurMip.y = m_Height;
	m_cbBlurH.f2ViewportDimensions_blurMip.x = m_Width;
	m_cbBlurH.f2ViewportDimensions_blurMip.z = blurVmip;


	if (m_buffers.m_pVertexBuffer) m_buffers.m_pVertexBuffer->Release();
	if (m_buffers.m_pIndexBuffer)  m_buffers.m_pIndexBuffer->Release();

	m_deviceContext.m_pImmediateContext->PSSetShaderResources(0, 1, &SResourceBrightPass);
	m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBlurH, 0, NULL, &m_cbBlurH, 0, 0);

	DcreateVertexBuffer(m_SAQ.m_numTries);
	m_device.InitData.pSysMem = m_SAQ.m_vertex;
	m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pVertexBuffer);

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_deviceContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &m_buffers.m_pVertexBuffer, &stride, &offset);

	// Create index buffer
	DcreateIndexBuffer(m_SAQ.m_numIndex);
	m_device.InitData.pSysMem = m_SAQ.m_index;
	m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pIndexBuffer);
	// Set index buffer
	m_deviceContext.m_pImmediateContext->IASetIndexBuffer(m_buffers.m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	m_deviceContext.m_pImmediateContext->DrawIndexed(m_SAQ.m_numIndex, 0, 0);

	m_deviceContext.m_pImmediateContext->GenerateMips(SResourceBlurVPass);
	/////////////////////////////////////////
#elif OPENGL
	glViewport(0, 0, WindowSize.m_Width, WindowSize.m_Height);
	glUseProgram(m_programSBlur_V.ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_BlurVPass.m_RendeTargetID);

	GLint blurMipID = glGetUniformLocation(m_programSBlur_V.ID, "blurMipv");
	glUniform1i(blurMipID, blurVmip);
	GLint fViewportDimensionsID = glGetUniformLocation(m_programSBlur_V.ID, "fViewportDimensions");
	glUniform1f(fViewportDimensionsID, fViewportDimensions[0]);

	glClearColor(0.0f, 1.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_SAQ.vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), (void*)0);

	GLuint texID = glGetUniformLocation(m_programSBlur_V.ID, "BrightTexture");
	glUniform1i(texID, renderedTextureBright);
	glActiveTexture(GL_TEXTURE0 + renderedTextureBright);
	glBindTexture(GL_TEXTURE_2D, renderedTextureBright);
	glGenerateMipmap(GL_TEXTURE_2D);




	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), (unsigned char*)NULL + (3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, m_SAQ.numTris * 3 * 2); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo


	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
#endif // DX
}

void CManager::AddBrightPass()
{
#ifdef DX 
	float ClearColor[4] = { 0.0f, 1.0f, 0.60f, 1.0f }; // red, green, blue, alpha
	m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &m_AddBrightPass.m_pRenderTargetView, m_AddBrightPass.m_pDepthStencilView);
	m_deviceContext.m_pImmediateContext->ClearRenderTargetView(m_AddBrightPass.m_pRenderTargetView, ClearColor);
	m_deviceContext.m_pImmediateContext->ClearDepthStencilView(m_AddBrightPass.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_deviceContext.m_pImmediateContext->VSSetShader(m_programSAddBirght.m_VertexS.m_pVertexShader, NULL, 0);
	m_deviceContext.m_pImmediateContext->PSSetShader(m_programSAddBirght.m_PixelS.m_pPixelShader, NULL, 0);
	m_deviceContext.m_pImmediateContext->PSSetSamplers(0, 1, &m_texture.m_pSamplerLinear);
	m_deviceContext.m_pImmediateContext->PSSetConstantBuffers(5, 1, &m_buffers.m_pCBAddBirght);

	CDevice::CBAddBright m_cbAddBright;
	m_cbAddBright.blurMip.x = AddBmip;


	if (m_buffers.m_pVertexBuffer) m_buffers.m_pVertexBuffer->Release();
	if (m_buffers.m_pIndexBuffer)  m_buffers.m_pIndexBuffer->Release();

	m_deviceContext.m_pImmediateContext->PSSetShaderResources(0, 1, &SResourceBrightPass);
	m_deviceContext.m_pImmediateContext->PSSetShaderResources(1, 1, &SResourceBlurHPass);
	m_deviceContext.m_pImmediateContext->PSSetShaderResources(2, 1, &SResourceBlurVPass);
	m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCBAddBirght, 0, NULL, &m_cbAddBright, 0, 0);

	DcreateVertexBuffer(m_SAQ.m_numTries);
	m_device.InitData.pSysMem = m_SAQ.m_vertex;
	m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pVertexBuffer);

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_deviceContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &m_buffers.m_pVertexBuffer, &stride, &offset);

	// Create index buffer
	DcreateIndexBuffer(m_SAQ.m_numIndex);
	m_device.InitData.pSysMem = m_SAQ.m_index;
	m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pIndexBuffer);
	// Set index buffer
	m_deviceContext.m_pImmediateContext->IASetIndexBuffer(m_buffers.m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	m_deviceContext.m_pImmediateContext->DrawIndexed(m_SAQ.m_numIndex, 0, 0);

	m_deviceContext.m_pImmediateContext->GenerateMips(SResourceAddBrightPass);
#elif OPENGL
	glViewport(0, 0, WindowSize.m_Width, WindowSize.m_Height);
	glUseProgram(m_programSAddBirght.ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_AddBrightPass.m_RendeTargetID);

	GLint blurMipID = glGetUniformLocation(m_programSAddBirght.ID, "blurMip");
	glUniform1i(blurMipID, AddBmip);

	glClearColor(0.0f, 1.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_SAQ.vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), (void*)0);

	GLuint BrightID = glGetUniformLocation(m_programSAddBirght.ID, "BrightTexture");
	glUniform1i(BrightID, renderedTextureBright);
	glActiveTexture(GL_TEXTURE0 + renderedTextureBright);
	glBindTexture(GL_TEXTURE_2D, renderedTextureBright);
	glGenerateMipmap(GL_TEXTURE_2D);
	GLuint Blur_HID = glGetUniformLocation(m_programSAddBirght.ID, "Blur_HTexture");
	glActiveTexture(GL_TEXTURE0 + renderedTextureBlur_H);
	glBindTexture(GL_TEXTURE_2D, renderedTextureBlur_H);
	glUniform1i(Blur_HID, renderedTextureBlur_H);
	glGenerateMipmap(GL_TEXTURE_2D);
	GLuint Blur_VID = glGetUniformLocation(m_programSAddBirght.ID, "Blur_VTexture");
	glActiveTexture(GL_TEXTURE0 + renderedTextureBlur_V);
	glBindTexture(GL_TEXTURE_2D, renderedTextureBlur_V);
	glUniform1i(Blur_VID, renderedTextureBlur_V);
	glGenerateMipmap(GL_TEXTURE_2D);


	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), (unsigned char*)NULL + (3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, m_SAQ.numTris * 3 * 2); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo


	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	
#endif // DX	void RenderToscreenPass();
}
void CManager::tonePass()
{
#ifdef DX 
	float ClearColor[4] = { 0.0f, 1.0f, 0.60f, 1.0f }; // red, green, blue, alpha
	m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &m_ToneMapPass.m_pRenderTargetView, m_ToneMapPass.m_pDepthStencilView);
	m_deviceContext.m_pImmediateContext->ClearRenderTargetView(m_ToneMapPass.m_pRenderTargetView, ClearColor);
	m_deviceContext.m_pImmediateContext->ClearDepthStencilView(m_ToneMapPass.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_deviceContext.m_pImmediateContext->VSSetShader(m_programSToneMap.m_VertexS.m_pVertexShader, NULL, 0);
	m_deviceContext.m_pImmediateContext->PSSetShader(m_programSToneMap.m_PixelS.m_pPixelShader, NULL, 0);
	m_deviceContext.m_pImmediateContext->PSSetSamplers(0, 1, &m_texture.m_pSamplerLinear);
	m_deviceContext.m_pImmediateContext->PSSetConstantBuffers(6, 1, &m_buffers.m_pCToneMap);

	CDevice::CBToneMap m_cbToneMap;
	m_cbToneMap.Exposure_BloomMuiltiplier.x = Exposure;
	m_cbToneMap.Exposure_BloomMuiltiplier.y = BloomMuiltiplier;


	if (m_buffers.m_pVertexBuffer) m_buffers.m_pVertexBuffer->Release();
	if (m_buffers.m_pIndexBuffer)  m_buffers.m_pIndexBuffer->Release();

	m_deviceContext.m_pImmediateContext->PSSetShaderResources(0, 1, &SResourceAddBrightPass);
	m_deviceContext.m_pImmediateContext->PSSetShaderResources(1, 1, &SResourceLightPass);
	m_deviceContext.m_pImmediateContext->UpdateSubresource(m_buffers.m_pCToneMap, 0, NULL, &m_cbToneMap, 0, 0);

	DcreateVertexBuffer(m_SAQ.m_numTries);
	m_device.InitData.pSysMem = m_SAQ.m_vertex;
	m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pVertexBuffer);

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_deviceContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &m_buffers.m_pVertexBuffer, &stride, &offset);

	// Create index buffer
	DcreateIndexBuffer(m_SAQ.m_numIndex);
	m_device.InitData.pSysMem = m_SAQ.m_index;
	m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pIndexBuffer);
	// Set index buffer
	m_deviceContext.m_pImmediateContext->IASetIndexBuffer(m_buffers.m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	m_deviceContext.m_pImmediateContext->DrawIndexed(m_SAQ.m_numIndex, 0, 0);

	m_deviceContext.m_pImmediateContext->GenerateMips(SResourceToneMapPass);
#elif OPENGL
	glViewport(0, 0, WindowSize.m_Width, WindowSize.m_Height);
	glUseProgram(m_programSToneMap.ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ToneMapPass.m_RendeTargetID);

	GLint ExposureID = glGetUniformLocation(m_programSToneMap.ID, "Exposure");
	glUniform1f(ExposureID, Exposure);
	GLint BloomMuiltiplierID = glGetUniformLocation(m_programSToneMap.ID, "BloomMuiltiplier");
	glUniform1f(BloomMuiltiplierID, BloomMuiltiplier);


	glClearColor(0.0f, 1.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_SAQ.vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), (void*)0);

	GLuint AddBrightID = glGetUniformLocation(m_programSToneMap.ID, "AddBright");
	glUniform1i(AddBrightID, renderedTextureAddBright);
	glActiveTexture(GL_TEXTURE0 + renderedTextureAddBright);
	glBindTexture(GL_TEXTURE_2D, renderedTextureAddBright);

	GLuint LightD = glGetUniformLocation(m_programSToneMap.ID, "Light");
	glUniform1i(LightD, renderedTextureLight);
	glActiveTexture(GL_TEXTURE0 + renderedTextureLight);
	glBindTexture(GL_TEXTURE_2D, renderedTextureLight);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), (unsigned char*)NULL + (3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, m_SAQ.numTris * 3 * 2); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo


	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glGenerateMipmap(GL_TEXTURE_2D);
#endif // DX	void RenderToscreenPass();
}
void CManager::RenderToscreenPass()
{
#ifdef DX 
	float ClearColor[4] = { 0.0f, 1.0f, 0.60f, 1.0f }; // red, green, blue, alpha
	m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &m_renderTargetV.m_pRenderTargetView, m_renderTargetV.m_pDepthStencilView);
	m_deviceContext.m_pImmediateContext->ClearRenderTargetView(m_renderTargetV.m_pRenderTargetView, ClearColor);
	m_deviceContext.m_pImmediateContext->ClearDepthStencilView(m_renderTargetV.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_deviceContext.m_pImmediateContext->VSSetShader(m_programSRenderScreen.m_VertexS.m_pVertexShader, NULL, 0);
	m_deviceContext.m_pImmediateContext->PSSetShader(m_programSRenderScreen.m_PixelS.m_pPixelShader, NULL, 0);
	m_deviceContext.m_pImmediateContext->PSSetSamplers(0, 1, &m_texture.m_pSamplerLinear);

	CDevice::CBAddBright m_cbAddBright;
	m_cbAddBright.blurMip.x = AddBmip;


	if (m_buffers.m_pVertexBuffer) m_buffers.m_pVertexBuffer->Release();
	if (m_buffers.m_pIndexBuffer)  m_buffers.m_pIndexBuffer->Release();

	m_deviceContext.m_pImmediateContext->PSSetShaderResources(0, 1, &SResourceActual);

	DcreateVertexBuffer(m_SAQ.m_numTries);
	m_device.InitData.pSysMem = m_SAQ.m_vertex;
	m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pVertexBuffer);

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_deviceContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &m_buffers.m_pVertexBuffer, &stride, &offset);

	// Create index buffer
	DcreateIndexBuffer(m_SAQ.m_numIndex);
	m_device.InitData.pSysMem = m_SAQ.m_index;
	m_device.m_pd3dDevice->CreateBuffer(&m_device.bd, &m_device.InitData, &m_buffers.m_pIndexBuffer);
	// Set index buffer
	m_deviceContext.m_pImmediateContext->IASetIndexBuffer(m_buffers.m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	m_deviceContext.m_pImmediateContext->DrawIndexed(m_SAQ.m_numIndex, 0, 0);
#elif OPENGL


	glViewport(0, 0, WindowSize.m_Width, WindowSize.m_Height);
	glUseProgram(m_programSRenderScreen.ID);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.0f, 1.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_SAQ.vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), (void*)0);

	GLuint TextID = glGetUniformLocation(m_programSRenderScreen.ID, "renderTexture");
	glUniform1i(TextID, renderedTextureToScreen);
	glActiveTexture(GL_TEXTURE0 + renderedTextureToScreen);
	glBindTexture(GL_TEXTURE_2D, renderedTextureToScreen);



	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), (unsigned char*)NULL + (3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, m_SAQ.numTris * 3 * 2); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo


	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
#endif // DX	void RenderToscreenPass();
}
void CManager::createRenderTaregetLightPass()
{
#ifdef DX
#elif OPENGL
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &renderedTextureLight);
	glBindTexture(GL_TEXTURE_2D, renderedTextureLight);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WindowSize.m_Width, WindowSize.m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, glbit, GL_REPEAT);

	//gen a frame buffer for de texture
	glGenFramebuffers(1, &m_lightPass.m_RendeTargetID);
	//bind the frame buffer
	glGenRenderbuffers(1, &depthrenderbuffer2);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer2);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowSize.m_Width, WindowSize.m_Height);

	glBindFramebuffer(GL_FRAMEBUFFER, m_lightPass.m_RendeTargetID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer2);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTextureLight, 0);
	GLenum DrawBuffers2[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers2);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR CREATE RENDER TARGET";

	glBindTexture(GL_TEXTURE_2D, 0);
#endif // DX

}
void CManager::createRenderTaregetLuminancePass()
{
#ifdef DX
#elif OPENGL
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &renderedTextureLuminance);
	glBindTexture(GL_TEXTURE_2D, renderedTextureLuminance);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WindowSize.m_Width, WindowSize.m_Height, 0, GL_RGBA, GL_FLOAT, NULL);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameterf(renderedTextureLuminance, GL_TEXTURE_MIN_LOD, 0);
	glSamplerParameterf(renderedTextureLuminance, GL_TEXTURE_MAX_LOD,10);
	glSamplerParameterf(renderedTextureLuminance, GL_TEXTURE_LOD_BIAS, 0.0);
	glGenerateMipmap(GL_TEXTURE_2D);
	//glTexParameteri(GL_TEXTURE_2D, glbit, GL_REPEAT);

	//gen a frame buffer for de texture
	glGenFramebuffers(1, &m_LuminancePass.m_RendeTargetID);
	//bind the frame buffer
	glGenRenderbuffers(1, &depthrenderbufferLuminance);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbufferLuminance);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowSize.m_Width, WindowSize.m_Height);

	glBindFramebuffer(GL_FRAMEBUFFER, m_LuminancePass.m_RendeTargetID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbufferLuminance);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTextureLuminance, 0);
	GLenum DrawBuffers2[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers2);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR CREATE RENDER TARGET";

	glBindTexture(GL_TEXTURE_2D, 0);
#endif // DX

}
void CManager::createRenderTaregetBrightPass()
{
#ifdef DX
#elif OPENGL
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &renderedTextureBright);
	glBindTexture(GL_TEXTURE_2D, renderedTextureBright);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WindowSize.m_Width, WindowSize.m_Height, 0, GL_RGBA, GL_FLOAT, NULL);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, glbit, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);
	//gen a frame buffer for de texture
	glGenFramebuffers(1, &m_BrightPass.m_RendeTargetID);
	//bind the frame buffer
	glGenRenderbuffers(1, &depthrenderbufferBright);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbufferBright);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowSize.m_Width, WindowSize.m_Height);

	glBindFramebuffer(GL_FRAMEBUFFER, m_BrightPass.m_RendeTargetID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbufferBright);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTextureBright, 0);
	GLenum DrawBuffers2[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers2);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR CREATE RENDER TARGET";

	glBindTexture(GL_TEXTURE_2D, 0);
#endif // DX

}
void CManager::createRenderTaregetBlur_HPass()
{
#ifdef DX
#elif OPENGL
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &renderedTextureBlur_H);
	glBindTexture(GL_TEXTURE_2D, renderedTextureBlur_H);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WindowSize.m_Width, WindowSize.m_Height, 0, GL_RGBA, GL_FLOAT, NULL);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, glbit, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);
	//gen a frame buffer for de texture
	glGenFramebuffers(1, &m_BlurHPass.m_RendeTargetID);
	//bind the frame buffer
	glGenRenderbuffers(1, &depthrenderbufferBlur_H);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbufferBlur_H);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowSize.m_Width, WindowSize.m_Height);

	glBindFramebuffer(GL_FRAMEBUFFER, m_BlurHPass.m_RendeTargetID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbufferBlur_H);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTextureBlur_H, 0);

	GLenum DrawBuffers2[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers2);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR CREATE RENDER TARGET";

	glBindTexture(GL_TEXTURE_2D, 0);
#endif // DX

}
void CManager::createRenderTaregetBlur_VPass()
{
#ifdef DX
#elif OPENGL
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &renderedTextureBlur_V);
	glBindTexture(GL_TEXTURE_2D, renderedTextureBlur_V);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WindowSize.m_Width, WindowSize.m_Height, 0, GL_RGBA, GL_FLOAT, NULL);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, glbit, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);
	//gen a frame buffer for de texture
	glGenFramebuffers(1, &m_BlurVPass.m_RendeTargetID);
	//bind the frame buffer
	glGenRenderbuffers(1, &depthrenderbufferBlur_V);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbufferBlur_V);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowSize.m_Width, WindowSize.m_Height);

	glBindFramebuffer(GL_FRAMEBUFFER, m_BlurVPass.m_RendeTargetID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbufferBlur_V);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTextureBlur_V, 0);
	GLenum DrawBuffers2[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers2);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR CREATE RENDER TARGET";

	glBindTexture(GL_TEXTURE_2D, 0);
#endif // DX

}
void CManager::createRenderTaregetAddBrightPass()
{
#ifdef DX
#elif OPENGL
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &renderedTextureAddBright);
	glBindTexture(GL_TEXTURE_2D, renderedTextureAddBright);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WindowSize.m_Width, WindowSize.m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, glbit, GL_REPEAT);

	//gen a frame buffer for de texture
	glGenFramebuffers(1, &m_AddBrightPass.m_RendeTargetID);
	//bind the frame buffer
	glGenRenderbuffers(1, &depthrenderbufferAddBright);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbufferAddBright);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowSize.m_Width, WindowSize.m_Height);

	glBindFramebuffer(GL_FRAMEBUFFER, m_AddBrightPass.m_RendeTargetID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbufferAddBright);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTextureAddBright, 0);
	GLenum DrawBuffers2[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers2);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR CREATE RENDER TARGET";

	glBindTexture(GL_TEXTURE_2D, 0);
#endif // DX

}
void CManager::createRenderTaregetTonePass()
{
#ifdef DX
#elif OPENGL
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &renderedTextureToneMap);
	glBindTexture(GL_TEXTURE_2D, renderedTextureToneMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WindowSize.m_Width, WindowSize.m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, glbit, GL_REPEAT);

	//gen a frame buffer for de texture
	glGenFramebuffers(1, &m_ToneMapPass.m_RendeTargetID);
	//bind the frame buffer
	glGenRenderbuffers(1, &depthrenderbufferToneMap);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbufferToneMap);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowSize.m_Width, WindowSize.m_Height);

	glBindFramebuffer(GL_FRAMEBUFFER, m_ToneMapPass.m_RendeTargetID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbufferToneMap);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTextureToneMap, 0);
	GLenum DrawBuffers2[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers2);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR CREATE RENDER TARGET";

	glBindTexture(GL_TEXTURE_2D, 0);
#endif // DX

}