#include "Common.hlsl"

Texture2D Color: register(t0);
sampler ColorSampler: register(s0);

float4 main(VPS_INOUT input) : SV_TARGET
{
	float4 color = Color.Sample(ColorSampler, input.texcoord);

	float3 final_light = ComputePhongLighting(
		camera,
		light,
		input.world_pos.xyz,
		input.normal.xyz,
		4,
		color.rgb * float3(0.09, 0.09, 0.09),
		0.7,
		color.rgb,
		0.0,
		float3(1, 1, 1),
		10.0
	);

	final_light = ComputeFogColor(fog, camera, input.world_pos.xyz, final_light);
	return float4(final_light, 1.0);
}