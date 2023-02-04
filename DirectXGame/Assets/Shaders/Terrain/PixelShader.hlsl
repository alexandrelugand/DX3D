#include "Common.hlsl"

Texture2D HeightMap: register(t0);
sampler HeightMapSampler: register(s0);

Texture2D Ground: register(t1);
sampler GroundSampler: register(s1);

Texture2D Wall: register(t2);
sampler WallSampler: register(s2);

float4 main(VPS_INOUT input) : SV_TARGET
{
	float3 normal = ComputeNormalFromHeightMap(
		HeightMap,
		HeightMapSampler,
		terrain.heightMapSize,
		input.texcoord,
		terrain.size.y
	);

	normal = normalize(mul(normal, (float3x3)world));

	float4 ground = Ground.Sample(GroundSampler, input.texcoord * 100.0);
	float4 wall = Wall.Sample(WallSampler, input.texcoord * 60);

	float4 color = ground;
	float angle = abs(dot(normal, float3(0, 1, 0)));

	float minAngle = 0.5;
	float maxAngle = 1.0;
	
	if (angle >= minAngle && angle <= maxAngle)
		color = lerp(wall, color, (angle - minAngle) * (1.0 / (maxAngle - minAngle)));

	if (angle < minAngle)
		color = wall;

	float alpha = 0.0;

	float height = HeightMap.SampleLevel(HeightMapSampler, input.texcoord, 0).r;
	if (height < 0.01)
		alpha = 0;
	else
		alpha = 1;

	float3 final_light = ComputePhongLighting(
		camera,
		light,
		input.world_pos.xyz,
		normal.xyz,
		4,
		color.rgb * float3(0.09, 0.09, 0.09),
		0.7,
		color.rgb,
		0.0,
		float3(1, 1, 1),
		10.0
	);

	final_light = ComputeFogColor(fog, camera, input.world_pos.xyz, final_light);
	return float4(final_light, alpha);
}