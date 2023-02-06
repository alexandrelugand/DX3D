#include <DX3D/Game/Display.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Window/Window.h>

namespace DX3D
{
	void Game::Run()
	{
		OnCreate();

		MSG msg = {};
		while (m_isRunning)
		{
			if (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					m_isRunning = false;
					continue;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (m_display->GetHwnd())
				OnInternalUpdate();
		}

		OnQuit();
	}
}
