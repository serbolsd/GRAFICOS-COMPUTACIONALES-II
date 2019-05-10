//--------------------------------------------------------------------------------------
// File: Tutorial07.cpp
//
// This application demonstrates texturing
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-------------------------------------------------------------------------------·
#include "Header.h"
#include "CManager.h"


//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
CManager g_manager;

clock_t current_ticks, delta_ticks;
clock_t fps = 0;

#ifdef DX
struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
	XMFLOAT3 Norm;
};

struct CBNeverChanges
{
	XMMATRIX mView;
};

struct CBChangeOnResize
{
	XMMATRIX mProjection;
};

struct CBChangesEveryFrame
{
	XMMATRIX mWorld;
	XMFLOAT4 vMeshColor;
	XMFLOAT4 lightDir;
	XMFLOAT4 vViewPosition;
	//float SpecularPower;                // Specular pow factor
	XMFLOAT4 SpecularColor;               // Specular color (sRGBA)
	XMFLOAT4 DifuseColor;
	XMFLOAT4 SPpower;
};


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
std::string texturas[4] = { "seafloor.dds", "buho.dds", "textura4.dds","miku.dds" };

RECT g_rect;
RECT g_Anterect;


HINSTANCE                           g_hInst = NULL;
HWND                                g_hWnd = NULL;
XMFLOAT4                            g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);
XMFLOAT4                            g_vMeshColorTri(0.7f, 0.7f, 0.7f, 1.0f);
float tim = 0.0f;
XMMATRIX RESULT = XMMatrixIdentity();
#elif OPENGL
struct Vertex
{
	vec3 Position;												/*!< Vertex position */
	vec2 TexCoord;
	vec3 Normals;
};
void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(g_manager.m_camera[g_manager.m_cameraNum].frame, timer, 0);
	glutPostRedisplay();
}
void changeViewPort(int x, int y)
{
	if (y == 0 || x == 0) return;
	glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
	glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
	gluPerspective(39.0, (GLdouble)x / (GLdouble)y, 0.6, 21.0);
	glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world

								// specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
	glViewport(0, 0, x, y);  //Use the whole window for rendering}retr
	
	return;
}
void checkWindowSize()
{
	g_manager.WindowSize.m_Width = glutGet(GLUT_WINDOW_WIDTH);
	g_manager.WindowSize.m_Height = glutGet(GLUT_WINDOW_HEIGHT);
	if (g_manager.WindowSize.m_Width != g_manager.WindowSizeAnte.m_Width || g_manager.WindowSize.m_Height != g_manager.WindowSizeAnte.m_Height)
	{
		if (!(g_manager.WindowSize.m_Width <= 160) || !(g_manager.WindowSize.m_Height <= 28))
		{
			//InitDevice();
			g_manager.resizeCamera(g_manager.WindowSize.m_Width, g_manager.WindowSize.m_Height);
			//g_manager.resizeCamera(g_rect.right - g_rect.left, g_rect.bottom - g_rect.top, g_manager.m_deviceContext.m_pImmediateContext, g_manager.m_buffers.m_pCBChangeOnResize);
			//g_manager.DResize(g_rect.right - g_rect.left, g_rect.bottom - g_rect.top, g_manager.m_deviceContext.m_pImmediateContext, g_manager.m_buffers.m_pCBChangeOnResize);
			//g_manager.resizeCamera(g_rect.right - g_rect.left, g_rect.bottom - g_rect.top, g_manager.m_deviceContextTri.m_pImmediateContext, g_manager.m_deviceContextTri.m_pCBChangeOnResize);
			//g_manager.DResize(g_rect.right - g_rect.left, g_rect.bottom - g_rect.top, g_manager.m_deviceContext.m_pImmediateContext, g_manager.m_deviceContext.m_pCBChangeOnResize);
			//g_manager.DResize(g_rect.right - g_rect.left, g_rect.bottom - g_rect.top, g_manager.m_deviceContextTri.m_pImmediateContext, g_manager.m_deviceContextTri.m_pCBChangeOnResize);
			g_manager.WindowSizeAnte.m_Width = g_manager.WindowSize.m_Width;
			g_manager.WindowSizeAnte.m_Height = g_manager.WindowSize.m_Height;

		}
	}
}
void specialKeys(int key, int x, int y)
{
	if (g_manager.m_cameraNum!=0)
	{
		return;
	}
	// Move Camera
	if (key == GLUT_KEY_RIGHT)		// Derecha
		g_manager.m_camera[g_manager.m_cameraNum].at += g_manager.m_camera[g_manager.m_cameraNum].cameraRight*g_manager.m_camera[g_manager.m_cameraNum].m_speed;
	if (key == GLUT_KEY_LEFT)		// Izquierda
		g_manager.m_camera[g_manager.m_cameraNum].at -= g_manager.m_camera[g_manager.m_cameraNum].cameraRight*g_manager.m_camera[g_manager.m_cameraNum].m_speed;
	if (key == GLUT_KEY_UP)			// Arriba
	{
		g_manager.m_camera[g_manager.m_cameraNum].eye += g_manager.m_camera[g_manager.m_cameraNum].cameraUp*g_manager.m_camera[g_manager.m_cameraNum].m_speed;
		g_manager.m_camera[g_manager.m_cameraNum].at += g_manager.m_camera[g_manager.m_cameraNum].cameraUp*g_manager.m_camera[g_manager.m_cameraNum].m_speed;
	}
	if (key == GLUT_KEY_DOWN)		// Abajo
	{
		g_manager.m_camera[g_manager.m_cameraNum].eye -= g_manager.m_camera[g_manager.m_cameraNum].cameraUp*g_manager.m_camera[g_manager.m_cameraNum].m_speed;
		g_manager.m_camera[g_manager.m_cameraNum].at -= g_manager.m_camera[g_manager.m_cameraNum].cameraUp*g_manager.m_camera[g_manager.m_cameraNum].m_speed;

	}
	glutPostRedisplay();

}

