#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class VertexBuffer
	{
	public:
		VertexBuffer(RenderSystem* render_system, void* list_vertices, uint size_vertex, uint size_list);

		uint GetSizeVertexList() const { return m_size_list; }

	private:
		uint m_size_list = 0;
		uint m_size_vertex = 0;

		ComPtr<ID3D11Buffer> m_buffer = nullptr;
		ComPtr<ID3D11InputLayout> m_layout = nullptr;
		RenderSystem* m_render = nullptr;

		friend class DeviceContext;
	};
}
