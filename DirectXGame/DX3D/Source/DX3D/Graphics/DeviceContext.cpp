#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/PixelShader.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/Texture2D.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/VertexShader.h>

namespace DX3D
{
	DeviceContext::DeviceContext(RenderSystem* render_system, ID3D11DeviceContext* device)
		: m_context(device), m_render(render_system) { }

	void DeviceContext::ClearRenderTargetColor(const SwapChainPtr& swap_chain, float red, float green, float blue, float alpha) const
	{
		const FLOAT clear_color[] = {red, green, blue, alpha};
		const auto rtv = swap_chain->m_rtv.Get();
		const auto dsv = swap_chain->m_dsv.Get();

		m_context->ClearRenderTargetView(rtv, clear_color);
		m_context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
		m_context->OMSetRenderTargets(1, &rtv, dsv); // Which target we want render (back buffer)
	}

	void DeviceContext::ClearDepthStencil(const SwapChainPtr& swap_chain) const
	{
		const auto dsv = swap_chain->m_dsv.Get();
		m_context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	}

	void DeviceContext::SetVertexBuffer(const VertexBufferPtr& vertex_buffer) const
	{
		const size_t stride = vertex_buffer->m_size_vertex;
		constexpr size_t offset = 0;
		const auto buffer = vertex_buffer->m_buffer.Get();
		const auto layout = vertex_buffer->m_layout.Get();

		m_context->IASetVertexBuffers(0, 1, &buffer, reinterpret_cast<const UINT*>(&stride), reinterpret_cast<const UINT*>(&offset));
		m_context->IASetInputLayout(layout);
	}

	void DeviceContext::SetIndexBuffer(const IndexBufferPtr& index_buffer) const
	{
		const auto buffer = index_buffer->m_buffer.Get();
		m_context->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void DeviceContext::DrawTriangleList(size_t vertex_count, size_t start_vertex_index) const
	{
		m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_context->Draw(static_cast<UINT>(vertex_count), static_cast<UINT>(start_vertex_index));
	}

	void DeviceContext::DrawIndexedTriangleList(size_t index_count, size_t start_vertex_index, size_t start_index_location) const
	{
		m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_context->DrawIndexed(static_cast<UINT>(index_count), static_cast<UINT>(start_index_location), static_cast<INT>(start_vertex_index));
	}

	void DeviceContext::DrawTriangleStrip(size_t vertex_count, size_t start_vertex_index) const
	{
		m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		m_context->Draw(static_cast<UINT>(vertex_count), static_cast<UINT>(start_vertex_index));
	}

	void DeviceContext::SetViewportSize(const Rect& size) const
	{
		D3D11_VIEWPORT vp = {};
		vp.Width = static_cast<float>(size.Width());
		vp.Height = static_cast<float>(size.Height());
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		m_context->RSSetViewports(1, &vp);
	}

	void DeviceContext::SetVertexShader(const VertexShaderPtr& vs) const
	{
		const auto shader = vs->m_shader.Get();
		m_context->VSSetShader(shader, nullptr, 0);
	}

	void DeviceContext::SetPixelShader(const PixelShaderPtr& ps) const
	{
		const auto shader = ps->m_shader.Get();
		m_context->PSSetShader(shader, nullptr, 0);
	}

	void DeviceContext::SetTexture(const Texture2DPtr* texture, size_t num_textures) const
	{
		ID3D11ShaderResourceView* list_res[32];
		ID3D11SamplerState* list_sampler_state[32];

		for (uint i = 0; i < num_textures; i++)
		{
			list_res[i] = texture[i]->m_srv.Get();
			list_sampler_state[i] = texture[i]->m_sampler_state.Get();
		}

		m_context->VSSetShaderResources(0, static_cast<unsigned>(num_textures), list_res);
		m_context->VSSetSamplers(0, static_cast<unsigned>(num_textures), list_sampler_state);
		m_context->PSSetShaderResources(0, static_cast<unsigned>(num_textures), list_res);
		m_context->PSSetSamplers(0, static_cast<unsigned>(num_textures), list_sampler_state);
	}

	void DeviceContext::SetConstantBuffer(const ConstantBufferPtr& cb) const
	{
		const auto buffer = cb->m_buffer.Get();
		m_context->VSSetConstantBuffers(0, 1, &buffer);
		m_context->PSSetConstantBuffers(0, 1, &buffer);
	}
}
