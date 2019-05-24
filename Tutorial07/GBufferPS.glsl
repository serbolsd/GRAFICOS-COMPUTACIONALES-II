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
uniform vec4 SC;//Specular Color
uniform vec4 DC;//Difuse Color
uniform vec4 AC;//Ambiental Color
uniform vec4 SP;//speular Power 
uniform vec4 KDASL;//const of difuse, ambiental and specular 
uniform vec4 LPLQ;//LIGHT POINT LINEAR QUADRATIC 
uniform vec4 SLCOC; //SPOTLIGHT CUTOFF OTHECUTOFF
uniform vec4 LP;//light Position
//uniform vec4 SDOC;
layout(location = 0) out vec4 color;
vec4 calculateDirectionalLight(vec4 posWS, vec4 normalWS)
{
	vec3 wsViewDir = -normalize((posWS.xyz - VP.xyz));
	vec4 lightDirWS = normalize(-LD);
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
	vec3 difuse = KDASL.x* DC.xyz*wsNdl;
	vec3 specular = KDASL.z*SC.xyz* SpecularFactor;
	vec3 ambient = KDASL.y*(1.0 - wsNdl)*AC.xyz;

	vec4 colord = vec4((difuse.xyz + specular.xyz + ambient.xyz), 1.0);
	return colord;
}

vec4 calculatePointLight(vec4 posWS, vec4 normalWS)
{
	vec3 wsViewDir = -normalize((posWS.xyz - VP.xyz));
	vec4 lightDirWS = posWS - LP;
	float dist = length(lightDirWS);
	lightDirWS = normalize(-lightDirWS);
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
	float lin = KDASL.w + (LPLQ.x * dist);
	float quad = (LPLQ.y * (dist* dist));
	float attenuation = (1.0 / (lin + quad));

	vec3 difuse = KDASL.x* DC.xyz*wsNdl;
	vec3 specular = KDASL.z*SC.xyz* SpecularFactor;
	vec3 ambient = KDASL.y*(1.0 - wsNdl)*AC.xyz;

	vec4 colorp = vec4((difuse.xyz + specular.xyz + ambient.xyz), 1.0) / attenuation;
	return colorp;
}

vec4 calculateSpotLight(vec4 posWS, vec4 normalWS)
{
	vec3 wsViewDir = -normalize((posWS.xyz - VP.xyz));
	vec4 lightDirWS = (VP - posWS);
	vec3 LightToPixel = normalize(posWS.xyz - VP.xyz);
	float SpotFactor = dot(LightToPixel, lightDirWS.xyz);

	//vec3 wsViewDir = -normalize((posWS.xyz - VP.xyz));
	float wsNdl = max(0.0f, dot(lightDirWS, normalWS));
	//if (SpotFactor > Cutoff)
	if (SpotFactor > SLCOC.x)
	{
		lightDirWS = VP - posWS;
		float dist = length(lightDirWS);
		lightDirWS = normalize(-lightDirWS);
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
		float lin = KDASL.w + (LPLQ.x * dist);
		float quad = (LPLQ.y * (dist* dist));
		float attenuation = (1.0 / (lin + quad));

		vec3 difuse = KDASL.x* DC.xyz*wsNdl;
		vec3 ambient = KDASL.y*(1.0 - wsNdl)*AC.xyz;
		vec3 specular = KDASL.z*SC.xyz* SpecularFactor;

		vec4 colors = vec4((difuse.xyz + specular.xyz + ambient.xyz), 1.0) / attenuation;
		color *= (1.0 - (1.0 - SpotFactor) * 1.0 / (1.0 - SLCOC.x));
		//color *= (1.0 - (1.0 - SpotFactor) * 1.0 / (1.0 - 0.9));
		return colors;
	}
	else
	{
		return vec4(0, 0, 0, 0);
	}
}


void main()
{
#ifdef VER_LIGTH
	//color = texture(renderedTexture, TexCoord).rgb;
	color = colorfiesta.rgb;
	//color *= texture(normalTexture, TexCoord).rgb;
	//color = vec3( 1.0,0.5,0.3 );
#else

	vec3 wsBinormal = normalize(cross(wsNormal.xyz, wsTangent.xyz));
	vec3 TexNormal = texture(normalTexture, TexCoord.xy).rgb;
	TexNormal.xyz = normalize(2.0f * TexNormal.xyz - 1.0f);
	mat3 TBN = mat3(wsTangent.xyz, wsBinormal.xyz, wsNormal.xyz);

	vec3 wssNormal = normalize((TexNormal.xyz*TBN));
	vec4 norms = vec4(wssNormal.xyz, 1);
#ifdef DIR_LIGHT
	//color += calculateDirectionalLight(wsPos, norms);
	color += calculateDirectionalLight(wsPos, wsNormal);
#endif
#ifdef POINT_LIGHT
	//color += calculatePointLight(wsPos, norms);
	color += calculatePointLight(wsPos, wsNormal);
#endif
#ifdef CONE_LIGHT
	//color += calculateSpotLight(wsPos, norms);
	color += calculateSpotLight(wsPos, wsNormal);
#endif
		//color = ambient.xyz+difuse.xyz + specular.xyz *attenuation;
		//color*= texture(normalTexture, TexCoord).rgb;
		//colorfiesta = fiesta* NDLR;
	//color = vec4(DC*NDLR).xyz;
	//color += vec4(vec4(SpecularFactor, SpecularFactor, SpecularFactor, SpecularFactor)*SC).xyz;
	
	//color *= texture(renderedTexture, TexCoord).rgb;
#endif
}