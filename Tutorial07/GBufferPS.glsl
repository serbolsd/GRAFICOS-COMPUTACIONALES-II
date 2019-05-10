////////////////////////////////////////////////////////////////////////////////
// Filename: GBuffer.ps
////////////////////////////////////////////////////////////////////////////////
#version 400
#define M_PI 3.14159265359f;
//#define VER_LIGTH
in vec2 TexCoord;
in vec4 colorfiesta;
in vec4 wsNormal;
in vec4 wsPos;
//out vec3 color;
//out vec3 color;
uniform sampler2D renderedTexture;

uniform vec4 LD;//light Direction
uniform vec4 VP;//View Position
uniform vec4 SC;//Specular Color
uniform vec4 DC;//Difuse Color
uniform vec4 SP;//speular Power 

layout(location = 0) out vec3 color;
void main() 
{
#ifdef VER_LIGTH
	color = texture(renderedTexture, TexCoord).rgb;
	color = color * colorfiesta.rgb;
	//color = vec3( 1.0,0.5,0.3 );
#else
	vec4 lightDirWS = normalize(-LD);
	float Ndl = dot(lightDirWS, wsNormal);
	vec4 NDLR = vec4(Ndl, Ndl, Ndl, Ndl);
	
	vec3 wsViewDir = -normalize((wsPos.xyz - VP.xyz));
	vec3 wsReflect = normalize(reflect(-lightDirWS.xyz, wsNormal.xyz));
	float wsVdR = max(0.0f, dot(wsViewDir.xyz, wsReflect.xyz));
	float SpecularFactor = pow(wsVdR, SP.x) * Ndl;

	//colorfiesta = fiesta* NDLR;
	color = vec4(NDLR * DC).xyz;
	color += vec4(vec4(SpecularFactor, SpecularFactor, SpecularFactor, SpecularFactor)*SC).xyz;
	color *= texture(renderedTexture, TexCoord).rgb;
#endif
}