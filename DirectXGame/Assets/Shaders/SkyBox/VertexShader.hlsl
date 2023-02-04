#include "Common.hlsl"

VPS_INOUT main(VS_INPUT input)
{
	VPS_INOUT output = (VPS_INOUT)0;

	//Vertex in World space coordinate
	output.position = mul(input.position, world);
	output.world_pos = output.position.xyz;

	//Vertex in View space coordinate
	output.position = mul(output.position, camera.view);

	//Vertex in Project space coordinate
	output.position = mul(output.position, camera.proj);

	//Set texture coordinates and normals in output structure
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float3x3)world));

	return output;
}