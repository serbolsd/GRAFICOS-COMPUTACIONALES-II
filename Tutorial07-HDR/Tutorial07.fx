//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// DEFINES
//--------------------------------------------------------------------------------------

//�


//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
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
	float4 SpecularColorDir;
	float4 SpecularColorPoint;
	float4 SpecularColorSpot;
	float4 DifuseColor;
	float4 AmbientalColor;
	float4 SPpower;
	float4 KDASL;
	float4 LPLQLD;
	float4 lightPosition;
	float4 SLCOC;
	float4 SLDPS;
	float4 ADPS;
};

//cbuffer cbLightsEveryFrame : register(b3)
//{
//	
//};

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
#ifdef VER_LIGTH
	float4 Color : COLOR0;
#else
	float3 wsNormal    : NORMAL;  // Vertex normal (World Space)
	float3 wsPos       : TEXCOORD2;  // Vertex position (World Space)
	float3 wsTan      : TANGENT;  // Vertex position (World Space)
#endif
};


float4 calculateDirectionalLight(float4 posWS, float4 normalWS, float3 difuu)
{

	float3 lightDirWS = -normalize(lightDir.xyz);
	float wsNdl = max(0.0f, dot(lightDirWS.xyz, normalWS.xyz));
	//float3 wsViewDir = normalize(vViewPosition.xyz);// Direction from at to camera position

	float3 wsViewDir = normalize((posWS.xyz - vViewPosition.xyz));
	//float4 lightDirWS = -normalize(lightDir.xyzw);
	//float wsNdl = max(0.0f, dot(lightDirWS, normalWS));

#ifdef BLINN // Blinn specular
	float3 wsReflect = normalize(reflect(-lightDirWS.xyz, normalWS.xyz));
	float wsVdR = max(0.0f, dot(wsViewDir.xyz, wsReflect.xyz));
	float SpecularFactor = pow(wsVdR, SPpower.x) * wsNdl;
#else // (Blinn Phong)
	float3 wsHalf = normalize(wsViewDir.xyz + lightDirWS.xyz);
	float wsNdH = max(0.0f, dot(normalWS.xyz, wsHalf.xyz));
	float SpecularFactor = pow(wsNdH, SPpower.x) * wsNdl;
#endif

//	float4 colorDifuse = txDiffuse.Sample(samLinear, input.Tex);
	//float attenuation = (1.0 / (lin + quad));
	//float3 dif = txDiffuse.Sample(samLinear, texcord).xyz;
	//float3 difuse = difuu*KDASL.x* DifuseColor.xyz*wsNdl;
	float3 difuse = KDASL.x* difuu.xyz*wsNdl;
	//float3 difuse = KDASL.x*float3(txDiffuse.Sample(samLinear, texcord).xyz)*wsNdl;
	float3 ambient = KDASL.y*(1.0 - wsNdl)*AmbientalColor.xyz;
	float3 specular = KDASL.z*SpecularColorDir.xyz* SpecularFactor;

	float4 color = float4((difuse.xyz + specular.xyz + ambient.xyz), 1.0f);
	return color / ADPS.x;
}

float4 calculatePointLight(float4 posWS, float4 normalWS, float3 difuu)
{
	float3 wsViewDir = -normalize((posWS.xyz - vViewPosition.xyz));
	float4 lightDirWS = -normalize(posWS - lightPosition);
	float3 dist = posWS.xyz - lightPosition.xyz;
	//lightDirWS = -normalize(lightDirWS);
	float wsNdl = max(0.0f, dot(lightDirWS.xyz, normalWS.xyz));
	wsNdl = wsNdl * (LPLQLD.z / length(dist));
	//float3 wsLightDir = -normalize(wsPos.xyz - vViewPosition.xyz);
	//float wsNdL = max(0.0f, dot(wsLightDir.xyz, wsNormal.xyz));
	//float3 Dist = wsPos.xyz - vViewPosition.xyz;
	//wsNdL = wsNdL * (PointMaxDistance / length(Dist));
#ifdef BLINN
	float3 wsReflect = normalize(reflect(-lightDirWS.xyz, normalWS.xyz)); //calculo la direccion del reflej
	float wsVdR = max(0.0f, dot(wsViewDir.xyz, wsReflect.xyz)); //me aseguro de optener la intencidad con la que se ve el reflejo
	float SpecularFactor = pow(wsVdR, SPpower.x) * wsNdl; //calculo el factor specular
#else//blin-phong
	float3 wsHalf = normalize(wsViewDir.xyz + lightDirWS.xyz); //calculo el vector medio entre la direccion de vista y la direccion de la luz
	float wsNdH = max(0.0f, dot(normalWS.xyz, wsHalf.xyz));
	float SpecularFactor = pow(wsNdH, SPpower.x) * wsNdl;
#endif
	//float lin = KDASL.w + (LPLQLD.x * dist);
	//float quad = (LPLQLD.y * (dist* dist));
	//float attenuation = (1.0 / (lin + quad));
	//float4 dif = float3(txDiffuse.Sample(samLinear, texcord.xy).xyz);
	//float4 dif = texture2D(txDiffuse, texcord.xy)
	//float3 difuse = difuu*KDASL.x* DifuseColor.xyz*wsNdl;
	float3 difuse = KDASL.x* difuu.xyz*wsNdl;
	//float3 difuse = KDASL.x* txDiffuse.Sample(samLinear, input.Tex).xyz*wsNdl;
	//float3 difuse = dif.xyz;

	float3 ambient = KDASL.y*(1.0 - wsNdl)*AmbientalColor.xyz;
	float3 specular = KDASL.z*SpecularColorPoint.xyz* SpecularFactor;

	float4 color = float4((difuse.xyz + specular.xyz + ambient.xyz), 1.0);
	return color / ADPS.y;
}

