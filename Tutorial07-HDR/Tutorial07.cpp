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
	XMFLOAT3 tan;
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
	XMFLOAT4 DifuseColor;                 // Difuse color (sRGBA)
	XMFLOAT4 AmbientalColor;
	XMFLOAT4 SPpower;
	XMFLOAT4 KDASL;
	XMFLOAT4 LPLQ;
	XMFLOAT4 lightPosition;
	XMFLOAT4 SLCOC;
};


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
std::string texturas[4] = { "seafloor.dds", "buho.dds", "textura4.dds","miku.dds" };

RECT g_rect;
RECT g_Anterect;


HINSTANCE                           g_hInst = NULL;

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
	vec3 tangs;
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
			g_manager.WindowSizeAnte.m_Width = g_manager.WindowSize.m_Width;
			g_manager.WindowSizeAnte.m_Height = g_manager.WindowSize.m_Height;
			g_manager.fViewportDimensions = vec2(g_manager.WindowSize.m_Width, g_manager.WindowSize.m_Height);
			g_manager.DResize(g_manager.WindowSize.m_Width, g_manager.WindowSize.m_Height);

		}
	}
}

void specialKeys(int key, int x, int y)
{
	//if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	//{
	//	return true;
	//}
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
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureKeyboard)
	{
		ImGui_ImplGLUT_KeyboardFunc(key,x,y);
		return;
	}


	if (key == 'c' || key == 'C')
		g_manager.changeCamera();
	if (g_manager.m_cameraNum != 0)
	{
		return;
	}
	if (key == 'D' || key == 'd')		// Derecha
	{
		if (key=='d')
		{
			g_manager.ligthPosition.z += .1;
		}
		else
		{
			g_manager.m_camera[g_manager.m_cameraNum].eye += g_manager.m_camera[g_manager.m_cameraNum].cameraRight*g_manager.m_camera[g_manager.m_cameraNum].m_speed;
			g_manager.m_camera[g_manager.m_cameraNum].at += g_manager.m_camera[g_manager.m_cameraNum].cameraRight*g_manager.m_camera[g_manager.m_cameraNum].m_speed;
		}
	}
	if (key == 'A' || key == 'a')		// Izquierda
	{
		if (key == 'a')
		{
			g_manager.ligthPosition.z -= .1;
		}
		else
		{
			//g_manager.m_camera.eye -= g_manager.m_camera.cameraRight*g_manager.m_camera.cameraSpeed;
			g_manager.m_camera[g_manager.m_cameraNum].eye -= g_manager.m_camera[g_manager.m_cameraNum].cameraRight*g_manager.m_camera[g_manager.m_cameraNum].m_speed;
			g_manager.m_camera[g_manager.m_cameraNum].at -= g_manager.m_camera[g_manager.m_cameraNum].cameraRight*g_manager.m_camera[g_manager.m_cameraNum].m_speed;

		}
	}
	if (key == 'W' || key == 'w')	// Zoom adelante
	{
		if (key == 'w')
		{
			g_manager.ligthPosition.y += .1;
		}
		else
		{
			g_manager.m_camera[g_manager.m_cameraNum].eye -= (g_manager.m_camera[g_manager.m_cameraNum].cameraFront*g_manager.m_camera[g_manager.m_cameraNum].m_speed);
			g_manager.m_camera[g_manager.m_cameraNum].at -= g_manager.m_camera[g_manager.m_cameraNum].cameraFront*g_manager.m_camera[g_manager.m_cameraNum].m_speed;

		}
	}
	if (key == 'S' || key == 's')	// Zoom atras
	{
		if (key == 's')
		{
			g_manager.ligthPosition.y -= .1;
		}
		else
		{
			g_manager.m_camera[g_manager.m_cameraNum].eye += (g_manager.m_camera[g_manager.m_cameraNum].cameraFront*g_manager.m_camera[g_manager.m_cameraNum].m_speed);
			g_manager.m_camera[g_manager.m_cameraNum].at += g_manager.m_camera[g_manager.m_cameraNum].cameraFront*g_manager.m_camera[g_manager.m_cameraNum].m_speed;

		}
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
	if (key == 'z' )
	{
		g_manager.changeSpecularPower(0);
	}
	if ( key == 'x')
	{
		g_manager.changeSpecularPower(1);
	}
	if (key == 'Z')
	{
		g_manager.SLCOC.x -= 0.1;
	}
	if (key == 'X')
	{
		g_manager.SLCOC.x += 0.1;
	}
	glutPostRedisplay();
}


void mouseInput(int button, int state, int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
	{
		ImGui_ImplGLUT_MouseFunc(button, state,  x, y);
		g_manager.bMousePressed = false;
		return;
	}
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
HWND  g_hWnd = NULL;




//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
#ifdef DX
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );

