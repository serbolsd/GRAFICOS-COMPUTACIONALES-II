////////////////////////////////////////////////////////////////////////////////
// Filename: GBuffer.vs
////////////////////////////////////////////////////////////////////////////////
#version 400


layout(location = 0) in vec3 inVertexPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;


out vec2 TexCoord;
out vec4 colorfiesta;
//out vec4 wPosition;
//out mat4 wPosition;

uniform mat4 WM;
uniform mat4 VM;
uniform mat4 PM;
uniform mat4 MM;
uniform vec4 LD;
uniform vec4 fiesta;

void main()
{
	mat4 VP = PM * VM * MM;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	gl_Position = VP * WM * vec4(inVertexPosition, 1.0f);

	// Calculate position in world space
	//wPosition = WM * vec4(inVertexPosition, 1.0f);

	// Store the texture coordinates for the pixel shader.
	TexCoord = inTexCoord;

	vec4 normalWS = vec4(inNormal,1)*WM;
	vec4 lightDirWS = normalize(-LD);
	float Ndl = dot(lightDirWS, normalWS);
	vec4 NDLR = vec4(Ndl, Ndl, Ndl, Ndl);

	colorfiesta = fiesta* NDLR;
	//printf(TexCoord);
}