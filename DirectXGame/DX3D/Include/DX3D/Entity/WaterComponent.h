#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Entity/Component.h>

namespace DX3D
{
	class WaterComponent final : public Component
	{
	public:
		WaterComponent();
		~WaterComponent() override;

		void SetWaveHeightMap(const TexturePtr& wave_height_map) { m_wave_height_map = wave_height_map; }
		const TexturePtr& GetWaveHeightMap() { return m_wave_height_map; }

		void SetSize(const Vector3D& size) { m_size = size; }
		const Vector3D& GetSize() { return m_size; }

	protected:
		void OnCreateInternal() override;

	private:
		TexturePtr m_wave_height_map;

		Vector3D m_size = {4096, 8, 4096};

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
