//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D  Light : register(t0);
Texture2D  Luminance : register(t1);
SamplerState samLinear : register(s0);

cbuffer cbBright : register(b3)
{
	//int BrightLod;                               // Luminance mip lvl for bright
	float4 BrightLod_BloomThreshold;                        // Bloom clamp
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
   float4 Color = Light.Sample(samLinear, Input.Tex);
   float Brigth = Luminance.SampleLevel(samLinear, Input.Tex, BrightLod_BloomThreshold.x).x;
   Brigth = max(Brigth - BrightLod_BloomThreshold.y, 0.0f);

   return Color * Brigth;
  // return Color2* Brigth;
}
