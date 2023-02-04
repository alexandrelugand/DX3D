#ifndef DATA
#define DATA

struct VS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
	float3 tangent: TANGENT0;
	float3 binormal: BINORMAL0;
};

struct VPS_INOUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
	float3 world_pos: TEXCOORD1;
	float3 camera_direction: TEXCOORD2;
	row_major float4x4 tbn: TEXCOORD3;
};

struct CameraData
{
	row_major float4x4 view;
	row_major float4x4 proj;
	float4 position;
};

struct LightData
{
	float4 color;
	float3 direction;
	float3 position;
	float radius;
};

struct TerrainData
{
	float4 size;
	float heightMapSize;
};

struct WaterData
{
	float4 size;
	float heightMapSize;
};

struct FogData
{
	float4 color;
	float start;
	float end;
	float enable;
};

cbuffer Constant : register(b0)
{
	row_major float4x4 world;
	float time;
	CameraData camera;
	LightData light;
	TerrainData terrain;
	WaterData water;
	FogData fog;
};

float3 ComputePhongLighting(
	CameraData camera,
	LightData light,
	float3 worldPosition,
	float3 normal,
	float ka,
	float3 ia,
	float kd,
	float3 id,
	float ks,
	float3 is,
	float shininess
)
{
	float3 light_direction = -normalize(light.direction.xyz);
	float3 direction_to_camera = normalize(worldPosition - camera.position.xyz);

	//PHONG REFLECTION
	//Ambient light
	float3 ambient_light = ka * ia;

	//Diffuse light
	float amount_diffuse_light = max(dot(light_direction.xyz, normal), 0.0f);
	float3 diffuse_light = kd * (light.color.rgb * id) * amount_diffuse_light;

	//Specular light
	float3 reflected_light = reflect(light_direction.xyz, normal);
	float amount_specular_light = pow(max(0.0, dot(reflected_light, direction_to_camera)), shininess);
	float3 specular_light = ks * amount_specular_light * is;

	//Final light
	return  ambient_light + diffuse_light + specular_light;
}

float3 ComputeNormalFromHeightMap(
	Texture2D heightMap,
	sampler heightMapSampler,
	float heightMapSize,
	float2 texcoord,
	float normalFactor
)
{
	//Compute height map size in range [0 ... 1]
	float texelSize = 1.0 / heightMapSize;
	float t = heightMap.SampleLevel(heightMapSampler, float2(texcoord.x, texcoord.y - texelSize), 0).r;
	float b = heightMap.SampleLevel(heightMapSampler, float2(texcoord.x, texcoord.y + texelSize), 0).r;
	float l = heightMap.SampleLevel(heightMapSampler, float2(texcoord.x - texelSize, texcoord.y), 0).r;
	float r = heightMap.SampleLevel(heightMapSampler, float2(texcoord.x + texelSize, texcoord.y), 0).r;

	t *= normalFactor;
	b *= normalFactor;
	l *= normalFactor;
	r *= normalFactor;

	float3 normal = float3(-(r - l) * 0.5, 1, -(b - t) * 0.5);
	return normalize(normal);
}

float ComputeFog(
	float distance,
	float start,
	float end
)
{
	float res = (end - distance) / (end - start);
	return clamp(res, 0, 1); //Clamp value between [0...1]
}

float3 ComputeFogColor(
	FogData fog,
	CameraData camera,
	float3 worldPosition,
	float3 sceneColor
)
{
	if (fog.enable == false)
		return sceneColor.rgb;

	float3 dir = worldPosition - camera.position.xyz;
	float dist = length(dir);
	float fogAmount = ComputeFog(dist, fog.start, fog.end);

	return float3(lerp(fog.color.rgb, sceneColor.rgb, fogAmount));
}

#endif //DATA
