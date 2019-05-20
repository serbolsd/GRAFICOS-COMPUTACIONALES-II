////////////////////////////////////////////////////////////////////////////////
// Filename: GBuffer.vs
////////////////////////////////////////////////////////////////////////////////
#version 400

#define VER_LIGTH
//#define BLINN
//#define CONE_LIGHT
#define POINT_LIGHT

layout(location = 0) in vec3 inVertexPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;



out vec2 TexCoord;
out vec4 colorfiesta;
out vec4 wsNormal;
out vec4 wsPos;
out vec4 wsTangent;
//out vec4 wPosition;
//out mat4 wPosition;

uniform mat4 WM;//World Matrix
uniform mat4 VM;//View Matrix
uniform mat4 PM;//Projection Matrix
uniform mat4 MM;//Model Matrxi
uniform vec4 LD;//light Direction
uniform vec4 LP;//light Position
uniform vec4 VP;//View Position
uniform vec4 SC;//Specular Color
uniform vec4 DC;//Difuse Color
uniform vec4 AC;//Ambient Color
uniform vec4 SP;//speular Power 
uniform vec4 KDASL;//const of difuse, ambiental and specular AND LIGHT
uniform vec4 LPLQ;//LIGHT POINT LINEAR QUADRATIC 
uniform vec4 SLCOC; //SPOTLIGHT CUTOFF OTHECUTOFF
uniform vec4 fiesta;

void main()
{
	mat4 MVP = PM * VM * MM;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	gl_Position = MVP * WM * vec4(inVertexPosition, 1.0f);

	// Calculate position in world space
	//wPosition = WM * vec4(inVertexPosition, 1.0f);

	// Store the texture coordinates for the pixel shader.
	

	//vec4 normalWS = vec4(inNormal,1)*WM;
	vec4 posWS = vec4(inVertexPosition, 1)*WM;    //calculate the vertex position in world space
	vec4 normalWS = normalize(vec4(inNormal,1)*WM); //calculate the normals position in world space
	vec4 Tangentws = normalize(vec4(inTangent, 1)*WM);
#ifdef VER_LIGTH

	vec3 wsViewDir = -normalize((posWS.xyz - VP.xyz));// calculo la direcion en la que se esta viendo
	#ifdef POINT_LIGHT
		vec4 lightDirWS = -normalize(LP - posWS);
	#else
		vec4 lightDirWS = -normalize(VP - posWS);
		//vec4 lightDirWS = -normalize(LD);   /// calculate de light direction in world space
	#endif
	//vec4 lightDirWS = -normalize(LD);   /// calculate de light direction in world space
	float wsNdl =max(0.0f ,dot(lightDirWS, normalWS)); //calcula si las normales estan viendo la luz o no en el espacio de mundo
	vec4 NDLR = vec4(wsNdl, wsNdl, wsNdl, wsNdl); //lo transformo en un vec4
	#ifdef BLINN
		vec3 wsReflect = normalize(reflect(-lightDirWS.xyz, normalWS.xyz)); //calculo la direccion del reflej
		float wsVdR = max(0.0f, dot(wsViewDir.xyz, wsReflect.xyz)); //me aseguro de optener la intencidad con la que se ve el reflejo
		float SpecularFactor = pow(wsVdR, SP.x) * wsNdl; //calculo el factor specular
	#else//blin-phong
		vec3 wsHalf = normalize(wsViewDir.xyz + lightDirWS.xyz); //calculo el vector medio entre la direccion de vista y la direccion de la luz
		float wsNdH = max(0.0f, dot(normalWS.xyz, wsHalf.xyz));
		float SpecularFactor = pow(wsNdH, SP.x) * wsNdl;
	#endif

	#ifdef POINT_LIGHT
			float dist = length(LP.xyz - inVertexPosition.xyz);
			float lin = KDASL.w+(LPLQ.x * dist);
			float quad = (LPLQ.y * (dist* dist));
			float attenuation = (1.0 / (lin + quad));
			//float attenuation = 1;   /// calculate de light direction in world space
	#else

	#endif
	vec3 difuse = KDASL.x* DC.xyz*wsNdl;
	vec3 specular = KDASL.z*SC.xyz* SpecularFactor/attenuation;
	vec3 ambient = KDASL.y*(1.0 - wsNdl)*AC.xyz;
	//colorfiesta = fiesta* NDLR;
	//colorfiesta = NDLR*DC;
	//colorfiesta += vec4(SpecularFactor, SpecularFactor, SpecularFactor, SpecularFactor)*SC;
	colorfiesta = vec4(difuse+ambient+ specular,1);

#else
	wsPos = posWS;
	wsNormal = normalWS;
	TexCoord = inTexCoord;
	wsTangent = Tangentws;

#endif
	//printf(TexCoord);
}