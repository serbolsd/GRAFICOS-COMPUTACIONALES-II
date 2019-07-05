////////////////////////////////////////////////////////////////////////////////
// Filename: GBuffer.ps
////////////////////////////////////////////////////////////////////////////////
#version 400
#define M_PI 3.14159265359f;

in vec2 TexCoord;
//out vec3 color;
//out vec3 color;
uniform sampler2D BrightTexture;
uniform sampler2D Blur_HTexture;
uniform sampler2D Blur_VTexture;
uniform int blurMip;         //luminance mip leve
layout(location = 0) out vec4 color;

void main()
{

	vec4 bright = texture2D(BrightTexture, TexCoord);
	vec4 blurH = textureLod(Blur_HTexture, vec2(TexCoord), blurMip + 1);
	vec4 blurV = textureLod(Blur_VTexture, vec2(TexCoord), blurMip + 1);
	//combination
  //float4 color= float4(1,1,0,1);
  //return color;
	vec4 output = (0.5f*(blurH + blurV)*bright);
	color= output;

}