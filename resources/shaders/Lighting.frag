#version 450 core

layout (location=0) in vec3 vPosition;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec2 vUV;

layout (location=3) in vec3 tLightPosition;
layout (location=4) in vec3 tViewPosition;
layout (location=5) in vec3 tFragPosition;

layout (location=6) in vec4 lFragPosition;

layout (location=0) out vec4 fFragClr;
layout (location=1) out vec4 fBrightClr;

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

	mat4 Transform;
};

uniform float uShadowBias;
uniform Material uMat;
uniform Light uLight;
uniform sampler2D uShadowMap;
uniform vec3 uCameraPosition;

float ShadowValue(vec4 lFragPosition, vec3 Normal, vec3 LightDir)
{
	float Shadow = 0.0f;
	vec3 ProjectedCoords = lFragPosition.xyz / lFragPosition.w;

	if(ProjectedCoords.z <= 1.0f)
	{
		ProjectedCoords = (ProjectedCoords + 1.0f) / 2.0f;
		float CurrentDepth = ProjectedCoords.z;
		float Bias = max(uShadowBias * 0.0025f * (1.0f - dot(Normal, LightDir)), uShadowBias * 0.00005f);

		int SampleRadius = 2;
		vec2 TexelSize = 1.0 / textureSize(uShadowMap, 0);

		for(int y = -SampleRadius; y <= SampleRadius; y++)
		{
			for(int x = -SampleRadius; x <= SampleRadius; x++)
			{
				float ClosestDepth = texture(uShadowMap, ProjectedCoords.xy + vec2(x,y) * TexelSize).r;

				if(CurrentDepth > ClosestDepth + Bias)
					Shadow += 1.0f;
			}
		}

		Shadow /= pow(SampleRadius * 2 + 1, 2);
	}

	return Shadow;
}

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

		float Shadow = ShadowValue(lFragPosition, normalize(vNormal), normalize(-uLight.Direction));

		// Calculate final color and extract bright colors
		vec3 Final = Ambient + (1.0 - Shadow) * (Diffuse + Specular);
		float Brightness = dot(Final, vec3(0.2126, 0.7152, 0.0722));

		if(Brightness > 1.0)
			fBrightClr = vec4(Final, 1.0);
		else
			fBrightClr = vec4(0.0, 0.0, 0.0, 1.0);

		fFragClr = vec4(Final, 1.0);
	}
	else
	{
		vec3 Normal = normalize(vNormal);

		float Shadow = ShadowValue(lFragPosition, normalize(vNormal), normalize(-uLight.Direction));

		fBrightClr = vec4(0.0, 0.0, 0.0, 1.0);
		fFragClr = (1.0 - Shadow) * vec4(1.0, 0.0, 0.5, 1.0);
	}
}