float4 calculateSpotLight2(float4 posWS, float4 normalWS, float3 difuu)
{
	float3 wsViewDir = -normalize((posWS.xyz - vViewPosition.xyz));
	float3 wsLightDir = -normalize(wsViewDir);
	float3 wsSpotLightDir = normalize(vViewPosition.xyz - posWS.xyz);
	float Theta = dot(wsSpotLightDir, wsLightDir.xyz);
	float Spot = Theta - cos(SLCOC.y * 0.5);
	Spot = Spot / (cos(SLCOC.x * 0.5) - cos(SLCOC.y * 0.5));
	float wsNdL = max(0.0, dot(wsLightDir, normalWS.xyz) * Spot);

#ifdef BLINN
	float3 wsReflect = normalize(reflect(-wsLightDir.xyz, normalWS.xyz)); //calculo la direccion del reflej
	float wsVdR = max(0.0f, dot(wsViewDir.xyz, wsReflect.xyz)); //me aseguro de optener la intencidad con la que se ve el reflejo
	float SpecularFactor = pow(wsVdR, SPpower.x) * wsNdL; //calculo el factor specular
#else//blin-phong
	float3 wsHalf = normalize(wsViewDir.xyz + wsLightDir.xyz); //calculo el vector medio entre la direccion de vista y la direccion de la luz
	float wsNdH = max(0.0f, dot(normalWS.xyz, wsHalf.xyz));
	float SpecularFactor = pow(wsNdH, SPpower.x) * wsNdL;
#endif

	float3 difuse = KDASL.x* difuu.xyz*wsNdL;
	//float3 difuse = KDASL.x* txDiffuse.Sample(samLinear, input.Tex).xyz*wsNdl;
	//float3 difuse = dif.xyz;

	float3 ambient = KDASL.y*(1.0 - wsNdL)*AmbientalColor.xyz;
	float3 specular = KDASL.z*SpecularColorPoint.xyz* SpecularFactor;

	float4 color = float4((difuse.xyz + specular.xyz + ambient.xyz), 1.0);
	return color / ADPS.y;
}
float4 calculateSpotLight(float4 posWS, float4 normalWS, float3 difuu)
{
	float3 wsViewDir = normalize(posWS.xyz - vViewPosition.xyz);
	float4 lightDirWS = normalize(posWS - vViewPosition);
	float3 LightToPixel = -normalize(posWS.xyz - vViewPosition.xyz);
	float SpotFactor = dot(LightToPixel, lightDirWS.xyz);
	
	//vec3 wsViewDir = -normalize((posWS.xyz - VP.xyz));
	float wsNdl = max(0.0f, dot(lightDirWS, normalWS));
	//if (SpotFactor > Cutoff)
	if (SpotFactor > SLCOC.x)
	{
		lightDirWS = vViewPosition - posWS;
		float dist = length(lightDirWS);
		lightDirWS = normalize(lightDirWS);
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
		float lin = KDASL.w + (LPLQLD.x * dist);
		float quad = (LPLQLD.y * (dist* dist));
		float attenuation = (1.0 / (lin + quad));

		float3 difuse = KDASL.x* difuu.xyz*wsNdl;
		float3 ambient = difuu*KDASL.y*(1.0 - wsNdl)*AmbientalColor.xyz;
		float3 specular = KDASL.z*SpecularColorSpot.xyz* SpecularFactor;

		float4 color = float4((difuse.xyz + specular.xyz + ambient.xyz), 1.0) ;
		color *= (1.0 - (1.0 - SpotFactor) * 1.0 / (1.0 - SLCOC.x));
		color *= (1.0 - (1.0 - SpotFactor) * 1.0 / (1.0 - 0.9));
		return color / ADPS.z;
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
	//output.Pos = mul(input.Pos, World);
	//output.Pos = mul(output.Pos, View);
	//output.Pos = mul(output.Pos, Projection);
	output.Tex.x = input.Tex.x;
	output.Tex.y = input.Tex.y;
	matrix matViewProjection = View * Projection;
	
	float4 wsPos = mul(float4(input.Pos.xyz, 1.0), World);
	output.Pos = mul(wsPos, View);
	output.Pos = mul(output.Pos, Projection);
	
	//float4 wsPos= mul(input.Pos, World);
	float4 normalWS = mul(input.Nor, World);

	float4 Nws = float4(normalWS.xyz, 1);
	float4 Tangentws = normalize(mul(input.tan, World));
#ifdef VER_LIGTH
	//float4 dif =txDiffuse.Sample(samLinear, input.Tex);
	float4 dif = float4(1, 1, 1, 1);
	//DifuseColor = float4(1,1,1,1);
	float3 wsBinormal = cross(normalWS.xyz, Tangentws.xyz);
	float4 TexNormal = txNormal.Sample(samLinear, input.Tex);
	TexNormal = normalize(TexNormal*2.0f - 1.0f);
	float3x3 TBN = float3x3(Tangentws.xyz, wsBinormal.xyz, normalWS.xyz);
	float3 wssNormal = normalize(mul(TexNormal.xyz, TBN));
	float4 norms = float4(wssNormal.xyz, 0);

#ifdef DIR_LIGHT
	if (SLDPS.x)
	{
		output.Color += calculateDirectionalLight(wsPos, normalWS, dif.xyz);
	}
#endif
#ifdef POINT_LIGHT
	if (SLDPS.y) 
	{
		output.Color += calculatePointLight(wsPos, normalWS, dif.xyz);
	}
#endif
#ifdef CONE_LIGHT
	if(SLDPS.z)
	{
		//output.Color += calculateSpotLight(wsPos, normalWS, dif.xyz);
		output.Color += calculateSpotLight2(wsPos, normalWS, dif.xyz);
	}
#endif
#else
	output.wsNormal = normalWS.xyz;
	output.wsPos = wsPos.xyz;
	output.wsTan = Tangentws.xyz;
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
	//return txDiffuse.Sample(samLinear, input.Tex)* input.Color;
	//return txDiffuse.Sample(samLinear, input.Tex) * vMeshColor;
	return txDiffuse.Sample(samLinear, input.Tex)* input.Color;
	//return input.Color;
#else
	//float3 wsLightDir = -normalize(lightDir.xyz);
	//float wsNdL = max(0.0f, dot(wsLightDir.xyz, input.wsNormal.xyz));
	//float3 wsViewDir = -normalize(input.wsPos.xyz - vViewPosition.xyz);
	float4 dif=txDiffuse.Sample(samLinear, input.Tex);
	float3 wsBinormal = cross(input.wsNormal.xyz, input.wsTan.xyz);
	float4 TexNormal = txNormal.Sample(samLinear, input.Tex);;
	TexNormal = normalize(TexNormal*2.0f - 1.0f);
	float3x3 TBN = float3x3(input.wsTan.xyz, wsBinormal.xyz, input.wsNormal.xyz);
	float3 wssNormal = normalize(mul(TexNormal.xyz,TBN));
	float4 norms = float4(wssNormal.xyz, 0);
	float4 color=float4(0,0,0,0);
#ifdef DIR_LIGHT
	if (SLDPS.x)
	{
		color += calculateDirectionalLight(float4(input.wsPos.xyz, 1), norms, dif.xyz);
	}
#endif
#ifdef POINT_LIGHT
	if(SLDPS.y)
	{
		color += calculatePointLight(float4(input.wsPos.xyz, 1), norms, dif.xyz);
	}
#endif
#ifdef CONE_LIGHT
	if(SLDPS.z)
	{
		//color += calculateSpotLight(float4(input.wsPos.xyz,1), norms, dif.xyz);
		color += calculateSpotLight2(float4(input.wsPos.xyz,1), norms, dif.xyz);
	}
#endif

	return color;
#endif
}
