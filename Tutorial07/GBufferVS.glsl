////////////////////////////////////////////////////////////////////////////////
// Filename: GBuffer.vs
////////////////////////////////////////////////////////////////////////////////
#version 400

//#define VER_LIGTH

layout(location = 0) in vec3 inVertexPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;


out vec2 TexCoord;
out vec4 colorfiesta;
out vec4 wsNormal;
out vec4 wsPos;
//out vec4 wPosition;
//out mat4 wPosition;

uniform mat4 WM;//World Matrix
uniform mat4 VM;//View Matrix
uniform mat4 PM;//Projection Matrix
uniform mat4 MM;//Model Matrxi
uniform vec4 LD;//light Direction
uniform vec4 VP;//View Position
uniform vec4 SC;//Specular Color
uniform vec4 DC;//Difuse Color
uniform vec4 SP;//speular Power 
uniform vec4 fiesta;

void main()
{
	mat4 MVP = PM * VM * MM;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	gl_Position = MVP * WM * vec4(inVertexPosition, 1.0f);

	// Calculate position in world space
	//wPosition = WM * vec4(inVertexPosition, 1.0f);

	// Store the texture coordinates for the pixel shader.
	TexCoord = inTexCoord;

	//vec4 normalWS = vec4(inNormal,1)*WM;
	vec4 posWS = vec4(inVertexPosition, 1)*WM;
	vec4 normalWS = vec4(inNormal,1)*WM;
#ifdef VER_LIGTH

	vec4 lightDirWS = normalize(-LD);
	float Ndl = dot(lightDirWS, normalWS);
	vec4 NDLR = vec4(Ndl, Ndl, Ndl, Ndl);
	vec3 wsViewDir = -normalize((posWS.xyz - VP.xyz));
	vec3 wsReflect = normalize(reflect(-lightDirWS.xyz, normalWS.xyz));
	float wsVdR = max(0.0f, dot(wsViewDir.xyz, wsReflect.xyz));
	float SpecularFactor = pow(wsVdR, SP.x) * Ndl;

	//colorfiesta = fiesta* NDLR;
	colorfiesta = NDLR*DC;
	colorfiesta += vec4(SpecularFactor, SpecularFactor, SpecularFactor, SpecularFactor)*SC;
#else
	wsPos = posWS;
	wsNormal = normalWS;
#endif
	//printf(TexCoord);
}