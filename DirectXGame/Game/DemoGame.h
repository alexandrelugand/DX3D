#pragma once
#include <DX3D/Includes.h>
#include <DX3D/Game/Options.h>

class DemoGame : public DX3D::Game
{
public:
	DemoGame(DX3D::Options& options);
	~DemoGame() override {}

protected:
	void OnCreate() override;
	void OnUpdate(float delta_time) override;

private:
	DX3D::Options& m_options;
	float m_elapsed_seconds = 0.0f;
	bool m_locked = true;
	bool m_fullscreen = false;

	DX3D::Player* m_player = nullptr;
	DX3D::Entity* m_infos = nullptr;
	DX3D::Entity* m_version_info = nullptr;
};