void keyBoardKey(unsigned char key, int x, int y)
{
	if (key == 'c' || key == 'C')
		g_manager.changeCamera();
	if (g_manager.m_cameraNum != 0)
	{
		return;
	}
	if (key == 'D' || key == 'd')		// Derecha
	{
		g_manager.m_camera[g_manager.m_cameraNum].eye += g_manager.m_camera[g_manager.m_cameraNum].cameraRight*g_manager.m_camera[g_manager.m_cameraNum].m_speed;
		g_manager.m_camera[g_manager.m_cameraNum].at += g_manager.m_camera[g_manager.m_cameraNum].cameraRight*g_manager.m_camera[g_manager.m_cameraNum].m_speed;
	}
	if (key == 'A' || key == 'a')		// Izquierda
	{
		//g_manager.m_camera.eye -= g_manager.m_camera.cameraRight*g_manager.m_camera.cameraSpeed;
		g_manager.m_camera[g_manager.m_cameraNum].eye -= g_manager.m_camera[g_manager.m_cameraNum].cameraRight*g_manager.m_camera[g_manager.m_cameraNum].m_speed;
		g_manager.m_camera[g_manager.m_cameraNum].at -= g_manager.m_camera[g_manager.m_cameraNum].cameraRight*g_manager.m_camera[g_manager.m_cameraNum].m_speed;
	}
	if (key == 'W' || key == 'w')	// Zoom adelante
	{
		g_manager.m_camera[g_manager.m_cameraNum].eye -= (g_manager.m_camera[g_manager.m_cameraNum].cameraFront*g_manager.m_camera[g_manager.m_cameraNum].m_speed);
		g_manager.m_camera[g_manager.m_cameraNum].at -= g_manager.m_camera[g_manager.m_cameraNum].cameraFront*g_manager.m_camera[g_manager.m_cameraNum].m_speed;
	}
	if (key == 'S' || key == 's')	// Zoom atras
	{
		g_manager.m_camera[g_manager.m_cameraNum].eye += (g_manager.m_camera[g_manager.m_cameraNum].cameraFront*g_manager.m_camera[g_manager.m_cameraNum].m_speed);
		g_manager.m_camera[g_manager.m_cameraNum].at += g_manager.m_camera[g_manager.m_cameraNum].cameraFront*g_manager.m_camera[g_manager.m_cameraNum].m_speed;
	}
	if (key == 'J' || key == 'j')	// rotation mesh
	{
		if (g_manager.bRotationMesh)
		{
			g_manager.bRotationMesh = false;
		}
		else
		{
			g_manager.bRotationMesh = true;

		}
	}
	if (key == 'I' || key == 'i')//disminuir escala
	{
		for (int i = 0; i < g_manager.m_meshes.size(); i++)
		{
			g_manager.m_meshes[i].escalar -= 0.1;
		}
	}
	if (key == 'O' || key == 'o')//aumentar escala
	{
		for (int i = 0; i < g_manager.m_meshes.size(); i++)
		{
			g_manager.m_meshes[i].escalar += 0.1;
		}
	}
	
	if (key == 'r' || key == 'R')//reset view
		g_manager.resetView();
	if (key == 'm' || key == 'M' || key == 'n' || key == 'N')
		g_manager.changeSpeed(key);//change spped
	if (key == 'f' || key == 'F')
	{
		if (g_manager.bFiestaMode)
			g_manager.bFiestaMode = false;
		else
			g_manager.bFiestaMode = true;
	}
	if (key == 'k' || key == 'K' || key == 'l' || key == 'L')
	{
		g_manager.m_camera[g_manager.m_cameraNum].zoom(key);
	}
	if (key == 'H' || key == 'h')//escalar model
	{
		for (int i = 0; i < g_manager.m_meshes.size(); i++)
		{
			if (g_manager.m_meshes[i].bescalar)
				g_manager.m_meshes[i].bescalar = false;
			else
				g_manager.m_meshes[i].bescalar = true;

		}
	}
	glutPostRedisplay();
}

void mouseInput(int button, int state, int x, int y)
{
	if (g_manager.m_cameraNum != 0)
	{
		return;
	}
	if (button == 0)
	{
		GetCursorPos(&g_manager.m_camera[g_manager.m_cameraNum].m_antePosCur);
		cout << "lefht\n";
		if (state == 0)
		{
			g_manager.bMousePressed = true;
			cout << "estado 0\n";
		}
	}

	if (state == 1)
	{
		g_manager.bMousePressed = false;
		cout << "estado 1\n";
	}
	if (state == 2)
	{
		cout << "estado 2\n";
	}
	glutPostRedisplay();
}
#endif // !DX




//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
#ifdef DX
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );

void drawnm_meshes(XMMATRIX mvp);
#endif // DX

void Render();

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
#ifdef DX
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (FAILED(InitWindow(hInstance, nCmdShow)))
		return 0;

	if (FAILED(InitDevice()))
	{
		CleanupDevice();
		return 0;
	}

	// Main message loop
	MSG msg = { 0 };

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(g_manager.m_device.m_pd3dDevice, g_manager.m_deviceContext.m_pImmediateContext);
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Render();
		}
	}

	CleanupDevice();
	ImGui_ImplDX11_Shutdown();
	//ImGui_ImplFreeGLUT_Shutdown();
	ImGui::DestroyContext();
	return (int)msg.wParam;

	return 0;
}
#elif OPENGL
int main(int argc, char* argv[])
{
	//Initialize(argc, argv);
	//Initialise GLUT with command-line parameters. 
	glutInit(&argc, argv);

	g_manager.initWindow(argc, argv);

	g_manager.WindowSize.m_Width = glutGet(GLUT_WINDOW_WIDTH);
	g_manager.WindowSize.m_Height = glutGet(GLUT_WINDOW_HEIGHT);
	g_manager.WindowSizeAnte.m_Width = g_manager.WindowSize.m_Width;
	g_manager.WindowSizeAnte.m_Height = g_manager.WindowSize.m_Height;
	glewInit();

	g_manager.initDevice();

	glutDisplayFunc(Render);
	glutSpecialFunc(specialKeys);
	glutReshapeFunc(changeViewPort);
	glutKeyboardFunc(keyBoardKey);
	glutMouseFunc(mouseInput);

	IMGUI_CHECKVERSION();
	//	ImGui::CreateContext();
	ImGui::CreateContext();
	HWND Winhandle = FindWindow(NULL, L"OPENGL");
	ImGui_ImplWin32_Init(Winhandle);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();

	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	//return 0;
	//
	//glutMainLoop();
	ImGui_ImplOpenGL3_Shutdown();
	//ImGui_ImplFreeGLUT_Shutdown();
	ImGui::DestroyContext();
	
	
	
	exit(EXIT_SUCCESS);
}
#elif gusanito
int main(int argc, char* argv[])
{
	//Initialize(argc, argv);
	//Initialise GLUT with command-line parameters. 
	glutInit(&argc, argv);

	g_manager.initWindow(argc, argv);

	g_manager.WindowSize.m_Width = glutGet(GLUT_WINDOW_WIDTH);
	g_manager.WindowSize.m_Height = glutGet(GLUT_WINDOW_HEIGHT);
	g_manager.WindowSizeAnte.m_Width = g_manager.WindowSize.m_Width;
	g_manager.WindowSizeAnte.m_Height = g_manager.WindowSize.m_Height;
	glewInit();

	g_manager.initDevice();

	glutDisplayFunc(Render);
	glutSpecialFunc(specialKeys);
	glutReshapeFunc(changeViewPort);
	glutKeyboardFunc(keyBoardKey);
	glutMouseFunc(mouseInput);

	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	//return 0;
	//
	//glutMainLoop();
	exit(EXIT_SUCCESS);
#endif // !DX


void changeTriColor(float t)
{
	//g_vMeshColorTri.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
	//g_vMeshColorTri.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
	//g_vMeshColorTri.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;
#ifdef DX
	g_vMeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
	g_vMeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
	g_vMeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;

#endif // !DX

}

//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
#ifdef DX
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( L"TutorialWindowClass", L"Direct3D 11 Tutorial 7", WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                           NULL );
    if( !g_hWnd )
        return E_FAIL;
	
	//const aiScene * model = aiImportFile("m_meshes/m_meshes.x",aiProcessPreset_TargetRealtime_MaxQuality);
	//std::cout << "num meshe: " << model->mNumMeshes;
	//std::cout << "num meshe: " << model->mMeshes[0].ver;

    ShowWindow( g_hWnd, nCmdShow );
	GetWindowRect(g_hWnd, &g_rect);
	GetWindowRect(g_hWnd, &g_Anterect);
    return S_OK;
}
#endif // !DX


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
#ifdef DX
HRESULT CompileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
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
    hr = D3DX11CompileFromFile( szFileName, NULL, NULL, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}
#endif // DX


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
#ifdef DX
HRESULT InitDevice()
{
	HRESULT hr = S_OK;
	if (!g_manager.DInitDevice(hr, g_hWnd))
	{
		return hr;
	}
    return S_OK;
}

