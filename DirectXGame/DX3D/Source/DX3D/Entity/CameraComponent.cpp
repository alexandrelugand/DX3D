#include <DX3D/Entity/CameraComponent.h>
#include <DX3D/Entity/Entity.h>
#include <DX3D/Entity/TransformComponent.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Game/World.h>
#include <DX3D/Graphics/GraphicsEngine.h>

namespace DX3D
{
	CameraComponent::CameraComponent() {}

	CameraComponent::~CameraComponent()
	{
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->RemoveComponent(this);
	}

	void CameraComponent::GetViewMatrix(Matrix4x4& view) const
	{
		m_entity->GetTransform()->GetWorldMatrix(view);
		view.Inverse();
	}

	void CameraComponent::SetNearPlane(float near_plane)
	{
		m_near_plane = near_plane;
		ComputeProjectionMatrix();
	}

	void CameraComponent::SetFarPlane(float far_plane)
	{
		m_far_plane = far_plane;
		ComputeProjectionMatrix();
	}

	void CameraComponent::SetFov(float fov)
	{
		m_fov = fov;
		ComputeProjectionMatrix();
	}

	void CameraComponent::SetType(CameraType type)
	{
		m_type = type;
		ComputeProjectionMatrix();
	}

	void CameraComponent::SetScreenArea(const Rect& screen_area)
	{
		m_screen_area = screen_area;
		ComputeProjectionMatrix();
	}

	void CameraComponent::OnCreateInternal()
	{
		Component::OnCreateInternal();
		m_entity->GetWorld()->GetGame()->GetGraphicsEngine()->AddComponent(this);
	}

	void CameraComponent::ComputeProjectionMatrix()
	{
		if (m_screen_area.Height() == 0)
			return;

		if (m_type == CameraType::Perspective)
			m_projection.SetPerspectiveFovLH(m_fov, static_cast<float>(m_screen_area.Width()) / static_cast<float>(m_screen_area.Height()), m_near_plane, m_far_plane);
		else if (m_type == CameraType::Orthogonal)
			m_projection.SetOrthoLH(static_cast<float>(m_screen_area.Width()), static_cast<float>(m_screen_area.Height()), m_near_plane, m_far_plane);
	}
}
