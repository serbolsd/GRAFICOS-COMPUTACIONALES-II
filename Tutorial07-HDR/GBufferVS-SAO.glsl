////////////////////////////////////////////////////////////////////////////////
// Filename: GBuffer.vs
////////////////////////////////////////////////////////////////////////////////
#version 400
//¿
//include "Header.h"
//#define VER_LIGTH
//#define BLINN
//#define DIR_LIGHT
//#define CONE_LIGHT
//#define POINT_LIGHT

layout(location = 0) in vec3 inVertexPosition;
layout(location = 1) in vec2 inTexCoord;

out vec2 TexCoord;


void main()
{
	vec2 texCordbien;
	texCordbien.x = inTexCoord.x;
	texCordbien.y = 1.0f -inTexCoord.y;
	gl_Position = vec4(inVertexPosition.xyz, 1.0f);

	TexCoord = texCordbien;
}