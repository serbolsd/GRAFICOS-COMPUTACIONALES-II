//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D  Bright : register(t0);
Texture2D  Blur_H : register(t1);
Texture2D  Blur_V : register(t2);
SamplerState samLinear : register(s0);
cbuffer cbBright : register(b5)
{
	//int BrightLod;                               // Luminance mip lvl for bright
	float4 blurMip;                        // Bloom clamp
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
    float4 texBright = Bright.Sample(samLinear, Input.Tex);
	//float4 texBlurH = Blur_H.SampleLevel(samLinear, Input.Tex.xy, blurMip.x + 1);
	float4 texBlurH = Blur_H.SampleLevel(samLinear, Input.Tex.xy, blurMip.x);
	//float4 texBlurV = Blur_V.SampleLevel(samLinear, Input.Tex.xy, blurMip.x + 1);
	float4 texBlurV = Blur_V.SampleLevel(samLinear, Input.Tex.xy, blurMip.x);
	//combination
  //float4 color= float4(1,1,0,1);
  //return color;
	float4 output = (texBlurH + texBlurV)+texBright;
	return output;
}
