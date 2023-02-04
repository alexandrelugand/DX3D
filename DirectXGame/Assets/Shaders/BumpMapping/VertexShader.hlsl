#include "Common.hlsl"

VPS_INOUT main(VS_INPUT input)
{
	VPS_INOUT output = (VPS_INOUT)0;

	//Vertex in World space coordinate
	output.position = mul(input.position, world);
	output.world_pos = output.position.xyz;
	output.camera_direction = normalize(output.position.xyz - camera.position.xyz);

	//Vertex in View space coordinate
	output.position = mul(output.position, camera.view);

	//Vertex in Project space coordinate
	output.position = mul(output.position, camera.proj);

	//Set texture coordinates and normals in output structure
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float3x3)world));

	//Set TBN normalized vectors in matrix
	output.tbn[0] = normalize(mul(input.tangent, world));
	output.tbn[1] = normalize(mul(input.binormal, world));
	output.tbn[2] = normalize(mul(input.normal, world));

	return output;
}