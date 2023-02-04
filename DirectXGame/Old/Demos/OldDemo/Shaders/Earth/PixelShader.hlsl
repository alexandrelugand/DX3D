Texture2D Color: register(t0);
sampler ColorSampler: register(s0);

Texture2D EarthSpecular: register(t1);
sampler EarthSpecularSampler: register(s1);

Texture2D Clouds: register(t2);
sampler CloudsSampler: register(s2);

Texture2D EarthNight: register(t3);
sampler EarthNightSampler: register(s3);

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
	float4 earth_color = Color.Sample(ColorSampler, 1.0 - input.texcoord);
	float4 earth_specular = EarthSpecular.Sample(EarthSpecularSampler, 1.0 - input.texcoord).r; //Only need 1st value
	float4 clouds = Clouds.Sample(CloudsSampler, 1.0 - input.texcoord + float2(time/1000.0, 0)).r;
	float4 earth_night = EarthNight.Sample(EarthNightSampler, 1.0 - input.texcoord);

	//Phong reflection model
	//Ambient light
	float ka = 1.5;
	float3 ia = float3(0.09, 0.082, 0.082); //ambient light color = white
	ia *= (earth_color.rgb + clouds);
	float3 ambient_light = ka * ia;

	//Diffuse light
	float kd = 0.7;
	float3 id_day = float3(1.0, 1.0, 1.0); //Diffuse light color = white
	id_day *= (earth_color.rgb + clouds);

	float3 id_night = float3(1.0, 1.0, 1.0); //Diffuse light color = white
	id_night *= (earth_night.rgb + clouds);

	float amount_diffuse_light = dot(light_direction.xyz, input.normal);

	//Lerp function accept variation value between [0...1], then make a decal
	float3 id = lerp(id_night, id_day, (amount_diffuse_light + 1.0) / 2);

	float3 diffuse_light = kd * id;

	//Specular light
	float ks = earth_specular;
	float3 is = float3(1.0, 1.0, 1.0); //Specular light color = white
	float3 reflected_light = reflect(light_direction.xyz, input.normal);
	float shininess = 30;
	float amount_specular_light = pow(max(0.0, dot(reflected_light, input.direction_to_camera)), shininess);
	float3 specular_light = ks * amount_specular_light * is;

	//Final light
	float3 final_light = ambient_light + diffuse_light + specular_light;

	return float4(final_light, 1.0);
}