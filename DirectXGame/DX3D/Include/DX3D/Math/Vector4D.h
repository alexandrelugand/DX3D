#pragma once
#include <DX3D/Math/Vector3D.h>

class Vector4D
{
public:
	Vector4D()
		: x(0), y(0), z(0), w(0) { }

	Vector4D(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w) { }

	Vector4D(const Vector4D& v)
		: x(v.x), y(v.y), z(v.z), w(v.w) { }

	Vector4D(const Vector3D& v)
		: x(v.x), y(v.y), z(v.z), w(1.0f) { }

	void Cross(Vector4D& v1, Vector4D& v2, Vector4D& v3)
	{
		x = v1.y * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.y * v3.w - v3.y * v2.w) + v1.w * (v2.y * v3.z - v2.z * v3.y);
		y = -(v1.x * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.z - v3.x * v2.z));
		z = v1.x * (v2.y * v3.w - v3.y * v2.w) - v1.y * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.y - v3.x * v2.y);
		w = -(v1.x * (v2.y * v3.z - v3.y * v2.z) - v1.y * (v2.x * v3.z - v3.x * v2.z) + v1.z * (v2.x * v3.y - v3.x * v2.y));
	}

	float x, y, z, w;
};
