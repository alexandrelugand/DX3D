#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Entity/Component.h>

namespace DX3D
{
	class CameraComponent final : public Component
	{
	public:
		CameraComponent();
		~CameraComponent() override;

		void GetProjectionMatrix(Matrix4x4& projection) const { projection.SetMatrix(m_projection); }
		void GetViewMatrix(Matrix4x4& view) const;

		float GetNearPlane() const { return m_near_plane; }
		void SetNearPlane(float near_plane);

		float GetFarPlane() const { return m_far_plane; }
		void SetFarPlane(float far_plane);

		float GetFov() const { return m_fov; }
		void SetFov(float fov);

		CameraType GetType() const { return m_type; }
		void SetType(CameraType type);

		Rect GetScreenArea() const { return m_screen_area; }
		void SetScreenArea(const Rect& screen_area);

	protected:
		void OnCreateInternal() override;

	private:
		Matrix4x4 m_projection;
		float m_near_plane = 0.01f;
		float m_far_plane = 100.0f;
		float m_fov = 1.33f;
		CameraType m_type = CameraType::Perspective;
		Rect m_screen_area;

		void ComputeProjectionMatrix();
	};
}
