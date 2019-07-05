//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txLight : register(t0);

SamplerState samLinear : register(s0);


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
float4 PS(PS_INPUT input) : SV_Target
{
	
	//return txDiffuse.Sample(samLinear, input.Tex)* input.Color;
//Luminance Factor
	float4 LuminanceFactor = float4(0.2f, 0.587f, 0.15f, 1.0f);

//Texture color Sampler
	float4 col = txLight.Sample(samLinear, input.Tex);
	//float4 col = txDiffuse,input.Tex);

// Calcule Luminance
	float Luminance = dot(col, LuminanceFactor);
	Luminance = log(Luminance + 0.000001f);

	float4 color = float4(Luminance, Luminance, Luminance, Luminance);
	//float4 color = float4(255,0,255,1);

	return color;
}
