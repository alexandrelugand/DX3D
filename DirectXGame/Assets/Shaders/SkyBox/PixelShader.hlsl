#include "Common.hlsl"

Texture2D Color: register(t0);
sampler ColorSampler: register(s0);

float4 main(VPS_INOUT input) : SV_TARGET
{
	float4 color = Color.Sample(ColorSampler, input.texcoord);
	float3 final_light = ComputeFogColor(fog, camera, input.world_pos.xyz, color.rgb);
	return float4(final_light, 1.0);
}