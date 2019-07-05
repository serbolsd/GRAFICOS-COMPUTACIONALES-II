////////////////////////////////////////////////////////////////////////////////
// Filename: GBuffer.ps
////////////////////////////////////////////////////////////////////////////////
#version 400
//¿

//include "Header.h"
//#define VER_LIGTH
//#define BLINN
//#define DIR_LIGHT
//#define CONE_LIGHT
//#define POINT_LIGHT
in vec2 TexCoord;
in vec4 colorfiesta;
in vec4 wsNormal;
in vec4 wsPos;
in vec4 wsTangent;
//out vec3 color;
//out vec3 color;
uniform sampler2D renderedTexture;
uniform sampler2D normalTexture;

uniform vec4 LD;//light Direction
uniform vec4 VP;//View Position
uniform vec4 SCD;//Specular Color DIRECTION
uniform vec4 SCP;//Specular Color POINT 
uniform vec4 SCS;//Specular Color SPOT
uniform vec4 DC;//Difuse Color
uniform vec4 AC;//Ambiental Color
uniform vec4 SP;//speular Power 
uniform vec4 KDASL;//const of difuse, ambiental and specular 
uniform vec4 LPLQLD;//LIGHT POINT LINEAR QUADRATIC 
uniform vec4 SLCOC; //SPOTLIGHT CUTOFF OTHECUTOFF
uniform vec4 SLDPS;// SWITCH LIGHT DIRECTION, POINT, SPOT
uniform vec4 ADPS;// ATTENUATION LIGHT DIRECTION, POINT, SPOT
uniform vec4 LP;//light Position
//uniform vec4 SDOC;
layout(location = 0) out vec4 color;
#ifdef VER_LIGTH
#else
vec4 calculateDirectionalLight(vec4 posWS, vec4 normalWS)
{
	vec3 wsViewDir = -normalize((posWS.xyz - VP.xyz));
	vec4 lightDirWS = normalize(LD);
	float wsNdl = max(0.0f, dot(lightDirWS, normalWS));
#ifdef BLINN
	vec3 wsReflect = normalize(reflect(-lightDirWS.xyz, normalWS.xyz)); //calculo la direccion del reflej
	float wsVdR = max(0.0f, dot(wsViewDir.xyz, wsReflect.xyz)); //me aseguro de optener la intencidad con la que se ve el reflejo
	float SpecularFactor = pow(wsVdR, SP.x) * wsNdl; //calculo el factor specular
#else//blin-phong
	vec3 wsHalf = normalize(wsViewDir.xyz + lightDirWS.xyz); //calculo el vector medio entre la direccion de vista y la direccion de la luz
	float wsNdH = max(0.0f, dot(normalWS.xyz, wsHalf.xyz));
	float SpecularFactor = pow(wsNdH, SP.x) * wsNdl;
#endif
	//float attenuation = (1.0 / (lin + quad));
	//vec3 difuse = KDASL.x* DC.xyz*wsNdl*texture(renderedTexture, TexCoord).rgb;
	vec3 difuse = KDASL.x*texture(renderedTexture, TexCoord).rgb*wsNdl;
//	vec3 difuse = KDASL.x* DC.xyz*wsNdl;
	vec3 specular = KDASL.z*SCD.xyz* SpecularFactor;
	vec3 ambient = KDASL.y*(1.0 - wsNdl)*AC.xyz;

	vec4 colord = vec4((difuse.xyz + specular.xyz + ambient.xyz), 1.0);
	return colord/ ADPS.x;
}

