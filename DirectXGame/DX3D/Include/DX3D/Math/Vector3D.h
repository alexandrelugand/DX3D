#pragma once

class Vector3D
{
public:
	Vector3D()
		: x(0), y(0), z(0) { }

	Vector3D(float x, float y, float z)
		: x(x), y(y), z(z) { }

	Vector3D(const Vector3D& v)
		: x(v.x), y(v.y), z(v.z) { }

	static Vector3D Lerp(const Vector3D& start, const Vector3D& end, float delta)
	{
		Vector3D v;
		v.x = start.x * (1.0f - delta) + end.x * delta;
		v.y = start.y * (1.0f - delta) + end.y * delta;
		v.z = start.z * (1.0f - delta) + end.z * delta;

		return v;
	}

	Vector3D operator *(float num) const
	{
		return Vector3D(x * num, y * num, z * num);
	}

	Vector3D operator +(const Vector3D& vec) const
	{
		return Vector3D(x + vec.x, y + vec.y, z + vec.z);
	}

	Vector3D operator -(const Vector3D& vec) const
	{
		return Vector3D(x - vec.x, y - vec.y, z - vec.z);
	}

	Vector3D& operator +=(const Vector3D& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	static Vector3D Normalize(const Vector3D& vec)
	{
		Vector3D res;
		float len = sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
		if (len == 0.0f)
			return Vector3D();

		res.x = vec.x / len;
		res.y = vec.y / len;
		res.z = vec.z / len;

		return res;
	}

	static Vector3D Cross(const Vector3D& v1, const Vector3D& v2)
	{
		Vector3D res;
		res.x = (v1.y * v2.z) - (v1.z * v2.y);
		res.y = (v1.z * v2.x) - (v1.x * v2.z);
		res.z = (v1.x * v2.y) - (v1.y * v2.x);
		return res;
	}

	float x, y, z;
};