#endif // !DX


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
#ifdef DX
void CleanupDevice()
{
	g_manager.cleanup();

}
#else
#endif // !DX


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
#ifdef DX
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;
	GetWindowRect(g_hWnd, &g_rect);
	if ((g_Anterect.right-g_Anterect.left)!=(g_rect.right-g_rect.left)|| (g_Anterect.bottom - g_Anterect.top) != (g_rect.bottom - g_rect.top))
	{
		if (!((g_rect.right - g_rect.left) <= 160) || !((g_rect.bottom - g_rect.top) <= 28))
		{
			//InitDevice();
			//g_manager.DResize();
			g_manager.resizeCamera(g_rect.right - g_rect.left, g_rect.bottom - g_rect.top);
			g_manager.DResize(g_rect.right - g_rect.left, g_rect.bottom - g_rect.top);
			//g_manager.resizeCamera(g_rect.right - g_rect.left, g_rect.bottom - g_rect.top, g_manager.m_deviceContextTri.m_pImmediateContext, g_manager.m_deviceContextTri.m_pCBChangeOnResize);
			//g_manager.DResize(g_rect.right - g_rect.left, g_rect.bottom - g_rect.top, g_manager.m_deviceContext.m_pImmediateContext, g_manager.m_deviceContext.m_pCBChangeOnResize);
			//g_manager.DResize(g_rect.right - g_rect.left, g_rect.bottom - g_rect.top, g_manager.m_deviceContextTri.m_pImmediateContext, g_manager.m_deviceContextTri.m_pCBChangeOnResize);
			g_Anterect = g_rect;

		}
	}

	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return true;
	}

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;
		case WM_KEYFIRST:
			if (wParam=='X')
			{
				g_manager.textureChange(g_manager.m_texture.m_pTextureRV);
			}
			else if (wParam=='A'|| wParam == 'D'|| wParam == 'S'|| wParam == 'W'||wParam == 37 || wParam == 39 || wParam == 40 || wParam == 38)
			{
				g_manager.moveCamera(wParam);
			}
			else if (wParam == 'R')
			{
				g_manager.resetView();
			}
			else if (wParam == 'C')
			{
				g_manager.changeCamera();
			}
			else if (wParam == 'F')
			{
				if (g_manager.bFiestaMode)
					g_manager.bFiestaMode = false;
				else
					g_manager.bFiestaMode = true;
			}
			else if (wParam == 'I'|| wParam == 'O')
			{
				for (int i = 0; i < g_manager.m_meshes.size(); i++)
				{
					g_manager.m_meshes[i].mesh_escalar(wParam,g_manager.m_meshes[i].Esmatris);
				}
			}
			else if (wParam == 'M' || wParam == 'N')
			{
				g_manager.changeSpeed(wParam);
			}
			else if (wParam == 'K' || wParam == 'L')
			{
				if (wParam == 'K')
					g_manager.m_camera[g_manager.m_cameraNum].zoom('K');
				else
					g_manager.m_camera[g_manager.m_cameraNum].zoom('L');

			}
			else if (wParam == 'J')
			{
				for (int i = 0; i < g_manager.m_meshes.size(); i++)
				{
					if(!g_manager.m_meshes[i].rotateOff)
						g_manager.m_meshes[i].rotateOff = true;
					else
						g_manager.m_meshes[i].rotateOff = false;

				}
			}
			else if (wParam=='H')
			{
				for (int i = 0; i < g_manager.m_meshes.size(); i++)
				{
					if (g_manager.m_meshes[i].bescalar)
						g_manager.m_meshes[i].bescalar = false;
					else
						g_manager.m_meshes[i].bescalar = true;

				}
			}
			break;
		//case WM_LBUTTONDOWN:
		case  WM_LBUTTONDOWN:
				g_manager.rotateCamera();
			break;
		case WM_LBUTTONUP:
			g_manager.m_camera[g_manager.m_cameraNum].isLClick = false;
			break;
        default:
			if (g_manager.dvice_init&&g_manager.m_camera[g_manager.m_cameraNum].isLClick)
			{
				g_manager.rotateCamera();
			}
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}

#endif // !DX
void renderSegurityCam()
{
#ifdef DX


	float ClearColor[4] = { 0.50f, 0.2f, 0.60f, 1.0f }; // red, green, blue, alpha
	g_manager.m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &g_manager.m_renderTargetV2.m_pRenderTargetView, g_manager.m_renderTargetV2.m_pDepthStencilView);
	g_manager.m_deviceContext.m_pImmediateContext->UpdateSubresource(g_manager.m_buffers.m_pCBChangeOnResize, 0, NULL, &g_manager.m_camera[1].m_cbChangesOnResize, 0, 0);
	g_manager.m_deviceContext.m_pImmediateContext->UpdateSubresource(g_manager.m_buffers.m_pCBNeverChanges, 0, NULL, &g_manager.m_camera[1].m_cbNeverChanges, 0, 0);
	//g_manager.m_deviceContext.m_pImmediateContext->UpdateSubresource(g_manager.m_buffers.m_pCBChangeOnResize, 0, NULL, &g_manager.m_camera[1].m_cbChangesOnResize, 0, 0);
	g_manager.m_deviceContext.m_pImmediateContext->ClearRenderTargetView(g_manager.m_renderTargetV2.m_pRenderTargetView, ClearColor);
	g_manager.m_deviceContext.m_pImmediateContext->ClearDepthStencilView(g_manager.m_renderTargetV2.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//XMMATRIX mvp = g_manager.m_camera[1].m_View*g_manager.m_camera[1].m_Projection;
	XMMATRIX mvp = g_manager.m_camera[1].m_View*g_manager.m_camera[1].m_Projection;

	drawnm_meshes(mvp);
	//g_manager.m_swapChain.m_pSwapChain->Present(0, 0);
#elif OPENGL

	glBindFramebuffer(GL_FRAMEBUFFER, g_manager.m_renderTargetV2.m_RendeTargetID);
	g_manager.bRotationMesh = false;
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, g_manager.WindowSize.m_Width, g_manager.WindowSize.m_Width);
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
	//glm::mat4 mvp = g_manager.m_camera[g_manager.numCamera].m_matProjection * g_manager.m_camera[g_manager.numCamera].m_matView * Model;

	GLuint MatrixID = glGetUniformLocation(g_manager.programID, "WM");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &worldMatrix[0][0]);

	MatrixID = glGetUniformLocation(g_manager.programID, "VM");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &g_manager.m_camera[1].m_matView[0][0]);

	MatrixID = glGetUniformLocation(g_manager.programID, "PM");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &g_manager.m_camera[1].m_matProjection[0][0]);



	//for (int i = 0; i < g_manager.m_meshes.size(); i++)
	//{
	//	g_manager.m_meshes[i].escaleModel();
	//
	//	g_manager.m_meshes[i].voidescalar();
	//	MatrixID = glGetUniformLocation(g_manager.programID, "MM");
	//	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &g_manager.m_meshes[i].matModel[0][0]);
	//}

	g_manager.fiesta();

	if (g_manager.bMousePressed)
	{
		g_manager.rotateCamera();
	}



	glClearColor(0.f, 0.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	g_manager.m_camera[g_manager.m_cameraNum].setView();
	g_manager.bRotationMesh = true;
	for (int i = 0; i < g_manager.m_meshes.size(); i++)
	{
		if (g_manager.bRotationMesh)
		{
			g_manager.m_meshes[i].rotation(0);
		}

		g_manager.m_meshes[i].escaleModel();
		g_manager.m_meshes[i].voidescalar();
		MatrixID = glGetUniformLocation(g_manager.programID, "MM");
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &g_manager.m_meshes[i].matModel[0][0]);
		//lBindTexture(GL_TEXTURE_2D, m_TextureArray.at(i)->m_GLHandleID);


		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, g_manager.m_meshes[i].vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);


		GLuint texID = glGetUniformLocation(g_manager.programID, "renderedTexture");

		glUniform1i(texID, g_manager.m_meshes[i].m_texture.m_textureID);
		glActiveTexture(GL_TEXTURE0 + g_manager.m_meshes[i].m_texture.m_textureID);
		//glBindBuffer(GL_ARRAY_BUFFER, g_manager.m_meshes[i].uvBuffer);
		//glActiveTexture(g_manager.m_meshes[i].m_tex.m_textureID);
		glBindTexture(GL_TEXTURE_2D, g_manager.m_meshes[i].m_texture.m_textureID);
		//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, g_manager.WindowSize.m_Width, g_manager.WindowSize.m_Height);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (unsigned char*)NULL + (3 * sizeof(float)));

		glVertexAttribPointer(
			2,                  // atributo 0. No hay razón particular para el 0, pero debe corresponder en el shader.
			3,                  // tamaño
			GL_FLOAT,           // tipo
			GL_FALSE,           // normalizado?
			sizeof(Vertex),                    // Paso
			(unsigned char*)NULL + (5 * sizeof(float))            // desfase del buffer
		);


		glDrawArrays(GL_TRIANGLES, 0, g_manager.m_meshes[i].numTris * 3); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo


		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);

		glDisableVertexAttribArray(0);

	}
	g_manager.bRotationMesh = false;
	for (int i = 0; i < g_manager.m_meshes2.size(); i++)
	{
		//if (g_manager.bRotationMesh)
		//{
		//	g_manager.m_meshes2[i].rotation(0);
		//}

		g_manager.m_meshes2[i].escaleModel();
		g_manager.m_meshes2[i].voidescalar();
		MatrixID = glGetUniformLocation(g_manager.programID, "MM");
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &g_manager.m_meshes2[i].matModel[0][0]);

		//lBindTexture(GL_TEXTURE_2D, m_TextureArray.at(i)->m_GLHandleID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, g_manager.m_meshes2[i].vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		if (g_manager.m_meshes2[i].m_texture.m_textureID != 0)
		{
			GLuint texID = glGetUniformLocation(g_manager.programID, "renderedTexture");

			glUniform1i(texID, g_manager.m_meshes2[i].m_texture.m_textureID);
			glActiveTexture(GL_TEXTURE0 + g_manager.m_meshes2[i].m_texture.m_textureID);
			//glBindBuffer(GL_ARRAY_BUFFER, g_manager.m_meshes[i].uvBuffer);
			//glActiveTexture(g_manager.m_meshes[i].m_tex.m_textureID);
			glBindTexture(GL_TEXTURE_2D, g_manager.m_meshes2[i].m_texture.m_textureID);
			//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, g_manager.WindowSize.m_Width, g_manager.WindowSize.m_Height);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (unsigned char*)NULL + (3 * sizeof(float)));
		}


		glVertexAttribPointer(
			2,                  // atributo 0. No hay razón particular para el 0, pero debe corresponder en el shader.
			3,                  // tamaño
			GL_FLOAT,           // tipo
			GL_FALSE,           // normalizado?
			sizeof(Vertex),                    // Paso
			(unsigned char*)NULL + (5 * sizeof(float))            // desfase del buffer
		);


		glDrawArrays(GL_TRIANGLES, 0, g_manager.m_meshes[i].numTris * 3); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo


		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);

		glDisableVertexAttribArray(0);

	}
