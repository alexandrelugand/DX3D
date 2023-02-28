#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Entity/Component.h>

namespace DX3D
{
	class TerrainComponent final : public Component
	{
	public:
		TerrainComponent();
		~TerrainComponent() override;

		void SetHeightMap(const TexturePtr& height_map) { m_height_map = height_map; }
		const TexturePtr& GetHeightMap() { return m_height_map; }

		void SetGroundMap(const TexturePtr& ground_map) { m_ground_map = ground_map; }
		const TexturePtr& GetGroundMap() { return m_ground_map; }

		void SetWallMap(const TexturePtr& wall_map) { m_wall_map = wall_map; }
		const TexturePtr& GetWallMap() { return m_wall_map; }

		void SetSize(const Vector3D& size) { m_size = size; }
		const Vector3D& GetSize() { return m_size; }

	protected:
		void OnCreateInternal() override;

	private:
		TexturePtr m_height_map;
		TexturePtr m_ground_map;
		TexturePtr m_wall_map;

		Vector3D m_size = {512, 100, 512};

		VertexBufferPtr m_mesh_vb;
		IndexBufferPtr m_mesh_ib;
		ConstantBufferPtr m_cb;
		VertexShaderPtr m_vs;
		PixelShaderPtr m_ps;

		VertexMesh* m_terrain_mesh_vertices;
		uint* m_terrain_mesh_indices;

		void GenerateMesh();
		void UpdateData(void* data, uint size);

		friend class GraphicsEngine;
	};
}
