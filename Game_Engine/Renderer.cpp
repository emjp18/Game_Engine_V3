#include "stdafx.h"
#include "Renderer.h"
WRL::ComPtr<ID3D11Device> Renderer::m_device = nullptr;
WRL::ComPtr<ID3D11DeviceContext> Renderer::m_context = nullptr;
WRL::ComPtr<ID3D11Debug> Renderer::m_debug = nullptr;
WRL::ComPtr<IDXGISwapChain> Renderer::m_swapchain = nullptr;
BOOL Renderer::g_currentlyInFullscreen = 0;
bool Renderer::g_isInitiated =false;
BOOL Renderer:: g_pastInFullscreen = 0;
void Renderer::setShapes(const std::vector<Shape*>& shapes, const SCENE& scene, const std::vector<Shape*>& shapesQuad, const std::vector<Shape*>& shapesOc, Shape* bsp, Shape* bb)
{
	m_shapes = shapes;
	RenderPassHandler::getInstance().reCreate(m_device.Get(), m_currResX, m_currResY, scene);
	m_updateDraw.setShapes(shapes, scene, m_device.Get(),m_context.Get(), shapesQuad, shapesOc, bsp, bb);
	setRenderPassShapes();
}
void Renderer::setRenderPassShapes()
{

	RenderPassHandler& rph = RenderPassHandler::getInstance();

	for (size_t i = 0; i < m_shapes.size(); i++)
	{
		if (m_shapes[i]->getDrawCall()->g_renderType == RENDER_TYPE::BOUNDING_BOX)
		{
			continue;
		}
		if (m_shapes[i]->getDrawCall()->g_renderType == RENDER_TYPE::PLAYER_CHARACTER)
		{
			m_player = m_shapes[i];
		}
		if (m_shapes[i]->getDrawCall()->g_renderType == RENDER_TYPE::CLOTH)
		{
			m_playerCloth = m_shapes[i];
		}
		int flag = (int)m_shapes[i]->getDrawCall()->g_renderPassTypes;
	
		if ((flag & (int)RENDERPASS::DEPTH_LIGHT_PASS) != 0)
		{
			if (rph.renderPassExists(RENDERPASS::DEPTH_LIGHT_PASS))
			{
				rph.getRenderPass(RENDERPASS::DEPTH_LIGHT_PASS).setShape(m_shapes[i]);
			}
			
		}
		if ((flag & (int)RENDERPASS::BASIC_PASS_TRANSPARENT) != 0)
		{
			if (rph.renderPassExists(RENDERPASS::BASIC_PASS_TRANSPARENT))
			{
				rph.getRenderPass(RENDERPASS::BASIC_PASS_TRANSPARENT).setShape(m_shapes[i]);
			}

		}
		if ((flag & (int)RENDERPASS::BASIC_PASS_SINGLE) != 0)
		{
			if (rph.renderPassExists(RENDERPASS::BASIC_PASS_SINGLE))
			{
				rph.getRenderPass(RENDERPASS::BASIC_PASS_SINGLE).setShape(m_shapes[i]);
			}
			
		}
		if ((flag & (int)RENDERPASS::BASIC_PASS_QUAD) != 0)
		{
			if (rph.renderPassExists(RENDERPASS::BASIC_PASS_QUAD))
			{
				rph.getRenderPass(RENDERPASS::BASIC_PASS_QUAD).setShape(m_shapes[i]);
			}
			
		}
		if ((flag & (int)RENDERPASS::DEPTH_SHADOW_DIR) != 0)
		{
			if (rph.renderPassExists(RENDERPASS::DEPTH_SHADOW_DIR))
			{
				rph.getRenderPass(RENDERPASS::DEPTH_SHADOW_DIR).setShape(m_shapes[i]);
			}
			
		}
		if ((flag & (int)RENDERPASS::DEPTH_SHADOW_POINT) != 0)
		{
			if (rph.renderPassExists(RENDERPASS::DEPTH_SHADOW_POINT))
			{
				rph.getRenderPass(RENDERPASS::DEPTH_SHADOW_POINT).setShape(m_shapes[i]);
			}
			
		}
		if ((flag & (int)RENDERPASS::DYNAMIC_CUBE_MAP_PASS) != 0)
		{
			if (rph.renderPassExists(RENDERPASS::DYNAMIC_CUBE_MAP_PASS))
			{
				rph.getRenderPass(RENDERPASS::DYNAMIC_CUBE_MAP_PASS).setShape(m_shapes[i]);
			}
			
		}
		if ((flag & (int)RENDERPASS::DOWN_SAMPLE_EMISSIVE_PASS) != 0)
		{
			if (rph.renderPassExists(RENDERPASS::DOWN_SAMPLE_EMISSIVE_PASS))
			{
				rph.getRenderPass(RENDERPASS::DOWN_SAMPLE_EMISSIVE_PASS).setShape(m_shapes[i]);
			}
			
		}
		if ((flag & (int)RENDERPASS::DOWN_SAMPLE_RAY_MARCH_PASS) != 0)
		{
			if (rph.renderPassExists(RENDERPASS::DOWN_SAMPLE_RAY_MARCH_PASS))
			{
				rph.getRenderPass(RENDERPASS::DOWN_SAMPLE_RAY_MARCH_PASS).setShape(m_shapes[i]);
			}
			
		}
		if ((flag & (int)RENDERPASS::UP_SAMPLE_EMISSIVE_PASS) != 0)
		{
			if (rph.renderPassExists(RENDERPASS::UP_SAMPLE_EMISSIVE_PASS))
			{
				rph.getRenderPass(RENDERPASS::UP_SAMPLE_EMISSIVE_PASS).setShape(m_shapes[i]);
			}
			
		}
		if ((flag & (int)RENDERPASS::UP_SAMPLE_RAY_MARCH_PASS) != 0)
		{
			if (rph.renderPassExists(RENDERPASS::UP_SAMPLE_RAY_MARCH_PASS))
			{
				rph.getRenderPass(RENDERPASS::UP_SAMPLE_RAY_MARCH_PASS).setShape(m_shapes[i]);
			}
		
		}
		if ((flag & (int)RENDERPASS::RAY_MARCH) != 0)
		{
			if (rph.renderPassExists(RENDERPASS::RAY_MARCH))
			{
				rph.getRenderPass(RENDERPASS::RAY_MARCH).setShape(m_shapes[i]);
			}
		
		}
		if ((flag & (int)RENDERPASS::BILATERA_HOR) != 0)
		{
			if (rph.renderPassExists(RENDERPASS::BILATERA_HOR))
			{
				rph.getRenderPass(RENDERPASS::BILATERA_HOR).setShape(m_shapes[i]);
			}
			
		}
		if ((flag & (int)RENDERPASS::BILATERA_VER) != 0)
		{
			if (rph.renderPassExists(RENDERPASS::BILATERA_VER))
			{
				rph.getRenderPass(RENDERPASS::BILATERA_VER).setShape(m_shapes[i]);
			}
			
		}
		if ((flag & (int)RENDERPASS::EDGE_DETECTION_PASS) != 0)
		{
			if (rph.renderPassExists(RENDERPASS::EDGE_DETECTION_PASS))
			{
				rph.getRenderPass(RENDERPASS::EDGE_DETECTION_PASS).setShape(m_shapes[i]);
			}
			
		}
		if ((flag & (int)RENDERPASS::FXAA) != 0)
		{
			if (rph.renderPassExists(RENDERPASS::FXAA))
			{
				rph.getRenderPass(RENDERPASS::FXAA).setShape(m_shapes[i]);
			}
			
		}
		if ((flag & (int)RENDERPASS::GAUSSIAN_VER) != 0)
		{
			if (rph.renderPassExists(RENDERPASS::GAUSSIAN_VER))
			{
				DC_COMPUTE* dc = dynamic_cast<DC_COMPUTE*>(m_shapes[i]->getDrawCall());

				UINT numGroupsY = (UINT)ceilf(((float)m_currResY / 4.0f) / 256.0f);
				dc->g_dispatchThreads.x = (UINT)((float)m_currResX / 4.0f);
				dc->g_dispatchThreads.y = numGroupsY;
				dc->g_dispatchThreads.z = 1;
				rph.getRenderPass(RENDERPASS::GAUSSIAN_VER).setShape(m_shapes[i]);
			}
			
		}
		if ((flag & (int)RENDERPASS::GAUSSIAN_HOR) != 0)
		{
			if (rph.renderPassExists(RENDERPASS::GAUSSIAN_HOR))
			{
				DC_COMPUTE* dc = dynamic_cast<DC_COMPUTE*>(m_shapes[i]->getDrawCall());
				UINT numGroupsX = (UINT)ceilf(((float)m_currResX / 4.0f) / 256.0f);

				dc->g_dispatchThreads.x = numGroupsX;
				dc->g_dispatchThreads.y = (UINT)((float)m_currResY / 4.0f);
				dc->g_dispatchThreads.z = 1;
				rph.getRenderPass(RENDERPASS::GAUSSIAN_HOR).setShape(m_shapes[i]);
			}
			
		}
	}
	
	rph.getRenderPass(RENDERPASS::LIGHT_CULL_PASS).setShape(m_updateDraw.getCSCullingShape());
	
}