vec4 calculatePointLight(vec4 posWS, vec4 normalWS)
{
	vec3 wsViewDir = -normalize((posWS.xyz - VP.xyz));
	vec4 lightDirWS = posWS - LP;
	vec3 dist = posWS.xyz - LP.xyz;
	lightDirWS = -normalize(lightDirWS);
	float wsNdl = max(0.0f, dot(lightDirWS, normalWS));
	//float wsNdl = max(0.0f, dot(lightDirWS.xyz, normalWS.xyz));
	wsNdl = wsNdl * (LPLQLD.z / length(dist));
#ifdef BLINN
	vec3 wsReflect = normalize(reflect(lightDirWS.xyz, normalWS.xyz)); //calculo la direccion del reflej
	float wsVdR = max(0.0f, dot(wsViewDir.xyz, wsReflect.xyz)); //me aseguro de optener la intencidad con la que se ve el reflejo
	float SpecularFactor = pow(wsVdR, SP.x) * wsNdl; //calculo el factor specular
#else//blin-phong
	vec3 wsHalf = normalize(wsViewDir.xyz + lightDirWS.xyz); //calculo el vector medio entre la direccion de vista y la direccion de la luz
	float wsNdH = max(0.0f, dot(normalWS.xyz, wsHalf.xyz));
	float SpecularFactor = pow(wsNdH, SP.x) * wsNdl;
#endif
	//float lin = KDASL.w + (LPLQ.x * dist);
	//float quad = (LPLQ.y * (dist* dist));
	//float attenuation = (1.0 / (lin + quad));

	//vec3 difuse = KDASL.x* DC.xyz*wsNdl*texture(renderedTexture, TexCoord).rgb;
	vec3 difuse = KDASL.x*texture(renderedTexture, TexCoord).rgb*wsNdl;
//	vec3 difuse = KDASL.x* DC.xyz*wsNdl;
	vec3 specular = KDASL.z*SCP.xyz* SpecularFactor;
	vec3 ambient = KDASL.y*(1.0 - wsNdl)*AC.xyz;

	vec4 colorp = vec4((difuse.xyz + specular.xyz + ambient.xyz), 1.0);
	//vec4 colorp = vec4((difuse.xyz + specular.xyz + ambient.xyz), 1.0);
	return colorp;
}

vec4 calculateSpotLight(vec4 posWS, vec4 normalWS)
{
	vec3 wsViewDir = normalize((posWS.xyz - VP.xyz));
	//vec4 lightDirWS = (VP - posWS);
	vec4 lightDirWS = normalize(posWS - VP);
	vec3 LightToPixel = -normalize(posWS.xyz - VP.xyz);
	float SpotFactor = dot(LightToPixel, lightDirWS.xyz);

	//vec3 wsViewDir = -normalize((posWS.xyz - VP.xyz));
	float wsNdl = max(0.0f, dot(lightDirWS, normalWS));
	//if (SpotFactor > Cutoff)
	if (SpotFactor > SLCOC.x)
	{
		lightDirWS = VP - posWS;
		float dist = length(lightDirWS);
		lightDirWS = normalize(lightDirWS);
		float wsNdl = max(0.0f, dot(lightDirWS, normalWS));
#ifdef BLINN
		vec3 wsReflect = normalize(reflect(-lightDirWS.xyz, normalWS.xyz)); //calculo la direccion del reflej
		float wsVdR = max(0.0f, dot(wsViewDir.xyz, wsReflect.xyz)); //me aseguro de optener la intencidad con la que se ve el reflejo
		float SpecularFactor = pow(wsVdR, SP.x) * wsNdl; //calculo el factor specular
#else//blin-phong
		vec3 wsHalf = normalize(wsViewDir.xyz + lightDirWS.xyz); //calculo el vector medio entre la direccion de vista y la direccion de la luz
		float wsNdH = max(0.0f, dot(normalWS.xyz, wsHalf.xyz));
		float SpecularFactor = pow(wsNdH, SP.x) * wsNdl;
#endif


//		vec3 difuse = KDASL.x* DC.xyz*wsNdl*texture(renderedTexture, TexCoord).rgb;
		vec3 difuse = KDASL.x*texture(renderedTexture, TexCoord).rgb*wsNdl;
//		vec3 difuse = KDASL.x* DC.xyz*wsNdl;
		vec3 ambient = KDASL.y*(1.0 - wsNdl)*AC.xyz;
		vec3 specular = KDASL.z*SCS.xyz* SpecularFactor;

		vec4 colors = vec4((difuse.xyz + specular.xyz + ambient.xyz), 1.0);
		color *= (1.0 - (1.0 - SpotFactor) * 1.0 / (1.0 - SLCOC.x));
		//color *= (1.0 - (1.0 - SpotFactor) * 1.0 / (1.0 - 0.9));
		return colors;
	}
	else
	{
		return vec4(0, 0, 0, 0);
	}
}
vec4 calculateSpotLight2(vec4 posWS, vec4 normalWS)
{
	vec3 wsViewDir = -normalize((posWS.xyz - VP.xyz));
	vec4 lightDirWS = -normalize(posWS - VP);

	vec4 wsSpotLightDir = normalize(VP - posWS);
	float Theta = dot(wsSpotLightDir.xyz, lightDirWS.xyz);
	float Spot = Theta - cos(SLCOC.y * 0.5);
	Spot = Spot / (cos(SLCOC.x * 0.5) - cos(SLCOC.y * 0.5));
	float wsNdl = max(0.0, dot(lightDirWS.xyz, normalWS.xyz) * Spot);

	//float wsNdl = max(0.0f, dot(lightDirWS, normalWS));
#ifdef BLINN
	vec3 wsReflect = normalize(reflect(lightDirWS.xyz, normalWS.xyz)); //calculo la direccion del reflej
	float wsVdR = max(0.0f, dot(wsViewDir.xyz, wsReflect.xyz)); //me aseguro de optener la intencidad con la que se ve el reflejo
	float SpecularFactor = pow(wsVdR, SP.x) * wsNdl; //calculo el factor specular
#else//blin-phong
	vec3 wsHalf = normalize(wsViewDir.xyz + lightDirWS.xyz); //calculo el vector medio entre la direccion de vista y la direccion de la luz
	float wsNdH = max(0.0f, dot(normalWS.xyz, wsHalf.xyz));
	float SpecularFactor = pow(wsNdH, SP.x) * wsNdl;
#endif
	vec3 difuse = KDASL.x*texture(renderedTexture, TexCoord).rgb*wsNdl;
	//		vec3 difuse = KDASL.x* DC.xyz*wsNdl;
	vec3 ambient = KDASL.y*(1.0 - wsNdl)*AC.xyz;
	vec3 specular = KDASL.z*SCS.xyz* SpecularFactor;

	vec4 colors = vec4((difuse.xyz + specular.xyz + ambient.xyz), 1.0);
	//color *= (1.0 - (1.0 - SpotFactor) * 1.0 / (1.0 - SLCOC.x));
	//color *= (1.0 - (1.0 - SpotFactor) * 1.0 / (1.0 - 0.9));
	return colors;
}
#endif