void drawnm_meshes(XMMATRIX mvp);
#elif OPENGL



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
	
	IMGUI_CHECKVERSION();
	//	ImGui::CreateContext();
	ImGui::CreateContext();
	HWND Winhandle = FindWindow(NULL, L"OPENGL");
	ImGui_ImplWin32_Init(Winhandle);
	//WNDCLASSEX wcex;
	//wcex.lpfnWndProc = WndProc;

	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	glewInit();
	ImGui_ImplGLUT_Init();
	//GetWindow(g_hWnd,);
	g_manager.initDevice();
	//ImGui_ImplGLUT_InstallFuncs();
	glutDisplayFunc(Render);
	glutSpecialFunc(specialKeys);
	glutReshapeFunc(changeViewPort);
	//glutKeyboardFunc(keyBoardKey);
	glutKeyboardFunc(keyBoardKey);
	glutMouseFunc(mouseInput);



	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	//return 0;
	//
	//glutMainLoop();
	ImGui_ImplOpenGL3_Shutdown();
	//ImGui_ImplFreeGLUT_Shutdown();
	ImGui::DestroyContext();
	for (size_t i = 0; i < g_manager.m_meshes.size(); i++)
	{
		delete g_manager.m_meshes[i].buffer;

	}
	
	exit(EXIT_SUCCESS);
}

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
    RECT rc = { 0, 0, 1920, 1080 };
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	GetWindowRect(g_hWnd, &g_rect);
	if ((g_Anterect.right - g_Anterect.left) != (g_rect.right - g_rect.left) || (g_Anterect.bottom - g_Anterect.top) != (g_rect.bottom - g_rect.top))
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
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
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
			if (wParam=='Y')
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
			else if (wParam == 'Z')
			{
				g_manager.changeSpecularPower(0);
			}
			else if (wParam == 'X')
			{
				g_manager.changeSpecularPower(1);

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

#elif OPENGL

#endif // !DX
void renderSegurityCam()
{
#ifdef DX


	float ClearColor[4] = { 0.50f, 0.2f, 0.60f, 1.0f }; // red, green, blue, alpha
	g_manager.m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &g_manager.m_lightPass.m_pRenderTargetView, g_manager.m_lightPass.m_pDepthStencilView);
	g_manager.m_deviceContext.m_pImmediateContext->UpdateSubresource(g_manager.m_buffers.m_pCBChangeOnResize, 0, NULL, &g_manager.m_camera[1].m_cbChangesOnResize, 0, 0);
	g_manager.m_deviceContext.m_pImmediateContext->UpdateSubresource(g_manager.m_buffers.m_pCBNeverChanges, 0, NULL, &g_manager.m_camera[1].m_cbNeverChanges, 0, 0);
	//g_manager.m_deviceContext.m_pImmediateContext->UpdateSubresource(g_manager.m_buffers.m_pCBChangeOnResize, 0, NULL, &g_manager.m_camera[1].m_cbChangesOnResize, 0, 0);
	g_manager.m_deviceContext.m_pImmediateContext->ClearRenderTargetView(g_manager.m_lightPass.m_pRenderTargetView, ClearColor);
	g_manager.m_deviceContext.m_pImmediateContext->ClearDepthStencilView(g_manager.m_lightPass.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//XMMATRIX mvp = g_manager.m_camera[1].m_View*g_manager.m_camera[1].m_Projection;
	XMMATRIX mvp = g_manager.m_camera[1].m_View*g_manager.m_camera[1].m_Projection;

	drawnm_meshes(mvp);
	//g_manager.m_swapChain.m_pSwapChain->Present(0, 0);
#elif OPENGL

	//glBindFramebuffer(GL_FRAMEBUFFER, g_manager.m_renderTargetV2.m_RendeTargetID);
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

	GLuint MatrixID = glGetUniformLocation(g_manager.m_programSActual.ID, "WM");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &worldMatrix[0][0]);

	MatrixID = glGetUniformLocation(g_manager.m_programSActual.ID, "VM");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &g_manager.m_camera[1].m_matView[0][0]);

	MatrixID = glGetUniformLocation(g_manager.m_programSActual.ID, "PM");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &g_manager.m_camera[1].m_matProjection[0][0]);



	//for (int i = 0; i < g_manager.m_meshes.size(); i++)
	//{
	//	g_manager.m_meshes[i].escaleModel();
	//
	//	g_manager.m_meshes[i].voidescalar();
	//	MatrixID = glGetUniformLocation(g_manager.ActualProgramID, "MM");
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
	g_manager.bRotationMesh = false;
	for (int i = 0; i < g_manager.m_meshes.size(); i++)
	{
		if (g_manager.bRotationMesh)
		{
			g_manager.m_meshes[i].rotation(0);
		}

		g_manager.m_meshes[i].escaleModel();
		g_manager.m_meshes[i].voidescalar();
		MatrixID = glGetUniformLocation(g_manager.m_programSActual.ID, "MM");
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &g_manager.m_meshes[i].matModel[0][0]);
		//lBindTexture(GL_TEXTURE_2D, m_TextureArray.at(i)->m_GLHandleID);


		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, g_manager.m_meshes[i].vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);


		GLuint texID = glGetUniformLocation(g_manager.m_programSActual.ID, "renderedTexture");

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
	
