////////////////////////////////////////////////////////////////////////////////
// Filename: GBuffer.ps
////////////////////////////////////////////////////////////////////////////////
#version 400
#define M_PI 3.14159265359f;

in vec2 TexCoord;
//out vec3 color;
//out vec3 color;
uniform sampler2D BrightTexture;
uniform vec2 fViewportDimensions;
uniform int blurMip;         //luminance mip leve
layout(location = 0) out vec4 color;


void main()
{
	vec2 OFFSETS0 = vec2(0, -4); ///pixel sampler
	vec2 OFFSETS1 = vec2(0, -3); ///pixel sampler
	vec2 OFFSETS2 = vec2(0, -2); ///pixel sampler
	vec2 OFFSETS3 = vec2(0, -1); ///pixel sampler
	vec2 OFFSETS4 = vec2(0, 0); ///pixel sampler
	vec2 OFFSETS5 = vec2(0, 1); ///pixel sampler
	vec2 OFFSETS6 = vec2(0, 2); ///pixel sampler
	vec2 OFFSETS7 = vec2(0, 3); ///pixel sampler
	vec2 OFFSETS8 = vec2(0, 4); ///pixel sampler

	float g_BlurWeights0 = 0.004815026f; // gaus  dsitribution
	float g_BlurWeights1 = 0.028716039f; // gaus  dsitribution
	float g_BlurWeights2 = 0.102818575f; // gaus  dsitribution
	float g_BlurWeights3 = 0.221024189f; // gaus  dsitribution
	float g_BlurWeights4 = 0.285252340f; // gaus  dsitribution
	float g_BlurWeights5 = 0.221024189f; // gaus  dsitribution
	float g_BlurWeights6 = 0.102818575f; // gaus  dsitribution
	float g_BlurWeights7 = 0.028716039f; // gaus  dsitribution
	float g_BlurWeights8 = 0.004815026f; // gaus  dsitribution

	//float bright =tex2Dlod(LuminancePass,float4(Input.TexCoord,0.0f,BrightLod)).x;
	//bright=max(bright-BloomThreshold,0.0f);
	//bright =0.5f*bright;
	//brigth calc

	//pixelsize calc
	float Texel = (1.0f / fViewportDimensions.y)*(blurMip + 1);
	vec4
		output = textureLod(BrightTexture,	vec2(TexCoord + (OFFSETS0 * Texel)), blurMip)*g_BlurWeights0;
	output += textureLod(BrightTexture,		vec2(TexCoord + (OFFSETS1 * Texel)), blurMip)*g_BlurWeights1;
	output += textureLod(BrightTexture,		vec2(TexCoord + (OFFSETS2 * Texel)), blurMip)*g_BlurWeights2;
	output += textureLod(BrightTexture,		vec2(TexCoord + (OFFSETS3 * Texel)), blurMip)*g_BlurWeights3;
	output += textureLod(BrightTexture,		vec2(TexCoord + (OFFSETS4 * Texel)), blurMip)*g_BlurWeights4;
	output += textureLod(BrightTexture,		vec2(TexCoord + (OFFSETS5 * Texel)), blurMip)*g_BlurWeights5;
	output += textureLod(BrightTexture,		vec2(TexCoord + (OFFSETS6 * Texel)), blurMip)*g_BlurWeights6;
	output += textureLod(BrightTexture,		vec2(TexCoord + (OFFSETS7 * Texel)), blurMip)*g_BlurWeights7;
	output += textureLod(BrightTexture,		vec2(TexCoord + (OFFSETS8 * Texel)), blurMip)*g_BlurWeights8;

	//float4 color= float4(1,1,0,1);
	//return color;
	color = output;

}