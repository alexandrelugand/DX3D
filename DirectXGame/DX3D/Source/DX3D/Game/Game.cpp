#include <DX3D/Version.h>
#include <DX3D/Game/Display.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Game/World.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Input/InputSystem.h>
#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Scene/SceneManager.h>

namespace DX3D
{
	Game::Game()
	{
		m_input_system = std::make_unique<InputSystem>();
		m_graphics_engine = std::make_unique<GraphicsEngine>(this);
		m_display = std::make_unique<Display>(this);
		m_resource_manager = std::make_unique<ResourceManager>(this);
		m_world = std::make_unique<World>(this);
		m_scene_manager = std::make_unique<SceneManager>(m_world.get(), m_resource_manager.get());

		m_input_system->SetLockArea(m_display->GetClientSize());
		m_version = L"Version " + StringToUnicode(DX3D_VERSION);
	}

	Game::~Game() {}

	void Game::Quit()
	{
		m_isRunning = false;
	}

	void Game::OnCreate()
	{
		m_graphics_engine->Initialize();
	}

	void Game::OnUpdate(float delta_time) { }

	void Game::OnQuit() { }

	void Game::OnDisplaySize(const Rect& size)
	{
		m_input_system->SetLockArea(m_display->GetClientSize());
		OnInternalUpdate();
	}

	void Game::OnInternalUpdate()
	{
		//computing delta time
		const auto current_time = std::chrono::system_clock::now();
		auto elapsed_seconds = std::chrono::duration<double>();
		if (m_previous_time.time_since_epoch().count())
			elapsed_seconds = current_time - m_previous_time;
		m_previous_time = current_time;

		const auto delta_time = static_cast<float>(elapsed_seconds.count());
		m_total_time += delta_time;

		m_input_system->Update();

		OnUpdate(delta_time);
		m_world->Update(delta_time);

		m_graphics_engine->Update();
	}
}
