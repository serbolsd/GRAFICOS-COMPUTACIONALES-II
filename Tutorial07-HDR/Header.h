#pragma once

//#define DX 2
#define OPENGL 3
//#define VER_LIGTH
//#define BLINN
#define DIR_LIGHT
#define CONE_LIGHT
#define POINT_LIGHT
#include <vector>
#include <string>
#include <iostream>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <windows.h>
#include <ctime>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include <fstream>
#include <sstream>
using namespace std;

#ifdef DX
#define _XM_NO_INTRINSICS_
#include "imgui/imgui_impl_dx11.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include "resource.h"
#include <assert.h>
#include <xnamath.h> 
#include "comutil.h" 
#elif OPENGL
#include "imgui/imgui_impl_opengl3.h"
#include "imGui_Glut.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/GL.h>
#include "glm.hpp"
#include "SOIL.h"
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
struct myRECT
{
	int m_Width;
	int	m_Height;
};

using namespace std;

using namespace glm;
#endif // DX

