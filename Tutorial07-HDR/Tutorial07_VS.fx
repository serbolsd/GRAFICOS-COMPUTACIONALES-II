//?//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// DEFINES
//--------------------------------------------------------------------------------------


//#define VER_LIGTH
//#define BLINN
//#define DIR_LIGHT
//#define CONE_LIGHT
//#define POINT_LIGHT

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
	float4 KDASL;
	float4 LPLQ;
	float4 lightPosition;
	float4 SLCOC;
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


float4 calculateDirectionalLight(float4 posWS, float4 normalWS)
{
	float3 wsViewDir = -normalize((posWS.xyz - vViewPosition.xyz));
	float4 lightDirWS = -normalize(lightDir);
	float wsNdl = max(0.0f, dot(lightDirWS, normalWS));
#ifdef BLINN
	float3 wsReflect = normalize(reflect(-lightDirWS.xyz, normalWS.xyz)); //calculo la direccion del reflej
	float wsVdR = max(0.0f, dot(wsViewDir.xyz, wsReflect.xyz)); //me aseguro de optener la intencidad con la que se ve el reflejo
	float SpecularFactor = pow(wsVdR, SPpower.x) * wsNdl; //calculo el factor specular
#else//blin-phong
	float3 wsHalf = normalize(wsViewDir.xyz + lightDirWS.xyz); //calculo el vector medio entre la direccion de vista y la direccion de la luz
	float wsNdH = max(0.0f, dot(normalWS.xyz, wsHalf.xyz));
	float SpecularFactor = pow(wsNdH, SPpower.x) * wsNdl;
#endif

	//float attenuation = (1.0 / (lin + quad));
	float3 difuse = KDASL.x* DifuseColor.xyz*wsNdl;
	float3 ambient = KDASL.y*(1.0 - wsNdl)*AmbientalColor.xyz;
	float3 specular = KDASL.z*SpecularColor.xyz* SpecularFactor;

	float4 color = float4((difuse.xyz + specular.xyz + ambient.xyz), 1.0);
	return color;
}

float4 calculatePointLight(float4 posWS, float4 normalWS)
{
	float3 wsViewDir = -normalize((posWS.xyz - vViewPosition.xyz));
	float4 lightDirWS = posWS - lightPosition;
	float dist = length(lightDirWS);
	lightDirWS = normalize(-lightDirWS);
	float wsNdl = max(0.0f, dot(lightDirWS, normalWS));
#ifdef BLINN
	float3 wsReflect = normalize(reflect(-lightDirWS.xyz, normalWS.xyz)); //calculo la direccion del reflej
	float wsVdR = max(0.0f, dot(wsViewDir.xyz, wsReflect.xyz)); //me aseguro de optener la intencidad con la que se ve el reflejo
	float SpecularFactor = pow(wsVdR, SPpower.x) * wsNdl; //calculo el factor specular
#else//blin-phong
	float3 wsHalf = normalize(wsViewDir.xyz + lightDirWS.xyz); //calculo el vector medio entre la direccion de vista y la direccion de la luz
	float wsNdH = max(0.0f, dot(normalWS.xyz, wsHalf.xyz));
	float SpecularFactor = pow(wsNdH, SPpower.x) * wsNdl;
#endif
	float lin = KDASL.w + (LPLQ.x * dist);
	float quad = (LPLQ.y * (dist* dist));
	float attenuation = (1.0 / (lin + quad));

	float3 difuse = KDASL.x* DifuseColor.xyz*wsNdl;
	float3 ambient = KDASL.y*(1.0 - wsNdl)*AmbientalColor.xyz;
	float3 specular = KDASL.z*SpecularColor.xyz* SpecularFactor;

	float4 color = float4((difuse.xyz + specular.xyz + ambient.xyz), 1.0)/ attenuation;
	return color;
}

float4 calculateSpotLight(float4 posWS, float4 normalWS)
{
	float3 wsViewDir = -normalize((posWS.xyz - vViewPosition.xyz));
	float4 lightDirWS = vViewPosition - lightPosition;
	float3 LightToPixel = normalize(posWS.xyz - vViewPosition.xyz);
	float SpotFactor = dot(LightToPixel, lightDirWS.xyz);

	//vec3 wsViewDir = -normalize((posWS.xyz - VP.xyz));
	float wsNdl = max(0.0f, dot(lightDirWS, normalWS));
	//if (SpotFactor > Cutoff)
	if (SpotFactor > SLCOC.x)
	{
		lightDirWS = vViewPosition - posWS;
		float dist = length(lightDirWS);
		lightDirWS = normalize(-lightDirWS);
		float wsNdl = max(0.0f, dot(lightDirWS, normalWS));
#ifdef BLINN
		float3 wsReflect = normalize(reflect(-lightDirWS.xyz, normalWS.xyz)); //calculo la direccion del reflej
		float wsVdR = max(0.0f, dot(wsViewDir.xyz, wsReflect.xyz)); //me aseguro de optener la intencidad con la que se ve el reflejo
		float SpecularFactor = pow(wsVdR, SPpower.x) * wsNdl; //calculo el factor specular
#else//blin-phong
		float3 wsHalf = normalize(wsViewDir.xyz + lightDirWS.xyz); //calculo el vector medio entre la direccion de vista y la direccion de la luz
		float wsNdH = max(0.0f, dot(normalWS.xyz, wsHalf.xyz));
		float SpecularFactor = pow(wsNdH, SPpower.x) * wsNdl;
#endif
		float lin = KDASL.w + (LPLQ.x * dist);
		float quad = (LPLQ.y * (dist* dist));
		float attenuation = (1.0 / (lin + quad));

		float3 difuse = KDASL.x* DifuseColor.xyz*wsNdl;
		float3 ambient = KDASL.y*(1.0 - wsNdl)*AmbientalColor.xyz;
		float3 specular = KDASL.z*SpecularColor.xyz* SpecularFactor;

		float4 color = float4((difuse.xyz + specular.xyz + ambient.xyz), 1.0) / attenuation;
		color *= (1.0 - (1.0 - SpotFactor) * 1.0 / (1.0 - SLCOC.x));
		color *= (1.0 - (1.0 - SpotFactor) * 1.0 / (1.0 - 0.9));
		return color;
	}
	else
	{
		return float4(0, 0, 0, 0);
	}
}
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
	float4 Nws = float4(normalWS.xyz, 1);
	float4 Tangentws = normalize(mul(input.tan, World));
#ifdef VER_LIGTH

#ifdef DIR_LIGHT
	output.Color += calculateDirectionalLight(wsPos, Nws);
#endif
#ifdef POINT_LIGHT
	output.Color += calculatePointLight(wsPos, Nws);
#endif
#ifdef CONE_LIGHT
	output.Color += calculateSpotLight(wsPos, Nws);
#endif
#else
	output.wsNormal = normalWS;
	output.wsPos = wsPos.xyz;
	output.wsTan = Tangentws.xyz;
#endif

	return output;
}