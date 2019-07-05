//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D  Bright : register(t0);
SamplerState samLinear : register(s0);

cbuffer cbBlurH : register(b4)
{
	float4 f2ViewportDimensions_blurMip;
};

struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
	float4 Nor : NORMAL;
	float4 tan : TANGENT;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	float2 texCordbien;
	texCordbien.x = 1.0f - input.Tex.x;
	texCordbien.y = input.Tex.y;
	PS_INPUT output;
	output.Pos = input.Pos;
	output.Tex = texCordbien;
	//output.Tex = input.Tex;
	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT Input) : SV_Target
{
	float2 OFFSETS0 = float2(0, -4); ///pixel sampler
	float2 OFFSETS1 = float2(0, -3); ///pixel sampler
	float2 OFFSETS2 = float2(0, -2); ///pixel sampler
	float2 OFFSETS3 = float2(0, -1); ///pixel sampler
	float2 OFFSETS4 = float2(0, 0); ///pixel sampler
	float2 OFFSETS5 = float2(0, 1); ///pixel sampler
	float2 OFFSETS6 = float2(0, 2); ///pixel sampler
	float2 OFFSETS7 = float2(0, 3); ///pixel sampler
	float2 OFFSETS8 = float2(0, 4); ///pixel sampler

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
	float Texel = (1.0f / f2ViewportDimensions_blurMip.y)*(f2ViewportDimensions_blurMip.z + 1);
	//Luminance.SampleLevel(samLinear, Input.Tex, BrightLod_BloomThreshold.x).x;
	float4
	output = Bright.SampleLevel(samLinear, float2(Input.Tex+(OFFSETS0 * Texel)), f2ViewportDimensions_blurMip.z)*g_BlurWeights0;
	output += Bright.SampleLevel(samLinear, float2(Input.Tex+(OFFSETS1 * Texel)), f2ViewportDimensions_blurMip.z)*g_BlurWeights1;
	output += Bright.SampleLevel(samLinear, float2(Input.Tex+(OFFSETS2 * Texel)), f2ViewportDimensions_blurMip.z)*g_BlurWeights2;
	output += Bright.SampleLevel(samLinear, float2(Input.Tex+(OFFSETS3 * Texel)), f2ViewportDimensions_blurMip.z)*g_BlurWeights3;
	output += Bright.SampleLevel(samLinear, float2(Input.Tex+(OFFSETS4 * Texel)), f2ViewportDimensions_blurMip.z)*g_BlurWeights4;
	output += Bright.SampleLevel(samLinear, float2(Input.Tex+(OFFSETS5 * Texel)), f2ViewportDimensions_blurMip.z)*g_BlurWeights5;
	output += Bright.SampleLevel(samLinear, float2(Input.Tex+(OFFSETS6 * Texel)), f2ViewportDimensions_blurMip.z)*g_BlurWeights6;
	output += Bright.SampleLevel(samLinear, float2(Input.Tex+(OFFSETS7 * Texel)), f2ViewportDimensions_blurMip.z)*g_BlurWeights7;
	output += Bright.SampleLevel(samLinear, float2(Input.Tex+(OFFSETS8 * Texel)), f2ViewportDimensions_blurMip.z)*g_BlurWeights8;

	//float4 color= float4(1,1,0,1);
	//return color;
	return output;
}
