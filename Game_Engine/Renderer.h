#pragma once
#ifndef RENDERER_H
#define RENDERER_H
#include "UpdateDrawClass.h"
class Renderer
{
private:
	MENU_OPTIONS m_click = MENU_OPTIONS::NONE;
	D3D11_VIEWPORT m_viewport = {};
	UpdateDrawClass m_updateDraw;
	static WRL::ComPtr<ID3D11Device> m_device;
	static WRL::ComPtr<ID3D11DeviceContext> m_context;
	static WRL::ComPtr<ID3D11Debug> m_debug;
	static WRL::ComPtr<IDXGISwapChain> m_swapchain;
	DXGI_MODE_DESC m_modeDesc = {};
	WRL::ComPtr<IDXGIFactory> m_factory = nullptr;
	WRL::ComPtr<IDXGIAdapter> m_adapter0 = nullptr;
	WRL::ComPtr<IDXGIFactory> m_factory1 = nullptr;
	WRL::ComPtr<IDXGIAdapter> m_adapter1 = nullptr;
	WRL::ComPtr<IDXGIAdapter4> m_adapterFour = nullptr;
	WRL::ComPtr<IDXGIDevice> m_idxgiDevice = nullptr;
	DXGI_QUERY_VIDEO_MEMORY_INFO m_vRamInfo;
	HANDLE m_hProcess;
	DXGI_MODE_DESC* m_supportedModes = nullptr;
	float m_ram;
	float m_vRAM;
	int m_currentModeIndex;
	UINT m_numberOfSupportedModes;
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
	WRL::ComPtr< ID3D11BlendState> m_blendOn = nullptr;
	WRL::ComPtr< ID3D11BlendState> m_blendOff = nullptr;
	WRL::ComPtr< ID3D11BlendState> m_blendParticle = nullptr;
	WRL::ComPtr<ID3D11RasterizerState> m_shadowRS = nullptr;
	WRL::ComPtr<ID3D11RasterizerState> m_mainRS = nullptr;
	WRL::ComPtr<ID3D11RasterizerState> m_cullNoneRS = nullptr;
	WRL::ComPtr<ID3D11RasterizerState> m_clockWiseRS = nullptr;
	WRL::ComPtr<ID3D11RasterizerState> m_wireframeRS = nullptr;
	WRL::ComPtr<ID3D11DepthStencilState> m_depthDisableDSS = nullptr;
	WRL::ComPtr<ID3D11DepthStencilState> m_mainDSS = nullptr;
	WRL::ComPtr<ID3D11DepthStencilView> m_mainDSV = nullptr;
	WRL::ComPtr<ID3D11RenderTargetView> m_mainRTV = nullptr;
	Shape* m_player =  nullptr;
	Shape* m_playerCloth = nullptr;
	int m_fireSize = 10;
	int m_leafSize = 10;
	int m_smokeSize = 10;
	int m_textSize = 10;
	std::vector<Shape*> m_shapes;
	void setRenderPassShapes();
	float m_roll = 0;
	float m_pitch = 0;
	float m_yaw = 0;
	float m_mouseSensitivity = 0.006f;
	float m_walkSpeed = 5.50f;
	float m_turnSpeed = 10.0f;
	float m_runSpeed = 10.0f;
	Light* m_dirLight = nullptr;
	bool m_renderBoundingBoxes = false;
	bool m_renderBoundingBoxesWireFrame = false;
	int m_currInstanceCount = 0;
	int m_trianglesDrawn = 0;
	int m_e = (int)PLAYER_SKELETON::pelvis;
	int m_trackSelectionA;
	int m_trackSelectionB;
	float m_lerpT = 0;
	Skeleton* m_combinedTrackPlayer = nullptr;
	Skeleton* m_combinedTrackCloth = nullptr;
	bool m_combineTrack = false;
	bool m_playOnce = false;
	bool m_playContinueously = false;
	bool m_playBlendOnce = false;
	bool m_playBlendContineously = false;
	float m_timeScale = 1;
	std::unordered_map<int, std::string> m_boneMap;
	Timer m_timer;
public:
	Renderer(const HWND& hwnd, const SCENE& scene);
	~Renderer();
	void present(bool vsync = true);
	void resize(const HWND& hwnd);
	void changeRes(size_t newResX, size_t newResY, const HWND& hwnd, const SCENE& scene);
	void restart();
	void render(const SCENE &scene = SCENE::MENU);
	void updateTransform();
	const MENU_OPTIONS& renderMenu(const SCENE& scene = SCENE::MENU);
	void renderIMGUI(const SCENE& scene = SCENE::MENU);
	static ID3D11DeviceContext* getContext() { return m_context.Get(); }
	static ID3D11Device* getDevice() { return m_device.Get(); }
	static ID3D11Debug* getDebug() { return m_debug.Get(); }
	static IDXGISwapChain* getswapChain() { return m_swapchain.Get(); }
	static BOOL g_pastInFullscreen;
	static bool g_isInitiated;
	bool g_pause = false;
	static BOOL g_currentlyInFullscreen;
	void setShapes(const std::vector<Shape*>& shapes, const SCENE& scene, const std::vector<Shape*>& shapesQuad, const std::vector<Shape*>& shapesOc, Shape* bspShape = nullptr, Shape* bb = nullptr);
	void setLights(const std::vector<Light*>& lights, Light* dirLight = nullptr);
	void combineTracks(std::string point, std::string trackA, std::string trackB);
};

#endif