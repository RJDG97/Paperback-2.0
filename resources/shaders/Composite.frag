#version 450 core

layout (location=0) in vec2 vUV;

layout (location=0) out vec4 fFragClr;

uniform sampler2D uImage;
uniform sampler2D uBlur;
uniform float uExposure;

void main()
{
	vec3 HDRColor = texture(uImage, vUV).rgb;
	vec3 BlurColor = texture(uBlur, vUV).rgb;

	// Bloom blending
	HDRColor += BlurColor;

	// Tone mapping
	vec3 Result = vec3(1.0) - exp(-HDRColor * uExposure);

	// Gamma correction
	Result = pow(Result, vec3(1.0 / 2.2));
	fFragClr = vec4(Result, 1.0);
}