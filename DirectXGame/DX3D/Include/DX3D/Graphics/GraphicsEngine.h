#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class GraphicsEngine
	{
	public:
		GraphicsEngine(Game* game);
		~GraphicsEngine();

		void Update();
		RenderSystem* GetRenderSystem() const { return m_render.get(); }

		void AddComponent(Component* component);
		void RemoveComponent(Component* component);

	private:
		std::unique_ptr<RenderSystem> m_render = nullptr;
		Game* m_game = nullptr;

		std::set<MeshComponent*> m_meshes;
		std::set<CameraComponent*> m_cameras;
		std::set<LightComponent*> m_lights;
		std::set<TerrainComponent*> m_terrains;
		std::set<WaterComponent*> m_waters;
		std::set<FogComponent*> m_fogs;
	};
}
