// ReSharper disable CppClangTidyClangDiagnosticLanguageExtensionToken
#include <DX3D/Prerequisites.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/Font2D.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/PixelShader.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/VertexShader.h>

namespace DX3D
{
	RenderSystem::RenderSystem()
	{
		D3D_DRIVER_TYPE driver_types[] =
		{
			D3D_DRIVER_TYPE_HARDWARE, // Execute on GPU
			D3D_DRIVER_TYPE_WARP, // All execution CPU with high performance (SSE instructions)
			D3D_DRIVER_TYPE_REFERENCE // Reference driver (best performance)
		};
		constexpr uint num_driver_types = ARRAYSIZE(driver_types);


		D3D_FEATURE_LEVEL feature_levels[] =
		{
			D3D_FEATURE_LEVEL_11_0
		};
		constexpr uint num_feature_levels = ARRAYSIZE(feature_levels);

		D3D_FEATURE_LEVEL feature_level = {};

		HRESULT res = 0;
		for (uint driver_type_index = 0; driver_type_index < num_driver_types;)
		{
			res = D3D11CreateDevice(nullptr, driver_types[driver_type_index], nullptr, NULL, feature_levels,
			                        num_feature_levels,
			                        D3D11_SDK_VERSION, &m_d3d_device, &feature_level, &m_imm_context);

			if (SUCCEEDED(res))
				break;

			++driver_type_index;
		}

		if (FAILED(res))
		{
			DX3DException("Failed to initialize Render system.");
		}

		m_imm_device_context = std::make_shared<DeviceContext>(this, m_imm_context.Get());

		m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), &m_dxgi_device);
		m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), &m_dxgi_adapter);
		m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), &m_dxgi_factory);

		InitRasterizerState();
		CompilePrivateShaders();

		DX3DInfo("Render system initialized.");
	}

	SwapChainPtr RenderSystem::CreateSwapChain(HWND hwnd, const Rect& size)
	{
		return std::make_shared<SwapChain>(this, hwnd, size);
	}

	DeviceContextPtr RenderSystem::GetImmediateDeviceContext()
	{
		return m_imm_device_context;
	}

	VertexBufferPtr RenderSystem::CreateVertexBuffer(void* list_vertices, uint size_vertex, uint size_list)
	{
		return std::make_shared<VertexBuffer>(this, list_vertices, size_vertex, size_list);
	}

	IndexBufferPtr RenderSystem::CreateIndexBuffer(void* list_indices, uint size_list)
	{
		return std::make_shared<IndexBuffer>(this, list_indices, size_list);
	}

	ConstantBufferPtr RenderSystem::CreateConstantBuffer(void* buffer, uint size_buffer)
	{
		return std::make_shared<ConstantBuffer>(this, buffer, size_buffer);
	}

	VertexShaderPtr RenderSystem::CreateVertexShader(const wchar_t* full_path, const char* entry_point)
	{
		return std::make_shared<VertexShader>(this, full_path, entry_point);
	}

	PixelShaderPtr RenderSystem::CreatePixelShader(const wchar_t* full_path, const char* entry_point)
	{
		return std::make_shared<PixelShader>(this, full_path, entry_point);
	}

	Texture2DPtr RenderSystem::CreateTexture(const wchar_t* full_path)
	{
		return std::make_shared<Texture2D>(this, full_path);
	}

	Texture2DPtr RenderSystem::CreateTexture(const Rect& size, Texture2D::Type type)
	{
		return std::make_shared<Texture2D>(this, size, type);
	}

	Font2DPtr RenderSystem::CreateFont(const wchar_t* full_path)
	{
		return std::make_shared<Font2D>(this, full_path);
	}

	void RenderSystem::SetCullMode(const CullMode& cull_mode) const
	{
		if (cull_mode == CullMode::Front)
			m_imm_context->RSSetState(m_cull_front_state.Get());
		else if (cull_mode == CullMode::Back)
			m_imm_context->RSSetState(m_cull_back_state.Get());
		else if (cull_mode == CullMode::None)
			m_imm_context->RSSetState(m_cull_none_state.Get());
	}

	void RenderSystem::SetWireFrameMode(const bool wire_frame) const
	{
		if (wire_frame)
			m_imm_context->RSSetState(m_wire_frame_state.Get());
		else
			m_imm_context->RSSetState(m_cull_front_state.Get());
	}

	void RenderSystem::ClearState()
	{
		m_imm_context->ClearState();
		m_imm_context->OMSetBlendState(m_alpha_blend_state.Get(), nullptr, 0xffffffff);
	}

	void RenderSystem::InitRasterizerState()
	{
		D3D11_RASTERIZER_DESC desc = {};
		desc.CullMode = D3D11_CULL_NONE;
		desc.DepthClipEnable = false;
		desc.FrontCounterClockwise = false;
		desc.FillMode = D3D11_FILL_WIREFRAME;

		m_d3d_device->CreateRasterizerState(&desc, &m_wire_frame_state);

		desc.CullMode = D3D11_CULL_FRONT;
		desc.DepthClipEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.FrontCounterClockwise = true;
		m_d3d_device->CreateRasterizerState(&desc, &m_cull_front_state);

		desc.CullMode = D3D11_CULL_BACK;
		m_d3d_device->CreateRasterizerState(&desc, &m_cull_back_state);

		desc.CullMode = D3D11_CULL_NONE;
		m_d3d_device->CreateRasterizerState(&desc, &m_cull_none_state);

		D3D11_BLEND_DESC blend_desc = {};
		blend_desc.AlphaToCoverageEnable = false;
		blend_desc.IndependentBlendEnable = false;
		blend_desc.RenderTarget[0].BlendEnable = true;
		blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		m_d3d_device->CreateBlendState(&blend_desc, &m_alpha_blend_state);
		m_imm_context->OMSetBlendState(m_alpha_blend_state.Get(), nullptr, 0xffffffff);
	}

	void RenderSystem::CompilePrivateShaders()
	{
		ComPtr<ID3DBlob> blob;
		ComPtr<ID3DBlob> error_blob;

		const auto meshLayoutCode = R"(
			struct VS_INPUT
			{
				float4 position: POSITION0;
				float2 texcoord: TEXCOORD0;
				float3 normal: NORMAL0;
				float3 tangent: TANGENT0;
				float3 binormal: BINORMAL0;
			};

			struct VS_OUTPUT
			{
				float4 position: POSITION0;
				float2 texcoord: TEXCOORD0;
			};

			VS_OUTPUT main(VS_INPUT input)
			{
				VS_OUTPUT output = (VS_OUTPUT)0;
				return output;
			}
		)";

		const auto code_size = strlen(meshLayoutCode);

		if (FAILED(D3DCompile(meshLayoutCode, code_size, "VertexMeshLayoutShader", nullptr, nullptr, "main", "vs_5_0", 0, 0, &blob, &error_blob)))
		{
			DX3DException("Failed to compile VertexMeshLayoutShader");
		}

		memcpy(m_mesh_layout_byte_code, blob->GetBufferPointer(), blob->GetBufferSize());
		m_mesh_layout_size = blob->GetBufferSize();
	}
}
