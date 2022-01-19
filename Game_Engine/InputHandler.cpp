#include "stdafx.h"
#include "InputHandler.h"

InputHandler::InputHandler()
{
#if START_RESOLUTION_HIGH
	m_resX = 1920;
	m_resY = 1080;
#elif START_RESOLUTION_MEDIUM
	m_resX = 1280;
	m_resY = 720;
#else
	m_resX = 640;
	m_resY = 480;
#endif

	

}

InputHandler::~InputHandler()
{
}

void InputHandler::changeRes(size_t newResX, size_t newResY, const HWND& hwnd)
{
	m_resX = newResX;
	m_resY = newResY;
	init(hwnd);
}

void InputHandler::init(const HWND& hwnd)
{
	m_mouse.SetWindow(hwnd);
	GetClientRect(hwnd, &m_windowRect);
	MapWindowPoints(hwnd, nullptr, reinterpret_cast<POINT*>(&m_windowRect), 2);
	m_mouse.SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);
	m_scrollValue = 0;
}

void InputHandler::update()
{
	m_mouseButtons.Update(m_mouse.GetState());
	m_keyboardButtons.Update(m_keyboard.GetState());
}

bool InputHandler::GetKey(DirectX::Keyboard::Keys key)
{
	return m_keyboardButtons.IsKeyPressed(key);
}

bool InputHandler::GetKeyDown(DirectX::Keyboard::Keys key)
{
	return m_keyboard.GetState().IsKeyDown(key);
}

bool InputHandler::GetKeyUp(DirectX::Keyboard::Keys key)
{
	return m_keyboardButtons.IsKeyReleased(key);
}

void InputHandler::SetMouseMode(DirectX::Mouse::Mode mode)
{
	m_mouse.SetMode(mode);
}

void InputHandler::ConfineMouse()
{
	RECT mouseConfineRect = {};
	mouseConfineRect.top = mouseConfineRect.bottom = (LONG)(m_resX / 2);
	mouseConfineRect.left = mouseConfineRect.right = (LONG)(m_resY / 2);

	ClipCursor(&mouseConfineRect);
}

void InputHandler::FreeMouse()
{
	ClipCursor(nullptr);
}

void InputHandler::ResetRelative()
{
	g_prevMouseX = g_mouseX;
	g_prevMouseY = g_mouseY;
	g_mouseX = 0;
	g_mouseY = 0;
}

bool InputHandler::GetLeftMouseKey()
{
	return  m_mouse.GetState().leftButton;
}

bool InputHandler::GetLeftMouseKeyDown()
{
	return m_mouseButtons.leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED;
}

bool InputHandler::GetLeftMouseKeyUp()
{
	return m_mouseButtons.leftButton == DirectX::Mouse::ButtonStateTracker::RELEASED;
}

bool InputHandler::GetRightMouseKey()
{
	return m_mouse.GetState().rightButton;
}

bool InputHandler::GetRightMouseKeyDown()
{
	return m_mouseButtons.rightButton == DirectX::Mouse::ButtonStateTracker::PRESSED;
}

bool InputHandler::GetRightMouseKeyUp()
{
	return m_mouseButtons.rightButton == DirectX::Mouse::ButtonStateTracker::RELEASED;
}

bool InputHandler::isScrollingDown()
{
	if (m_mouse.GetState().scrollWheelValue > m_scrollValue)
	{
		m_scrollValue = m_mouse.GetState().scrollWheelValue;
		return true;
	}
	else
	{
		return false;
	}
}

bool InputHandler::isScrollingUp()
{
	if (m_mouse.GetState().scrollWheelValue < m_scrollValue)
	{
		m_scrollValue = m_mouse.GetState().scrollWheelValue;
		return true;
	}
	else
	{
		return false;
	}
}

POINT InputHandler::GetMousePos()
{
	int x = m_mouse.GetState().x;
	int y = m_mouse.GetState().y;

	return POINT{ m_mouse.GetState().x,m_mouse.GetState().y };
}

POINT InputHandler::GetMousePosRelative() const
{
	return { g_mouseX,g_mouseY };
}

POINT InputHandler::GetPrevMousePosRelative() const
{
	return { g_prevMouseX,g_prevMouseY };
}
