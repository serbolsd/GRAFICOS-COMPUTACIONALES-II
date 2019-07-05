////////////////////////////////////////////////////////////////////////////////
// Filename: GBuffer.ps
////////////////////////////////////////////////////////////////////////////////
#version 400
#define M_PI 3.14159265359f;

in vec2 TexCoord;
in vec4 colorfiesta;
//out vec3 color;
//out vec3 color;
uniform sampler2D renderedTexture;

layout(location = 0) out vec3 color;
void main() 
{
	//Luminance Factor
	vec4 LuminanceFactor = vec4(0.2f, 0.587f, 0.15f, 1.0f);

	//Texture color Sampler
	vec4 col = texture2D(renderedTexture, TexCoord.xy);

	// Calcule Luminance
	float Luminance = dot(col, LuminanceFactor);
	Luminance = log(Luminance + 0.000001f);

	color =vec3(Luminance, Luminance, Luminance);
	//color = texture(renderedTexture, TexCoord).rgb;
	//color = color * colorfiesta.rgb;
	//color = vec3( 1.0,0.5,0.3 );

}