Renderer::Renderer(const HWND& hwnd, const SCENE& scene)
{
	UINT swapchainFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	swapchainFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
	/*swapchainFlags |= D3D11_CREATE_DEVICE_DISABLE_GPU_TIMEOUT;*/
#ifdef  _DEBUG
	swapchainFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_0 };
	HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, swapchainFlags, NULL, 0, D3D11_SDK_VERSION, m_device.GetAddressOf(), featureLevel, m_context.GetAddressOf());
	assert(SUCCEEDED(hr));
	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferDesc.Width = 0;
	scd.BufferDesc.Height = 0;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 2;
	scd.OutputWindow = hwnd;
	scd.Windowed = true;
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	IDXGIOutput* output = nullptr;
	hr = (m_device.Get())->QueryInterface(m_idxgiDevice.GetAddressOf());
	assert(SUCCEEDED(hr));
	hr = (m_idxgiDevice.Get())->GetAdapter(m_adapter0.GetAddressOf());
	assert(SUCCEEDED(hr));
	hr = (m_adapter0.Get())->GetParent(__uuidof(IDXGIFactory), (void**)m_factory.GetAddressOf());
	assert(SUCCEEDED(hr));
	hr = (m_factory.Get())->CreateSwapChain(m_device.Get(), &scd, m_swapchain.GetAddressOf());
	assert(SUCCEEDED(hr));
	m_device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(m_debug.GetAddressOf()));
	hr = (m_swapchain.Get())->GetContainingOutput(&output);
	assert(SUCCEEDED(hr));
	hr = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &m_numberOfSupportedModes, NULL);
	assert(SUCCEEDED(hr));
	m_supportedModes = new DXGI_MODE_DESC[m_numberOfSupportedModes];
	ZeroMemory(m_supportedModes, sizeof(DXGI_MODE_DESC) * m_numberOfSupportedModes);
	hr = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &m_numberOfSupportedModes, m_supportedModes);
	assert(SUCCEEDED(hr));
	output->Release();
	bool supportedMode = false;
	for (unsigned int i = 0; i < m_numberOfSupportedModes; i++)
	{
		if ((UINT)m_currResX == m_supportedModes[i].Width && (UINT)m_currResY == m_supportedModes[i].Height)
		{
			supportedMode = true;
			m_modeDesc = m_supportedModes[i];
			m_currentModeIndex = i;
			break;
		}
	}
	if (!supportedMode)
	{
		m_currentModeIndex = 0;
		m_modeDesc = m_supportedModes[0];
	}
	ID3D11Texture2D* backBufferPtr = nullptr;
	HRESULT getBackbufferResult = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	assert(SUCCEEDED(getBackbufferResult));
	m_device->CreateRenderTargetView(backBufferPtr, nullptr, m_mainRTV.GetAddressOf());
	D3D11_TEXTURE2D_DESC bbTexDesc;
	backBufferPtr->GetDesc(&bbTexDesc);

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDsc = {};
	depthStencilStateDsc.DepthEnable = true;
	depthStencilStateDsc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDsc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilStateDsc.StencilEnable = false;
	depthStencilStateDsc.StencilReadMask = 0xFF;
	depthStencilStateDsc.StencilWriteMask = 0xFF;
	depthStencilStateDsc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDsc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDsc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilStateDsc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilStateDsc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDsc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDsc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilStateDsc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	HRESULT createDepthStencilResult = m_device->CreateDepthStencilState(&depthStencilStateDsc, m_mainDSS.GetAddressOf());
	assert(SUCCEEDED(createDepthStencilResult));
	m_context->OMSetDepthStencilState(m_mainDSS.Get(), 1);
	depthStencilStateDsc.DepthEnable = false;
	depthStencilStateDsc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	hr = m_device->CreateDepthStencilState(&depthStencilStateDsc, m_depthDisableDSS.GetAddressOf());
	assert(SUCCEEDED(hr));
	
	CD3D11_TEXTURE2D_DESC textureDesc = {};
	ID3D11Texture2D* textureD = nullptr;
	textureDesc.Width = (UINT)bbTexDesc.Width;
	textureDesc.Height = (UINT)bbTexDesc.Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	RELEASE_COM(backBufferPtr);
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = textureDesc.Format;
	dsvDesc.Flags = 0;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = m_device->CreateTexture2D(&textureDesc, NULL, &textureD);
	assert(SUCCEEDED(hr));
	hr = m_device->CreateDepthStencilView(textureD, &dsvDesc, m_mainDSV.GetAddressOf());
	assert(SUCCEEDED(hr));
	RELEASE_COM(textureD);
	D3D11_RASTERIZER_DESC rasterizerDescription = {};
	rasterizerDescription.CullMode = D3D11_CULL_BACK;
	rasterizerDescription.FillMode = D3D11_FILL_SOLID;
	rasterizerDescription.DepthClipEnable = true;
	rasterizerDescription.FrontCounterClockwise = true;
	hr = m_device->CreateRasterizerState(&rasterizerDescription, m_mainRS.GetAddressOf());
	assert(SUCCEEDED(hr));
	rasterizerDescription.CullMode = D3D11_CULL_NONE;
	hr = m_device->CreateRasterizerState(&rasterizerDescription, m_cullNoneRS.GetAddressOf());
	assert(SUCCEEDED(hr));
	rasterizerDescription.CullMode = D3D11_CULL_BACK;
	rasterizerDescription.FrontCounterClockwise = false;
	hr = m_device->CreateRasterizerState(&rasterizerDescription, m_clockWiseRS.GetAddressOf());
	assert(SUCCEEDED(hr));
	rasterizerDescription.FrontCounterClockwise = true;
	rasterizerDescription.FillMode = D3D11_FILL_WIREFRAME;
	hr = m_device->CreateRasterizerState(&rasterizerDescription, m_wireframeRS.GetAddressOf());
	assert(SUCCEEDED(hr));
	rasterizerDescription.CullMode = D3D11_CULL_BACK;
	rasterizerDescription.FillMode = D3D11_FILL_SOLID;
	rasterizerDescription.DepthClipEnable = true;
	rasterizerDescription.FrontCounterClockwise = true;
	rasterizerDescription.DepthBias = 112500;
	rasterizerDescription.DepthBiasClamp = 0.0f;
	rasterizerDescription.SlopeScaledDepthBias = 1.0f;
	hr = m_device->CreateRasterizerState(&rasterizerDescription, m_shadowRS.GetAddressOf());
	assert(SUCCEEDED(hr));
	D3D11_BLEND_DESC blendDescOn;
	ZeroMemory(&blendDescOn, sizeof(D3D11_BLEND_DESC));
	blendDescOn.AlphaToCoverageEnable = TRUE;
	blendDescOn.IndependentBlendEnable = TRUE;
	blendDescOn.RenderTarget[0].BlendEnable = TRUE;
	blendDescOn.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDescOn.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDescOn.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDescOn.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDescOn.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDescOn.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDescOn.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	D3D11_BLEND_DESC blendDescOff;
	ZeroMemory(&blendDescOff, sizeof(D3D11_BLEND_DESC));
	blendDescOff.RenderTarget[0].BlendEnable = false;
	blendDescOff.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = m_device->CreateBlendState(&blendDescOn, m_blendOn.GetAddressOf());
	assert(SUCCEEDED(hr));
	hr = m_device->CreateBlendState(&blendDescOff, m_blendOff.GetAddressOf());
	assert(SUCCEEDED(hr));
	blendDescOn.AlphaToCoverageEnable = true;
	blendDescOn.IndependentBlendEnable = false;	 // can be true 
	blendDescOn.RenderTarget[0].BlendEnable = true;
	blendDescOn.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDescOn.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDescOn.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDescOn.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDescOn.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDescOn.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDescOn.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	hr = m_device->CreateBlendState(&blendDescOn, m_blendParticle.GetAddressOf());
	assert(SUCCEEDED(hr));
