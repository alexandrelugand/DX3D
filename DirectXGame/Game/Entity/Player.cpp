#include "Player.h"

using namespace DX3D;

void Player::OnCreate()
{
	Entity::OnCreate();
	const auto camera_component = CreateComponent<CameraComponent>();
	camera_component->SetFarPlane(7000.0f);

	GetTransform()->SetPosition(Vector3D(135.0f, 26.0f, 154.0f));
	GetTransform()->SetRotation(Vector3D(0, 1.57f, 0));
}

void Player::OnUpdate(float delta_time)
{
	Entity::OnUpdate(delta_time);

	const auto transform = GetTransform();
	const auto input_system = GetInputSystem();

	m_forward = 0.0f;
	m_rightward = 0.0f;
	m_upward = 0.0f;

	if (input_system->IsKeyDown(Key::Z))
	{
		m_forward = 1.0f;
	}

	if (input_system->IsKeyDown(Key::S))
	{
		m_forward = -1.0f;
	}

	if (input_system->IsKeyDown(Key::D))
	{
		m_rightward = 1.0f;
	}

	if (input_system->IsKeyDown(Key::Q))
	{
		m_rightward = -1.0f;
	}

	if (input_system->IsKeyDown(Key::Space))
	{
		m_upward = 1.0f;
	}

	if (input_system->IsKeyDown(Key::Ctrl))
	{
		m_upward = -1.0f;
	}

	const auto delta_pos = input_system->GetDeltaMousePosition();
	auto rot = transform->GetRotation();
	rot += Vector3D(delta_pos.y * 0.001f, delta_pos.x * 0.001f, 0);
	transform->SetRotation(rot);

	Matrix4x4 world;
	transform->GetWorldMatrix(world);

	auto pos = transform->GetPosition();
	pos = pos + world.GetZDirection() * m_forward * delta_time * 12.0f;
	pos = pos + world.GetXDirection() * m_rightward * delta_time * 12.0f;
	pos = pos + world.GetYDirection() * m_upward * delta_time * 12.0f;
	transform->SetPosition(pos);
}
