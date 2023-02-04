#pragma once
#include <DX3D/Entity/Component.h>

namespace DX3D
{
	class TransformComponent final : public Component
	{
	public:
		TransformComponent();
		~TransformComponent() override;

		void SetPosition(const Vector3D& position);
		Vector3D GetPosition() { return m_position; }

		void SetRotation(const Vector3D& rotation);
		Vector3D GetRotation() { return m_rotation; }

		void SetScale(const Vector3D& scale);
		Vector3D GetScale() { return m_scale; }

		void GetWorldMatrix(Matrix4x4& mat);

	protected:
		Vector3D m_position;
		Vector3D m_rotation;
		Vector3D m_scale = {1, 1, 1};
		Matrix4x4 m_world_matrix;

		void UpdateWorldMatrix();
	};
}
