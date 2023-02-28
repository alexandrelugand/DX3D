#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Scene/ISceneLoaderVisitor.h>

namespace DX3D
{
	class SceneLoaderVisitor : ISceneLoaderVisitor
	{
	public:
		SceneLoaderVisitor(World* world, ResourceManager* resource_manager);

		void Visit(RootLoader* loader) override;
		void Visit(EntityLoader* loader) override;
		void Visit(MeshComponentLoader* loader) override;
		void Visit(LightComponentLoader* loader) override;
		void Visit(TerrainComponentLoader* loader) override;
		void Visit(WaterComponentLoader* loader) override;
		void Visit(FogComponentLoader* loader) override;
		void Visit(MaterialLoader* loader) override;
		void Visit(TransformComponentLoader* loader) override;

	private:
		World* m_world = nullptr;
		ResourceManager* m_resource_manager = nullptr;
	};
}