#if USE_IMGUI

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(m_device.Get(), m_context.Get());
	ImGui::StyleColorsDark();

#endif



#if _DEBUG
	WRL::ComPtr<ID3D11InfoQueue> d3dInfoQueue;
	hr = m_debug.As(&d3dInfoQueue);
	if (SUCCEEDED(hr))
	{
		D3D11_MESSAGE_ID hide[] =
		{

			D3D11_MESSAGE_ID::D3D11_MESSAGE_ID_DEVICE_DRAW_RENDERTARGETVIEW_NOT_SET

		};
		D3D11_INFO_QUEUE_FILTER filter;
		memset(&filter, 0, sizeof(filter));
		filter.DenyList.NumIDs = _countof(hide);
		filter.DenyList.pIDList = hide;
		d3dInfoQueue->AddStorageFilterEntries(&filter);
	}
#endif
	HRESULT ret_code = ::CreateDXGIFactory(
		__uuidof(IDXGIFactory),
		reinterpret_cast<void**>(m_factory1.GetAddressOf()));
	if (SUCCEEDED(ret_code))
	{

		if (SUCCEEDED(m_factory1->EnumAdapters(0, m_adapter1.GetAddressOf())))
		{
			if (SUCCEEDED(m_adapter1->QueryInterface(__uuidof(IDXGIAdapter4), (void**)m_adapterFour.GetAddressOf())))
			{
				DXGI_QUERY_VIDEO_MEMORY_INFO info;

				if (SUCCEEDED(m_adapterFour->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &info)))
				{
					m_vRAM = float(info.CurrentUsage / 1024.0 / 1024.0); //MB

				}


			}

		}

	}
	DWORD currentProcessID = GetCurrentProcessId();

	m_hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, currentProcessID);

	PROCESS_MEMORY_COUNTERS pmc{};

	if (GetProcessMemoryInfo(m_hProcess, &pmc, sizeof(pmc)))
	{

		m_ram = float(pmc.PagefileUsage / 1024.0 / 1024.0); //MB
	}
