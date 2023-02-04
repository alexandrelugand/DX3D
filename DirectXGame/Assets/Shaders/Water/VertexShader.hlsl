#include "Common.hlsl"

Texture2D HeightMap: register(t0);
sampler HeightMapSampler: register(s0);

VPS_INOUT main(VS_INPUT input)
{
	VPS_INOUT output = (VPS_INOUT)0;

	float2 texcoord1 = (input.texcoord * 8) + time * 0.009;
	float2 texcoord2 = (float2(-input.texcoord.x, input.texcoord.y) * 8) + time * 0.01;

	//Get the height for current vertex
	float height1 = HeightMap.SampleLevel(HeightMapSampler, texcoord1, 0).r;
	float height2 = HeightMap.SampleLevel(HeightMapSampler, texcoord2, 0).r;

	float height = lerp(height1, height2, 0.5);

	//Vertex in World space coordinate
	output.position = mul(float4(input.position.x * water.size.x, height * water.size.y, input.position.z * water.size.z, 1), world);
	output.world_pos = output.position.xyz;

	//Vertex in View space coordinate
	output.position = mul(output.position, camera.view);

	//Vertex in Project space coordinate
	output.position = mul(output.position, camera.proj);

	//Set texture coordinates and normals in output structure
	output.texcoord = input.texcoord;

	return output;
}
