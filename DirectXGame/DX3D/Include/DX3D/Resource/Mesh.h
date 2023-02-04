#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Resource/MaterialSlot.h>
#include <DX3D/Resource/Resource.h>

namespace DX3D
{
	class Mesh : public Resource
	{
	public:
		Mesh(ResourceManager* manager, const wchar_t* full_path);
		Mesh(ResourceManager* manager,
		     VertexMesh* vertex_list_data, unsigned vertex_list_size,
		     unsigned* index_list_data, unsigned index_list_size,
		     MaterialSlot* material_slot_list, unsigned material_slot_list_size
		);
		~Mesh() override;

		MaterialSlot GetMaterialSlot(size_t slot) const { return m_material_slots[slot]; }
		size_t GetNumMaterialSlots() const { return m_material_slots.size(); }

	private:
		VertexBufferPtr m_vertex_buffer = nullptr;
		IndexBufferPtr m_index_buffer = nullptr;

		std::vector<MaterialSlot> m_material_slots;

		static void ComputeTangents(
			const Vector3D& v0, const Vector3D& v1, const Vector3D& v2,
			const Vector2D& t0, const Vector2D& t1, const Vector2D& t2,
			Vector3D& tangent, Vector3D& binormal
		);

		friend class DeviceContext;
		friend class GraphicsEngine;
	};
}
