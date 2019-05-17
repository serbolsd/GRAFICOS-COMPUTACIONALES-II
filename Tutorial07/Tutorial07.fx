//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// DEFINES
//--------------------------------------------------------------------------------------

#define VER_LIGTH
#define BINN
#define POINT_LIGHT

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer cbNeverChanges : register(b0)
{
	matrix View;
};

cbuffer cbChangeOnResize : register(b1)
{
	matrix Projection;
};

cbuffer cbChangesEveryFrame : register(b2)
{
	matrix World;
	float4 vMeshColor;
	float4 lightDir;
	float4 vViewPosition;
	float4 SpecularColor;
	float4 DifuseColor;
	float4 AmbientalColor;
	float4 SPpower;
	float4 KDAS;
};


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
	float4 Nor : NORMAL0;
	float4 tan : TANGENT0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
#ifdef VER_LIGTH
	float4 Color : COLOR0;
#else
	float3 wsNormal    : TEXCOORD1;  // Vertex normal (World Space)
	float3 wsPos       : TEXCOORD2;  // Vertex position (World Space)
	float3 wsTan      : TEXCOORD3;  // Vertex position (World Space)
#endif
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex = input.Tex;
	float4 wsPos= mul(input.Pos, World);
	float3 normalWS = mul(input.Nor, World);
#ifdef VER_LIGTH
	float3 lightDirWS = normalize(-lightDir);
	#ifdef POINT_LIGHT
		float3 wsViewDir = -lightDirWS;
	#else
		float3 wsViewDir = -normalize(wsPos.xyz - vViewPosition.xyz);
	#endif
	float wsNdl = dot(lightDirWS, normalWS);
	#ifdef BINN
		float3 wsReflect = normalize(reflect(-lightDirWS.xyz, normalWS.xyz));
		float wsVdR = max(0.0f, dot(wsViewDir.xyz, wsReflect.xyz));
		float SpecularFactor = pow(wsVdR, SPpower.x) * wsNdl;
	#else
		float3 wsHalf = normalize(wsViewDir.xyz + lightDirWS.xyz); //calculo el vector medio entre la direccion de vista y la direccion de la luz
		float wsNdH = max(0.0f, dot(normalWS.xyz, wsHalf.xyz));
		float SpecularFactor = pow(wsNdH, SP.x) * wsNdl;
	#endif
	//output.Color = Ndl.xxxx
	//output.Color = (Ndl.xxx * DifuseColor.xyz) + (SpecularFactor.xxx * SpecularColor.xyz);
	//output.Color = float4(Ndl.xxx*DifuseColor.xyz,1);
	//output.Color += float4 (SpecularFactor.xxx * SpecularColor.xyz,1);
#ifdef POINT_LIGHT
		float dist = length(vViewPosition.xyz - wsPos.xyz);
		float lin = KDAS.w + (1 * dist);
		float quad = (1 * (dist* dist));
		float attenuation = (1.0 / (lin + quad));
		//float attenuation=1;
#else
		float attenuation=1;
#endif
	float3 difuse = KDAS.x* DifuseColor.xyz*wsNdl*attenuation;
	float3 specular = KDAS.z*SpecularColor.xyz* SpecularFactor*attenuation;
	float3 ambient = KDAS.y*(1.0 - wsNdl)*AmbientalColor.xyz*attenuation;
	output.Color = float4(difuse + ambient + specular, 1);
		//output.Color = float4(255, 0, 0, 0);

#else
	output.wsNormal = normalWS;
	output.wsPos = wsPos.xyz;
	output.wsTan = tan.xyz;
#endif

	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
#ifdef VER_LIGTH
	//return txDiffuse.Sample(samLinear, input.Tex) * vMeshColor*input.Nor;
	//return txDiffuse.Sample(samLinear, input.Tex) *input.Color;
	return input.Color;
	//return txDiffuse.Sample(samLinear, input.Tex) * vMeshColor;
#else
	float3 wsLightDir = -normalize(lightDir.xyz);
	float wsNdL = max(0.0f, dot(wsLightDir.xyz, input.wsNormal.xyz));
	float3 wsViewDir = -normalize(input.wsPos.xyz - vViewPosition.xyz);

	#ifdef BLINN
		float3 wsReflect = normalize(reflect(-wsLightDir.xyz, input.wsNormal.xyz));
		float wsVdR = max(0.0f, dot(wsViewDir.xyz, wsReflect.xyz));
		float SpecularFactor = pow(wsVdR, SPpower.x) * wsNdL;
	#else
		float3 wsHalf = normalize(wsViewDir.xyz + lightDir.xyz); //calculo el vector medio entre la direccion de vista y la direccion de la luz
		float wsNdH = max(0.0f, dot(input.wsNormal.xyz, wsHalf.xyz));
		float SpecularFactor = pow(wsNdH, SP.x) * wsNdl;
	#endif
	//return txDiffuse.Sample(samLinear, input.Tex);
	return txDiffuse.Sample(samLinear, input.Tex)*float4((wsNdL.xxx * DifuseColor.xyz) + (SpecularFactor.xxx * SpecularColor.xyz), 1.0);;
#endif
}