#endif // DX
}
void renderFirtsPersonCam()
{
#ifdef DX


	g_manager.m_camera[g_manager.m_cameraNum].setTransform();
	g_manager.m_mesh.posx = -8;
	float ClearColor[4] = { 0.50f, 0.0f, 0.60f, 1.0f }; // red, green, blue, alpha
	g_manager.m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &g_manager.m_renderTargetV.m_pRenderTargetView, g_manager.m_renderTargetV.m_pDepthStencilView);
	g_manager.m_deviceContext.m_pImmediateContext->UpdateSubresource(g_manager.m_buffers.m_pCBChangeOnResize, 0, NULL, &g_manager.m_camera[g_manager.m_cameraNum].m_cbChangesOnResize, 0, 0);
	g_manager.m_deviceContext.m_pImmediateContext->UpdateSubresource(g_manager.m_buffers.m_pCBNeverChanges, 0, NULL, &g_manager.m_camera[g_manager.m_cameraNum].m_cbNeverChanges, 0, 0);
	//g_manager.m_deviceContext.m_pImmediateContext->UpdateSubresource(g_manager.m_buffers.m_pCBChangeOnResize, 0, NULL, &g_manager.m_camera[g_manager.m_cameraNum].m_cbChangesOnResize, 0, 0);

	g_manager.m_deviceContext.m_pImmediateContext->ClearRenderTargetView(g_manager.m_renderTargetV.m_pRenderTargetView, ClearColor);
	g_manager.m_deviceContext.m_pImmediateContext->ClearDepthStencilView(g_manager.m_renderTargetV.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	CBChangesEveryFrame Pantalla;
	CBChangesEveryFrame hands;


	XMMATRIX transformPantalla = XMMATRIX(
		1.0f, 0.0f, 0.0f, 0.0f
		, 0.0f, 1.0f, 0.0f, 0.0f
		, 0.0f, 0.0f, 1.0f, 0.0f
		, 0.0f, 0.0f, 0.0f, 1.0f);

	XMMATRIX transforhands= XMMATRIX(
		.010f, 0.0f, 0.0f, 0.0f
		, 0.0f, .010f, 0.0f, 0.0f
		, 0.0f, 0.0f, .010f, 0.0f
		, 0.0f, 0.0f, 0.00f, .10f);
	g_manager.m_mesh.voidescalar();
	Pantalla.vMeshColor = g_vMeshColorTri;
	Pantalla.mWorld = transformPantalla* g_manager.m_mesh.Esmatris;
	Pantalla.lightDir = g_manager.m_lightDir;
	Pantalla.vViewPosition = { g_manager.m_camera[g_manager.m_cameraNum].m_Eye.x,g_manager.m_camera[g_manager.m_cameraNum].m_Eye.y,g_manager.m_camera[g_manager.m_cameraNum].m_Eye.z,g_manager.m_camera[g_manager.m_cameraNum].m_Eye.w };
	Pantalla.DifuseColor = g_manager.DifuseColor;
	Pantalla.SpecularColor = g_manager.SpecularColor;
	Pantalla.SPpower = g_manager.SPpower;
	//Pantalla.mWorld = transformPantalla * g_manager.m_camera[0].matPosition;

	hands.vMeshColor = {.5,.5,0.2,0};

	hands.mWorld = transforhands;
	hands.lightDir = g_manager.m_lightDir;

	hands.vViewPosition = { g_manager.m_camera[g_manager.m_cameraNum].m_Eye.x,g_manager.m_camera[g_manager.m_cameraNum].m_Eye.y,g_manager.m_camera[g_manager.m_cameraNum].m_Eye.z,g_manager.m_camera[g_manager.m_cameraNum].m_Eye.w };
	hands.DifuseColor = g_manager.DifuseColor;
	hands.SpecularColor = g_manager.SpecularColor;
	hands.SPpower = g_manager.SPpower;

	if (g_manager.m_buffers.m_pVertexBuffer) g_manager.m_buffers.m_pVertexBuffer->Release();
	if (g_manager.m_buffers.m_pIndexBuffer)g_manager.m_buffers.m_pIndexBuffer->Release();

	g_manager.m_deviceContext.m_pImmediateContext->PSSetShaderResources(0, 1, &g_manager.m_hand.m_texture.m_pTextureRV);


	
	//g_manager.m_mesh.mesh_pantalla();
	g_manager.DcreateVertexBuffer(g_manager.m_hand.m_numTries);
	//	DcreateVertexBuffer(3);
	g_manager.m_device.InitData.pSysMem = g_manager.m_hand.m_vertex;
	//g_manager.m_device.InitData.pSysMem = verticestri;
	g_manager.m_device.m_pd3dDevice->CreateBuffer(&g_manager.m_device.bd, &g_manager.m_device.InitData, &g_manager.m_buffers.m_pVertexBuffer);
	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	g_manager.m_deviceContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &g_manager.m_buffers.m_pVertexBuffer, &stride, &offset);
	// Create vertex buffer
	g_manager.DcreateIndexBuffer(g_manager.m_hand.m_numIndex);
	//DcreateIndexBuffer(3);
	g_manager.m_device.InitData.pSysMem = g_manager.m_hand.m_index;
	// g_manager.m_device.InitData.pSysMem = indicesTri;
	g_manager.m_device.m_pd3dDevice->CreateBuffer(&g_manager.m_device.bd, &g_manager.m_device.InitData, &g_manager.m_buffers.m_pIndexBuffer);

	// Set index buffer
	g_manager.m_deviceContext.m_pImmediateContext->IASetIndexBuffer(g_manager.m_buffers.m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	g_manager.m_deviceContext.m_pImmediateContext->UpdateSubresource(g_manager.m_buffers.m_pCBChangesEveryFrame, 0, NULL, &hands, 0, 0);
	g_manager.m_deviceContext.m_pImmediateContext->DrawIndexed(g_manager.m_hand.m_numIndex, 0, 0);


	g_manager.m_deviceContext.m_pImmediateContext->PSSetShaderResources(0, 1, &g_manager.shaderResourceViewMap);
	//g_manager.m_mesh.mesh_pantalla();
	g_manager.DcreateVertexBuffer(g_manager.m_mesh.m_numTries);
	//	DcreateVertexBuffer(3);
	g_manager.m_device.InitData.pSysMem = g_manager.m_mesh.m_vertex;
	//g_manager.m_device.InitData.pSysMem = verticestri;
	g_manager.m_device.m_pd3dDevice->CreateBuffer(&g_manager.m_device.bd, &g_manager.m_device.InitData, &g_manager.m_buffers.m_pVertexBuffer);
	// Set vertex buffer
	stride = sizeof(SimpleVertex);
	offset = 0;
	g_manager.m_deviceContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &g_manager.m_buffers.m_pVertexBuffer, &stride, &offset);
	// Create vertex buffer
	g_manager.DcreateIndexBuffer(g_manager.m_mesh.m_numIndex);
	//DcreateIndexBuffer(3);
	g_manager.m_device.InitData.pSysMem = g_manager.m_mesh.m_index;
	// g_manager.m_device.InitData.pSysMem = indicesTri;
	g_manager.m_device.m_pd3dDevice->CreateBuffer(&g_manager.m_device.bd, &g_manager.m_device.InitData, &g_manager.m_buffers.m_pIndexBuffer);

	// Set index buffer
	g_manager.m_deviceContext.m_pImmediateContext->IASetIndexBuffer(g_manager.m_buffers.m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	g_manager.m_deviceContext.m_pImmediateContext->UpdateSubresource(g_manager.m_buffers.m_pCBChangesEveryFrame, 0, NULL, &Pantalla, 0, 0);
	g_manager.m_deviceContext.m_pImmediateContext->DrawIndexed(g_manager.m_mesh.m_numIndex, 0, 0);
	//XMMATRIX mvp = g_manager.m_camera[g_manager.m_cameraNum].m_View*g_manager.m_camera[g_manager.m_cameraNum].m_Projection;
	XMMATRIX mvp = g_manager.m_camera[g_manager.m_cameraNum].m_View;
	drawnm_meshes(mvp);
#elif OPENGL
	//glBindFramebuffer(GL_FRAMEBUFFER, g_manager.m_renderTarget.m_RendeTargetID);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	g_manager.bRotationMesh = false;
	g_manager.m_camera[g_manager.m_cameraNum].setView();
	glViewport(0, 0, g_manager.WindowSize.m_Width, g_manager.WindowSize.m_Width);

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
	//mvp = g_manager.m_camera[g_manager.numCamera].m_matProjection * g_manager.m_camera[g_manager.numCamera].m_matView * Model;




	GLuint MatrixID = glGetUniformLocation(g_manager.programID, "WM");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &worldMatrix[0][0]);

	MatrixID = glGetUniformLocation(g_manager.programID, "VM");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &g_manager.m_camera[g_manager.m_cameraNum].m_matView[0][0]);

	MatrixID = glGetUniformLocation(g_manager.programID, "PM");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &g_manager.m_camera[0].m_matProjection[0][0]);


	for (int i = 0; i < g_manager.m_meshes.size(); i++)
	{
		g_manager.m_meshes[i].escaleModel();

		g_manager.m_meshes[i].voidescalar();
		MatrixID = glGetUniformLocation(g_manager.programID, "MM");
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &g_manager.m_meshes[i].matModel[0][0]);
	}

	g_manager.fiesta();

	if (g_manager.bMousePressed)
	{
		g_manager.rotateCamera();
	}

	glClearColor(0.0f, 1.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	////glLoadIdentity();
	//
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	g_manager.m_hand.escaleModel();
	//mat4 wordHands = worldMatrix* g_manager.m_camera[0].camTransform;
	mat4 wordHands = worldMatrix;
	//mat4 wordHands =  g_manager.m_camera[0].camMatPos ;
	g_manager.m_hand.voidescalar();
	MatrixID = glGetUniformLocation(g_manager.programID, "WM");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &wordHands[0][0]);


	g_manager.m_hand.matModel *= g_manager.m_camera[0].camTransform;
	g_manager.m_hand.matModel *= translate(vec3(0, 0, 0));
	//g_manager.m_hands.matModel *=g_manager.m_camera[0].camMatRot;
	//g_manager.m_hands.matModel;
	MatrixID = glGetUniformLocation(g_manager.programID, "MM");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &g_manager.m_hand.matModel[0][0]);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, g_manager.m_hand.vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);


	GLuint texID = glGetUniformLocation(g_manager.programID, "renderedTexture");

	glUniform1i(texID, g_manager.m_hand.m_texture.m_textureID);
	glActiveTexture(GL_TEXTURE0 + g_manager.m_hand.m_texture.m_textureID);
	//glBindBuffer(GL_ARRAY_BUFFER, g_manager.m_meshes[i].uvBuffer);
	//glActiveTexture(g_manager.m_meshes[i].m_tex.m_textureID);
	glBindTexture(GL_TEXTURE_2D, g_manager.m_hand.m_texture.m_textureID);
	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, g_manager.WindowSize.m_Width, g_manager.WindowSize.m_Height);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (unsigned char*)NULL + (3 * sizeof(float)));


	glVertexAttribPointer(
		2,                  // atributo 0. No hay razón particular para el 0, pero debe corresponder en el shader.
		3,                  // tamaño
		GL_FLOAT,           // tipo
		GL_FALSE,           // normalizado?
		sizeof(Vertex),                    // Paso
		(unsigned char*)NULL + (5 * sizeof(float))            // desfase del buffer
	);


	glDrawArrays(GL_TRIANGLES, 0, g_manager.m_hand.numTris * 3); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo


	glDisableVertexAttribArray(2); 
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	MatrixID = glGetUniformLocation(g_manager.programID, "WM");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &worldMatrix[0][0]);

	for (int i = 0; i < 1; i++)
	{
		g_manager.m_mesh.escaleModel();

		g_manager.m_mesh.voidescalar();
		MatrixID = glGetUniformLocation(g_manager.programID, "MM");
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &g_manager.m_mesh.matModel[0][0]);
		//lBindTexture(GL_TEXTURE_2D, m_TextureArray.at(i)->m_GLHandleID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, g_manager.m_mesh.vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);


		GLuint texID = glGetUniformLocation(g_manager.programID, "renderedTexture");

		glUniform1i(texID, g_manager.m_mesh.m_texture.m_textureID);
		glActiveTexture(GL_TEXTURE0 + g_manager.m_mesh.m_texture.m_textureID);
		//glBindBuffer(GL_ARRAY_BUFFER, g_manager.m_meshes[i].uvBuffer);
		//glActiveTexture(g_manager.m_meshes[i].m_tex.m_textureID);
		glBindTexture(GL_TEXTURE_2D, g_manager.m_mesh.m_texture.m_textureID);
		//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, g_manager.WindowSize.m_Width, g_manager.WindowSize.m_Height);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (unsigned char*)NULL + (3 * sizeof(float)));




		glVertexAttribPointer(
			2,                  // atributo 0. No hay razón particular para el 0, pero debe corresponder en el shader.
			3,                  // tamaño
			GL_FLOAT,           // tipo
			GL_FALSE,           // normalizado?
			sizeof(Vertex),                    // Paso
			(unsigned char*)NULL + (5 * sizeof(float))            // desfase del buffer
		);


		glDrawArrays(GL_TRIANGLES, 0, g_manager.m_mesh.numTris * 3); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo


		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);

		glDisableVertexAttribArray(0);

	}

	g_manager.bRotationMesh = false;
	for (int i = 0; i < g_manager.m_meshes.size(); i++)
	{
		g_manager.m_meshes[i].escaleModel();

		g_manager.m_meshes[i].voidescalar();
		MatrixID = glGetUniformLocation(g_manager.programID, "MM");
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &g_manager.m_meshes[i].matModel[0][0]);
		//lBindTexture(GL_TEXTURE_2D, m_TextureArray.at(i)->m_GLHandleID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, g_manager.m_meshes[i].vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);


		GLuint texID = glGetUniformLocation(g_manager.programID, "renderedTexture");

		glUniform1i(texID, g_manager.m_meshes[i].m_texture.m_textureID);
		glActiveTexture(GL_TEXTURE0 + g_manager.m_meshes[i].m_texture.m_textureID);
		//glBindBuffer(GL_ARRAY_BUFFER, g_manager.m_meshes[i].uvBuffer);
		//glActiveTexture(g_manager.m_meshes[i].m_tex.m_textureID);
		glBindTexture(GL_TEXTURE_2D, g_manager.m_meshes[i].m_texture.m_textureID);
		//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, g_manager.WindowSize.m_Width, g_manager.WindowSize.m_Height);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (unsigned char*)NULL + (3 * sizeof(float)));

		glVertexAttribPointer(
			2,                  // atributo 0. No hay razón particular para el 0, pero debe corresponder en el shader.
			3,                  // tamaño
			GL_FLOAT,           // tipo
			GL_FALSE,           // normalizado?
			sizeof(Vertex),                    // Paso
			(unsigned char*)NULL + (5 * sizeof(float))            // desfase del buffer
		);


		glDrawArrays(GL_TRIANGLES, 0, g_manager.m_meshes[i].numTris * 3); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo


		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);

		glDisableVertexAttribArray(0);

	}
	g_manager.bRotationMesh = false;
	for (int i = 0; i < g_manager.m_meshes2.size(); i++)
	{
		//if (g_manager.bRotationMesh)
		//{
		//	g_manager.m_meshes2[i].rotation(0);
		//}

		g_manager.m_meshes2[i].escaleModel();
		g_manager.m_meshes2[i].voidescalar();
		MatrixID = glGetUniformLocation(g_manager.programID, "MM");
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &g_manager.m_meshes2[i].matModel[0][0]);

		//lBindTexture(GL_TEXTURE_2D, m_TextureArray.at(i)->m_GLHandleID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, g_manager.m_meshes2[i].vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);


		GLuint texID = glGetUniformLocation(g_manager.programID, "renderedTexture");

		glUniform1i(texID, g_manager.m_meshes2[i].m_texture.m_textureID);
		glActiveTexture(GL_TEXTURE0 + g_manager.m_meshes2[i].m_texture.m_textureID);
		//glBindBuffer(GL_ARRAY_BUFFER, g_manager.m_meshes[i].uvBuffer);
		//glActiveTexture(g_manager.m_meshes[i].m_tex.m_textureID);
		glBindTexture(GL_TEXTURE_2D, g_manager.m_meshes2[i].m_texture.m_textureID);
		//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, g_manager.WindowSize.m_Width, g_manager.WindowSize.m_Height);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (unsigned char*)NULL + (3 * sizeof(float)));

		glVertexAttribPointer(
			2,                  // atributo 0. No hay razón particular para el 0, pero debe corresponder en el shader.
			3,                  // tamaño
			GL_FLOAT,           // tipo
			GL_FALSE,           // normalizado?
			sizeof(Vertex),                    // Paso
			(unsigned char*)NULL + (5 * sizeof(float))            // desfase del buffer
		);


		glDrawArrays(GL_TRIANGLES, 0, g_manager.m_meshes2[i].numTris * 3); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo


		glDisableVertexAttribArray(2); 
		glDisableVertexAttribArray(1);

		glDisableVertexAttribArray(0);

	}


	//ImGui_ImplDX11_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("test");


	float fps = 1000.0f / ImGui::GetIO().Framerate;
	std::string frames = "fps: " + std::to_string(ImGui::GetIO().Framerate) + "\n" + "Num Meshse:" + std::to_string(g_manager.m_meshes.size()+g_manager.m_meshes2.size()+2);
	ImGui::Text(frames.c_str());
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	glutSwapBuffers();
#endif // DX
}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render()
{
	
#ifdef DX
	if (!g_manager.dvice_init)
	{
		return;
	}


	// Update our time
	static float t = 0.0f;
	if (g_manager.m_deviceContext.m_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
		tim = t;
	}
	else
	{
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();
		if (dwTimeStart == 0)
			dwTimeStart = dwTimeCur;
		t = (dwTimeCur - dwTimeStart) / 1000.0f;
		tim = t;
	}
	static float t2 = 0.0f;

	//
	// Clear the back buffer
	//
	float ClearColor[4] = { 0.50f, 0.0f, 0.60f, 1.0f }; // red, green, blue, alpha
	if (g_manager.m_cameraNum == 0)
	{
		//g_manager.m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &g_manager.m_renderTargetV.m_pRenderTargetView, g_manager.m_renderTargetV.m_pDepthStencilView);
	
	}
	if (g_manager.m_cameraNum == 1)
	{
		//g_manager.m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &g_manager.m_renderTargetV2.m_pRenderTargetView, g_manager.m_renderTargetV2.m_pDepthStencilView);
	}
	//g_manager.m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &g_manager.m_renderTargetV.m_pRenderTargetView, g_manager.m_renderTargetV.m_pDepthStencilView);
	//g_manager.m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &g_manager.m_renderTargetV.m_pRenderTargetView, g_manager.m_renderTargetV.m_pDepthStencilView);
	//
	if (g_manager.m_cameraNum == 0)
	{
	//	g_manager.m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &g_manager.m_renderTargetV.m_pRenderTargetView, g_manager.m_renderTargetV.m_pDepthStencilView);
	//	g_manager.m_deviceContext.m_pImmediateContext->ClearRenderTargetView(g_manager.m_renderTargetV.m_pRenderTargetView, ClearColor);
	//	g_manager.m_deviceContext.m_pImmediateContext->ClearDepthStencilView(g_manager.m_renderTargetV.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
	if (g_manager.m_cameraNum == 1)
	{
		//g_manager.m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &g_manager.m_renderTargetV2.m_pRenderTargetView, g_manager.m_renderTargetV2.m_pDepthStencilView);
		//g_manager.m_deviceContext.m_pImmediateContext->ClearDepthStencilView(g_manager.m_renderTargetV2.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		//g_manager.m_deviceContext.m_pImmediateContext->ClearRenderTargetView(g_manager.m_renderTargetV2.m_pRenderTargetView, ClearColor);
	}
	//	g_manager.m_deviceContext.m_pImmediateContext->ClearRenderTargetView(g_manager.m_renderTargetV.m_pRenderTargetView, ClearColor);
	//	g_manager.m_deviceContext.m_pImmediateContext->ClearDepthStencilView(g_manager.m_renderTargetV.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//g_manager.m_deviceContextTri.m_pImmediateContext->ClearRenderTargetView(g_manager.m_swapChainTri.m_pRenderTargetView, ClearColor);

	CBChangesEveryFrame cb;
	CBChangesEveryFrame Tri;
	CBChangesEveryFrame Pantalla;

	//CBChangesEveryFrame cb2;
	//CBChangesEveryFrame cb3;

	XMMATRIX move = XMMATRIX(
		0.250f, 0.0f, 0.0f, 0.0f
		, 0.0f, 0.250f, 0.0f, 0.50f
		, 0.0f, 0.0f, 0.250f, 0.0f
		, 0.0f, 0.0f, 0.0f, 1.0f);

	XMMATRIX transform = XMMATRIX(
		0.50f, 0.0f, 0.0f, 2.0f
		, 0.0f, 0.50f, 0.0f, 0.0f
		, 0.0f, 0.0f, 0.50f, 0.0f
		, 0.0f, 0.0f, 0.0f, 1.0f);
	XMMATRIX escala = XMMATRIX(
		0.50f*sin(t), 0.0f, 0.0f, -2.0f
		, 0.0f, 0.50f*sin(t), 0.0f, 0.0f
		, 0.0f, 0.0f, 0.50f*sin(t), 0.0f
		, 0.0f, 0.0f, 0.0f, 1.0f);

	XMMATRIX moveALL = XMMATRIX(
		0.250f*cos(t), 0.0f, 0.0f, 0.0f
		, 0.0f, 0.250f*cos(t), 0.0f, 1.50f
		, 0.0f, 0.0f, 0.250f*cos(t), 0.0f
		, 0.0f, 0.0f, 0.0f, 1.0f);

	XMMATRIX transformPantalla = XMMATRIX(
		1.0f, 0.0f, 0.0f, 0.0f
		, 0.0f, 1.0f, 0.0f, -1.0f
		, 0.0f, 0.0f, 1.0f, 0.0f
		, 0.0f, 0.0f, 0.0f, 1.0f);

	//cb.mWorld = XMMATRIX( move );
	//cb.mWorld = (move);
	cb.mWorld = (transform)*XMMatrixRotationY(t);

	cb.vMeshColor = g_vMeshColor;
	cb.lightDir = g_manager.m_lightDir;
	Tri.vMeshColor = g_vMeshColorTri;
	Tri.lightDir = g_manager.m_lightDir;
	Pantalla.vMeshColor = g_vMeshColorTri;
	Pantalla.mWorld = transformPantalla;
	Pantalla.lightDir = g_manager.m_lightDir;
	//
	// General Recurse
	//

	g_manager.m_deviceContext.m_pImmediateContext->VSSetShader(g_manager.m_vs.m_pVertexShader, NULL, 0);
	g_manager.m_deviceContext.m_pImmediateContext->VSSetConstantBuffers(0, 1, &g_manager.m_buffers.m_pCBNeverChanges);
	g_manager.m_deviceContext.m_pImmediateContext->VSSetConstantBuffers(1, 1, &g_manager.m_buffers.m_pCBChangeOnResize);
	g_manager.m_deviceContext.m_pImmediateContext->VSSetConstantBuffers(2, 1, &g_manager.m_buffers.m_pCBChangesEveryFrame);
	g_manager.m_deviceContext.m_pImmediateContext->PSSetShader(g_manager.m_ps.m_pPixelShader, NULL, 0);
	g_manager.m_deviceContext.m_pImmediateContext->PSSetConstantBuffers(2, 1, &g_manager.m_buffers.m_pCBChangesEveryFrame);
	//g_manager.m_deviceContext.m_pImmediateContext->PSSetShaderResources(0, 1, &g_manager.m_texture.m_pTextureRV);
	g_manager.m_deviceContext.m_pImmediateContext->PSSetSamplers(0, 1, &g_manager.m_texture.m_pSamplerLinear);
	g_manager.m_deviceContext.m_pImmediateContext->UpdateSubresource(g_manager.m_buffers.m_pCBChangeOnResize, 0, NULL, &g_manager.m_camera[g_manager.m_cameraNum].m_cbChangesOnResize, 0, 0);

	renderSegurityCam();

	renderFirtsPersonCam();
	
	//drawnm_meshes();
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("test");
	delta_ticks = clock() - current_ticks; //the time, in ms, that took to render the scene



	std::string frames = "fps: " + std::to_string(ImGui::GetIO().Framerate) + "\n" + "Num Meshse:" + std::to_string(g_manager.m_meshes.size() + g_manager.m_meshes2.size() + 2);
	ImGui::Text(frames.c_str());
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    g_manager.m_swapChain.m_pSwapChain->Present( 0, 0 );

#elif OPENGL

	GLuint LDID = glGetUniformLocation(g_manager.programID, "LD");
	glUniform4fv(LDID, 1, &g_manager.ligthDir[0]);
	GLuint VPID = glGetUniformLocation(g_manager.programID, "VP");
	glUniform4fv(VPID, 1, &g_manager.m_camera[g_manager.m_cameraNum].eye[0]);
	GLuint SCID = glGetUniformLocation(g_manager.programID, "SC");
	glUniform4fv(SCID, 1, &g_manager.SpecularColor[0]);
	GLuint DCID = glGetUniformLocation(g_manager.programID, "DC");
	glUniform4fv(DCID, 1, &g_manager.DifuseColor[0]);
	GLuint SPID = glGetUniformLocation(g_manager.programID, "SP");
	glUniform4fv(SPID, 1, &g_manager.SPpower[0]);
	checkWindowSize();
	
	renderSegurityCam();
	
	renderFirtsPersonCam();
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, g_manager.WindowSize.m_Width, g_manager.WindowSize.m_Width);
	glutPostRedisplay();
#endif // !DX
}

