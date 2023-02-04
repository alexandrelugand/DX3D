Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
	float3 direction_to_camera: TEXCOORD1;
};

cbuffer Constant : register(b0)
{
	row_major float4x4 world;
	row_major float4x4 view;
	row_major float4x4 proj;
	float4 light_direction;
	float4 camera_position;
	//UInt32 time;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	//Linear interpolation example
	//return float4(lerp(input.color, input.color1, (sin(time / 1000.0f) + 1.0f) / 2.0f), 1.0f);

	//Texture mapping
	//return Texture.Sample(TextureSampler, input.texcoord*0.5);

	//Phong reflection model
	//Ambient light
	float ka = 0.1;
	float3 ia = float3(1.0, 1.0, 1.0); //ambient light color = white

	float3 ambient_light = ka * ia;

	//Diffuse light
	float kd = 0.7;
	float3 id = float3(1.0, 1.0, 1.0); //Diffuse light color = white
	float amount_diffuse_light = max(0.0, dot(light_direction.xyz, input.normal));
	float3 diffuse_light = kd * amount_diffuse_light * id;

	//Specular light
	float ks = 1.0;
	float3 is = float3(1.0, 1.0, 1.0); //Specular light color = white
	float3 reflected_light = reflect(light_direction.xyz, input.normal);
	float shininess = 30;
	float amount_specular_light = pow(max(0.0, dot(reflected_light, input.direction_to_camera)), shininess);
	float3 specular_light = ks * amount_specular_light * is;

	//Final light
	float3 final_light = ambient_light + diffuse_light + specular_light;

	return float4(final_light, 1.0);
}