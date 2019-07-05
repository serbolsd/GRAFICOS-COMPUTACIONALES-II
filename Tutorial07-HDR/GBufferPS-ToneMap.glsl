////////////////////////////////////////////////////////////////////////////////
// Filename: GBuffer.ps
////////////////////////////////////////////////////////////////////////////////
#version 400

in vec2 TexCoord;
uniform sampler2D AddBright;
uniform sampler2D Light;
uniform float Exposure;
uniform float BloomMuiltiplier;


layout(location = 0) out vec4 color;

vec3 BasicExposure(vec3 inColor)
{
	inColor *= Exposure;
	inColor = pow(inColor, vec3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));

	return inColor;
}

vec3 Reinhard(vec3 inColor)
{
	inColor *= Exposure;
	inColor /= (1.0f + inColor);
	inColor = pow(inColor, vec3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));

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
vec3 Burgess_Dawson(vec3 inColor)
{
	inColor *= Exposure;
	vec3 x = inColor - 0.004f;
	inColor = (x * (6.2 * x + 0.5f)) / (x * (6.2f * x + 1.7f) + 0.06f);

	return inColor;
}

vec3 Uncharted2Tonemap(vec3 inx)
{
	float A = 0.15f;
	float B = 0.5f;
	float C = 0.1f;
	float D = 0.2f;
	float E = 0.02f;
	float F = 0.3f;
	
	vec3 ton = A*inx;
	ton += C * B;
	ton *= inx;
	ton += D * E;
	vec3 ton2 = A*inx;
	ton2 += B;
	ton2 *= inx;
	ton2 += D * F;
	ton2 -= E / F;
	vec3 output = ton / ton2;
	//return ((inx * (A * inx + C * B) + D * E) / (inx * (A * inx + B) + D * F)) - E / F;
	return output;
}

vec3 Uncharted2(vec3 inColor)
{
	float W = 11.2f;
	inColor *= Exposure;
	float ExposureBias = 2.0f;
	vec3 curr = Uncharted2Tonemap(vec3(ExposureBias * inColor.x, ExposureBias * inColor.y, ExposureBias * inColor.z));

	vec3 wscale = Uncharted2Tonemap(vec3(W, W, W));
	vec3 whiteScale =vec3( 1.0f / wscale.x, 1.0f / wscale.y, 1.0f / wscale.z);
	inColor = curr * whiteScale;

	inColor = pow(inColor, vec3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
	return inColor;
}
void main()
{

	//float4 color=renderText.Sample(samLinear, input.Tex);
//return color;
   // Take color 
	//float3 inColor = Light.Sample(samLinear, Input.Tex).xyz;
	vec3 inColor = texture2D(Light, TexCoord.xy).xyz;

	// add HDR
	inColor += BloomMuiltiplier * texture2D(AddBright, TexCoord.xy).xyz;

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
	if (TexCoord.x > 0.5f)
	{
		if (TexCoord.y > 0.5f)
			inColor = Burgess_Dawson(inColor);
		else
			inColor = BasicExposure(inColor);
	}
	if (TexCoord.x < 0.5f)
	{
		if (TexCoord.y > 0.5f)
			inColor = Uncharted2(inColor);
		else
			inColor = Reinhard(inColor);
	}
	color=vec4(inColor, 1.0f);

}