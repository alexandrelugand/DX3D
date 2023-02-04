#include <stdexcept>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/RenderSystem.h>

namespace DX3D
{
	ConstantBuffer::ConstantBuffer(RenderSystem* render_system, void* buffer, uint size_buffer)
		: m_render(render_system)
	{
		D3D11_BUFFER_DESC buff_desc = {};
		buff_desc.Usage = D3D11_USAGE_DEFAULT; // Accessible by GPU
		buff_desc.ByteWidth = size_buffer;
		buff_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Bind buffer to constant buffer of pipeline 
		buff_desc.CPUAccessFlags = 0;
		buff_desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA init_data = {};
		init_data.pSysMem = buffer;

		if (FAILED(m_render->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
		{
			DX3DException("Failed to create constant buffer.");
		}
	}

	void ConstantBuffer::Update(const DeviceContextPtr& context, void* buffer)
	{
		context->m_context->UpdateSubresource(m_buffer.Get(), 0, nullptr, buffer, 0, 0);
	}
}
