#include <DX3D/Entity/TransformComponent.h>

namespace DX3D
{
	TransformComponent::TransformComponent() {}

	TransformComponent::~TransformComponent() {}

	void TransformComponent::SetPosition(const Vector3D& position)
	{
		m_position = position;
		UpdateWorldMatrix();
	}

	void TransformComponent::SetRotation(const Vector3D& rotation)
	{
		m_rotation = rotation;
		UpdateWorldMatrix();
	}

	void TransformComponent::SetScale(const Vector3D& scale)
	{
		m_scale = scale;
		UpdateWorldMatrix();
	}

	void TransformComponent::GetWorldMatrix(Matrix4x4& mat)
	{
		UpdateWorldMatrix();
		mat.SetMatrix(m_world_matrix);
	}

	void TransformComponent::UpdateWorldMatrix()
	{
		Matrix4x4 temp;

		m_world_matrix.SetIdentity();

		temp.SetIdentity();
		temp.SetScale(m_scale);
		m_world_matrix *= temp;

		temp.SetIdentity();
		temp.SetRotationX(m_rotation.x);
		m_world_matrix *= temp;

		temp.SetIdentity();
		temp.SetRotationY(m_rotation.y);
		m_world_matrix *= temp;

		temp.SetIdentity();
		temp.SetRotationZ(m_rotation.z);
		m_world_matrix *= temp;

		temp.SetIdentity();
		temp.SetTranslation(m_position);
		m_world_matrix *= temp;
	}
}
