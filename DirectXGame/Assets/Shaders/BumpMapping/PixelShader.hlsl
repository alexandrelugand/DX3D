#include "Common.hlsl"

Texture2D Color: register(t0);
sampler ColorSampler: register(s0);

Texture2D Normal: register(t1);
sampler NormalSampler: register(s1);

float4 main(VPS_INOUT input) : SV_TARGET
{
	float4 color = Color.Sample(ColorSampler, float2(input.texcoord.x, 1.0 - input.texcoord.y));
	float4 normal = Normal.Sample(NormalSampler, float2(input.texcoord.x, 1.0 - input.texcoord.y));

	normal.xyz = (normal.xyz * 2.0) - 1.0; //Make offset to place in [0-1] range
	normal.xyz = mul(normal.xyz, input.tbn);

	//Dot product between light direction and vertice normal, if greater 0 we are in the light side, else dark side
	float dot_nl = dot(light.direction.xyz, input.tbn[2]); 
	
	//PHONG REFLECTION
	//Ambient light
	float ka = 8.5;
	float3 ia = float3(0.09, 0.09, 0.09);
	ia *= color.rgb;
	float3 ambient_light = ka * ia;

	//Diffuse light
	float kd = 0.7;
	float amount_diffuse_light = dot(-light.direction.xyz, normal.xyz);
	float3 id = float3(1, 1, 1);
	id *= color.rgb;
	float3 diffuse_light = kd * id * amount_diffuse_light;

	//Specular light
	float ks = 0.55;
	float3 is = float3(1.0, 1.0, 1.0); //Specular light color = white
	float3 reflected_light = reflect(-light.direction.xyz, normal.xyz);
	float shininess = 10;
	float amount_specular_light = 0;
	if(dot_nl > 0)
		amount_specular_light = pow(max(0.0, dot(reflected_light, input.camera_direction)), shininess);
	float3 specular_light = ks * amount_specular_light * is;

	//Final light
	float3 final_light = ambient_light + diffuse_light + specular_light;

	return float4(final_light, 1.0);
}