#version 450 core

layout (location=0) in vec2 vUV;

layout (location=0) out vec4 fFragClr;
layout (location=1) out vec4 fBrightClr;

struct DirectionalLight
{
	vec3 Direction;
	
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;

	mat4 Transform;
};

struct Light
{
	vec3 Position;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

uniform Light uLights[10];
uniform sampler2D uPositions;
uniform sampler2D uNormals;
uniform sampler2D uAmbients;
uniform sampler2D uDiffuses;
uniform sampler2D uShadowMap;
uniform sampler2D uSSAO;
uniform DirectionalLight uDirectionalLight;
uniform int uNumLights;

float ShadowValue(vec4 LightFragPosition, vec3 Normal, vec3 LightDir, float ShadowBias)
{
	float Shadow = 0.0f;
	vec3 ProjectedCoords = LightFragPosition.xyz / LightFragPosition.w;

	if(ProjectedCoords.z <= 1.0f)
	{
		ProjectedCoords = (ProjectedCoords + 1.0f) / 2.0f;
		float CurrentDepth = ProjectedCoords.z;
		float Bias = max(ShadowBias * 0.0025f * (1.0f - dot(Normal, LightDir)), ShadowBias * 0.00005f);

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
	float AO = texture(uSSAO, vUV).r;
	vec3 Position = texture(uPositions, vUV).rgb;
	vec3 Normal = normalize(texture(uNormals, vUV).rgb);
	vec3 Ambient = texture(uAmbients, vUV).rgb * AO;
	vec3 Diffuse = texture(uDiffuses, vUV).rgb;
	float Specular = texture(uDiffuses, vUV).a;
	float ShadowBias = texture(uAmbients, vUV).a * 10.0;

	vec3 LightDir = normalize(-uDirectionalLight.Direction);
	vec3 ViewDir = normalize(-Position);

	// Ambient
	vec3 DirectionalAmbient = uDirectionalLight.Ambient * Ambient;

	// Diffuse
	float Diff = max(dot(Normal, LightDir), 0.0);
	vec3 DirectionalDiffuse = uDirectionalLight.Diffuse * Diff * Diffuse;

	// Specular
	vec3 HalfVec = normalize(LightDir + ViewDir);
	float Spec = pow(max(dot(Normal, HalfVec), 0.0), 32.0);
	vec3 DirectionalSpecular = uDirectionalLight.Specular * Spec * Specular;

	// Shadow
	vec4 LightFragPosition = uDirectionalLight.Transform * vec4(Position, 1.0);
	float Shadow = ShadowValue(LightFragPosition, Normal, LightDir, ShadowBias);

	// Final color
	vec3 Final = DirectionalAmbient + (1.0 - Shadow) * (DirectionalDiffuse + DirectionalSpecular);

	// Point lights
	for(int i = 0; i < uNumLights; ++i)
	{
		// Find vector
		vec3 PostionToLight = uLights[i].Position - Position;
		LightDir = normalize(PostionToLight);

		// Ambient
		vec3 PointAmbient = uLights[i].Ambient * Ambient;

		// Diffuse
		Diff = max(dot(Normal, LightDir), 0.0);
		vec3 PointDiffuse = uLights[i].Diffuse * Diff * Diffuse;

		// Specular
		HalfVec = normalize(LightDir + ViewDir);
		Spec = pow(max(dot(Normal, HalfVec), 0.0), 32.0);
		vec3 PointSpecular = uLights[i].Specular * Spec * Specular;

		// Attenuation
		float DistanceSquared = dot(PostionToLight, PostionToLight);
		float Attenuation = 1.0 / DistanceSquared;

		PointAmbient *= Attenuation;
		PointDiffuse *= Attenuation;
		PointSpecular *= Attenuation;

		// Additive blend
		Final += (PointAmbient + PointDiffuse + PointSpecular);
	}

	// Find bright pixels
	float Brightness = dot(Final, vec3(0.2126, 0.7152, 0.0722));

	// Save to bright texture
	if(Brightness > 1.0)
		fBrightClr = vec4(Final, 1.0);
	else
		fBrightClr = vec4(0.0, 0.0, 0.0, 1.0);

	fFragClr = vec4(Final, 1.0);
}