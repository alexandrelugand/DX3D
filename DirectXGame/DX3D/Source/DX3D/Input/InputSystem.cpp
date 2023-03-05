// ReSharper disable CppClangTidyBugproneNarrowingConversions
#include <DX3D/Input/InputSystem.h>

namespace DX3D
{
	InputSystem::InputSystem()
	{
		POINT current_mouse_pos = {};
		GetCursorPos(&current_mouse_pos);
		m_old_mouse_pos = Vector2D(static_cast<float>(current_mouse_pos.x), static_cast<float>(current_mouse_pos.y));
	}

	InputSystem::~InputSystem() {}

	bool InputSystem::IsKeyDown(const Key& key) const
	{
		return m_final_key_states[GetInternalKeyCode(key)] == 0;
	}

	bool InputSystem::IsKeyUp(const Key& key) const
	{
		return m_final_key_states[GetInternalKeyCode(key)] == 1;
	}

	void InputSystem::LockCursor(bool lock)
	{
		m_cursor_locked = lock;
		ShowCursor(!lock);
	}

	void InputSystem::SetLockArea(const Rect& area)
	{
		m_lock_area = area;
		m_lock_area_center = Vector2D(floor(static_cast<float>(area.left) + static_cast<float>(area.Width()) / 2.0f), floor(static_cast<float>(area.top) + static_cast<float>(area.Height()) / 2.0f));
	}

	void InputSystem::Update()
	{
		POINT current_mouse_pos = {};
		GetCursorPos(&current_mouse_pos);

		if (m_cursor_locked)
		{
			if (static_cast<float>(current_mouse_pos.x) > m_old_mouse_pos.x ||
				static_cast<float>(current_mouse_pos.x) < m_old_mouse_pos.x ||
				static_cast<float>(current_mouse_pos.y) > m_old_mouse_pos.y ||
				static_cast<float>(current_mouse_pos.y) < m_old_mouse_pos.y)
			{
				//There is mouse move event
				m_delta_mouse_pos = Vector2D(static_cast<float>(current_mouse_pos.x) - m_old_mouse_pos.x, static_cast<float>(current_mouse_pos.y) - m_old_mouse_pos.y);
			}
			else
			{
				m_delta_mouse_pos = {};
			}

			SetCursorPos(static_cast<int>(m_lock_area_center.x), static_cast<int>(m_lock_area_center.y));
			m_old_mouse_pos = m_lock_area_center;
		}
		else
		{
			m_delta_mouse_pos = {};
		}

		for (int i = 0; i < 256; i++)
		{
			m_key_states[i] = GetAsyncKeyState(i);

			// Key is down
			if (m_key_states[i] & 0x8001) //If the high-order bit is 1, the key is down, then check for byte value 0x80 mask
			{
				m_final_key_states[i] = 0; //Key down
			}
			else // Key is up
			{
				if (m_key_states[i] != m_old_key_states[i])
				{
					m_final_key_states[i] = 1; //Key up
				}
				else
				{
					m_final_key_states[i] = 2; //Key rest
				}
			}
		}
		//Store current keys state to old keys state buffer
		memcpy(m_old_key_states, m_key_states, sizeof(short) * 256);
	}

	short InputSystem::GetInternalKeyCode(const Key& key)
	{
		short keyWin = 0;
		if (key >= Key::A && key <= Key::Z)
			keyWin = static_cast<short>(key) - static_cast<short>(Key::A) + 'A';
		else if (key >= Key::_0 && key <= Key::_9)
			keyWin = static_cast<short>(key) - static_cast<short>(Key::_0) + '0';
		else if (key >= Key::F1 && key <= Key::F12)
			keyWin = static_cast<short>(key) - static_cast<short>(Key::F1) + VK_F1;
		else if (key == Key::Shift)
			keyWin = VK_SHIFT;
		else if (key == Key::Ctrl)
			keyWin = VK_CONTROL;
		else if (key == Key::Escape)
			keyWin = VK_ESCAPE;
		else if (key == Key::Space)
			keyWin = VK_SPACE;
		else if (key == Key::Enter)
			keyWin = VK_RETURN;
		else if (key == Key::LeftMouse)
			keyWin = VK_LBUTTON;
		else if (key == Key::MiddleMouse)
			keyWin = VK_MBUTTON;
		else if (key == Key::RightMouse)
			keyWin = VK_RBUTTON;
		else if (key == Key::PrintScreen)
			keyWin = VK_SNAPSHOT;

		return keyWin;
	}
}