#if START_FULLSCREEN
	resize(true);
	g_pastInFullscreen = 1;
#else
	g_pastInFullscreen = 0;
#endif
	ShaderHandler::getInstance().init(m_device.Get());
	TextureHandler::getInstance().init(m_device.Get());
	GuiHandler::getInstance().init(m_currResX, m_currResY, m_context.Get(), m_device.Get());
	InputHandler::getInstance().init(hwnd);
	RenderPassHandler::getInstance().init(m_device.Get(), m_currResX, m_currResY, scene);
	
	m_updateDraw.setPointers(m_blendOn.Get(), m_blendOff.Get(), m_blendParticle.Get(), m_shadowRS.Get(),
		m_mainRS.Get(), m_cullNoneRS.Get(), m_clockWiseRS.Get(), m_wireframeRS.Get(), m_depthDisableDSS.Get(),
		m_mainDSS.Get(), m_mainDSV.Get(), m_mainRTV.Get());
	m_updateDraw.initBfrs(m_device.Get(), m_currResX, m_currResY, scene, true);
	
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = (float)m_currResX;
	m_viewport.Height = (float)m_currResY;
	m_viewport.MaxDepth = 1;
	m_viewport.MinDepth = 0;
	
	
	m_boneMap.insert({ (int)PLAYER_SKELETON::ball_l ,"ball_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::ball_r ,"ball_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::thumb_03_r ,"thumb_03_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::thumb_02_r ,"thumb_02_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::thumb_01_r ,"thumb_01_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::index_03_r ,"index_03_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::index_02_r ,"index_02_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::index_01_r ,"index_01_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::middle_01_r ,"middle_01_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::middle_03_r ,"middle_03_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::middle_02_r ,"middle_02_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::ring_03_r ,"ring_03_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::ring_02_r ,"ring_02_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::ring_01_r ,"ring_01_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::pinky_03_r ,"pinky_03_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::pinky_02_r ,"pinky_02_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::pinky_01_r ,"pinky_01_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::thumb_03_l ,"thumb_03_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::thumb_02_l ,"thumb_02_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::thumb_01_l ,"thumb_01_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::index_03_l ,"index_03_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::index_02_l ,"index_02_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::index_01_l ,"index_01_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::middle_03_l ,"middle_03_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::middle_02_l ,"middle_02_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::middle_01_l ,"middle_01_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::ring_03_l ,"ring_03_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::ring_02_l ,"ring_02_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::ring_01_l ,"ring_01_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::pinky_03_l ,"pinky_03_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::pinky_02_l ,"pinky_02_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::pinky_01_l ,"pinky_01_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::neck_02 ,"neck_02" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::calf_l ,"calf_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::foot_l ,"foot_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::hand_r ,"hand_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::foot_r ,"foot_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::calf_r ,"calf_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::neck_01 ,"neck_01" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::clavicle_l ,"clavicle_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::thigh_l ,"thigh_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::thigh_r ,"thigh_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::pelvis ,"pelvis" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::spine_02 ,"spine_02" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::spine_03 ,"spine_03" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::spine_01 ,"spine_01" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::lowerarm_l ,"lowerarm_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::hand_l ,"hand_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::upperarm_l ,"upperarm_l" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::upperarm_r ,"upperarm_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::lowerarm_r ,"lowerarm_r" });
	m_boneMap.insert({ (int)PLAYER_SKELETON::clavicle_r ,"clavicle_r" });
	m_timer.start();
	g_isInitiated = true;
}

Renderer::~Renderer()
{
	RELEASE_ARR(m_supportedModes);
	CloseHandle(m_hProcess);
#if USE_IMGUI
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
#endif
	RELEASE(m_combinedTrackCloth);
	RELEASE(m_combinedTrackPlayer);
}

void Renderer::present(bool vsync)
{
	if(vsync)
	{
		HRESULT hr = m_swapchain->Present(1, 0);
		assert(SUCCEEDED(hr));
		
		
	}
	else
	{
		HRESULT hr = m_swapchain->Present(0, 0);
		assert(SUCCEEDED(hr));
	}
}





void Renderer::resize(const HWND& hwnd)
{
	HRESULT hr;
	DXGI_MODE_DESC zeroRefreshRate = {};
	zeroRefreshRate = m_modeDesc;
	zeroRefreshRate.RefreshRate.Numerator = 0;
	zeroRefreshRate.RefreshRate.Denominator = 0;
	hr = m_swapchain->GetFullscreenState(&g_currentlyInFullscreen, nullptr);
	assert(SUCCEEDED(hr));
	if (g_currentlyInFullscreen != g_pastInFullscreen)
	{
		
		if (g_currentlyInFullscreen)
		{
			hr = m_swapchain->ResizeTarget(&zeroRefreshRate);
			assert(SUCCEEDED(hr));
			hr = m_swapchain->SetFullscreenState(true, nullptr);
			assert(SUCCEEDED(hr));
		}
		else
		{
			hr = m_swapchain->SetFullscreenState(false, nullptr);
			assert(SUCCEEDED(hr));

			RECT rect = { 0, 0, (long)m_modeDesc.Width,  (long)m_modeDesc.Height };
			AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);
			SetWindowPos(hwnd, HWND_TOP, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE);

		}
		g_pastInFullscreen = !g_pastInFullscreen;
	}
	hr = m_swapchain->ResizeTarget(&zeroRefreshRate);
	assert(SUCCEEDED(hr));
	UINT swapchainFlags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	m_context->ClearState();
	m_mainRTV = nullptr;
	m_mainDSV = nullptr;
	hr = m_swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, swapchainFlags);
	assert(SUCCEEDED(hr));
	ID3D11Texture2D* backBufferPtr = nullptr;
	HRESULT getBackbufferResult = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	assert(SUCCEEDED(getBackbufferResult));
	m_device->CreateRenderTargetView(backBufferPtr, nullptr, m_mainRTV.GetAddressOf());
	D3D11_TEXTURE2D_DESC bbTexDesc = {};
	backBufferPtr->GetDesc(&bbTexDesc);
	
	D3D11_TEXTURE2D_DESC textureDesc = {};
	ID3D11Texture2D* textureD = nullptr;
	textureDesc.Width = (UINT)bbTexDesc.Width;
	textureDesc.Height = (UINT)bbTexDesc.Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = textureDesc.Format;
	dsvDesc.Flags = 0;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = (float)m_currResX;
	m_viewport.Height = (float)m_currResY;
	m_viewport.MaxDepth = 1;
	m_viewport.MinDepth = 0;
	RELEASE_COM(backBufferPtr);
	hr = m_device->CreateTexture2D(&textureDesc, NULL, &textureD);
	assert(SUCCEEDED(hr));
	hr = m_device->CreateDepthStencilView(textureD, &dsvDesc, m_mainDSV.GetAddressOf());
	assert(SUCCEEDED(hr));
	RELEASE_COM(textureD);
	m_context->OMSetRenderTargets(1, m_mainRTV.GetAddressOf(), m_mainDSV.Get());
	m_context->RSSetViewports(1, &m_viewport);

	m_updateDraw.setPointers(m_blendOn.Get(), m_blendOff.Get(), m_blendParticle.Get(), m_shadowRS.Get(),
		m_mainRS.Get(), m_cullNoneRS.Get(), m_clockWiseRS.Get(), m_wireframeRS.Get(), m_depthDisableDSS.Get(),
		m_mainDSS.Get(), m_mainDSV.Get(), m_mainRTV.Get());

	g_pause = false;
	
	
}

