#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/RenderSystem.h>

namespace DX3D
{
	IndexBuffer::IndexBuffer(RenderSystem* render_system, void* list_indices, uint size_list)
		: m_size_list(0), m_render(render_system)
	{
		D3D11_BUFFER_DESC buff_desc = {};
		buff_desc.Usage = D3D11_USAGE_DEFAULT; // Accessible by GPU
		buff_desc.ByteWidth = sizeof(UINT) * size_list;
		buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER; // Bind buffer to index buffer of pipeline 
		buff_desc.CPUAccessFlags = 0;
		buff_desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA init_data = {};
		init_data.pSysMem = list_indices;

		m_size_list = size_list;

		if (FAILED(m_render->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
		{
			DX3DException("Failed to create index buffer.");
		}
	}
}
