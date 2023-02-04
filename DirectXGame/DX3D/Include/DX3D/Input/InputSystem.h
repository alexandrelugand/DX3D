#pragma once
#include <DX3D/Prerequisites.h>

namespace DX3D
{
	class InputSystem
	{
	public:
		InputSystem();
		~InputSystem();

		bool IsKeyDown(const Key& key) const;
		bool IsKeyUp(const Key& key) const;
		Vector2D GetDeltaMousePosition() const { return m_delta_mouse_pos; }

		void LockCursor(bool lock);
		void SetLockArea(const Rect& area);

		void Update();

	private:
		short m_key_states[256] = {};
		short m_old_key_states[256] = {};
		short m_final_key_states[256] = {};
		Vector2D m_old_mouse_pos = {0, 0};
		Vector2D m_delta_mouse_pos = {0, 0};
		bool m_cursor_locked = false;
		Rect m_lock_area = {0, 0};
		Vector2D m_lock_area_center = {0, 0};

		static short GetInternalKeyCode(const Key& key);
	};
}
