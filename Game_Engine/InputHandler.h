#pragma once
#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include "common.h"

#define KEY(T) Keyboard::T
#define KEY_DOWN(T) InputHandler::getInstance().GetKeyDown(Keyboard::T)
#define KEY_UP(T) InputHandler::getInstance().GetKeyUp(Keyboard::T)
#define KEY_PRESSED(T) InputHandler::getInstance().GetKey(Keyboard::T)

#define LMOUSE_DOWN InputHandler::getInstance().GetLeftMouseKeyDown()
#define LMOUSE_UP InputHandler::getInstance().GetLeftMouseKeyUp()
#define LMOUSE_PRESSED InputHandler::getInstance().GetLeftMouseKey()

#define RMOUSE_DOWN InputHandler::getInstance().GetRightMouseKeyDown()
#define RMOUSE_UP InputHandler::getInstance().GetRightMouseKeyUp()
#define RMOUSE_PRESSED InputHandler::getInstance().GetRightMouseKey()
class InputHandler
{
private:
	Mouse m_mouse;
	Mouse::ButtonStateTracker m_mouseButtons;
	Keyboard m_keyboard;
	Keyboard::KeyboardStateTracker	m_keyboardButtons;
	RECT m_windowRect;
	int m_scrollValue;
	int m_mouseX, m_mouseY;
	size_t m_resX;
	size_t m_resY;
public:
	InputHandler();
	~InputHandler();
	static InputHandler& getInstance() { static InputHandler instance; return instance; }
	InputHandler(const InputHandler&) = delete;
	InputHandler& operator=(const InputHandler&) = delete;
	InputHandler(InputHandler&&) = delete;
	InputHandler& operator=(InputHandler&&) = delete;
	void changeRes(size_t newResX, size_t newResY, const HWND& hwnd);
	void init(const HWND& hwnd);
	void update();
	void setMouseXY(int x, int y) { m_mouseX = x; m_mouseY = y; }
	int getX() { return m_mouseX; }
	int getY() { return m_mouseY; }
	int g_mouseY, g_mouseX;
	int g_prevMouseY, g_prevMouseX;
	bool GetKey(DirectX::Keyboard::Keys key);
	bool GetKeyDown(DirectX::Keyboard::Keys key);
	bool GetKeyUp(DirectX::Keyboard::Keys key);
	void SetMouseMode(DirectX::Mouse::Mode mode);
	void ConfineMouse();
	void FreeMouse();
	void ResetRelative();
	bool GetLeftMouseKey();
	bool GetLeftMouseKeyDown();
	bool GetLeftMouseKeyUp();
	bool GetRightMouseKey();
	bool GetRightMouseKeyDown();
	bool GetRightMouseKeyUp();
	bool isScrollingDown();
	bool isScrollingUp();
	bool GetIsVisible() const { return m_mouse.IsVisible(); }
	POINT GetMousePos();
	POINT GetMousePosRelative() const;
	POINT GetPrevMousePosRelative() const;

};

#endif