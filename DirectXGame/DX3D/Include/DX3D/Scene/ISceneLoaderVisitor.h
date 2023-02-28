#pragma once

namespace DX3D
{
	class ISceneLoaderVisitor
	{
	public:
		virtual ~ISceneLoaderVisitor() = default;
		virtual void Visit(RootLoader* loader) = 0;
		virtual void Visit(EntityLoader* loader) = 0;
		virtual void Visit(MeshComponentLoader* loader) = 0;
		virtual void Visit(LightComponentLoader* loader) = 0;
		virtual void Visit(TerrainComponentLoader* loader) = 0;
		virtual void Visit(WaterComponentLoader* loader) = 0;
		virtual void Visit(FogComponentLoader* loader) = 0;
		virtual void Visit(MaterialLoader* loader) = 0;
		virtual void Visit(TransformComponentLoader* loader) = 0;
	};
}
