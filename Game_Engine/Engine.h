#pragma once
#ifndef ENGINE_H
#define ENGINE_H
#include "Window.h"
#include "ResourceHandler.h"
class Engine
{
private:
	Renderer* m_renderer = nullptr;
	ResourceHandler* m_resourceHandler = nullptr;
	MENU_OPTIONS m_optionSelection;
#ifdef START_RESOLUTION_HIGH
	size_t m_currResX = G_HIGH_RES_X;
	size_t m_currResY = G_HIGH_RES_Y;
	MENU_OPTIONS m_currRes = MENU_OPTIONS::HIGH;
#elif START_RESOLUTION_MEDIUM
	size_t m_currResX = G_MEDIUM_RES_X;
	size_t m_currResY = G_MEDIUM_RES_Y;
	MENU_OPTIONS m_currRes = MENU_OPTIONS::MEDIUM;
#else
	size_t m_currResX = G_LOW_RES_X;
	size_t m_currResY = G_LOW_RES_Y;
	MENU_OPTIONS m_currRes = MENU_OPTIONS::LOW;
#endif
	Window* m_window = nullptr;
public:
	Engine(const HINSTANCE& hinstance);
	~Engine();
	void run();
	void resize(const HWND& hwnd);
	void changeRes(size_t newResX, size_t newResY, const HWND& hwnd);
	SCENE g_currentScene = SCENE::MENU;
	SCENE g_newScene = SCENE::MENU;
	Renderer* getRenderer() { return m_renderer; }
};

#endif