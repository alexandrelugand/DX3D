#pragma once

#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class Window
	{
	public:
		Window();
		virtual ~Window();

		void* GetHwnd() const { return m_hwnd; }
		Rect GetClientSize() const;
		static Rect GetScreenSize();

		// Events
		virtual void OnCreate();
		virtual void OnUpdate();
		virtual void OnDestroy();
		virtual void OnFocus();
		virtual void OnKillFocus();
		virtual void OnSize(const Rect& size);
		virtual void OnClose();

		virtual void Close();

	protected:
		void* m_hwnd = nullptr;
		Rect m_size = {1024, 768};
	};
}
