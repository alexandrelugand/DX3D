#pragma once
#include <DX3D/Includes.h>

namespace DX3D
{
	class Player : public Entity
	{
	public:
		Player() { }
		~Player() override {}

	protected:
		void OnUpdate(float delta_time) override;

	private:
		float m_forward = 0.0f;
		float m_rightward = 0.0f;
		float m_upward = 0.0f;
	};
}