#endif // DX
}
void renderFirtsPersonCam()
{
#ifdef DX


	g_manager.m_camera[g_manager.m_cameraNum].setTransform();

	float ClearColor[4] = { 0.50f, 0.0f, 0.60f, 1.0f }; // red, green, blue, alpha
	g_manager.m_deviceContext.m_pImmediateContext->OMSetRenderTargets(1, &g_manager.m_renderTargetV.m_pRenderTargetView, g_manager.m_renderTargetV.m_pDepthStencilView);
	g_manager.m_deviceContext.m_pImmediateContext->UpdateSubresource(g_manager.m_buffers.m_pCBChangeOnResize, 0, NULL, &g_manager.m_camera[g_manager.m_cameraNum].m_cbChangesOnResize, 0, 0);
	g_manager.m_deviceContext.m_pImmediateContext->UpdateSubresource(g_manager.m_buffers.m_pCBNeverChanges, 0, NULL, &g_manager.m_camera[g_manager.m_cameraNum].m_cbNeverChanges, 0, 0);
	//g_manager.m_deviceContext.m_pImmediateContext->UpdateSubresource(g_manager.m_buffers.m_pCBChangeOnResize, 0, NULL, &g_manager.m_camera[g_manager.m_cameraNum].m_cbChangesOnResize, 0, 0);

	g_manager.m_deviceContext.m_pImmediateContext->ClearRenderTargetView(g_manager.m_renderTargetV.m_pRenderTargetView, ClearColor);
	g_manager.m_deviceContext.m_pImmediateContext->ClearDepthStencilView(g_manager.m_renderTargetV.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


	if (g_manager.m_buffers.m_pVertexBuffer) g_manager.m_buffers.m_pVertexBuffer->Release();
	if (g_manager.m_buffers.m_pIndexBuffer)g_manager.m_buffers.m_pIndexBuffer->Release();

	//drawnm_meshes(mvp);
#elif OPENGL

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


	//renderFirtsPersonCam();
	g_manager.LightPass();
	g_manager.LuminancePass();
	g_manager.BrightPass();
	g_manager.Blur_HPass();
	g_manager.Blur_VPass();
	g_manager.AddBrightPass();
	g_manager.tonePass();
	g_manager.RenderToscreenPass();
	//drawnm_meshes();
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("test");
	delta_ticks = clock() - current_ticks; //the time, in ms, that took to render the scene
	std::string frames = "fps: " + std::to_string(ImGui::GetIO().Framerate) + "\n" + "Num Meshse:" + std::to_string(g_manager.m_meshes.size());
	ImGui::Text(frames.c_str());

	//
	//DIRECTIONAL LIGHT
	//
	ImGui::Text("DIRECTIONAL LIGHT");
	if (ImGui::Button("I/O DIRECTIONA LIGHT"))
	{
		if (g_manager.SLDPS.x == 1)
			g_manager.SLDPS.x = 0;
		else
			g_manager.SLDPS.x = 1;
	}
	if (ImGui::Button("ON/OFF ROTATE DIRECTIONA LIGHT"))
	{
		if (g_manager.rotateDirLight)
			g_manager.rotateDirLight = false;
		else
			g_manager.rotateDirLight = true;
	}

	float color[4] = { g_manager.SpecularColorDir.x, g_manager.SpecularColorDir.y, g_manager.SpecularColorDir.z, g_manager.SpecularColorDir.w };
	ImGui::ColorEdit4("Specular Color D", color);
	g_manager.SpecularColorDir = { color[0],color[1],color[2],color[3] };
	float dcolor[4] = { g_manager.DifuseColorDir.x, g_manager.DifuseColorDir.y, g_manager.DifuseColorDir.z, g_manager.DifuseColorDir.w };
	ImGui::ColorEdit4("Difuse Color D", dcolor);
	g_manager.DifuseColorDir = { dcolor[0],dcolor[1],dcolor[2],dcolor[3] };
	float *AD = new float(g_manager.ADPS.x);
	ImGui::SliderFloat("Attenuation DIRECTIONAL", AD, 10, .01);
	g_manager.ADPS.x = *AD;
	delete AD;
	//
	//POINT LIGHT
	//
	ImGui::Text("POINT LIGHT");
	if (ImGui::Button("I/O PONT LIGHT"))
	{
		if (g_manager.SLDPS.y == 1)
			g_manager.SLDPS.y = 0;
		else
			g_manager.SLDPS.y = 1;
	}
	float color2[4] = { g_manager.SpecularColorPoint.x, g_manager.SpecularColorPoint.y, g_manager.SpecularColorPoint.z, g_manager.SpecularColorPoint.w };
	ImGui::ColorEdit4("Specular Color P", color2);
	g_manager.SpecularColorPoint = { color2[0],color2[1],color2[2],color2[3] };
	float dcolor2[4] = { g_manager.DifuseColorPoint.x, g_manager.DifuseColorPoint.y, g_manager.DifuseColorPoint.z, g_manager.DifuseColorPoint.w };
	ImGui::ColorEdit4("Difuse Color P", dcolor2);
	g_manager.DifuseColorPoint = { dcolor2[0],dcolor2[1],dcolor2[2],dcolor2[3] };
	//float *KL = new float(g_manager.KDASL.w);
	//ImGui::SliderFloat("CONSTANTE LIGHT", KL, 0, 20);
	//g_manager.KDASL.w = *KL;
	//delete KL;
	//float *KPL = new float(g_manager.LPLQLD.x);
	//ImGui::SliderFloat("CONSTANTE LINEAR", KPL, 0, 20);
	//g_manager.LPLQLD.x = *KPL;
	//g_manager.LPLQLD.y = g_manager.LPLQLD.x*g_manager.LPLQLD.x;
	//delete KPL;
	float *AP = new float(g_manager.ADPS.y);
	ImGui::SliderFloat("Attenuation POINT", AP, 10, .01);
	g_manager.ADPS.y = *AP;
	delete AP;
	float *MDL = new float(g_manager.LPLQLD.z);
	ImGui::SliderFloat("Max Dist", MDL, 0, 20);
	g_manager.LPLQLD.z = *MDL;
	delete MDL;

	//
	//SPOT LIGHT
	//
	ImGui::Text("SPOT LIGHT");
	if (ImGui::Button("I/O SPOT LIGHT"))
	{
		cout << "si entra\n";
		if (g_manager.SLDPS.z == 1)
			g_manager.SLDPS.z = 0;
		else
			g_manager.SLDPS.z = 1;
	}
	float color3[4] = { g_manager.SpecularColorSpot.x, g_manager.SpecularColorSpot.y, g_manager.SpecularColorSpot.z, g_manager.SpecularColorSpot.w };
	ImGui::ColorEdit4("Specular Color S", color3);
	g_manager.SpecularColorSpot = { color3[0],color3[1],color3[2],color3[3] };
	float dcolor3[4] = { g_manager.DifuseColorSpot.x, g_manager.DifuseColorSpot.y, g_manager.DifuseColorSpot.z, g_manager.DifuseColorSpot.w };
	ImGui::ColorEdit4("Difuse Color S", dcolor3);
	g_manager.DifuseColorSpot = { dcolor3[0],dcolor3[1],dcolor3[2],dcolor3[3] };

	float *x = new float(g_manager.SLCOC.x);
	ImGui::SliderFloat("SPOT ALPHA", x, 0, 1);
	g_manager.SLCOC.x = *x;
	delete x;
	float *y = new float(g_manager.SLCOC.y);
	ImGui::SliderFloat("SPOT BETA", y, 0, 1);
	g_manager.SLCOC.y = *y;
	delete y;
	float *AS = new float(g_manager.ADPS.z);
	ImGui::SliderFloat("Attenuation Spot", AS, 10, .01);
	g_manager.ADPS.z = *AS;
	delete AS;
	//
	//CONSTANTES DIFUSE, AMBIENT, SPECULAR
	//
	ImGui::Text("CONSTANTS DIFUSE, AMBIENT, SPECULAR");
	float *KD = new float(g_manager.KDASL.x);
	ImGui::SliderFloat("CONSTANTE DIFUSE", KD, -1, 1);
	g_manager.KDASL.x = *KD;
	delete KD;
	float *KA = new float(g_manager.KDASL.y);
	ImGui::SliderFloat("CONSTANTE AMBIENTAL", KA, 0.001, 0.010);
	g_manager.KDASL.y = *KA;
	delete KA;
	float *KS = new float(g_manager.KDASL.z);
	ImGui::SliderFloat("CONSTANTE SPECULAR", KS, -20, 30);
	g_manager.KDASL.z = *KS;
	delete KS;
	//
	//Shader Programs;
	//
	ImGui::Text("SHADER POGRAMS");
	if (ImGui::Button("VERTEX LIGHT BLINN-PHONG"))
	{
		g_manager.m_programSActual = g_manager.m_programSVertexBP;
	}
	if (ImGui::Button("VERTEX LIGHT BLINN"))
	{
		g_manager.m_programSActual = g_manager.m_programSVertexB;
	}
	if (ImGui::Button("PIXEL LIGHT BLINN-PHONG"))
	{
		g_manager.m_programSActual = g_manager.m_programSPixelBP;
	}
	if (ImGui::Button("PIXEL LIGHT BLINN"))
	{
		g_manager.m_programSActual = g_manager.m_programSPixelB;
	}
	//
	//render Screen;
	//
	if (ImGui::Button("LightPass"))
	{
		for (int i = 0; i < 7; i++)
		{
			g_manager.passActual[i] = 0;
		}
		g_manager.passActual[0] = 1;
		g_manager.SResourceActual = g_manager.SResourceLightPass;
	}
	if (ImGui::Button("LuminancePass"))
	{
		for (int i = 0; i < 7; i++)
		{
			g_manager.passActual[i] = 0;
		}
		g_manager.passActual[1] = 1;
		g_manager.SResourceActual = g_manager.SResourceLuminancePass;
	}
	if (ImGui::Button("BrightPass"))
	{
		for (int i = 0; i < 7; i++)
		{
			g_manager.passActual[i] = 0;
		}
		g_manager.passActual[2] = 1;
		g_manager.SResourceActual = g_manager.SResourceBrightPass;
	}
	if (ImGui::Button("Blur-HPass"))
	{
		for (int i = 0; i < 7; i++)
		{
			g_manager.passActual[i] = 0;
		}
		g_manager.passActual[3] = 1;
		g_manager.SResourceActual = g_manager.SResourceBlurHPass;
	}
	if (ImGui::Button("Blur-VPass"))
	{
		for (int i = 0; i < 7; i++)
		{
			g_manager.passActual[i] = 0;
		}
		g_manager.passActual[4] = 1;
		g_manager.SResourceActual = g_manager.SResourceBlurVPass;
	}
	if (ImGui::Button("AddBrightPass"))
	{
		for (int i = 0; i < 7; i++)
		{
			g_manager.passActual[i] = 0;
		}
		g_manager.passActual[5] = 1;
		g_manager.SResourceActual = g_manager.SResourceAddBrightPass;
	}
	if (ImGui::Button("TonePass"))
	{
		for (int i = 0; i < 7; i++)
		{
			g_manager.passActual[i] = 0;
		}
		g_manager.passActual[6] = 1;
		g_manager.SResourceActual = g_manager.SResourceToneMapPass;
	}
	int *iBrightLod = new int(g_manager.BrightLod);
	ImGui::SliderInt("BrightLod", iBrightLod, 0, 10);
	g_manager.BrightLod = *iBrightLod;
	delete iBrightLod;
	float *fBrightLod = new float(g_manager.BloomThreshold);
	ImGui::SliderFloat("BloomThreshold", fBrightLod, 0, 1);
	g_manager.BloomThreshold = *fBrightLod;
	delete fBrightLod;
	int *iblurh = new int(g_manager.blurHmip);
	ImGui::SliderInt("Blur-Hmip", iblurh, 0, 10);
	g_manager.blurHmip = *iblurh;
	delete iblurh;
	int *iblurv = new int(g_manager.blurVmip);
	ImGui::SliderInt("Blur-Vmip", iblurv, 0, 10);
	g_manager.blurVmip = *iblurv;
	delete iblurv;
	int *iblurAddBmip = new int(g_manager.AddBmip);
	ImGui::SliderInt("blurAddBmip", iblurAddBmip, 0, 10);
	g_manager.AddBmip = *iblurAddBmip;
	delete iblurAddBmip;
	float *fExposure = new float(g_manager.Exposure);
	ImGui::SliderFloat("Exposure", fExposure, 0, 10);
	g_manager.Exposure = *fExposure;
	delete fExposure;
	float *fBloomMuiltiplier = new float(g_manager.BloomMuiltiplier);
	ImGui::SliderFloat("BloomMuiltiplier", fBloomMuiltiplier, 0, 50);
	g_manager.BloomMuiltiplier = *fBloomMuiltiplier;
	delete fBloomMuiltiplier;
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	g_manager.m_swapChain.m_pSwapChain->Present(0, 0);

#elif OPENGL
if (g_manager.rotateDirLight)
{
	g_manager.time += 0.01f;
}
	glUseProgram(g_manager.m_programSActual.ID);
	g_manager.ligthDir = vec4(cos(g_manager.time),0,sin(g_manager.time),0);
	GLuint LDID = glGetUniformLocation(g_manager.m_programSActual.ID, "LD");
	glUniform4fv(LDID, 1, &g_manager.ligthDir[0]);
	GLuint VPID = glGetUniformLocation(g_manager.m_programSActual.ID, "VP");
	glUniform4fv(VPID, 1, &g_manager.m_camera[g_manager.m_cameraNum].eye[0]);
	GLuint LPID = glGetUniformLocation(g_manager.m_programSActual.ID, "LP");
	glUniform4fv(LPID, 1, &g_manager.ligthPosition[0]);
	GLuint SCDID = glGetUniformLocation(g_manager.m_programSActual.ID, "SCD");
	glUniform4fv(SCDID, 1, &g_manager.SpecularColorDir[0]);
	GLuint SCPID = glGetUniformLocation(g_manager.m_programSActual.ID, "SCP");
	glUniform4fv(SCPID, 1, &g_manager.SpecularColorPoint[0]);
	GLuint SCSID = glGetUniformLocation(g_manager.m_programSActual.ID, "SCS");
	glUniform4fv(SCSID, 1, &g_manager.SpecularColorSpot[0]);
	GLuint LPLQID = glGetUniformLocation(g_manager.m_programSActual.ID, "LPLQLD");
	glUniform4fv(LPLQID, 1, &g_manager.LPLQLD[0]);
	GLuint DCDID = glGetUniformLocation(g_manager.m_programSActual.ID, "DCD");
	glUniform4fv(DCDID, 1, &g_manager.DifuseColorDir[0]);
	GLuint DCPID = glGetUniformLocation(g_manager.m_programSActual.ID, "DCP");
	glUniform4fv(DCPID, 1, &g_manager.DifuseColorPoint[0]);
	GLuint DCSID = glGetUniformLocation(g_manager.m_programSActual.ID, "DCS");
	glUniform4fv(DCSID, 1, &g_manager.DifuseColorSpot[0]);
	GLuint SPID = glGetUniformLocation(g_manager.m_programSActual.ID, "SP");
	glUniform4fv(SPID, 1, &g_manager.SPpower[0]);
	GLuint ACID = glGetUniformLocation(g_manager.m_programSActual.ID, "AC");
	glUniform4fv(ACID, 1, &g_manager.ambientColor[0]);
	GLuint KDASID = glGetUniformLocation(g_manager.m_programSActual.ID, "KDASL");
	glUniform4fv(KDASID, 1, &g_manager.KDASL[0]);
	GLuint SLCOCID = glGetUniformLocation(g_manager.m_programSActual.ID, "SLCOC");
	glUniform4fv(SLCOCID, 1, &g_manager.SLCOC[0]);
	GLuint SLDPSID = glGetUniformLocation(g_manager.m_programSActual.ID, "SLDPS");
	glUniform4fv(SLDPSID, 1, &g_manager.SLDPS[0]);
	GLuint ADPSID = glGetUniformLocation(g_manager.m_programSActual.ID, "ADPS");
	glUniform4fv(ADPSID, 1, &g_manager.ADPS[0]);

	checkWindowSize();
	
	//renderSegurityCam();

	g_manager.LightPass();
	g_manager.LuminancePass();
	g_manager.BrightPass();
	g_manager.Blur_HPass();
	g_manager.Blur_VPass();
	g_manager.AddBrightPass();
	g_manager.tonePass();
	g_manager.RenderToscreenPass();


	//ImGui_ImplDX11_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("test");


	float fps = 1000.0f / ImGui::GetIO().Framerate;
	std::string frames = "fps: " + std::to_string(ImGui::GetIO().Framerate) + "\n" + "Num Meshse:" + std::to_string(g_manager.m_meshes.size());
	ImGui::Text(frames.c_str());
	//
	//DIRECTIONAL LIGHT
	//
	ImGui::Text("DIRECTIONAL LIGHT");
	if (ImGui::Button("I/O DIRECTIONA LIGHT"))
	{
		if (g_manager.SLDPS.x == 1)
			g_manager.SLDPS.x = 0;
		else
			g_manager.SLDPS.x = 1;
	}
	if (ImGui::Button("ON/OFF ROTATE DIRECTIONA LIGHT"))
	{
		if (g_manager.rotateDirLight)
			g_manager.rotateDirLight = false;
		else
			g_manager.rotateDirLight = true;
	}

	float color[4] = { g_manager.SpecularColorDir.x, g_manager.SpecularColorDir.y, g_manager.SpecularColorDir.z, g_manager.SpecularColorDir.w };
	ImGui::ColorEdit4("SC Direction", color);
	g_manager.SpecularColorDir = { color[0],color[1],color[2],color[3] };
	float dcolor[4] = { g_manager.DifuseColorDir.x, g_manager.DifuseColorDir.y, g_manager.DifuseColorDir.z, g_manager.DifuseColorDir.w };
	ImGui::ColorEdit4("Difuse Color D", dcolor);
	g_manager.DifuseColorDir = { dcolor[0],dcolor[1],dcolor[2],dcolor[3] };
	float *AD = new float(g_manager.ADPS.x);
	ImGui::SliderFloat("Attenuation DIRECTIONAL", AD, 10, .01);
	g_manager.ADPS.x = *AD;
	delete AD;
	//
	//POINT LIGHT
	//
	ImGui::Text("POINT LIGHT");
	if (ImGui::Button("I/O PONT LIGHT"))
	{
		if (g_manager.SLDPS.y == 1)
			g_manager.SLDPS.y = 0;
		else
			g_manager.SLDPS.y = 1;
	}
	float color2[4] = { g_manager.SpecularColorPoint.x, g_manager.SpecularColorPoint.y, g_manager.SpecularColorPoint.z, g_manager.SpecularColorPoint.w };
	ImGui::ColorEdit4("SC Point", color2);
	g_manager.SpecularColorPoint = { color2[0],color2[1],color2[2],color2[3] };
	float dcolor2[4] = { g_manager.DifuseColorPoint.x, g_manager.DifuseColorPoint.y, g_manager.DifuseColorPoint.z, g_manager.DifuseColorPoint.w };
	ImGui::ColorEdit4("Difuse Color P", dcolor2);
	g_manager.DifuseColorPoint = { dcolor2[0],dcolor2[1],dcolor2[2],dcolor2[3] };
	float *KL = new float(g_manager.KDASL.w);
	ImGui::SliderFloat("CONSTANTE LIGHT", KL, 0, 20);
	g_manager.KDASL.w = *KL;
	delete KL;
	//float *KPL = new float(g_manager.LPLQLD.x);
	//ImGui::SliderFloat("CONSTANTE LINEAR", KPL, 0, 20);
	//g_manager.LPLQLD.x = *KPL;
	//g_manager.LPLQLD.y = g_manager.LPLQLD.x*g_manager.LPLQLD.x;
	//delete KPL;
	float *AP = new float(g_manager.ADPS.y);
	ImGui::SliderFloat("Attenuation POINT", AP, 10, .01);
	g_manager.ADPS.y = *AP;
	delete AP;
	float *MDL = new float(g_manager.LPLQLD.z);
	ImGui::SliderFloat("Max Dist", MDL, 0, 20);
	g_manager.LPLQLD.z = *MDL;
	delete MDL;

	//
	//SPOT LIGHT
	//
	ImGui::Text("SPOT LIGHT");
	if (ImGui::Button("I/O SPOT LIGHT"))
	{
		cout << "si entra\n";
		if (g_manager.SLDPS.z == 1)
			g_manager.SLDPS.z = 0;
		else
			g_manager.SLDPS.z = 1;
	}
	float color3[4] = { g_manager.SpecularColorSpot.x, g_manager.SpecularColorSpot.y, g_manager.SpecularColorSpot.z, g_manager.SpecularColorSpot.w };
	ImGui::ColorEdit4("SC Spot", color3);
	g_manager.SpecularColorSpot = { color3[0],color3[1],color3[2],color3[3] };
	float dcolor3[4] = { g_manager.DifuseColorSpot.x, g_manager.DifuseColorSpot.y, g_manager.DifuseColorSpot.z, g_manager.DifuseColorSpot.w };
	ImGui::ColorEdit4("Difuse Color S", dcolor3);
	g_manager.DifuseColorSpot = { dcolor3[0],dcolor3[1],dcolor3[2],dcolor3[3] };
	float *x = new float(g_manager.SLCOC.x);
	ImGui::SliderFloat("SPOT ALPHA", x, 0, 1);
	g_manager.SLCOC.x = *x;
	delete x;
	float *y = new float(g_manager.SLCOC.y);
	ImGui::SliderFloat("SPOT BETA", y, 0, 1);
	g_manager.SLCOC.y = *y;
	delete y;
	float *AS = new float(g_manager.ADPS.z);
	ImGui::SliderFloat("Attenuation Spot", AS, 10, .01);
	g_manager.ADPS.z = *AS;
	delete AS;
	//
	//CONSTANTES DIFUSE, AMBIENT, SPECULAR
	//
	ImGui::Text("CONSTANTS DIFUSE, AMBIENT, SPECULAR");
	float *KD = new float(g_manager.KDASL.x);
	ImGui::SliderFloat("CONSTANTE DIFUSE", KD, -1, 1);
	g_manager.KDASL.x = *KD;
	delete KD;
	float *KA = new float(g_manager.KDASL.y);
	ImGui::SliderFloat("CONSTANTE AMBIENTAL", KA, 0.001, 0.010);
	g_manager.KDASL.y = *KA;
	delete KA;
	float *KS = new float(g_manager.KDASL.z);
	ImGui::SliderFloat("CONSTANTE SPECULAR", KS, -20, 30);
	g_manager.KDASL.z = *KS;
	delete KS;
	//
	//Shader Programs;
	//
	ImGui::Text("SHADER POGRAMS");
	if (ImGui::Button("VERTEX LIGHT BLINN-PHONG"))
	{
		g_manager.m_programSActual.ID = g_manager.m_programSVertexBP.ID;
	}
	if (ImGui::Button("VERTEX LIGHT BLINN"))
	{
		g_manager.m_programSActual.ID = g_manager.m_programSVertexB.ID;
	}
	if (ImGui::Button("PIXEL LIGHT BLINN-PHONG"))
	{
		g_manager.m_programSActual.ID = g_manager.m_programSPixelBP.ID;
	}
	if (ImGui::Button("PIXEL LIGHT BLINN"))
	{
		g_manager.m_programSActual.ID = g_manager.m_programSPixelB.ID;
	}

	//
	//render Screen;
	//
	if (ImGui::Button("LightPass"))
	{
		for (int i = 0; i < 7; i++)
		{
			g_manager.passActual[i] = 0;
		}
		g_manager.passActual[0] = 1;
		g_manager.renderedTextureToScreen = g_manager.renderedTextureLight;
	}
	if (ImGui::Button("LuminancePass"))
	{
		for (int i = 0; i < 7; i++)
		{
			g_manager.passActual[i] = 0;
		}
		g_manager.passActual[1] = 1;
		g_manager.renderedTextureToScreen = g_manager.renderedTextureLuminance;
	}
	if (ImGui::Button("BrightPass"))
	{
		for (int i = 0; i < 7; i++)
		{
			g_manager.passActual[i] = 0;
		}
		g_manager.passActual[2] = 1;
		g_manager.renderedTextureToScreen = g_manager.renderedTextureBright;
	}
	if (ImGui::Button("Blur-HPass"))
	{
		for (int i = 0; i < 7; i++)
		{
			g_manager.passActual[i] = 0;
		}
		g_manager.passActual[3] = 1;
		g_manager.renderedTextureToScreen = g_manager.renderedTextureBlur_H;
	}
	if (ImGui::Button("Blur-VPass"))
	{

		for (int i = 0; i < 7; i++)
		{
			g_manager.passActual[i] = 0;
		}
		g_manager.passActual[4] = 1;
		g_manager.renderedTextureToScreen = g_manager.renderedTextureBlur_V;

	}
	if (ImGui::Button("AddBrightPass"))
	{
		for (int i = 0; i < 7; i++)
		{
			g_manager.passActual[i] = 0;
		}
		g_manager.passActual[5] = 1;
		g_manager.renderedTextureToScreen = g_manager.renderedTextureAddBright;
	}
	if (ImGui::Button("ToneMap"))
	{
		for (int i = 0; i < 7; i++)
		{
			g_manager.passActual[i] = 0;
		}
		g_manager.passActual[6] = 1;
		g_manager.renderedTextureToScreen = g_manager.renderedTextureToneMap;
	}
	int *iBrightLod = new int(g_manager.BrightLod);
	ImGui::SliderInt("BrightLod", iBrightLod, 0, 10);
	g_manager.BrightLod = *iBrightLod;
	delete iBrightLod;
	float *fBrightLod = new float(g_manager.BloomThreshold);
	ImGui::SliderFloat("BloomThreshold", fBrightLod, 0, 1);
	g_manager.BloomThreshold = *fBrightLod;
	delete fBrightLod;
	int *iblurh = new int(g_manager.blurHmip);
	ImGui::SliderInt("Blur-Hmip", iblurh, 0, 10);
	g_manager.blurHmip = *iblurh;
	delete iblurh;
	int *iblurv = new int(g_manager.blurVmip);
	ImGui::SliderInt("Blur-Vmip", iblurv, 0, 10);
	g_manager.blurVmip = *iblurv;
	delete iblurv;
	int *iblurAddBmip = new int(g_manager.AddBmip);
	ImGui::SliderInt("blurAddBmip", iblurAddBmip, 0, 10);
	g_manager.AddBmip = *iblurAddBmip;
	delete iblurAddBmip;
	float *fExposure = new float(g_manager.Exposure);
	ImGui::SliderFloat("Exposure", fExposure, 0, 10);
	g_manager.Exposure = *fExposure;
	delete fExposure;
	float *fBloomMuiltiplier = new float(g_manager.BloomMuiltiplier);
	ImGui::SliderFloat("BloomMuiltiplier", fBloomMuiltiplier, 0, 100);
	g_manager.BloomMuiltiplier = *fBloomMuiltiplier;
	delete fBloomMuiltiplier;

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glutSwapBuffers();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, g_manager.WindowSize.m_Width, g_manager.WindowSize.m_Width);

	glutPostRedisplay();
#endif // !DX
}

