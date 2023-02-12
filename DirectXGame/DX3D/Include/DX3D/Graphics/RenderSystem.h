#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Graphics/Texture2D.h>

namespace DX3D
{
	using namespace Microsoft::WRL;

	class RenderSystem
	{
	public:
		RenderSystem();

		SwapChainPtr CreateSwapChain(HWND hwnd, const Rect& size);
		DeviceContextPtr GetImmediateDeviceContext();
		VertexBufferPtr CreateVertexBuffer(void* list_vertices, uint size_vertex, uint size_list);
		IndexBufferPtr CreateIndexBuffer(void* list_indices, uint size_list);
		ConstantBufferPtr CreateConstantBuffer(void* buffer, uint size_buffer);
		VertexShaderPtr CreateVertexShader(const wchar_t* full_path, const char* entry_point);
		PixelShaderPtr CreatePixelShader(const wchar_t* full_path, const char* entry_point);
		Texture2DPtr CreateTexture(const wchar_t* full_path);
		Texture2DPtr CreateTexture(const Rect& size, Texture2D::Type type);
		Font2DPtr CreateFont(const wchar_t* full_path);

		void SetCullMode(const CullMode& cull_mode) const;
		void ClearState();

	private:
		void InitRasterizerState();
		void CompilePrivateShaders();

		DeviceContextPtr m_imm_device_context = nullptr;

		ComPtr<ID3D11Device> m_d3d_device = nullptr;
		ComPtr<IDXGIDevice> m_dxgi_device = nullptr;
		ComPtr<IDXGIAdapter> m_dxgi_adapter = nullptr;
		ComPtr<IDXGIFactory> m_dxgi_factory = nullptr;
		ComPtr<ID3D11DeviceContext> m_imm_context = nullptr;

		ComPtr<ID3DBlob> m_blob = nullptr;

		ComPtr<ID3D11VertexShader> m_vs = nullptr;
		ComPtr<ID3D11PixelShader> m_ps = nullptr;

		ComPtr<ID3D11RasterizerState> m_cull_front_state = nullptr;
		ComPtr<ID3D11RasterizerState> m_cull_back_state = nullptr;
		ComPtr<ID3D11RasterizerState> m_cull_none_state = nullptr;
		ComPtr<ID3D11BlendState> m_alpha_blend_state = nullptr;

		unsigned char m_mesh_layout_byte_code[1024] = {};
		size_t m_mesh_layout_size = 0;

		friend class SwapChain;
		friend class VertexBuffer;
		friend class IndexBuffer;
		friend class ConstantBuffer;
		friend class VertexShader;
		friend class PixelShader;
		friend class Texture2D;
		friend class Font2D;
	};
}
