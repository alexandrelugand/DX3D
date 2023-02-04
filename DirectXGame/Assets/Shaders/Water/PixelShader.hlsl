#include "Common.hlsl"

Texture2D HeightMap: register(t0);
sampler HeightMapSampler: register(s0);

//Texture2D Ground: register(t1);
//sampler GroundSampler: register(s1);
//
//Texture2D Wall: register(t2);
//sampler WallSampler: register(s2);

float4 main(VPS_INOUT input) : SV_TARGET
{
	float2 texcoord1 = (input.texcoord * 8) + time * 0.004;
	float2 texcoord2 = (float2(-input.texcoord.x, input.texcoord.y) * 8) + time * 0.004;

	float3 normal1 = ComputeNormalFromHeightMap(
		HeightMap,
		HeightMapSampler,
		water.heightMapSize,
		texcoord1,
		water.size.y * 3
	);

	float3 normal2 = ComputeNormalFromHeightMap(
		HeightMap,
		HeightMapSampler,
		water.heightMapSize,
		texcoord2,
		water.size.y * 3
	);

	float3 normal = normalize(lerp(normal1, normal2, 0.5));
	normal = normalize(mul(normal, (float3x3)world));

	float4 water = float4(0.24, 0.37, 0.49, 1); // Ground.Sample(GroundSampler, input.texcoord * 100.0);
	float4 crest = float4(1, 1, 1, 1);  //Wall.Sample(WallSampler, input.texcoord * 60);

	float4 color = water;
	float angle = abs(dot(normal, float3(0, 1, 0)));

	float minAngle = 0.5;
	float maxAngle = 1.0;
	
	if (angle >= minAngle && angle <= maxAngle)
		color = lerp(crest, color, (angle - minAngle) * (1.0 / (maxAngle - minAngle)));

	if (angle < minAngle)
		color = crest;

	float3 final_light = ComputePhongLighting(
		camera,
		light,
		input.world_pos.xyz,
		normal.xyz,
		4,
		color.rgb * float3(0.09, 0.09, 0.09),
		0.7,
		color.rgb,
		0.5,
		light.color.rgb,
		50.0
	);

	final_light = ComputeFogColor(fog, camera, input.world_pos.xyz, final_light);
	return float4(final_light, 0.8);
}