void main()
{
#ifdef VER_LIGTH
	//color = texture(renderedTexture, TexCoord).rgb;
	color = colorfiesta;
	//color *= texture(normalTexture, TexCoord).rgb;
	//color = vec3( 1.0,0.5,0.3 );
#else

	vec3 wsBinormal = cross(wsNormal.xyz, wsTangent.xyz);
	//vec3 wsBinormal = normalize(cross(wsTangent.xyz, wsNormal.xyz));
	vec4 TexNormal = texture(normalTexture, TexCoord.xy).rgba;
	//TexNormal *= wsNormal;
	TexNormal = normalize( TexNormal*2.0f -1.0f);
	mat3 TBN = mat3(wsTangent.xyz, wsBinormal.xyz, wsNormal.xyz);

	vec3 wssNormal = normalize((TBN*TexNormal.xyz));
	vec4 norms = vec4(wssNormal.xyz, 0);
#ifdef DIR_LIGHT
	//color += calculateDirectionalLight(wsPos, norms);
	if (SLDPS.x == 1)
		color += calculateDirectionalLight(wsPos, norms);
#endif
#ifdef POINT_LIGHT
	//color += calculatePointLight(wsPos, norms);
	if (SLDPS.y == 1)
		color += calculatePointLight(wsPos, norms)/ ADPS.y;
#endif
#ifdef CONE_LIGHT
	//color += calculateSpotLight(wsPos, norms);
	if (SLDPS.z == 1)
	{
		//color += calculateSpotLight(wsPos, norms)/ ADPS.z;
		color += calculateSpotLight2(wsPos, norms)/ ADPS.z;
	}
#endif
	//color = texture(normalTexture, TexCoord.xy).rgba;
	//color = texture(renderedTexture, TexCoord.xy).rgba;
//	color=vec4(texture(renderedTexture, TexCoord).rgb,1);
		//color = ambient.xyz+difuse.xyz + specular.xyz *attenuation;
		//color*= texture(normalTexture, TexCoord).rgba;
		//colorfiesta = fiesta* NDLR;
	//color = vec4(DC*NDLR).xyz;
	//color += vec4(vec4(SpecularFactor, SpecularFactor, SpecularFactor, SpecularFactor)*SC).xyz;
	
	//color *= texture(renderedTexture, TexCoord).rgb;
#endif
}