#version 450 core

layout (location=0) in vec2 vUV;
layout (location=1) in float vOpacity;

layout (location=0) out vec4 fFragClr;
layout (location=1) out vec4 fBrightClr;

uniform bool uTexturedDiffuse;
uniform sampler2D uDiffuse;

void main()
{
	vec4 Color;

	// Write to color texture;
	if(uTexturedDiffuse)
	{
		Color = texture(uDiffuse, vUV);
	}
	else
	{
		Color = vec4(1.0, 0.0, 0.5, 1.0);
	}

	if(Color.a < 0.1)
		discard;

	float Brightness = dot( vec3(Color), vec3(0.2126, 0.7152, 0.0722));

	if(Brightness > 1.0)
		fBrightClr = vec4(Color.rgb, Color.a * vOpacity);
	else
		fBrightClr = vec4(0.0, 0.0, 0.0, 1.0);

	fFragClr = vec4(Color.rgb, Color.a * vOpacity);;
}