#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	__declspec(align(16))
	struct CameraData
	{
		Matrix4x4 view;
		Matrix4x4 proj;
		Vector4D position;
	};

	__declspec(align(16))
	struct LightData
	{
		Vector4D color;
		Vector3D direction;
		Vector3D position;
		float radius;
	};

	__declspec(align(16))
	struct TerrainData
	{
		Vector4D size;
		float heightMapSize = 0.0f;
	};

	__declspec(align(16))
	struct WaterData
	{
		Vector4D size;
		float heightMapSize = 0.0f;
	};

	__declspec(align(16))
	struct FogData
	{
		Vector4D color;
		float start = 50.0f;
		float end = 300.0f;
		float enable = 0;
	};

	__declspec(align(16))
	struct ConstantData
	{
		Matrix4x4 world;
		float time = 0.0f;
		CameraData camera;
		LightData light;
		TerrainData terrain;
		WaterData water;
		FogData fog;
	};
}
