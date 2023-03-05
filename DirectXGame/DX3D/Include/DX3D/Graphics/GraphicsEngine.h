#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class GraphicsEngine
	{
	public:
		GraphicsEngine(Game* game);
		~GraphicsEngine();

		void Initialize();
		void Update();
		RenderSystem* GetRenderSystem() const { return m_render.get(); }

		void AddComponent(Component* component);
		void RemoveComponent(Component* component);

		bool GetWireFrameMode() const { return m_wire_frame; }
		void SetWireFrameMode(const bool wire_frame) { m_wire_frame = wire_frame; }

	private:
		std::unique_ptr<RenderSystem> m_render = nullptr;
		Game* m_game = nullptr;
		bool m_wire_frame = false;

		std::set<MeshComponent*> m_meshes{};
		std::set<CameraComponent*> m_cameras{};
		std::set<LightComponent*> m_lights{};
		std::set<TerrainComponent*> m_terrains{};
		std::set<WaterComponent*> m_waters{};
		std::set<FogComponent*> m_fogs{};
		std::set<TextComponent*> m_texts{};

		bool show_demo_window = false;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	};
}
