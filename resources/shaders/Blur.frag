#version 450 core

layout (location=0) in vec2 vUV;

layout (location=0) out vec4 fFragClr;

uniform bool uHorizontal;
uniform sampler2D uImage;

const float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{
	vec2 Offset = 1.0 / textureSize(uImage, 0);
	vec3 Result = texture(uImage, vUV).rgb * weight[0];

	if(uHorizontal)
	{
		for(int i = 1; i < 5; ++i)
		{
			Result += texture(uImage, vUV + vec2(Offset.x * i, 0.0)).rgb * weight[i];
			Result += texture(uImage, vUV - vec2(Offset.x * i, 0.0)).rgb * weight[i];
		}
	}
	else
	{
		for(int i = 1; i < 5; ++i)
		{
			Result += texture(uImage, vUV + vec2(0.0, Offset.y * i)).rgb * weight[i];
			Result += texture(uImage, vUV - vec2(0.0, Offset.y * i)).rgb * weight[i];
		}
	}

	fFragClr = vec4(Result, 1.0);
}