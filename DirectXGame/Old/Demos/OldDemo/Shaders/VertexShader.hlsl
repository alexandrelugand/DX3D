struct VS_INPUT
{
	float4 position: POSITION0;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
};

struct VS_OUTPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
	float3 direction_to_camera: TEXCOORD1;
	float3 world_pos: TEXCOORD2;
};

cbuffer Constant : register(b0)
{
	row_major float4x4 world;
	row_major float4x4 view;
	row_major float4x4 proj;
	float4 light_direction;
	float4 camera_position;
	float4 light_position;
	float light_radius;
	float time;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//output.position = lerp(input.position, input.position1, (sin(time / 1000.0f) + 1.0f) / 2.0f);

	//Vertex in World space coordinate
	output.position = mul(input.position, world);
	output.direction_to_camera = normalize(output.position.xyz - camera_position.xyz);
	output.world_pos = output.position.xyz;

	//Vertex in View space coordinate
	output.position = mul(output.position, view);

	//Vertex in Project space coordinate
	output.position = mul(output.position, proj);

	output.texcoord = input.texcoord;
	output.normal = input.normal;

	return output;
}