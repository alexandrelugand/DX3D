#include "Common.hlsl"

Texture2D HeightMap: register(t0);
sampler HeightMapSampler: register(s0);

VPS_INOUT main(VS_INPUT input)
{
	VPS_INOUT output = (VPS_INOUT)0;

	//Get the height for current vertex
	float height = HeightMap.SampleLevel(HeightMapSampler, input.texcoord, 0).r;

	//Vertex in World space coordinate
	output.position = mul(float4(input.position.x * terrain.size.x, height * terrain.size.y, input.position.z * terrain.size.z, 1), world);
	output.world_pos = output.position.xyz;

	//Vertex in View space coordinate
	output.position = mul(output.position, camera.view);

	//Vertex in Project space coordinate
	output.position = mul(output.position, camera.proj);

	//Set texture coordinates and normals in output structure
	output.texcoord = input.texcoord;

	return output;
}
