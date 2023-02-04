#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class Texture2D
	{
	public:
		enum class Type
		{
			Normal = 0,
			RenderTarget,
			DepthStencil
		};

		Texture2D(RenderSystem* system, const wchar_t* full_path);
		Texture2D(RenderSystem* system, const Rect& size, Type type);
		~Texture2D();

		const Rect& GetSize() const { return m_size; }
		Type GetType() const { return m_type; }

	private:
		ComPtr<ID3D11Resource> m_texture = nullptr;
		ComPtr<ID3D11ShaderResourceView> m_srv = nullptr;
		ComPtr<ID3D11RenderTargetView> m_rtv = nullptr;
		ComPtr<ID3D11DepthStencilView> m_dsv = nullptr;
		ComPtr<ID3D11SamplerState> m_sampler_state = nullptr;

		RenderSystem* m_system = nullptr;
		Type m_type = Type::Normal;
		Rect m_size = {0, 0};

		friend class DeviceContext;
	};
}
