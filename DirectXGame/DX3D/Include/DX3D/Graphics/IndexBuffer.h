#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class IndexBuffer
	{
	public:
		IndexBuffer(RenderSystem* render_system, void* list_indices, uint size_list);

		uint GetSizeIndexList() const { return m_size_list; }

	private:
		uint m_size_list;

		ComPtr<ID3D11Buffer> m_buffer = nullptr;
		RenderSystem* m_render = nullptr;

		friend class DeviceContext;
	};
}