#ifdef DX
void drawnm_meshes(XMMATRIX mvp)
{
	for (int i = 0; i < g_manager.m_meshes.size(); i++)
	{
		static float t = 0.0f;
		g_manager.m_meshes[i].posx = -2;
//		g_manager.m_meshes[i].bescalar=false;
		g_manager.m_meshes[i].bescalar=false;
		if (g_manager.m_deviceContext.m_driverType == D3D_DRIVER_TYPE_REFERENCE && !g_manager.m_meshes[i].rotateOff)
		{
			t += (float)XM_PI * 0.0125f;
			tim = t;
		}
		else if (!g_manager.m_meshes[i].rotateOff)
		{
			//static DWORD dwTimeStart = 0;
			//DWORD dwTimeCur = GetTickCount();
			//if (dwTimeStart == 0)
			//	dwTimeStart = dwTimeCur;
			//t = (dwTimeCur - dwTimeStart) / 1000.0f;
			t += (float)XM_PI * 0.0005f;
			tim = t;
		}
		g_manager.m_meshes[i].rotation(t);
		CBChangesEveryFrame m_meshes;
		if (g_manager.m_buffers.m_pVertexBuffer) g_manager.m_buffers.m_pVertexBuffer->Release();
		if (g_manager.m_buffers.m_pIndexBuffer)g_manager.m_buffers.m_pIndexBuffer->Release();
		g_manager.m_deviceContext.m_pImmediateContext->PSSetShaderResources(0, 1, &g_manager.m_meshes[i].m_texture.m_pTextureRV);

		//g_manager.m_mesh.mesh_pantalla();
		g_manager.DcreateVertexBuffer(g_manager.m_meshes[i].m_numTries);
		//	DcreateVertexBuffer(3);
		g_manager.m_device.InitData.pSysMem = g_manager.m_meshes[i].m_vertex;
		//g_manager.m_device.InitData.pSysMem = verticestri;
		g_manager.m_device.m_pd3dDevice->CreateBuffer(&g_manager.m_device.bd, &g_manager.m_device.InitData, &g_manager.m_buffers.m_pVertexBuffer);

		// Set vertex buffer
		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;
		g_manager.m_deviceContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &g_manager.m_buffers.m_pVertexBuffer, &stride, &offset);

		// Create index buffer
		// Create vertex buffer
		g_manager.DcreateIndexBuffer(g_manager.m_meshes[i].m_numIndex);
		//DcreateIndexBuffer(3);
		g_manager.m_device.InitData.pSysMem = g_manager.m_meshes[i].m_index;
		// g_manager.m_device.InitData.pSysMem = indicesTri;
		g_manager.m_device.m_pd3dDevice->CreateBuffer(&g_manager.m_device.bd, &g_manager.m_device.InitData, &g_manager.m_buffers.m_pIndexBuffer);
		// Set index buffer
		g_manager.m_deviceContext.m_pImmediateContext->IASetIndexBuffer(g_manager.m_buffers.m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		RESULT = g_manager.m_meshes[i].Esmatris*g_manager.m_meshes[i].matrixRotation;
		//m_meshes.mWorld = g_manager.m_meshes[i].Esmatris;
		//m_meshes.mWorld *= g_manager.m_meshes[i].matrixRotation;
		g_manager.m_meshes[i].voidescalar();
		m_meshes.mWorld = RESULT* g_manager.m_meshes[i].Escar;
		//m_meshes.mWorld *=mvp;
		//m_meshes.mWorld = g_manager.m_meshes[i].Esmatris*g_manager.m_meshes[i].matrixRotation;
		//m_meshes.vMeshColor = g_vMeshColorTri;
		g_manager.fiesta();
		m_meshes.vMeshColor = g_manager.color;
		m_meshes.lightDir = g_manager.m_lightDir;

		m_meshes.vViewPosition = { g_manager.m_camera[g_manager.m_cameraNum].m_Eye.x,g_manager.m_camera[g_manager.m_cameraNum].m_Eye.y,g_manager.m_camera[g_manager.m_cameraNum].m_Eye.z,g_manager.m_camera[g_manager.m_cameraNum].m_Eye.w };
		m_meshes.DifuseColor = g_manager.DifuseColor;
		m_meshes.SpecularColor = g_manager.SpecularColor;
		m_meshes.SPpower = g_manager.SPpower;

		g_manager.m_deviceContext.m_pImmediateContext->UpdateSubresource(g_manager.m_buffers.m_pCBChangesEveryFrame, 0, NULL, &m_meshes, 0, 0);
		g_manager.m_deviceContext.m_pImmediateContext->DrawIndexed(g_manager.m_meshes[i].m_numIndex, 0, 0);
	}

	for (int i = 0; i < g_manager.m_meshes2.size(); i++)
	{
		static float t = 0.0f;
		g_manager.m_meshes2[i].rotateOff = false;
		g_manager.m_meshes2[i].posx = 2;
		if (g_manager.m_deviceContext.m_driverType == D3D_DRIVER_TYPE_REFERENCE && !g_manager.m_meshes2[i].rotateOff)
		{
			t += (float)XM_PI * 0.0125f;
			tim = t;
		}
		else if (!g_manager.m_meshes2[i].rotateOff)
		{
			//static DWORD dwTimeStart = 0;
			//DWORD dwTimeCur = GetTickCount();
			//if (dwTimeStart == 0)
			//	dwTimeStart = dwTimeCur;
			//t = (dwTimeCur - dwTimeStart) / 1000.0f;
			t += (float)XM_PI * 0.0005f;
			tim = t;
		}
		g_manager.m_meshes2[i].rotation(t);
		CBChangesEveryFrame m_meshes;
		if (g_manager.m_buffers.m_pVertexBuffer) g_manager.m_buffers.m_pVertexBuffer->Release();
		if (g_manager.m_buffers.m_pIndexBuffer)g_manager.m_buffers.m_pIndexBuffer->Release();
		g_manager.m_deviceContext.m_pImmediateContext->PSSetShaderResources(0, 1, &g_manager.m_meshes2[i].m_texture.m_pTextureRV);

		//g_manager.m_mesh.mesh_pantalla();
		g_manager.DcreateVertexBuffer(g_manager.m_meshes2[i].m_numTries);
		//	DcreateVertexBuffer(3);
		g_manager.m_device.InitData.pSysMem = g_manager.m_meshes2[i].m_vertex;
		//g_manager.m_device.InitData.pSysMem = verticestri;
		g_manager.m_device.m_pd3dDevice->CreateBuffer(&g_manager.m_device.bd, &g_manager.m_device.InitData, &g_manager.m_buffers.m_pVertexBuffer);

		// Set vertex buffer
		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;
		g_manager.m_deviceContext.m_pImmediateContext->IASetVertexBuffers(0, 1, &g_manager.m_buffers.m_pVertexBuffer, &stride, &offset);

		// Create index buffer
		// Create vertex buffer
		g_manager.DcreateIndexBuffer(g_manager.m_meshes2[i].m_numIndex);
		//DcreateIndexBuffer(3);
		g_manager.m_device.InitData.pSysMem = g_manager.m_meshes2[i].m_index;
		// g_manager.m_device.InitData.pSysMem = indicesTri;
		g_manager.m_device.m_pd3dDevice->CreateBuffer(&g_manager.m_device.bd, &g_manager.m_device.InitData, &g_manager.m_buffers.m_pIndexBuffer);
		// Set index buffer
		g_manager.m_deviceContext.m_pImmediateContext->IASetIndexBuffer(g_manager.m_buffers.m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		RESULT = g_manager.m_meshes2[i].Esmatris*g_manager.m_meshes2[i].matrixRotation;
		//m_meshes.mWorld = g_manager.m_meshes[i].Esmatris;
		//m_meshes.mWorld *= g_manager.m_meshes[i].matrixRotation;
		g_manager.m_meshes2[i].voidescalar();
		m_meshes.mWorld = RESULT * g_manager.m_meshes2[i].Escar;
		
		//m_meshes.mWorld *= XMMatrixTranslation(1,0,0);
		//m_meshes.mWorld *=mvp;
		//m_meshes.mWorld = g_manager.m_meshes[i].Esmatris*g_manager.m_meshes[i].matrixRotation;
		//m_meshes.vMeshColor = g_vMeshColorTri;
		g_manager.fiesta();
		m_meshes.vMeshColor = g_manager.color;
		m_meshes.lightDir = g_manager.m_lightDir;

		m_meshes.vViewPosition = { g_manager.m_camera[g_manager.m_cameraNum].m_Eye.x,g_manager.m_camera[g_manager.m_cameraNum].m_Eye.y,g_manager.m_camera[g_manager.m_cameraNum].m_Eye.z,g_manager.m_camera[g_manager.m_cameraNum].m_Eye.w };
		m_meshes.DifuseColor = g_manager.DifuseColor;
		m_meshes.SpecularColor = g_manager.SpecularColor;
		m_meshes.SPpower = g_manager.SPpower;

		g_manager.m_deviceContext.m_pImmediateContext->UpdateSubresource(g_manager.m_buffers.m_pCBChangesEveryFrame, 0, NULL, &m_meshes, 0, 0);
		g_manager.m_deviceContext.m_pImmediateContext->DrawIndexed(g_manager.m_meshes2[i].m_numIndex, 0, 0);
	}
}
#else
#endif // !DX