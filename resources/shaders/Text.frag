#version 450 core

layout (location=0) in vec2 vUV;

layout (location=0) out vec4 fFragClr;

uniform bool uTextured;
uniform vec3 uColor;
uniform sampler2D uFontAtlas;

float median(float r, float g, float b) 
{
	return max(min(r, g), min(max(r, g), b));
}

void main()
{
	// Write to color texture
	if(uTextured)
	{
		vec3 msd = texture(uFontAtlas, vUV).rgb;
		float sd = median(msd.r, msd.g, msd.b);
		float screenPxDistance = 4.0 * (sd - 0.5);
		float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);

		fFragClr = vec4(uColor, 1.0 * opacity);
	}
	else
	{
		fFragClr = vec4(1.0, 0.0, 0.5, 1.0);
	}
}