void Renderer::changeRes(size_t newResX, size_t newResY, const HWND& hwnd, const SCENE& scene)
{
	
	bool supportedMode = false;
	for (unsigned int i = 0; i < m_numberOfSupportedModes; i++)
	{
		if ((UINT)newResX == m_supportedModes[i].Width && (UINT)newResY == m_supportedModes[i].Height)
		{
			
			supportedMode = true;
			m_modeDesc = m_supportedModes[i];
			m_currentModeIndex = i;
			break;
		}
	}
	if (!supportedMode)
	{
		if (m_currResX > newResX)
		{
			if (m_currentModeIndex < (int)m_numberOfSupportedModes - 1)
			{
				
				m_currentModeIndex++;
			}
		}

		if (m_currResX < newResX)
		{

			if (m_currentModeIndex > 0)
			{
				
				m_currentModeIndex--;
			}
		}

		m_modeDesc = m_supportedModes[m_currentModeIndex];
	}
	m_currResX = newResX;
	m_currResY = newResY;
	resize(hwnd);
	//will need to update other stuff as well.
	GuiHandler::getInstance().changeRes(newResX, newResY, m_device.Get(), m_context.Get());
	InputHandler::getInstance().changeRes(newResX, newResY,hwnd);
	RenderPassHandler::getInstance().reCreate(m_device.Get(), newResX, newResY, scene);
	
	m_updateDraw.changeRes(m_currResX, m_currResY, m_device.Get());
	setRenderPassShapes();
	

	
}

void Renderer::restart()
{
	m_updateDraw.restart();
	m_player = nullptr;
	m_playerCloth = nullptr;
}

