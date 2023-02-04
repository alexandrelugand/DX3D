#pragma once
#include <DX3D/Includes.h>

class Player;

class DemoGame : public DX3D::Game
{
public:
	DemoGame() {}
	~DemoGame() override {}

protected:
	void OnCreate() override;
	void OnUpdate(float delta_time) override;

private:
	Player* m_player = nullptr;

	float m_elapsed_seconds = 0.0f;
	bool m_locked = true;
	bool m_fullscreen = false;

	DX3D::Entity* m_light = nullptr;
};
