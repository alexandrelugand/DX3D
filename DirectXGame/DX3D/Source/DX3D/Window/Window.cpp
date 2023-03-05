#include <DX3D/Prerequisites.h>
#include <DX3D/Window/Window.h>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace DX3D
{
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
			return 1;

		switch (msg)
		{
		case WM_SETFOCUS:
			{
				// Event fired when the window will be destroyed
				if (const auto window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)))
					window->OnFocus();
				break;
			}

		case WM_KILLFOCUS:
			{
				// Event fired when the window will be destroyed
				if (const auto window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)))
					window->OnKillFocus();
				break;
			}

		case WM_SIZE:
			{
				// Event fired when the window resize
				if (const auto window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)))
					window->OnSize(window->GetClientSize());
				break;
			}

		case WM_CLOSE:
			{
				if (const auto window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)))
					window->OnClose();
				break;
			}

		case WM_DESTROY:
			{
				// Event fired when the window will be destroyed
				if (const auto window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)))
					window->OnDestroy();
				break;
			}

		default:
			return ::DefWindowProc(hwnd, msg, wparam, lparam);
		}

		return 0;
	}

	Window::Window()
	{
		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpszClassName = L"DX3DWindowClass";
		wc.lpfnWndProc = &WndProc;

		const auto classId = ::RegisterClassEx(&wc);
		if (!classId)
		{
			DX3DException("Failed to register DX3D window class.");
		}

		RECT rc = {0, 0, m_size.Width(), m_size.Height()};
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);


		m_hwnd = CreateWindowEx(NULL, MAKEINTATOM(classId), L"DirectX 3D Game",
		                        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		                        rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, nullptr, nullptr);

		if (!m_hwnd)
		{
			DX3DException("Failed to create game window.");
		}

		const auto hwnd = static_cast<HWND>(m_hwnd);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		//show up the window
		ShowWindow(hwnd, SW_SHOW);
		UpdateWindow(hwnd);

		//Center the window in screen
		const int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
		const int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;

		SetWindowPos(hwnd, nullptr, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

		DX3DInfo("Window initialized.");
	}

	Window::~Window()
	{
		DestroyWindow(static_cast<HWND>(m_hwnd));
		DX3DInfo("Window released.");
	}

	Rect Window::GetClientSize() const
	{
		RECT rc = {};
		const auto hwnd = static_cast<HWND>(m_hwnd);
		GetClientRect(hwnd, &rc);
		ClientToScreen(hwnd, reinterpret_cast<LPPOINT>(&rc.left));
		ClientToScreen(hwnd, reinterpret_cast<LPPOINT>(&rc.right));
		return {rc};
	}

	Rect Window::GetScreenSize()
	{
		const int width = GetSystemMetrics(SM_CXSCREEN);
		const int height = GetSystemMetrics(SM_CYSCREEN);
		return {width, height};
	}

	void Window::OnCreate() { }

	void Window::OnUpdate() { }

	void Window::OnDestroy() { }

	void Window::OnFocus() { }

	void Window::OnKillFocus() { }

	void Window::OnSize(const Rect& size) { }

	void Window::OnClose()
	{
		m_hwnd = nullptr;
		PostQuitMessage(0);
	}

	void Window::Close()
	{
		const auto hwnd = static_cast<HWND>(m_hwnd);
		::PostMessage(hwnd, WM_CLOSE, 0, 0);
	}
}
