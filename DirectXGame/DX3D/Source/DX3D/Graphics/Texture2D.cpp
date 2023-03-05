#include <DirectXTex.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/Texture2D.h>

using namespace DirectX;

namespace DX3D
{
	Texture2D::Texture2D(RenderSystem* system, const wchar_t* full_path)
		: m_system(system)
	{
		ScratchImage image_data;
		HRESULT res = LoadFromWICFile(full_path, WIC_FLAGS_IGNORE_SRGB, nullptr, image_data);

		if (FAILED(res))
		{
			DX3DException("Failed to create scratch image data.");
		}

		ScratchImage mip_chain = {};
		GenerateMipMaps(image_data.GetImages(), image_data.GetImageCount(), image_data.GetMetadata(), TEX_FILTER_DEFAULT | TEX_FILTER_SEPARATE_ALPHA, 0, mip_chain);

		res = CreateTexture(m_system->m_d3d_device.Get(), mip_chain.GetImages(), mip_chain.GetImageCount(), mip_chain.GetMetadata(), &m_texture);
		if (FAILED(res))
		{
			DX3DException("Failed to create texture.");
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = image_data.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = static_cast<uint>(image_data.GetMetadata().mipLevels);
		desc.Texture2D.MostDetailedMip = 0;

		D3D11_SAMPLER_DESC sampler_desc = {};
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = static_cast<float>(image_data.GetMetadata().mipLevels);

		res = m_system->m_d3d_device->CreateSamplerState(&sampler_desc, &m_sampler_state);
		if (FAILED(res))
		{
			DX3DException("Failed to create sampler state.");
		}

		res = m_system->m_d3d_device->CreateShaderResourceView(m_texture.Get(), &desc, &m_srv);
		if (FAILED(res))
		{
			DX3DException("Failed to create shader resource view.");
		}

		m_size = ::Rect(static_cast<int>(image_data.GetMetadata().width), static_cast<int>(image_data.GetMetadata().height));
	}

	Texture2D::Texture2D(RenderSystem* system, const ::Rect& size, Type type)
		: m_system(system), m_type(type), m_size(size)
	{
		D3D11_TEXTURE2D_DESC tex_desc = {};
		tex_desc.Width = size.Width();
		tex_desc.Height = size.Height();

		if (type == Type::Normal)
		{
			tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		}
		else if (type == Type::RenderTarget)
		{
			tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		}
		else if (type == Type::DepthStencil)
		{
			tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //24bits for depth data and 8bits for stencil data
			tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		}

		tex_desc.Usage = D3D11_USAGE_DEFAULT;
		tex_desc.MipLevels = 1;
		tex_desc.SampleDesc.Count = 1;
		tex_desc.SampleDesc.Quality = 0;
		tex_desc.MiscFlags = 0;
		tex_desc.ArraySize = 1;
		tex_desc.CPUAccessFlags = 0;

		auto res = m_system->m_d3d_device->CreateTexture2D(&tex_desc, nullptr, reinterpret_cast<ID3D11Texture2D**>(m_texture.GetAddressOf()));
		if (FAILED(res))
		{
			DX3DException("Failed to create texture.");
		}

		if (type == Type::Normal || type == Type::RenderTarget)
		{
			res = m_system->m_d3d_device->CreateShaderResourceView(m_texture.Get(), nullptr, &m_srv);
			if (FAILED(res))
			{
				DX3DException("Failed to create shader resource view for texture.");
			}
		}

		if (type == Type::RenderTarget)
		{
			res = m_system->m_d3d_device->CreateRenderTargetView(m_texture.Get(), nullptr, &m_rtv);
			if (FAILED(res))
			{
				DX3DException("Failed to create render target view for texture.");
			}
		}
		else if (type == Type::DepthStencil)
		{
			res = m_system->m_d3d_device->CreateDepthStencilView(m_texture.Get(), nullptr, &m_dsv);
			if (FAILED(res))
			{
				DX3DException("Failed to create depth stencil view for texture.");
			}
		}
	}

	Texture2D::~Texture2D() {}

	void Texture2D::SaveToFile(const wchar_t* file_path) const
	{
		ScratchImage image{};
		HRESULT res = CaptureTexture(m_system->m_d3d_device.Get(), m_system->m_imm_context.Get(), m_texture.Get(), image);
		if (SUCCEEDED(res))
		{
			SaveToDDSFile(image.GetImages(),
				image.GetImageCount(), image.GetMetadata(),
				DDS_FLAGS_NONE, file_path);
		}
	}
}
