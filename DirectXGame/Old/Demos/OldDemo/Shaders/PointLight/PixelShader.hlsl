Texture2D Color: register(t0);
sampler ColorSampler: register(s0);

struct PS_INPUT
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

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 text_color = Color.Sample(ColorSampler, float2(input.texcoord.x, 1.0 - input.texcoord.y));

	//Phong reflection model
	//Ambient light
	float ka = 1.5;
	float3 ia = float3(0.09, 0.082, 0.082); //ambient light color = white
	ia *= text_color.rgb;
	float3 ambient_light = ka * ia;

	//Diffuse light
	float kd = 0.7;
	float3 light_dir = normalize(light_position.xyz - input.world_pos.xyz);
	float distance_light_object = length(light_position.xyz - input.world_pos.xyz);

	float fade_area = max(0, distance_light_object - light_radius);

	//Linear quadratic attenuation
	float constant_func = 1.0;
	float linear_func = 2.0;
	float quadratic_func = 2.0;

	float attenuation = constant_func + linear_func * fade_area + quadratic_func * fade_area * fade_area;


	float amount_diffuse_light =max(0, dot(light_dir.xyz, input.normal));
	float3 id = float3(1.0, 1.0, 1.0); //Diffuse light color = white
	id *= text_color.rgb;
	float3 diffuse_light = (kd * id * amount_diffuse_light) / attenuation; //include attenuation in diffuse color calculation

	//Specular light
	float ks = 0.0;
	float3 direction_to_camera = normalize(input.world_pos.xyz - camera_position.xyz);
	float3 is = float3(1.0, 1.0, 1.0); //Specular light color = white
	float3 reflected_light = reflect(light_dir.xyz, input.normal);
	float shininess = 30;
	float amount_specular_light = pow(max(0.0, dot(reflected_light, direction_to_camera)), shininess);
	float3 specular_light = ks * amount_specular_light * is;

	//Final light
	float3 final_light = ambient_light + diffuse_light + specular_light;

	return float4(final_light, 1.0);
}