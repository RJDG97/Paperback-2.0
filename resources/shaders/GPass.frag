#version 450 core

layout (location=0) in vec3 vPosition;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec3 vTangent;
layout (location=3) in vec3 vBiTangent;
layout (location=4) in vec2 vUV;

layout (location=0) out vec4 fPosition;
layout (location=1) out vec4 fNormal;
layout (location=2) out vec4 fAmbient;
layout (location=3) out vec4 fDiffuse;

struct Material
{
	bool TexturedDiffuse;
	bool TexturedAmbient;
	bool TexturedSpecular;
	bool TexturedNormal;

	sampler2D Diffuse;
	sampler2D Ambient;
	sampler2D Specular;
	sampler2D Normal;
};

uniform Material uMat;
uniform float uShadowBias;

void main()
{
	mat3 TangentToWorld = mat3(normalize(vTangent), normalize(vBiTangent), normalize(vNormal));
	float Bias = uShadowBias / 10.0;

	// Write to gpass textures
	if(uMat.TexturedDiffuse)
	{
		vec3 Diffuse = texture(uMat.Diffuse, vUV).rgb;
		vec3 Normal;
		vec4 Ambient;
		float Specular;

		// Normal
		if(uMat.TexturedNormal)
		{
			Normal = texture(uMat.Normal, vUV).rgb;
			Normal = normalize(Normal * 2.0 - 1.0);
			Normal = TangentToWorld * Normal;
		}
		else
		{
			Normal = normalize(vNormal);
		}

		// Ambient
		if(uMat.TexturedAmbient)
		{
			Ambient = vec4(texture(uMat.Ambient, vUV).rgb, Bias);
		}
		else
		{
			Ambient = vec4(Diffuse, Bias);
		}

		// Specular
		if(uMat.TexturedSpecular)
		{
			Specular = texture(uMat.Specular, vUV).r;
		}
		else
		{
			Specular = 1.0;
		}

		fPosition = vec4(vPosition, 1.0);
		fNormal = vec4(Normal, 1.0);
		fAmbient = Ambient;
		fDiffuse = vec4(Diffuse, Specular);
	}
	else
	{
		fPosition = vec4(vPosition, 1.0);
		fNormal = vec4(normalize(vNormal), 1.0);
		fAmbient = vec4(1.0, 0.0, 0.5, Bias);
		fDiffuse = vec4(1.0, 0.0, 0.5, 1.0);
	}
}