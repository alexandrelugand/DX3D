Texture2D Color: register(t0);
sampler ColorSampler: register(s0);

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
	float4 light_position;
	float light_radius;
	float time;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 color = Color.Sample(ColorSampler, float2(input.texcoord.x, 1.0 - input.texcoord.y));
	
	//PHONG REFLECTION
	//Ambient light
	float ka = 8.5;
	float3 ia = float3(0.09, 0.082, 0.082);
	ia *= color.rgb;
	float3 ambient_light = ka * ia;

	//Diffuse light
	float kd = 0.7;
	float amount_diffuse_light = dot(light_direction.xyz, input.normal);
	float3 id = float3(1, 1, 1);
	id *= color.rgb;
	float3 diffuse_light = kd * id * amount_diffuse_light;

	//Specular light
	float ks = 0.0;
	float3 is = float3(1.0, 1.0, 1.0); //Specular light color = white
	float3 reflected_light = reflect(light_direction.xyz, input.normal);
	float shininess = 30;
	float amount_specular_light = pow(max(0.0, dot(reflected_light, input.direction_to_camera)), shininess);
	float3 specular_light = ks * amount_specular_light * is;

	//Final light
	float3 final_light = ambient_light + diffuse_light + specular_light;

	return float4(final_light, 1.0);
}