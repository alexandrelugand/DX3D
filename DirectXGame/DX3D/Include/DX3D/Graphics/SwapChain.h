#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class SwapChain
	{
	public:
		SwapChain(RenderSystem* render_system, HWND hwnd, const Rect& size);

		void Present(bool vsync) const;
		void Resize(const Rect& size);
		void SetFullScreen(bool fullscreen, const Rect& size);
		void SaveToFile(const wchar_t* file_path);

	private:
		ComPtr<IDXGISwapChain> m_swap_chain = nullptr;
		ComPtr<ID3D11RenderTargetView> m_rtv = nullptr;
		ComPtr<ID3D11DepthStencilView> m_dsv = nullptr;
		RenderSystem* m_render = nullptr;

		void ReloadBuffers(const Rect& size);

		friend class DeviceContext;
		friend class GraphicsEngine;
	};
}
