#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/VertexBuffer.h>

namespace DX3D
{
	VertexBuffer::VertexBuffer(RenderSystem* render_system, void* list_vertices, uint size_vertex, uint size_list)
		: m_render(render_system)
	{
		D3D11_BUFFER_DESC buff_desc = {};
		buff_desc.Usage = D3D11_USAGE_DEFAULT; // Accessible by GPU
		buff_desc.ByteWidth = size_vertex * size_list;
		buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Bind buffer to vertex buffer of pipeline 
		buff_desc.CPUAccessFlags = 0;
		buff_desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA init_data = {};
		init_data.pSysMem = list_vertices;

		m_size_list = size_list;
		m_size_vertex = size_vertex;

		if (FAILED(m_render->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
		{
			DX3DException("Failed to create vertex buffer.");
		}

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			//SEMANTIC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALIGNED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		constexpr uint size_layout = ARRAYSIZE(layout);

		const HRESULT res = m_render->m_d3d_device->CreateInputLayout(layout, size_layout, m_render->m_mesh_layout_byte_code, m_render->m_mesh_layout_size, &m_layout);
		if (FAILED(res))
		{
			DX3DException("Failed to create input layout.");
		}
	}
}
