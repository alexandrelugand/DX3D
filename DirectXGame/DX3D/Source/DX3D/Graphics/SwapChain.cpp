// ReSharper disable CppClangTidyClangDiagnosticLanguageExtensionToken
#include <DirectXTex.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/SwapChain.h>

namespace DX3D
{
	SwapChain::SwapChain(RenderSystem* render_system, HWND hwnd, const Rect& size)
		: m_render(render_system)
	{
		const auto device = m_render->m_d3d_device;

		DXGI_SWAP_CHAIN_DESC swap_chain_desc;
		ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));

		swap_chain_desc.BufferCount = 1;
		swap_chain_desc.BufferDesc.Width = size.Width();
		swap_chain_desc.BufferDesc.Height = size.Height();
		swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60; // 60 Hz
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 60;
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.OutputWindow = hwnd;
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.SampleDesc.Quality = 0;
		swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //Enable change mode (window / fullscreen)
		swap_chain_desc.Windowed = TRUE; //Enable window mode

		const HRESULT res = m_render->m_dxgi_factory->CreateSwapChain(device.Get(), &swap_chain_desc, &m_swap_chain);

		if (FAILED(res))
		{
			DX3DException("Failed to initialize DirectX swap chain.");
		}

		ReloadBuffers(size);
	}

	void SwapChain::Present(bool vsync) const
	{
		m_swap_chain->Present(vsync, NULL);
	}

	void SwapChain::Resize(const Rect& size)
	{
		m_rtv.Reset();
		m_dsv.Reset();

		m_swap_chain->ResizeBuffers(1, size.Width(), size.Height(), DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		ReloadBuffers(size);
	}

	void SwapChain::SetFullScreen(bool fullscreen, const Rect& size)
	{
		Resize(size);
		m_swap_chain->SetFullscreenState(fullscreen, nullptr);
	}

	void SwapChain::ReloadBuffers(const Rect& size)
	{
		auto device = m_render->m_d3d_device;
		ID3D11Texture2D* p_buffer = nullptr;
		HRESULT res = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&p_buffer));

		if (FAILED(res))
		{
			DX3DException("Failed to get back buffer.");
		}

		res = device->CreateRenderTargetView(p_buffer, nullptr, &m_rtv);
		p_buffer->Release();

		if (FAILED(res))
		{
			DX3DException("Failed to create render target view.");
		}

		D3D11_TEXTURE2D_DESC tex_desc = {};
		tex_desc.Width = size.Width();
		tex_desc.Height = size.Height();
		tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //24bits for depth data and 8bits for stencil data
		tex_desc.Usage = D3D11_USAGE_DEFAULT;
		tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		tex_desc.MipLevels = 1;
		tex_desc.SampleDesc.Count = 1;
		tex_desc.SampleDesc.Quality = 0;
		tex_desc.MiscFlags = 0;
		tex_desc.ArraySize = 1;
		tex_desc.CPUAccessFlags = 0;

		res = device->CreateTexture2D(&tex_desc, nullptr, &p_buffer);
		if (FAILED(res))
		{
			DX3DException("Failed to create depth / stencil texture.");
		}

		res = device->CreateDepthStencilView(p_buffer, nullptr, &m_dsv);
		p_buffer->Release();

		if (FAILED(res))
		{
			DX3DException("Failed to create depth / stencil view.");
		}
	}

	void SwapChain::SaveToFile(const wchar_t* file_path)
	{
		ID3D11Texture2D* p_buffer = nullptr;
		HRESULT res = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&p_buffer));
		if (SUCCEEDED(res))
		{
			DirectX::ScratchImage image{};
			res = CaptureTexture(m_render->m_d3d_device.Get(), m_render->m_imm_context.Get(), p_buffer, image);
			if (SUCCEEDED(res))
			{
				SaveToDDSFile(image.GetImages(),
				              image.GetImageCount(), image.GetMetadata(),
				              DirectX::DDS_FLAGS_NONE, file_path);
			}
		}
	}
}