#ifdef DX
void drawnm_meshes(XMMATRIX mvp)
{
	//g_manager.time += (float)XM_PI * 0.005f;
	g_manager.time +=  0.005f;
	g_manager.m_lightDir = {cos(g_manager.time),0, sin(g_manager.time),0 };
	//g_manager.m_lightDir = {0,0,-1,0 };
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
		//g_manager.m_meshes[i].rotation(t);
		CBChangesEveryFrame m_meshes;
		if (g_manager.m_buffers.m_pVertexBuffer) g_manager.m_buffers.m_pVertexBuffer->Release();
		//if (g_manager.m_buffers.m_pIndexBuffer) g_manager.m_buffers.m_pIndexBuffer->Release();
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
		//RESULT = g_manager.m_meshes[i].Esmatris*g_manager.m_meshes[i].matrixRotation;
		//m_meshes.mWorld = g_manager.m_meshes[i].Esmatris;
		//m_meshes.mWorld *= g_manager.m_meshes[i].matrixRotation;
		//g_manager.m_meshes[i].voidescalar();
		m_meshes.mWorld = XMMatrixIdentity();
		//m_meshes.mWorld *=mvp;
		//m_meshes.mWorld = g_manager.m_meshes[i].Esmatris*g_manager.m_meshes[i].matrixRotation;
		//m_meshes.vMeshColor = g_vMeshColorTri;
		g_manager.fiesta();
		m_meshes.vMeshColor = g_manager.color;
		m_meshes.lightDir = g_manager.m_lightDir;

		m_meshes.vViewPosition = { g_manager.m_camera[g_manager.m_cameraNum].m_View._41,-g_manager.m_camera[g_manager.m_cameraNum].m_View._42,g_manager.m_camera[g_manager.m_cameraNum].m_View._43,g_manager.m_camera[g_manager.m_cameraNum].m_View._44};
		//m_meshes.vViewPosition = { g_manager.m_camera[g_manager.m_cameraNum].m_View._14,-g_manager.m_camera[g_manager.m_cameraNum].m_View._24,g_manager.m_camera[g_manager.m_cameraNum].m_View._34,g_manager.m_camera[g_manager.m_cameraNum].m_View._44};

		//m_meshes.DifuseColor = g_manager.DifuseColor;
		//m_meshes.DifuseColor = g_manager.DifuseColor;
		//m_meshes.DifuseColor = g_manager.DifuseColor;
		//m_meshes.SpecularColor = g_manager.SpecularColor;
		m_meshes.SPpower = g_manager.SPpower;
		m_meshes.AmbientalColor = g_manager.ambientColor;
		m_meshes.KDASL = g_manager.KDASL;
		m_meshes.lightPosition=g_manager.ligthPosition;
		m_meshes.SLCOC=g_manager.SLCOC;
		m_meshes.LPLQ=g_manager.LPLQLD;



		g_manager.m_deviceContext.m_pImmediateContext->UpdateSubresource(g_manager.m_buffers.m_pCBChangesEveryFrame, 0, NULL, &m_meshes, 0, 0);
		g_manager.m_deviceContext.m_pImmediateContext->DrawIndexed(g_manager.m_meshes[i].m_numIndex, 0, 0);
	}

	
}
#else
#endif // !DX