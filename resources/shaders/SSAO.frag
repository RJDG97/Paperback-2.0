#version 450 core

layout (location=0) in vec2 vUV;

layout (location=0) out vec4 fFragClr;

uniform sampler2D uPositions;
uniform sampler2D uNormals;
uniform sampler2D uTexNoise;

uniform mat4 uProjection;
uniform vec3 uSamples[64];
uniform float uWidth;
uniform float uHeight;

int KernelSize = 64;
float Radius = 0.5;
float Bias = 0.025;

vec2 NoiseScale = vec2(uWidth / 4.0, uHeight / 4.0);

void main()
{
	vec3 Position = texture(uPositions, vUV).xyz;
	vec3 Normal = normalize(texture(uNormals, vUV).rgb);
	vec3 RandomVec = normalize(texture(uTexNoise, vUV * NoiseScale).rgb);

	vec3 Tangent = normalize(RandomVec - Normal * dot(RandomVec, Normal));
	vec3 Bitangent = cross(Normal, Tangent);
	mat3 TBN = mat3(Tangent, Bitangent, Normal);

	float Occlusion = 0.0;
	for(int i = 0; i < KernelSize; ++i)
	{
		vec3 SamplePos = TBN * uSamples[i];
		SamplePos = Position + SamplePos * Radius;

		vec4 Offset = vec4(SamplePos, 1.0);
		Offset = uProjection * Offset;
		Offset.xyz /= Offset.w;
		Offset.xyz = Offset.xyz * 0.5 + 0.5;

		float SampleDepth = texture(uPositions, Offset.xy).z;

		float RangeCheck = smoothstep(0.0, 1.0, Radius / abs(Position.z - SampleDepth));
		Occlusion += (SampleDepth >= SamplePos.z + Bias ? 1.0 : 0.0) * RangeCheck;
	}

	Occlusion = 1.0 - (Occlusion / KernelSize);
	fFragClr = vec4(Occlusion, Occlusion, Occlusion, 1.0);
}