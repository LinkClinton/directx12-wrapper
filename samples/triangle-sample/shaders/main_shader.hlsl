#include "fix_resharper_error.hlsl"

struct vs_output {
	SYSTEM_VALUE(float4, position, SV_POSITION);
};

struct constant_input {
	float4 color;
	uint width;
	uint height;
};

SHADER_CONSTANT_BUFFER_DEFINE(constant_input, config, b0, space0);

vs_output vs_main(SYSTEM_VALUE(float3, position, POSITION))
{
	vs_output output;

	output.position = float4(
		((position.x / config.width) - 0.5) * 2,
		((position.y / config.height) - 0.5) * 2,
		0, 1);
	
	return output;
}

SYSTEM_VALUE(float4, ps_main(vs_output input), SV_TARGET)
{
	return config.color;
}