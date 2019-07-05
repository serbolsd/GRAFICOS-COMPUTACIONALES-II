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
Texture2D AddBright : register(t0);
Texture2D Light : register(t1);
SamplerState samLinear : register(s0);

cbuffer cbToneMap : register(b6)
{
	//int BrightLod;                               // Luminance mip lvl for bright
	float4 Exposure_BloomMuiltiplier;                        // Bloom clamp
};

float3 BasicExposure(float3 inColor)
{
	inColor *= Exposure_BloomMuiltiplier.x;
	inColor = pow(abs (inColor), 1.0f / 2.2f);

	return inColor;
}

float3 Reinhard(float3 inColor)
{
	inColor *= Exposure_BloomMuiltiplier.x;
	inColor /= (1.0f + inColor);
	inColor = pow(abs (inColor), 1.0f / 2.2f);

	return inColor;
}
/*
float4 Haarm_Peter_Duiker(in float3 Color, in float exposure)
{
   float3 texColor = Color;
   texColor *= exposure;

   float3 ld = 0.002f;
   float linReference = 0.18;
   float logReference = 444;
   float logGamma = 0.45f;

   float3 LogColor;
   LogColor.rgb = (log10(0.4f * texColor.rgb / linReference) / ld * logGamma + logReference) / 1023.f;
   LogColor.rgb = saturate(LogColor.rgb);

   float FilmLutWidth = 256;
   float Padding = 0.5f / FilmLutWidth;

   float3 retColor;
   retColor.r = tex2D(DefaultSampler, float2(lerp(Padding, 1 - Padding, LogColor.r), 0.5f)).r;
   retColor.g = tex2D(DefaultSampler, float2(lerp(Padding, 1 - Padding, LogColor.g), 0.5f)).r;
   retColor.b = tex2D(DefaultSampler, float2(lerp(Padding, 1 - Padding, LogColor.b), 0.5f)).r;

   return float4(retColor, 1);
}
*/
float3 Burgess_Dawson(float3 inColor)
{
	inColor *= Exposure_BloomMuiltiplier.x;
	float3 x = max(0, inColor - 0.004f);
	inColor = (x * (6.2 * x + 0.5f)) / (x * (6.2f * x + 1.7f) + 0.06f);

	return inColor;
}

float3 Uncharted2Tonemap(float3 inx)
{
	float A = 0.15f;
	float B = 0.5f;
	float C = 0.1f;
	float D = 0.2f;
	float E = 0.02f;
	float F = 0.3f;

	return ((inx * (A * inx + C * B) + D * E) / (inx * (A * inx + B) + D * F)) - E / F;
}

float3 Uncharted2(float3 inColor)
{
	float W = 11.2f;
	inColor *= Exposure_BloomMuiltiplier.x;
	float ExposureBias = 2.0f;
	float3 curr = Uncharted2Tonemap(ExposureBias * inColor);

	float3 whiteScale = 1.0f / Uncharted2Tonemap(W);
	inColor = curr * whiteScale;

	inColor = pow(abs (inColor), 1 / 2.2);
	return inColor;
}
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



float4 PS(PS_INPUT Input) : SV_Target
{

	//float4 color=renderText.Sample(samLinear, input.Tex);
	//return color;
	   // Take color 
   float3 Color = Light.Sample(samLinear, Input.Tex).xyz;

   // add HDR
   Color += Exposure_BloomMuiltiplier.y * AddBright.Sample(samLinear, Input.Tex).xyz;

   // Take the linear color to standard RGBA color
   //if (Input.Tex.x < 0.5f)
   //{
	//   if (Input.Tex.y < 0.5f)
	//   {
	//		Color = Reinhard(Color);
	//   }
	//   else
	//   {
	//	   //Color = Burgess_Dawson(Color);
	//   }
   //}
   //else if (Input.Tex.x < 0.5f && Input.Tex.y > 0.5f)
   //{
	//  Color = Burgess_Dawson(Color);
   //}
   //else if (Input.Tex.x > 0.5f)
   //{
	//   if (Input.Tex.y < 0.5f)
	//   {
	//	   Color = BasicExposure(Color);
	//   }
	//   else
	//   {
	//	   //Color = Uncharted2(Color);
	//   }
   //  //Color = float3(1,0,0);
   //}
   //else if (Input.Tex.x > 0.5f && Input.Tex.y > 0.5f)
   //{
	//  Color = Uncharted2(Color);
   //}
   //Color = BasicExposure(Color);
   // Return to standard RGBA
   //if (Input.Tex.x < 0.25f)
   //  Color = Uncharted2(Color);
   //if(Input.Tex.x < 0.5f&&Input.Tex.x > 0.25f)
   //  Color = Reinhard(Color);
   //if (Input.Tex.x < 0.75f&&Input.Tex.x > 0.5f)
   //   Color = BasicExposure(Color);
   //if (Input.Tex.x < 1.0f&&Input.Tex.x > 0.75f)
   //   Color = Burgess_Dawson(Color);
   if (Input.Tex.x > 0.5f)
   {
	   if(Input.Tex.y > 0.5f)
		   Color = Burgess_Dawson(Color);
	   else
		   Color = BasicExposure(Color);
   }
   if (Input.Tex.x < 0.5f)
   {
	   if (Input.Tex.y > 0.5f)
		   Color = Uncharted2(Color);
	   else
		   Color = Reinhard(Color);
   }
   return float4(Color, 1.0f);
}
