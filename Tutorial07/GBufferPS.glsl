////////////////////////////////////////////////////////////////////////////////
// Filename: GBuffer.ps
////////////////////////////////////////////////////////////////////////////////
#version 400
#define M_PI 3.14159265359f;
#define VER_LIGTH
//#define BLINN
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
//uniform vec4 SDOC;

layout(location = 0) out vec3 color;
void main()
{
#ifdef VER_LIGTH
	//color = texture(renderedTexture, TexCoord).rgb;
	color = colorfiesta.rgb;
	//color *= texture(normalTexture, TexCoord).rgb;
	//color = vec3( 1.0,0.5,0.3 );
#else
	#ifdef POINT_LIGHT
		vec4 lightDirWS = -normalize(VP - wsPos);
	#else
		vec4 lightDirWS = -normalize(LD);   /// calculate de light direction in world space
	#endif
	//vec4 lightDirWS = -normalize(LD);
	float wsNdl = max(0.0f, dot(lightDirWS, wsNormal));
	vec4 NDLR = vec4(wsNdl, wsNdl, wsNdl, wsNdl); //lo transformo en un vec4
	vec3 wsViewDir = -normalize((wsPos.xyz - VP.xyz));

	vec3 wsBinormal = normalize(cross(wsNormal.xyz, wsTangent.xyz));
	vec3 TexNormal = texture(normalTexture, TexCoord.xy).rgb;
	TexNormal.xyz = normalize(2.0f * TexNormal.xyz - 1.0f);
	mat3 TBN = mat3(wsTangent.xyz, wsBinormal.xyz, wsNormal.xyz);

	vec3 wssNormal = normalize((TexNormal.xyz*TBN));

	#ifdef BLINN
		vec3 wsReflect = normalize(reflect(-lightDirWS.xyz, wssNormal.xyz)); //calculo la direccion del reflej
		float wsVdR = max(0.0f, dot(wssNormal.xyz, wsReflect.xyz)); //me aseguro de optener la intencidad con la que se ve el reflejo
		float SpecularFactor = pow(wsVdR, SP.x) * wsNdl; //calculo el factor specular
	#else//blin-phong
		vec3 wsHalf = normalize(wsViewDir.xyz + lightDirWS.xyz); //calculo el vector medio entre la direccion de vista y la direccion de la luz
		float wsNdH = max(0.0f, dot(wssNormal.xyz, wsHalf.xyz));
		float SpecularFactor = pow(wsNdH, SP.x) * wsNdl;
	#endif
	#ifdef POINT_LIGHT
		float dist = length(VP.xyz - wsPos.xyz);
		float lin = KDASL.w + (1 * dist);
		float quad = (1 * (dist* dist));
		float attenuation = (1.0 / (lin + quad));
		//float attenuation = 1;   /// calculate de light direction in world space
	#else
		float attenuation = 1;   /// calculate de light direction in world space
	#endif
		vec3 difuse = KDASL.x* DC.xyz*wsNdl*attenuation;
		vec3 specular = KDASL.z*SC.xyz* SpecularFactor*attenuation;
		vec3 ambient = KDASL.y*(1.0 - wsNdl)*AC.xyz*attenuation;
		color = ambient.xyz+difuse.xyz + specular.xyz *attenuation;
		//color*= texture(normalTexture, TexCoord).rgb;
		//colorfiesta = fiesta* NDLR;
	//color = vec4(DC*NDLR).xyz;
	//color += vec4(vec4(SpecularFactor, SpecularFactor, SpecularFactor, SpecularFactor)*SC).xyz;
	
	//color *= texture(renderedTexture, TexCoord).rgb;
#endif
}