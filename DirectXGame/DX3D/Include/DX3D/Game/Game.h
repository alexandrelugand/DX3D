#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class Game
	{
	public:
		Game();
		virtual ~Game();

		void Run();
		void Quit();

		GraphicsEngine* GetGraphicsEngine() const { return m_graphics_engine.get(); }
		World* GetWorld() const { return m_world.get(); }
		ResourceManager* GetResourceManager() const { return m_resource_manager.get(); }
		InputSystem* GetInputSystem() const { return m_input_system.get(); }
		Display* GetDisplay() const { return m_display.get(); }
		SceneManager* GetSceneManager() const { return m_scene_manager.get(); }

		float GetTotalTime() const { return m_total_time; }

	protected:
		virtual void OnCreate();
		virtual void OnUpdate(float delta_time);
		virtual void OnQuit();

		virtual void OnDisplaySize(const Rect& size);

		std::wstring m_version;

	private:
		std::unique_ptr<GraphicsEngine> m_graphics_engine;
		std::unique_ptr<Display> m_display;
		std::unique_ptr<ResourceManager> m_resource_manager;
		std::unique_ptr<InputSystem> m_input_system;
		std::unique_ptr<World> m_world;
		std::unique_ptr<SceneManager> m_scene_manager;

		bool m_isRunning = true;
		float m_total_time = 0.0f;
		std::chrono::system_clock::time_point m_previous_time;

		void OnInternalUpdate();

		friend class Display;
	};
}