void Renderer::setLights(const std::vector<Light*>& lights, Light* dirLight)
{
	m_updateDraw.setLights(lights, m_device.Get(), dirLight);
	m_dirLight = dirLight;

}
void Renderer::combineTracks(std::string point, std::string trackA, std::string trackB)
{
	DC_SKELETON* dcP = dynamic_cast<DC_SKELETON*>(m_player->getDrawCall());
	DC_SKELETON* dcC = dynamic_cast<DC_SKELETON*>(m_playerCloth->getDrawCall());
	Skeleton* t1P = dynamic_cast<Skeleton*>(dcP->g_animations[trackA]);
	Skeleton* t2P = dynamic_cast<Skeleton*>(dcP->g_animations[trackB]);

	Skeleton* t1C = dynamic_cast<Skeleton*>(dcC->g_animations[trackA]);
	Skeleton* t2C = dynamic_cast<Skeleton*>(dcC->g_animations[trackB]);
	RELEASE(m_combinedTrackPlayer);
	RELEASE(m_combinedTrackCloth);
	dcP->g_animations.erase({ G_USER_SELECTED_COMBINED});
	dcC->g_animations.erase({ G_USER_SELECTED_COMBINED});
	m_combinedTrackPlayer = t1P->combineTracks(t2P, point, G_USER_SELECTED_COMBINED);
	m_combinedTrackCloth = t1C->combineTracks(t2C, point, G_USER_SELECTED_COMBINED);
	dcP->g_animations.insert({ G_USER_SELECTED_COMBINED , m_combinedTrackPlayer });
	dcC->g_animations.insert({ G_USER_SELECTED_COMBINED , m_combinedTrackCloth });
}
void Renderer::updateTransform()
{
	m_timer.run();
	m_updateDraw.updateTransform(m_context.Get());

	Vector3 forward = XMFLOAT3(0, 0, 1);
	Matrix rotMat = XMMatrixRotationRollPitchYaw(m_roll, m_pitch, m_yaw);
	forward = XMVector3TransformNormal(forward, rotMat);
	if (m_dirLight)
	{
		m_dirLight->setDirection({ forward.x,forward.y,forward.z,0 });
	}
	m_updateDraw.updateIMGUI(m_fireSize, m_smokeSize, m_leafSize, m_textSize,m_mouseSensitivity, m_runSpeed,m_walkSpeed,m_turnSpeed, m_renderBoundingBoxes, m_renderBoundingBoxesWireFrame);

	if (m_combineTrack)
	{
		bool combine = true;
		std::string a;
		std::string b;
		
		switch (m_trackSelectionA)
		{
		default:
		{
			combine = false;
			break;
		}
		case 0:
		{
			a = G_S_WALK;
			break;
		}
		case 1:
		{
			a = G_S_ATTACK;
			break;
		}
		case 2:
		{
			a = G_S_RUN;
			break;
		}
		}
		switch (m_trackSelectionB)
		{
		default:
		{
			combine = false;
			break;
		}
		case 0:
		{
			b = G_S_WALK;
			break;
		}
		case 1:
		{
			b = G_S_ATTACK;
			break;
		}
		case 2:
		{
			b = G_S_RUN;
			break;
		}
		}
		if (a == b)
		{
			combine = false;
		}
		if (combine)
		{
			std::string bone = m_boneMap[m_e];
			combineTracks(bone, a, b);
		}
		
		
	}
	
	if (m_player && m_playerCloth)
	{
		DC_SKELETON* dcP = dynamic_cast<DC_SKELETON*>(m_player->getDrawCall());
		DC_SKELETON* dcC = dynamic_cast<DC_SKELETON*>(m_playerCloth->getDrawCall());
		
		float dt = (float)m_timer.getDeltaTime();
		if (m_playBlendOnce)
		{
			std::string a;
			std::string b;
			bool blend = true;
			switch (m_trackSelectionA)
			{
			default:
			{
				blend = false;
				break;
			}
			case 0:
			{
				a = G_S_WALK;
				break;
			}
			case 1:
			{
				a = G_S_ATTACK;
				break;
			}
			case 2:
			{
				a = G_S_RUN;
				break;
			}
			}
			switch (m_trackSelectionB)
			{
			default:
			{
				blend = false;
				break;
			}
			case 0:
			{
				b = G_S_WALK;
				break;
			}
			case 1:
			{
				b = G_S_ATTACK;
				break;
			}
			case 2:
			{
				b = G_S_RUN;
				break;
			}
			}
			if (b == a)
			{
				blend = false;
			}
			if (blend)
			{


				Skeleton* playerSA = dynamic_cast<Skeleton*>(dcP->g_animations.at(a));
				Skeleton* clothSA = dynamic_cast<Skeleton*>(dcC->g_animations.at(a));
				Skeleton* playerSB = dynamic_cast<Skeleton*>(dcP->g_animations.at(b));
				Skeleton* clothSB = dynamic_cast<Skeleton*>(dcC->g_animations.at(b));
				playerSA->play(playerSB, dt, m_lerpT, true, m_timeScale);

				clothSA->play(clothSB, dt, m_lerpT, true, m_timeScale);

				m_playBlendOnce = playerSA->g_done ? false : true;
				dcC->g_currentTrack = a;
				dcP->g_currentTrack = a;
			}
		}
		else if (m_playBlendContineously)
		{
			std::string a;
			std::string b;
			bool blend = true;
			switch (m_trackSelectionA)
			{
			default:
			{
				blend = false;
				break;
			}
			case 0:
			{
				a = G_S_WALK;
				break;
			}
			case 1:
			{
				a = G_S_ATTACK;
				break;
			}
			case 2:
			{
				a = G_S_RUN;
				break;
			}
			}
			switch (m_trackSelectionB)
			{
			default:
			{
				blend = false;
				break;
			}
			case 0:
			{
				b = G_S_WALK;
				break;
			}
			case 1:
			{
				b = G_S_ATTACK;
				break;
			}
			case 2:
			{
				b = G_S_RUN;
				break;
			}
			}
			if (b == a)
			{
				blend = false;
			}
			if (blend)
			{
				
				Skeleton* playerSA = dynamic_cast<Skeleton*>(dcP->g_animations.at(a));
				Skeleton* clothSA = dynamic_cast<Skeleton*>(dcC->g_animations.at(a));
				Skeleton* playerSB = dynamic_cast<Skeleton*>(dcP->g_animations.at(b));
				Skeleton* clothSB = dynamic_cast<Skeleton*>(dcC->g_animations.at(b));
				playerSA->play(playerSB, dt, m_lerpT, false, m_timeScale);
				clothSA->play(clothSB, dt, m_lerpT, false, m_timeScale);
				dcC->g_currentTrack = a;
				dcP->g_currentTrack = a;
			}
		}
		else if (m_playOnce)
		{
			
			switch (m_trackSelectionA)
			{
			default:
			{
				
				break;
			}
			case 0:
			{
			
				dcP->g_animations.at(G_S_WALK)->play(dt,true, m_timeScale);
				dcC->g_animations.at(G_S_WALK)->play(dt,true, m_timeScale);
				dcC->g_currentTrack = G_S_WALK;
				dcP->g_currentTrack = G_S_WALK;
				m_playOnce = dcP->g_animations.at(G_S_WALK)->g_done ? false : true;
				
				break;
			}
			case 1:
			{
				
				dcP->g_animations.at(G_S_RUN)->play(dt,true, m_timeScale);
				dcC->g_animations.at(G_S_RUN)->play(dt,true, m_timeScale);
				dcC->g_currentTrack = G_S_RUN;
				dcP->g_currentTrack = G_S_RUN;
				m_playOnce = dcP->g_animations.at(G_S_RUN)->g_done ? false : true;
				
				break;
			}
			case 2:
			{
				
				dcP->g_animations.at(G_S_ATTACK)->play(dt,true, m_timeScale);
				dcC->g_animations.at(G_S_ATTACK)->play(dt,true, m_timeScale);
				dcC->g_currentTrack = G_S_ATTACK;
				dcP->g_currentTrack = G_S_ATTACK;
				m_playOnce = dcP->g_animations.at(G_S_ATTACK)->g_done ? false : true;
				
				break;
			}
			case 3:
			{
				if (m_combinedTrackPlayer&&m_combinedTrackCloth)
				{
					
					dcP->g_animations.at(G_USER_SELECTED_COMBINED)->play(dt,true, m_timeScale);
					dcC->g_animations.at(G_USER_SELECTED_COMBINED)->play(dt,true, m_timeScale);
					dcC->g_currentTrack = G_USER_SELECTED_COMBINED;
					dcP->g_currentTrack = G_USER_SELECTED_COMBINED;
					m_playOnce = dcP->g_animations.at(G_USER_SELECTED_COMBINED)->g_done ? false : true;
					
				}
				
				break;
			}
			}
			
		}
		else if (m_playContinueously)
		{
			switch (m_trackSelectionA)
			{
			default:
			{

				break;
			}
			case 0:
			{
				dcP->g_animations.at(G_S_WALK)->play(dt,false, m_timeScale);
				dcC->g_animations.at(G_S_WALK)->play(dt,false, m_timeScale);
				dcC->g_currentTrack = G_S_WALK;
				dcP->g_currentTrack = G_S_WALK;
				break;
			}
			case 1:
			{
				dcP->g_animations.at(G_S_RUN)->play(dt,false, m_timeScale);
				dcC->g_animations.at(G_S_RUN)->play(dt,false, m_timeScale);
				dcC->g_currentTrack = G_S_RUN;
				dcP->g_currentTrack = G_S_RUN;
				break;
			}
			case 2:
			{
				dcP->g_animations.at(G_S_ATTACK)->play(dt,false, m_timeScale);
				dcC->g_animations.at(G_S_ATTACK)->play(dt,false, m_timeScale);
				dcC->g_currentTrack = G_S_ATTACK;
				dcP->g_currentTrack = G_S_ATTACK;
				break;
			}
			case 3:
			{
				if (m_combinedTrackPlayer && m_combinedTrackCloth)
				{
					dcP->g_animations.at(G_USER_SELECTED_COMBINED)->play(dt,false, m_timeScale);
					dcC->g_animations.at(G_USER_SELECTED_COMBINED)->play(dt,false, m_timeScale);
					dcC->g_currentTrack = G_USER_SELECTED_COMBINED;
					dcP->g_currentTrack = G_USER_SELECTED_COMBINED;
				}
				
				break;
			}
			}
		}
		
	}


}
#if USE_IMGUI
void Renderer::renderIMGUI(const SCENE& scene)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Window");
	
	if (SUCCEEDED(m_adapterFour->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &m_vRamInfo)))
	{
		m_vRAM = float(m_vRamInfo.CurrentUsage / 1024.0 / 1024.0); //MB

	}
	PROCESS_MEMORY_COUNTERS pmc{};
	if (GetProcessMemoryInfo(m_hProcess, &pmc, sizeof(pmc)))
	{

		m_ram = float(pmc.PagefileUsage / 1024.0 / 1024.0); //MB
	}
	
	float camP[4] = { m_updateDraw.getCamera()->getTransAsV4().x,m_updateDraw.getCamera()->getTransAsV4().y
	,m_updateDraw.getCamera()->getTransAsV4().z,m_updateDraw.getCamera()->getTransAsV4().w };
	ImGui::Text("SPACE TO RUN, ESCAPE FOR MENU AND E TO ACCESS IMGUI\nTOGGLE FULLSCREEN WITH ALT+ENTER");
	
	ImGui::InputFloat("VRAM", &m_vRAM);
	ImGui::InputFloat("RAM", &m_ram);
	ImGui::InputFloat4("CamPos", camP);
	if (scene != SCENE::HEAT)
	{
		ImGui::SliderFloat("SunRoll", &m_roll, 0, 360);
		ImGui::SliderFloat("SunPitch", &m_pitch, 0, 360);
	}
	else
	{
		ImGui::Text("The colors represent amount of lights affecting each group of pixels.");
	}
	if (scene == SCENE::PARTICLE)
	{
		ImGui::SliderInt("fireSize", &m_fireSize, 5, 300);
		ImGui::SliderInt("smokeSize", &m_smokeSize, 5, 300);
		ImGui::SliderInt("leafSize", &m_leafSize, 5, 300);
		ImGui::SliderInt("textSize", &m_textSize, 5, 300);
	}
	if (scene == SCENE::LOD)
	{
		m_updateDraw.updateIMGUI(m_trianglesDrawn, m_currInstanceCount);
		ImGui::Text(("Current Oc Tree Triangles Drawn: " + std::to_string(m_trianglesDrawn) + "\nCurrent Instances Drawn: " + std::to_string(m_currInstanceCount)).c_str());

		ImGui::Checkbox("BoundingBoxes", &m_renderBoundingBoxes);
		ImGui::Checkbox("BoundingBoxesWireFrame", &m_renderBoundingBoxesWireFrame);
	}
	else if (scene == SCENE::ANIMATION)
	{
		ImGui::Text("TrackA");
		ImGui::RadioButton("walkA", &m_trackSelectionA, 0);
		ImGui::RadioButton("runA", &m_trackSelectionA, 1); 
		ImGui::RadioButton("fireballA", &m_trackSelectionA, 2);
		ImGui::RadioButton("combinedA", &m_trackSelectionA, 3);
		
		ImGui::Text("TrackB");
		ImGui::RadioButton("walkB", &m_trackSelectionB, 0);
		ImGui::RadioButton("runB", &m_trackSelectionB, 1); 
		ImGui::RadioButton("fireballB", &m_trackSelectionB, 2);
		ImGui::RadioButton("combinedB", &m_trackSelectionB, 3);

	


		ImGui::SliderFloat("speedScale", &m_timeScale, 0.1f, 10.0f);
		ImGui::SliderFloat("interpolationValue", &m_lerpT, 0.0f, 1.0f);

		m_combineTrack = ImGui::Button("Combine Tracks");

		ImGui::Checkbox("Play TrackA Once", &m_playOnce);
		
		ImGui::Checkbox("Play TrackA Continuously", &m_playContinueously);

		ImGui::Checkbox("Play Lerped Track Once",&m_playBlendOnce);

		ImGui::Checkbox("Play Lerped Track Continuously", &m_playBlendContineously);

		ImGui::Text("Select Breaking Point for combined track:");
		ImGui::RadioButton("pelvis", &m_e, (int)PLAYER_SKELETON::pelvis); 
		ImGui::RadioButton("spine_01", &m_e, (int)PLAYER_SKELETON::spine_01);
		ImGui::RadioButton("spine_02", &m_e, (int)PLAYER_SKELETON::spine_02);
		ImGui::RadioButton("spine_03", &m_e, (int)PLAYER_SKELETON::spine_03);
		ImGui::RadioButton("neck_01", &m_e, (int)PLAYER_SKELETON::neck_01);
		ImGui::RadioButton("neck_02", &m_e, (int)PLAYER_SKELETON::neck_02);
		ImGui::RadioButton("clavicle_l", &m_e, (int)PLAYER_SKELETON::clavicle_l);
		ImGui::RadioButton("upperarm_l", &m_e, (int)PLAYER_SKELETON::upperarm_l);
		ImGui::RadioButton("lowerarm_l", &m_e, (int)PLAYER_SKELETON::lowerarm_l);
		ImGui::RadioButton("hand_l", &m_e, (int)PLAYER_SKELETON::hand_l);
		ImGui::RadioButton("pinky_01_l", &m_e, (int)PLAYER_SKELETON::pinky_01_l);
		ImGui::RadioButton("pinky_02_l", &m_e, (int)PLAYER_SKELETON::pinky_02_l);
		ImGui::RadioButton("pinky_03_l", &m_e, (int)PLAYER_SKELETON::pinky_03_l);
		ImGui::RadioButton("ring_01_l", &m_e, (int)PLAYER_SKELETON::ring_01_l);
		ImGui::RadioButton("ring_02_l", &m_e, (int)PLAYER_SKELETON::ring_02_l);
		ImGui::RadioButton("ring_03_l", &m_e, (int)PLAYER_SKELETON::ring_03_l);
		ImGui::RadioButton("middle_01_l", &m_e, (int)PLAYER_SKELETON::middle_01_l);
		ImGui::RadioButton("middle_02_l", &m_e, (int)PLAYER_SKELETON::middle_02_l);
		ImGui::RadioButton("middle_03_l", &m_e, (int)PLAYER_SKELETON::middle_03_l);
		ImGui::RadioButton("index_01_l", &m_e, (int)PLAYER_SKELETON::index_01_l);
		ImGui::RadioButton("index_02_l", &m_e, (int)PLAYER_SKELETON::index_02_l);
		ImGui::RadioButton("index_03_l", &m_e, (int)PLAYER_SKELETON::index_03_l);
		ImGui::RadioButton("thumb_01_l", &m_e, (int)PLAYER_SKELETON::thumb_01_l);
		ImGui::RadioButton("thumb_02_l", &m_e, (int)PLAYER_SKELETON::thumb_02_l);
		ImGui::RadioButton("thumb_03_l", &m_e, (int)PLAYER_SKELETON::thumb_03_l);
		ImGui::RadioButton("clavicle_r", &m_e, (int)PLAYER_SKELETON::clavicle_r);
		ImGui::RadioButton("upperarm_r", &m_e, (int)PLAYER_SKELETON::upperarm_r);
		ImGui::RadioButton("lowerarm_r", &m_e, (int)PLAYER_SKELETON::lowerarm_r);
		ImGui::RadioButton("hand_r", &m_e, (int)PLAYER_SKELETON::hand_r);
		ImGui::RadioButton("pinky_01_r", &m_e, (int)PLAYER_SKELETON::pinky_01_r);
		ImGui::RadioButton("pinky_02_r", &m_e, (int)PLAYER_SKELETON::pinky_02_r);
		ImGui::RadioButton("pinky_03_r", &m_e, (int)PLAYER_SKELETON::pinky_03_r);
		ImGui::RadioButton("ring_01_r", &m_e, (int)PLAYER_SKELETON::ring_01_r);
		ImGui::RadioButton("ring_02_r", &m_e, (int)PLAYER_SKELETON::ring_02_r);
		ImGui::RadioButton("ring_03_r", &m_e, (int)PLAYER_SKELETON::ring_03_r);
		ImGui::RadioButton("middle_02_r", &m_e, (int)PLAYER_SKELETON::middle_02_r);
		ImGui::RadioButton("middle_03_r", &m_e, (int)PLAYER_SKELETON::middle_03_r);
		ImGui::RadioButton("middle_01_r", &m_e, (int)PLAYER_SKELETON::middle_01_r);
		ImGui::RadioButton("index_01_r", &m_e, (int)PLAYER_SKELETON::index_01_r);
		ImGui::RadioButton("index_02_r", &m_e, (int)PLAYER_SKELETON::index_02_r);
		ImGui::RadioButton("index_03_r", &m_e, (int)PLAYER_SKELETON::index_03_r);
		ImGui::RadioButton("thumb_01_r", &m_e, (int)PLAYER_SKELETON::thumb_01_r);
		ImGui::RadioButton("thumb_02_r", &m_e, (int)PLAYER_SKELETON::thumb_02_r);
		ImGui::RadioButton("thumb_03_r", &m_e, (int)PLAYER_SKELETON::thumb_03_r);
		ImGui::RadioButton("thigh_r", &m_e, (int)PLAYER_SKELETON::thigh_l);
		ImGui::RadioButton("calf_r", &m_e, (int)PLAYER_SKELETON::calf_r);
		ImGui::RadioButton("foot_r", &m_e, (int)PLAYER_SKELETON::foot_r);
		ImGui::RadioButton("ball_r", &m_e, (int)PLAYER_SKELETON::ball_r);
		ImGui::RadioButton("thigh_l", &m_e, (int)PLAYER_SKELETON::thigh_l);
		ImGui::RadioButton("calf_l", &m_e, (int)PLAYER_SKELETON::calf_l);
		ImGui::RadioButton("foot_l", &m_e, (int)PLAYER_SKELETON::foot_l);
		ImGui::RadioButton("ball_l", &m_e, (int)PLAYER_SKELETON::ball_l);

		
	
	}
	ImGui::SliderFloat("mouseSense", &m_mouseSensitivity, 0.001f, 1.0f);
	ImGui::SliderFloat("turnSpeed", &m_turnSpeed, 0.001f, 30.0f);
	ImGui::SliderFloat("walkSpeed", &m_walkSpeed, 0.001f, 30.0f);
	ImGui::SliderFloat("RunSpeed", &m_runSpeed, 0.001f, 30.0f);

	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}




#endif
void Renderer::render(const SCENE& scene)
{
	
	switch (scene)
	{
	default:
	{
		m_updateDraw.renderMain(m_context.Get(), m_viewport, scene);
		break;
	}
	case SCENE::HEAT:
	{
		m_updateDraw.renderDebugLight(m_context.Get(), m_viewport);
		break;
	}
	}
}

const MENU_OPTIONS& Renderer::renderMenu(const SCENE& scene)
{
	GuiHandler &g = GuiHandler::getInstance();
	InputHandler& i = InputHandler::getInstance();
	m_click = MENU_OPTIONS::NONE;

	m_updateDraw.renderBackground(m_context.Get(), m_viewport);
	g.drawEverything(i.GetMousePos());
	if (i.GetLeftMouseKeyDown())
	{
		m_click = g.selectionEverything(i.GetMousePos());

	}
	
	return m_click;
}