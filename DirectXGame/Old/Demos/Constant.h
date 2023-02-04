#pragma once
#include <GameEngine/Math/Matrix4x4.h>

//DirectX align constant with 16 bits
_declspec(align(16))
struct Constant
{
	Matrix4x4 world;
	Matrix4x4 view;
	Matrix4x4 projection;
	Vector4D light_direction;
	Vector4D camera_position;
	Vector4D light_position = Vector4D(0, 1, 0, 0);
	float light_radius = 4.0f;
	float time = 0.0f;
};

_declspec(align(16))
struct DistortionEffectData
{
	float distortion_level = 0.5f;
};
