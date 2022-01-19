#include "stdafx.h"
#include "Engine.h"

Engine::Engine(const HINSTANCE& hinstance)
{
	m_window = new Window(hinstance);
	m_renderer = new Renderer(Window::getHWND(), g_currentScene);
	m_resourceHandler = new ResourceHandler(g_currentScene, m_currResX, m_currResY);
	m_renderer->setShapes(m_resourceHandler->getShapes(), g_currentScene, m_resourceHandler->getQuadShapes(), m_resourceHandler->getOcShapes(), m_resourceHandler->getBSP(), m_resourceHandler->getBB());
}

Engine::~Engine()
{
	RELEASE(m_resourceHandler);
	RELEASE(m_renderer);
	RELEASE(m_window);
	//Renderer::getDebug()->ReportLiveDeviceObjects(D3D11_RLDO_FLAGS::D3D11_RLDO_DETAIL);
}

void Engine::run()
{
	MSG msg = { };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{

			TranslateMessage(&msg);
			DispatchMessage(&msg);


		}
		else
		{
			bool resizing = false;
			if (m_window->g_shouldResize)
			{
				m_renderer->g_pause = true;
				resize(Window::getHWND());
				resizing = true;
			}
			if (resizing)
			{
				if (m_renderer->g_pause == false)
				{
					Window::g_shouldResize = false;

					resizing = false;
					
				}
			}
			if (g_currentScene != g_newScene)
			{
				m_resourceHandler->restart(g_newScene, m_currResX, m_currResY);
				m_renderer->restart();
				m_renderer->setShapes(m_resourceHandler->getShapes(), g_newScene, m_resourceHandler->getQuadShapes(), m_resourceHandler->getOcShapes(), m_resourceHandler->getBSP(), m_resourceHandler->getBB());
				m_renderer->setLights(m_resourceHandler->getLights(), m_resourceHandler->getDirLight());
				g_currentScene = g_newScene;
			}
			switch (g_currentScene)
			{
			default:
			{
				if (!m_renderer->g_pause)
				{
					m_resourceHandler->update();
					m_renderer->updateTransform();
					m_renderer->render(g_currentScene);
					if (InputHandler::getInstance().GetKey(Keyboard::Keys::Escape))
					{
						g_newScene = SCENE::MENU;
					}
				}
				break;
			}
			
			case SCENE::MENU:
			{
				if (!m_renderer->g_pause)
				{
					m_optionSelection = m_renderer->renderMenu(g_currentScene);
					
				}
				if (m_optionSelection == MENU_OPTIONS::HIGH || m_optionSelection == MENU_OPTIONS::MEDIUM || m_optionSelection == MENU_OPTIONS::LOW)
				{
					if (m_optionSelection != m_currRes)
					{
						switch (m_optionSelection)
						{
						default:
						{
							break;
						}
						case MENU_OPTIONS::HIGH:
						{
							changeRes(G_HIGH_RES_X, G_HIGH_RES_Y, Window::getHWND());
							m_currRes = m_optionSelection;
							break;
						}
						case MENU_OPTIONS::MEDIUM:
						{
							changeRes(G_MEDIUM_RES_X, G_MEDIUM_RES_Y, Window::getHWND());
							m_currRes = m_optionSelection;
							break;
						}
						case MENU_OPTIONS::LOW:
						{
							changeRes(G_LOW_RES_X, G_LOW_RES_Y, Window::getHWND());
							m_currRes = m_optionSelection;
							break;
						}
						}
					}
				}
				else if (m_optionSelection == MENU_OPTIONS::SCENE_CEL)
				{
					g_newScene = SCENE::PARTICLE;
				}
				else if (m_optionSelection == MENU_OPTIONS::SCENE_LOD)
				{
					g_newScene = SCENE::LOD;
				}
				else if (m_optionSelection == MENU_OPTIONS::SCENE_HEAT)
				{
					g_newScene = SCENE::HEAT;
				}
				else if (m_optionSelection == MENU_OPTIONS::SCENE_ANIMATION)
				{
					g_newScene = SCENE::ANIMATION;
				}
				else if (m_optionSelection == MENU_OPTIONS::QUIT)
				{
					
					PostQuitMessage(0);
				}

				break;
			}
			}
			if (!m_renderer->g_pause)
			{
				InputHandler::getInstance().update();
#if USE_IMGUI
				m_renderer->renderIMGUI(g_currentScene);
#endif
				m_renderer->present();
			}

		}
		
	}
}

void Engine::resize(const HWND& hwnd)
{
	m_renderer->resize(hwnd);
}

void Engine::changeRes(size_t newResX, size_t newResY, const HWND& hwnd)
{
	m_window->changeRes(newResX, newResY);
	m_renderer->changeRes(newResX, newResY, hwnd, g_currentScene);
	m_currResX = newResX;
	m_currResY = newResY;
}
