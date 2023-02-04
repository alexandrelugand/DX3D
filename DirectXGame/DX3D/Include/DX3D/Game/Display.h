#pragma once
#include <DX3D/Window/Window.h>

namespace DX3D
{
	class Display : public Window
	{
	public:
		Display(Game* game);
		~Display() override;

		SwapChainPtr GetSwapChain() const { return m_swap_chain; }

		void SetFullScreen(bool fullscreen) const;

	protected:
		void OnSize(const Rect& size) override;
		void OnClose() override;

	private:
		SwapChainPtr m_swap_chain;
		Game* m_game = nullptr;
	};
}
