#include <DX3D/Entity/Player.h>

namespace DX3D
{
	void Player::OnUpdate(float delta_time)
	{
		Entity::OnUpdate(delta_time);

		const auto transform = GetTransform();
		const auto input_system = GetInputSystem();

		auto speed = 12.0f;
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

		if (input_system->IsKeyDown(Key::Shift))
		{
			speed *= 4;
		}

		const auto delta_pos = input_system->GetDeltaMousePosition();
		auto rot = transform->GetRotation();
		rot += Vector3D(delta_pos.y * 0.001f, delta_pos.x * 0.001f, 0);
		transform->SetRotation(rot);

		Matrix4x4 world;
		transform->GetWorldMatrix(world);

		auto pos = transform->GetPosition();
		pos = pos + world.GetZDirection() * m_forward * delta_time * speed;
		pos = pos + world.GetXDirection() * m_rightward * delta_time * speed;
		pos = pos + world.GetYDirection() * m_upward * delta_time * speed;
		transform->SetPosition(pos);
	}
}
