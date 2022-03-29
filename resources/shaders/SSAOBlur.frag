#version 450 core

layout (location=0) in vec2 vUV;

layout (location=0) out vec4 fFragClr;

uniform sampler2D uSSAOInput;

void main()
{
	vec2 TexelSize = 1.0 / vec2(textureSize(uSSAOInput, 0));
	vec3 Result = vec3(0,0,0);

	for(int x = -2; x < 2; ++x)
	{
		for(int y = -2; y < 2; ++y)
		{
			vec2 Offset = vec2(float(x), float(y)) * TexelSize;
			Result += texture(uSSAOInput, vUV + Offset).rgb;
		}
	}

	fFragClr = vec4(Result / (4.0 * 4.0), 1.0);
}