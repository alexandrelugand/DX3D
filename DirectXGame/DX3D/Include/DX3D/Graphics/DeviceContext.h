#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class DeviceContext
	{
	public:
		DeviceContext(RenderSystem* render_system, ID3D11DeviceContext* device);

		void ClearRenderTargetColor(const SwapChainPtr& swap_chain, float red, float green, float blue, float alpha) const;
		void ClearDepthStencil(const SwapChainPtr& swap_chain) const;

		void SetVertexBuffer(const VertexBufferPtr& vertex_buffer) const;
		void SetIndexBuffer(const IndexBufferPtr& index_buffer) const;

		void DrawTriangleList(size_t vertex_count, size_t start_vertex_index) const;
		void DrawIndexedTriangleList(size_t index_count, size_t start_vertex_index, size_t start_index_location) const;
		void DrawTriangleStrip(size_t vertex_count, size_t start_vertex_index) const;

		void SetViewportSize(const Rect& size) const;

		void SetVertexShader(const VertexShaderPtr& vs) const;
		void SetPixelShader(const PixelShaderPtr& ps) const;

		void SetTexture(const Texture2DPtr* texture_list, size_t num_textures) const;

		void SetConstantBuffer(const ConstantBufferPtr& cb) const;

	private:
		ComPtr<ID3D11DeviceContext> m_context = nullptr;
		RenderSystem* m_render = nullptr;

		friend class ConstantBuffer;
	};
}
