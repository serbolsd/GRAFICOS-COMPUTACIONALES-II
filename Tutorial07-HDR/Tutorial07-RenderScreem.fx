//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// DEFINES
//--------------------------------------------------------------------------------------

//¿


//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D renderText : register(t0);
SamplerState samLinear : register(s0);

//--------------------------------------------------------------------------------------
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

	float4 color=renderText.Sample(samLinear, input.Tex);

	return color;
}
