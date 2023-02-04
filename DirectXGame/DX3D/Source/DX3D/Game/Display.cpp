#include <DX3D/Game/Display.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/SwapChain.h>

namespace DX3D
{
	Display::Display(Game* game)
		: m_game(game)
	{
		const auto size = GetClientSize();
		m_swap_chain = game->GetGraphicsEngine()->GetRenderSystem()->CreateSwapChain(static_cast<HWND>(m_hwnd), size);
	}

	Display::~Display() {}

	void Display::SetFullScreen(bool fullscreen) const
	{
		const auto size = fullscreen ? GetScreenSize() : GetClientSize();
		m_swap_chain->SetFullScreen(fullscreen, size);
	}

	void Display::OnSize(const Rect& size)
	{
		Window::OnSize(size);
		m_swap_chain->Resize(size);
		m_game->OnDisplaySize(size);
	}

	void Display::OnClose()
	{
		Window::OnClose();
		m_game->Quit();
	}
}
