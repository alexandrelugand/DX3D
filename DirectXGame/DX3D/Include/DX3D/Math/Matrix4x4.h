#pragma once
#include <cmath>
#include <DX3D/Math/Vector3D.h>
#include <DX3D/Math/Vector4D.h>

class Matrix4x4
{
public:
	Matrix4x4()
	{
		//Identity matrix by default
		m_matrix[0][0] = 1.0f;
		m_matrix[1][1] = 1.0f;
		m_matrix[2][2] = 1.0f;
		m_matrix[3][3] = 1.0f;
	}

	void SetIdentity()
	{
		memset(m_matrix, 0, sizeof(float) * 16);
		m_matrix[0][0] = 1.0f;
		m_matrix[1][1] = 1.0f;
		m_matrix[2][2] = 1.0f;
		m_matrix[3][3] = 1.0f;
	}

	void SetTranslation(const Vector3D& translation)
	{
		m_matrix[3][0] = translation.x;
		m_matrix[3][1] = translation.y;
		m_matrix[3][2] = translation.z;
	}

	void SetScale(const Vector3D& scale)
	{
		m_matrix[0][0] = scale.x;
		m_matrix[1][1] = scale.y;
		m_matrix[2][2] = scale.z;
	}

	void SetRotationX(const float teta)
	{
		m_matrix[1][1] = std::cos(teta);
		m_matrix[1][2] = std::sin(teta);
		m_matrix[2][1] = -std::sin(teta);
		m_matrix[2][2] = std::cos(teta);
	}

	void SetRotationY(const float teta)
	{
		m_matrix[0][0] = std::cos(teta);
		m_matrix[0][2] = -std::sin(teta);
		m_matrix[2][0] = std::sin(teta);
		m_matrix[2][2] = std::cos(teta);
	}

	void SetRotationZ(const float teta)
	{
		m_matrix[0][0] = std::cos(teta);
		m_matrix[0][1] = std::sin(teta);
		m_matrix[1][0] = -std::sin(teta);
		m_matrix[1][1] = std::cos(teta);
	}

	void SetPerspectiveFovLH(float fov, float aspect, float znear, float zfar)
	{
		float yscale = 1.0f / std::tan(fov / 2.0f);
		float xscale = yscale / aspect;

		m_matrix[0][0] = xscale;
		m_matrix[1][1] = yscale;
		m_matrix[2][2] = zfar / (zfar - znear);
		m_matrix[2][3] = 1.0f;
		m_matrix[3][2] = (-znear * zfar) / (zfar - znear);
		m_matrix[3][3] = 0.0f;
	}

	void SetOrthoLH(float width, float height, float near_plane, float far_plane)
	{
		SetIdentity();
		m_matrix[0][0] = 2.0f / width;
		m_matrix[1][1] = 2.0f / height;
		m_matrix[2][2] = 1.0f / (far_plane - near_plane);
		m_matrix[3][2] = -(near_plane / (far_plane - near_plane));
	}

	float GetDeterminant() const
	{
		Vector4D minor;

		auto v1 = Vector4D(this->m_matrix[0][0], this->m_matrix[1][0], this->m_matrix[2][0], this->m_matrix[3][0]);
		auto v2 = Vector4D(this->m_matrix[0][1], this->m_matrix[1][1], this->m_matrix[2][1], this->m_matrix[3][1]);
		auto v3 = Vector4D(this->m_matrix[0][2], this->m_matrix[1][2], this->m_matrix[2][2], this->m_matrix[3][2]);


		minor.Cross(v1, v2, v3);
		float det = -(this->m_matrix[0][3] * minor.x + this->m_matrix[1][3] * minor.y + this->m_matrix[2][3] * minor.z + this->m_matrix[3][3] * minor.w);
		return det;
	}

	void Inverse()
	{
		Matrix4x4 out;
		Vector4D v;

		float det = this->GetDeterminant();
		if (det == 0.0f)
			return;

		for (unsigned int i = 0; i < 4; i++)
		{
			Vector4D vec[3];
			for (unsigned int j = 0; j < 4; j++)
			{
				if (j != i)
				{
					unsigned int a = j;
					if (j > i)
						a = a - 1;
					vec[a].x = (this->m_matrix[j][0]);
					vec[a].y = (this->m_matrix[j][1]);
					vec[a].z = (this->m_matrix[j][2]);
					vec[a].w = (this->m_matrix[j][3]);
				}
			}
			v.Cross(vec[0], vec[1], vec[2]);

			out.m_matrix[0][i] = static_cast<float>(pow(-1.0f, i)) * v.x / det;
			out.m_matrix[1][i] = static_cast<float>(pow(-1.0f, i)) * v.y / det;
			out.m_matrix[2][i] = static_cast<float>(pow(-1.0f, i)) * v.z / det;
			out.m_matrix[3][i] = static_cast<float>(pow(-1.0f, i)) * v.w / det;
		}

		this->SetMatrix(out);
	}

	Vector3D GetXDirection() const
	{
		return Vector3D(m_matrix[0][0], m_matrix[0][1], m_matrix[0][2]);
	}

	Vector3D GetYDirection() const
	{
		return Vector3D(m_matrix[1][0], m_matrix[1][1], m_matrix[1][2]);
	}

	Vector3D GetZDirection() const
	{
		return Vector3D(m_matrix[2][0], m_matrix[2][1], m_matrix[2][2]);
	}

	Vector3D GetTranslation() const
	{
		return Vector3D(m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]);
	}

	void operator *=(const Matrix4x4& m)
	{
		Matrix4x4 out;
		for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
				out.m_matrix[i][j] =
					m_matrix[i][0] * m.m_matrix[0][j] +
					m_matrix[i][1] * m.m_matrix[1][j] +
					m_matrix[i][2] * m.m_matrix[2][j] +
					m_matrix[i][3] * m.m_matrix[3][j];
			}
		}

		this->SetMatrix(out);
	}

	void SetMatrix(const Matrix4x4& matrix)
	{
		memcpy(m_matrix, matrix.m_matrix, sizeof(float) * 16);
	}

private:
	float m_matrix[4][4] = {};
};
