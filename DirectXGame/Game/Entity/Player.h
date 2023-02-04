#pragma once
#include <DX3D/Includes.h>

class Player : public DX3D::Entity
{
public:
	Player() { }
	~Player() override {}

protected:
	void OnCreate() override;
	void OnUpdate(float delta_time) override;

private:
	float m_forward = 0.0f;
	float m_rightward = 0.0f;
	float m_upward = 0.0f;
};
