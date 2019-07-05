////////////////////////////////////////////////////////////////////////////////
// Filename: GBuffer.ps
////////////////////////////////////////////////////////////////////////////////
#version 400
#define M_PI 3.14159265359f;

in vec2 TexCoord;
//out vec3 color;
//out vec3 color;
uniform sampler2D LuminanceTexture;
uniform sampler2D lightTexture;
uniform float BrightLod;         //luminance mip leveñ
uniform float BloomThreshold;


layout(location = 0) out vec4 color;
void main()
{
	vec4  col = texture2D(lightTexture, TexCoord.xy);
	//float brighti = vec4(TexCoord, 0.0f, BrightLod).x;
	//float mipmapLevel = textureQueryLod(LuminanceTexture, TexCoord).x;
	float bright = textureLod(LuminanceTexture, vec2(TexCoord.xy),BrightLod).x;
	bright = max(bright - BloomThreshold, 0.0f);

	//color = vec4(1,0,0,0);
	color = col*vec4(bright, bright, bright, bright);
	//color * vec4(bright, bright, bright, bright);

}