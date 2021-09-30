#version 450 core

layout (location=0) in vec3 vPosition;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec2 vUV;

layout (location=3) in vec3 tLightPosition;
layout (location=4) in vec3 tViewPosition;
layout (location=5) in vec3 tFragPosition;

layout (location=0) out vec4 fFragClr;

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

struct Light
{
	vec3 Direction;
	
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

uniform vec3 uCameraPosition;
uniform Material uMat;
uniform Light uLight;

void main()
{
	// Write to color texture
	if(uMat.TexturedDiffuse)
	{
		vec3 Color = texture(uMat.Diffuse, vUV).rgb;
		vec3 Normal;
		vec3 Ambient;
		vec3 Diffuse;
		vec3 Specular;
		vec3 LightDir;
		vec3 ViewDir;

		// Normal
		if(uMat.TexturedNormal)
		{
			Normal = texture(uMat.Normal, vUV).rgb;
			Normal = normalize(Normal * 2.0 - 1.0);
			LightDir = normalize(tLightPosition - tFragPosition);
			ViewDir = normalize(tViewPosition - tFragPosition);
		}
		else
		{
			Normal = normalize(vNormal);
			LightDir = normalize(-uLight.Direction);
			ViewDir = normalize(uCameraPosition - vPosition);
		}

		// Ambient
		if(uMat.TexturedAmbient)
		{
			Ambient = uLight.Ambient * texture(uMat.Ambient, vUV).rgb;
		}
		else
		{
			Ambient = uLight.Ambient * Color;
		}

		// Diffuse
		float Diff = max(dot(Normal, LightDir), 0.0);
		Diffuse = uLight.Diffuse * Diff * Color;

		// Specular
		vec3 HalfVec = normalize(LightDir + ViewDir);
		float Spec = pow(max(dot(Normal, HalfVec), 0.0), 32.0);

		if(uMat.TexturedSpecular)
		{
			Specular = uLight.Specular * Spec * texture(uMat.Specular, vUV).rgb;
		}
		else
		{
			Specular = uLight.Specular * Spec;
		}

		vec3 Final = pow(Ambient + Diffuse + Specular, vec3(1.0/2.2));
		fFragClr = vec4(Final, 1.0);
	}
	else
	{
		fFragClr = vec4(1.0, 0.0, 0.5, 1.0);
	}
}