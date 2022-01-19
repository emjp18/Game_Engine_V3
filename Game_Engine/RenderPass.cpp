#include "stdafx.h"
#include "RenderPass.h"



RenderPass::RenderPass()
{
}

RenderPass::RenderPass(RENDERPASS type, size_t w, size_t h, ID3D11Device* device)
{

	m_type = type;
	switch (type)
	{
	default:
	{
		break;
	}
	case RENDERPASS::DEPTH_LIGHT_PASS:
	{
		g_w = SHADOW_MAP_SIZE_X;
		g_h = SHADOW_MAP_SIZE_Y;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		D3D11_TEXTURE2D_DESC textureDesc = {};
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};

		ID3D11Texture2D* depthTex2D = nullptr;
		textureDesc.Width = (UINT)w;
		textureDesc.Height = (UINT)h;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		HRESULT HR = device->CreateTexture2D(&textureDesc, NULL, &depthTex2D);
		assert(SUCCEEDED(HR));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		// Create the depth stencil view.
		HR = device->CreateDepthStencilView(depthTex2D, &depthStencilViewDesc, m_dsv.GetAddressOf());
		assert(SUCCEEDED(HR));

		shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;

		HR = device->CreateShaderResourceView(depthTex2D, &shaderResourceViewDesc, m_srv[0].GetAddressOf());
		assert(SUCCEEDED(HR));


		RELEASE_COM(depthTex2D);


		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;
		m_viewport.Width = (float)w;
		m_viewport.Height = (float)h;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;

		TextureHandler::getInstance().eraseSRV("depthLight");
		TextureHandler::getInstance().addTextureSRV("depthLight", m_srv[0]);
		break;
	}
	case RENDERPASS::CALC_FRUSTUM_PASS:
	{
		break;
	}
	case RENDERPASS::LIGHT_CULL_PASS:
	{
		break;
	}
	case RENDERPASS::DEPTH_SHADOW_DIR:
	{
		g_w = SHADOW_MAP_SIZE_X;
		g_h = SHADOW_MAP_SIZE_Y;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		D3D11_TEXTURE2D_DESC textureDesc = {};
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};

		ID3D11Texture2D* depthTex2D = nullptr;

		textureDesc.Width = (UINT)SHADOW_MAP_SIZE_X;
		textureDesc.Height = (UINT)SHADOW_MAP_SIZE_Y;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		HRESULT HR = device->CreateTexture2D(&textureDesc, NULL, &depthTex2D);
		assert(SUCCEEDED(HR));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		// Create the depth stencil view.
		HR = device->CreateDepthStencilView(depthTex2D, &depthStencilViewDesc, m_dsv.GetAddressOf());
		assert(SUCCEEDED(HR));

		shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;

		HR = device->CreateShaderResourceView(depthTex2D, &shaderResourceViewDesc, m_srv[0].GetAddressOf());
		assert(SUCCEEDED(HR));


		RELEASE_COM(depthTex2D);


		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;
		m_viewport.Width = (float)SHADOW_MAP_SIZE_X;
		m_viewport.Height = (float)SHADOW_MAP_SIZE_Y;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		TextureHandler::getInstance().eraseSRV("shadowDir");
		TextureHandler::getInstance().addTextureSRV("shadowDir", m_srv[0]);
		break;
	}
	case RENDERPASS::DEPTH_SHADOW_POINT:
	{
		g_w = SHADOW_MAP_SIZE_X;
		g_h = SHADOW_MAP_SIZE_Y;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		D3D11_TEXTURE2D_DESC textureDesc = {};
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};

		ID3D11Texture2D* depthTex2D = nullptr;
		textureDesc.Width = (UINT)SHADOW_MAP_SIZE_X;
		textureDesc.Height = (UINT)SHADOW_MAP_SIZE_Y;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 6;
		textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;


		HRESULT HR = device->CreateTexture2D(&textureDesc, NULL, &depthTex2D);
		assert(SUCCEEDED(HR));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		depthStencilViewDesc.Texture2DArray.MipSlice = 0;
		depthStencilViewDesc.Texture2DArray.FirstArraySlice = 0;
		depthStencilViewDesc.Texture2DArray.ArraySize = 6;
		depthStencilViewDesc.Flags = 0;
		// Create the depth stencil view.
		HR = device->CreateDepthStencilView(depthTex2D, &depthStencilViewDesc, m_dsv.GetAddressOf());
		assert(SUCCEEDED(HR));

		shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		shaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
		shaderResourceViewDesc.TextureCube.MipLevels = -1;

		HR = device->CreateShaderResourceView(depthTex2D, &shaderResourceViewDesc, m_srv[0].GetAddressOf());
		assert(SUCCEEDED(HR));

		TextureHandler::getInstance().eraseSRV("shadowPoint");
		TextureHandler::getInstance().addTextureSRV("shadowPoint", m_srv[0]);
		RELEASE_COM(depthTex2D);

		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;
		m_viewport.Width = (float)SHADOW_MAP_SIZE_X;
		m_viewport.Height = (float)SHADOW_MAP_SIZE_Y;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		break;
	}
	case RENDERPASS::DYNAMIC_CUBE_MAP_PASS:
	{
		g_w = DYNAMIC_CUBE_MAP_RES;
		g_h = DYNAMIC_CUBE_MAP_RES;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		D3D11_TEXTURE2D_DESC textureDesc = {};
		D3D11_TEXTURE2D_DESC textureDescDepth = {};
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		ID3D11Texture2D* tex2D = nullptr;
		ID3D11Texture2D* tex2DDepth = nullptr;
		textureDesc.Width = (UINT)DYNAMIC_CUBE_MAP_RES;
		textureDesc.Height = (UINT)DYNAMIC_CUBE_MAP_RES;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 6;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;


		HRESULT HR = device->CreateTexture2D(&textureDesc, NULL, &tex2D);
		assert(SUCCEEDED(HR));

	
		rtvDesc.Format = textureDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		rtvDesc.Texture2DArray.ArraySize =6;
		rtvDesc.Texture2DArray.MipSlice = 0;
		HR = device->CreateRenderTargetView(tex2D, &rtvDesc, m_rtv[0].GetAddressOf());
		assert(SUCCEEDED(HR));

		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		shaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
		shaderResourceViewDesc.TextureCube.MipLevels = -1;

		HR = device->CreateShaderResourceView(tex2D, &shaderResourceViewDesc, m_srv[0].GetAddressOf());
		assert(SUCCEEDED(HR));



		textureDescDepth.Width = (UINT)DYNAMIC_CUBE_MAP_RES;
		textureDescDepth.Height = (UINT)DYNAMIC_CUBE_MAP_RES;
		textureDescDepth.MipLevels = 1;
		textureDescDepth.ArraySize = 6;
		textureDescDepth.Format = DXGI_FORMAT_D32_FLOAT;
		textureDescDepth.SampleDesc.Count = 1;
		textureDescDepth.Usage = D3D11_USAGE_DEFAULT;
		textureDescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		textureDescDepth.CPUAccessFlags = 0;
		textureDescDepth.MiscFlags = 0;

		HR = device->CreateTexture2D(&textureDescDepth, NULL, &tex2DDepth);
		assert(SUCCEEDED(HR));

		depthStencilViewDesc.Format = textureDescDepth.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		depthStencilViewDesc.Texture2DArray.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Texture2DArray.FirstArraySlice = 0;
		depthStencilViewDesc.Texture2DArray.ArraySize = 6;
		
		HR = device->CreateDepthStencilView(tex2DDepth, &depthStencilViewDesc, m_dsv.GetAddressOf());
		assert(SUCCEEDED(HR));

		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;
		m_viewport.Width = (float)DYNAMIC_CUBE_MAP_RES;
		m_viewport.Height = (float)DYNAMIC_CUBE_MAP_RES;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;


		TextureHandler::getInstance().eraseSRV("dynamicCube");
		TextureHandler::getInstance().addTextureSRV("dynamicCube", m_srv[0]);
		RELEASE_COM(tex2D);
		RELEASE_COM(tex2DDepth);
		
		break;
	}
	case RENDERPASS::BASIC_PASS_SINGLE:
	{
		
		UINT width = (UINT)w;
		UINT height = (UINT)h;
		g_w = width;
		g_h = height;
		D3D11_TEXTURE2D_DESC blurredTexDesc;
		blurredTexDesc.Width = width;
		blurredTexDesc.Height = height;
		blurredTexDesc.MipLevels = 1;
		blurredTexDesc.ArraySize = 1;
		blurredTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		blurredTexDesc.SampleDesc.Count = 1;
		blurredTexDesc.SampleDesc.Quality = 0;
		blurredTexDesc.Usage = D3D11_USAGE_DEFAULT;
		blurredTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		blurredTexDesc.CPUAccessFlags = 0;
		blurredTexDesc.MiscFlags = 0;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		ID3D11Texture2D* blurredTex = 0;
		ID3D11Texture2D* blurredTex2 = 0;
		ID3D11Texture2D* blurredTex3 = 0;
		ID3D11Texture2D* blurredTex4 = 0;
		HRESULT HR = device->CreateTexture2D(&blurredTexDesc, 0, &blurredTex);
		assert(SUCCEEDED(HR));
		HR = device->CreateTexture2D(&blurredTexDesc, 0, &blurredTex2);
		assert(SUCCEEDED(HR));
		HR = device->CreateTexture2D(&blurredTexDesc, 0, &blurredTex3);
		assert(SUCCEEDED(HR));
		HR = device->CreateTexture2D(&blurredTexDesc, 0, &blurredTex4);
		assert(SUCCEEDED(HR));
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		HR = device->CreateShaderResourceView(blurredTex, &srvDesc, m_srv[0].GetAddressOf());
		assert(SUCCEEDED(HR));
		HR = device->CreateShaderResourceView(blurredTex2, &srvDesc, m_srv[1].GetAddressOf());
		assert(SUCCEEDED(HR));
		HR = device->CreateShaderResourceView(blurredTex3, &srvDesc, m_srv[2].GetAddressOf());
		assert(SUCCEEDED(HR));
		HR = device->CreateShaderResourceView(blurredTex4, &srvDesc, m_srv[3].GetAddressOf());
		assert(SUCCEEDED(HR));
		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;
		m_viewport.Width = (float)width;
		m_viewport.Height = (float)height;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = blurredTexDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		HR = device->CreateRenderTargetView(blurredTex, &rtvDesc, m_rtv[0].GetAddressOf());
		assert(SUCCEEDED(HR));
		HR = device->CreateRenderTargetView(blurredTex2, &rtvDesc, m_rtv[1].GetAddressOf());
		assert(SUCCEEDED(HR));
		HR = device->CreateRenderTargetView(blurredTex3, &rtvDesc, m_rtv[2].GetAddressOf());
		assert(SUCCEEDED(HR));
		HR = device->CreateRenderTargetView(blurredTex4, &rtvDesc, m_rtv[3].GetAddressOf());
		assert(SUCCEEDED(HR));
		D3D11_TEXTURE2D_DESC textureDescDepth = {};
		ID3D11Texture2D* depthTex2D = nullptr;
		textureDescDepth.Width = (UINT)width;
		textureDescDepth.Height = (UINT)height;
		textureDescDepth.MipLevels = 1;
		textureDescDepth.ArraySize = 1;
		textureDescDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		textureDescDepth.SampleDesc.Count = 1;
		textureDescDepth.Usage = D3D11_USAGE_DEFAULT;
		textureDescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		textureDescDepth.CPUAccessFlags = 0;
		textureDescDepth.MiscFlags = 0;

		 HR = device->CreateTexture2D(&textureDescDepth, 0, &depthTex2D);
		assert(SUCCEEDED(HR));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		HR = device->CreateDepthStencilView(depthTex2D, &depthStencilViewDesc, m_dsv.GetAddressOf());
		assert(SUCCEEDED(HR));
		RELEASE_COM(depthTex2D);
		RELEASE_COM(blurredTex);
		RELEASE_COM(blurredTex2);
		RELEASE_COM(blurredTex3);
		RELEASE_COM(blurredTex4);
		TextureHandler::getInstance().eraseSRV("normalFrame");
		TextureHandler::getInstance().addTextureSRV("normalFrame", m_srv[0]);
		TextureHandler::getInstance().eraseSRV("emissive");
		TextureHandler::getInstance().addTextureSRV("emissive", m_srv[1]);
		TextureHandler::getInstance().eraseSRV("viewNormal");
		TextureHandler::getInstance().addTextureSRV("viewNormal", m_srv[2]);
		TextureHandler::getInstance().eraseSRV("worldPos");
		TextureHandler::getInstance().addTextureSRV("worldPos", m_srv[3]);

		break;
		
	}
	case RENDERPASS::DOWN_SAMPLE_EMISSIVE_PASS:
	{

		UINT width = (UINT)w / 4;
		UINT height = (UINT)h / 4;
		g_w = width;
		g_h = height;
		D3D11_TEXTURE2D_DESC blurredTexDesc;
		blurredTexDesc.Width = width;
		blurredTexDesc.Height = height;
		blurredTexDesc.MipLevels = 1;
		blurredTexDesc.ArraySize = 1;
		blurredTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		blurredTexDesc.SampleDesc.Count = 1;
		blurredTexDesc.SampleDesc.Quality = 0;
		blurredTexDesc.Usage = D3D11_USAGE_DEFAULT;
		blurredTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		blurredTexDesc.CPUAccessFlags = 0;
		blurredTexDesc.MiscFlags = 0;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		ID3D11Texture2D* blurredTex = 0;
	
		HRESULT HR = device->CreateTexture2D(&blurredTexDesc, 0, &blurredTex);
		assert(SUCCEEDED(HR));
	
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		HR = device->CreateShaderResourceView(blurredTex, &srvDesc, m_srv[0].GetAddressOf());
		assert(SUCCEEDED(HR));
		
		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;
		m_viewport.Width = (float)width;
		m_viewport.Height = (float)height;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = blurredTexDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		HR = device->CreateRenderTargetView(blurredTex, &rtvDesc, m_rtv[0].GetAddressOf());
		assert(SUCCEEDED(HR));
		
		D3D11_TEXTURE2D_DESC textureDescDepth = {};
		ID3D11Texture2D* depthTex2D = nullptr;
		textureDescDepth.Width = (UINT)width;
		textureDescDepth.Height = (UINT)height;
		textureDescDepth.MipLevels = 1;
		textureDescDepth.ArraySize = 1;
		textureDescDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		textureDescDepth.SampleDesc.Count = 1;
		textureDescDepth.Usage = D3D11_USAGE_DEFAULT;
		textureDescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		textureDescDepth.CPUAccessFlags = 0;
		textureDescDepth.MiscFlags = 0;

		HR = device->CreateTexture2D(&textureDescDepth, 0, &depthTex2D);
		assert(SUCCEEDED(HR));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		HR = device->CreateDepthStencilView(depthTex2D, &depthStencilViewDesc, m_dsv.GetAddressOf());
		assert(SUCCEEDED(HR));
		RELEASE_COM(blurredTex);
		RELEASE_COM(depthTex2D);
		TextureHandler::getInstance().eraseSRV("downSampleEmissive");
		TextureHandler::getInstance().addTextureSRV("downSampleEmissive", m_srv[0]);
	

		break;

	}
	case RENDERPASS::DOWN_SAMPLE_RAY_MARCH_PASS:
	{

		UINT width = (UINT)w / 4;
		UINT height = (UINT)h / 4;
		g_w = width;
		g_h = height;
		D3D11_TEXTURE2D_DESC blurredTexDesc;
		blurredTexDesc.Width = width;
		blurredTexDesc.Height = height;
		blurredTexDesc.MipLevels = 1;
		blurredTexDesc.ArraySize = 1;
		blurredTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		blurredTexDesc.SampleDesc.Count = 1;
		blurredTexDesc.SampleDesc.Quality = 0;
		blurredTexDesc.Usage = D3D11_USAGE_DEFAULT;
		blurredTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		blurredTexDesc.CPUAccessFlags = 0;
		blurredTexDesc.MiscFlags = 0;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		ID3D11Texture2D* blurredTex = 0;

		HRESULT HR = device->CreateTexture2D(&blurredTexDesc, 0, &blurredTex);
		assert(SUCCEEDED(HR));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		HR = device->CreateShaderResourceView(blurredTex, &srvDesc, m_srv[0].GetAddressOf());
		assert(SUCCEEDED(HR));

		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;
		m_viewport.Width = (float)width;
		m_viewport.Height = (float)height;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = blurredTexDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		HR = device->CreateRenderTargetView(blurredTex, &rtvDesc, m_rtv[0].GetAddressOf());
		assert(SUCCEEDED(HR));

		D3D11_TEXTURE2D_DESC textureDescDepth = {};
		ID3D11Texture2D* depthTex2D = nullptr;
		textureDescDepth.Width = (UINT)width;
		textureDescDepth.Height = (UINT)height;
		textureDescDepth.MipLevels = 1;
		textureDescDepth.ArraySize = 1;
		textureDescDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		textureDescDepth.SampleDesc.Count = 1;
		textureDescDepth.Usage = D3D11_USAGE_DEFAULT;
		textureDescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		textureDescDepth.CPUAccessFlags = 0;
		textureDescDepth.MiscFlags = 0;

		HR = device->CreateTexture2D(&textureDescDepth, 0, &depthTex2D);
		assert(SUCCEEDED(HR));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		HR = device->CreateDepthStencilView(depthTex2D, &depthStencilViewDesc, m_dsv.GetAddressOf());
		assert(SUCCEEDED(HR));
		RELEASE_COM(blurredTex);
		RELEASE_COM(depthTex2D);
		TextureHandler::getInstance().eraseSRV("downSampleRay");
		TextureHandler::getInstance().addTextureSRV("downSampleRay", m_srv[0]);


		break;

	}
	case RENDERPASS::BILATERA_VER:
	{

		UINT width = (UINT)w / 4;
		UINT height = (UINT)h / 4;
		g_w = width;
		g_h = height;
		D3D11_TEXTURE2D_DESC blurredTexDesc;
		blurredTexDesc.Width = width;
		blurredTexDesc.Height = height;
		blurredTexDesc.MipLevels = 1;
		blurredTexDesc.ArraySize = 1;
		blurredTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		blurredTexDesc.SampleDesc.Count = 1;
		blurredTexDesc.SampleDesc.Quality = 0;
		blurredTexDesc.Usage = D3D11_USAGE_DEFAULT;
		blurredTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		blurredTexDesc.CPUAccessFlags = 0;
		blurredTexDesc.MiscFlags = 0;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		ID3D11Texture2D* blurredTex = 0;

		HRESULT HR = device->CreateTexture2D(&blurredTexDesc, 0, &blurredTex);
		assert(SUCCEEDED(HR));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		HR = device->CreateShaderResourceView(blurredTex, &srvDesc, m_srv[0].GetAddressOf());
		assert(SUCCEEDED(HR));

		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;
		m_viewport.Width = (float)width;
		m_viewport.Height = (float)height;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = blurredTexDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		HR = device->CreateRenderTargetView(blurredTex, &rtvDesc, m_rtv[0].GetAddressOf());
		assert(SUCCEEDED(HR));

		D3D11_TEXTURE2D_DESC textureDescDepth = {};
		ID3D11Texture2D* depthTex2D = nullptr;
		textureDescDepth.Width = (UINT)width;
		textureDescDepth.Height = (UINT)height;
		textureDescDepth.MipLevels = 1;
		textureDescDepth.ArraySize = 1;
		textureDescDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		textureDescDepth.SampleDesc.Count = 1;
		textureDescDepth.Usage = D3D11_USAGE_DEFAULT;
		textureDescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		textureDescDepth.CPUAccessFlags = 0;
		textureDescDepth.MiscFlags = 0;

		HR = device->CreateTexture2D(&textureDescDepth, 0, &depthTex2D);
		assert(SUCCEEDED(HR));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		HR = device->CreateDepthStencilView(depthTex2D, &depthStencilViewDesc, m_dsv.GetAddressOf());
		assert(SUCCEEDED(HR));
		RELEASE_COM(blurredTex);
		RELEASE_COM(depthTex2D);
		TextureHandler::getInstance().eraseSRV("bilateralVer");
		TextureHandler::getInstance().addTextureSRV("bilateralVer", m_srv[0]);


		break;

	}
	case RENDERPASS::BILATERA_HOR:
	{

		UINT width = (UINT)w / 4;
		UINT height = (UINT)h / 4;
		g_w = width;
		g_h = height;
		D3D11_TEXTURE2D_DESC blurredTexDesc;
		blurredTexDesc.Width = width;
		blurredTexDesc.Height = height;
		blurredTexDesc.MipLevels = 1;
		blurredTexDesc.ArraySize = 1;
		blurredTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		blurredTexDesc.SampleDesc.Count = 1;
		blurredTexDesc.SampleDesc.Quality = 0;
		blurredTexDesc.Usage = D3D11_USAGE_DEFAULT;
		blurredTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		blurredTexDesc.CPUAccessFlags = 0;
		blurredTexDesc.MiscFlags = 0;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		ID3D11Texture2D* blurredTex = 0;

		HRESULT HR = device->CreateTexture2D(&blurredTexDesc, 0, &blurredTex);
		assert(SUCCEEDED(HR));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		HR = device->CreateShaderResourceView(blurredTex, &srvDesc, m_srv[0].GetAddressOf());
		assert(SUCCEEDED(HR));

		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;
		m_viewport.Width = (float)width;
		m_viewport.Height = (float)height;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = blurredTexDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		HR = device->CreateRenderTargetView(blurredTex, &rtvDesc, m_rtv[0].GetAddressOf());
		assert(SUCCEEDED(HR));

		D3D11_TEXTURE2D_DESC textureDescDepth = {};
		ID3D11Texture2D* depthTex2D = nullptr;
		textureDescDepth.Width = (UINT)width;
		textureDescDepth.Height = (UINT)height;
		textureDescDepth.MipLevels = 1;
		textureDescDepth.ArraySize = 1;
		textureDescDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		textureDescDepth.SampleDesc.Count = 1;
		textureDescDepth.Usage = D3D11_USAGE_DEFAULT;
		textureDescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		textureDescDepth.CPUAccessFlags = 0;
		textureDescDepth.MiscFlags = 0;

		HR = device->CreateTexture2D(&textureDescDepth, 0, &depthTex2D);
		assert(SUCCEEDED(HR));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		HR = device->CreateDepthStencilView(depthTex2D, &depthStencilViewDesc, m_dsv.GetAddressOf());
		assert(SUCCEEDED(HR));
		RELEASE_COM(blurredTex);
		RELEASE_COM(depthTex2D);
		TextureHandler::getInstance().eraseSRV("bilateralHor");
		TextureHandler::getInstance().addTextureSRV("bilateralHor", m_srv[0]);


		break;

	}
	case RENDERPASS::RAY_MARCH:
	{

		UINT width = (UINT)w / 4;
		UINT height = (UINT)h / 4;
		g_w = width;
		g_h = height;
		D3D11_TEXTURE2D_DESC blurredTexDesc;
		blurredTexDesc.Width = width;
		blurredTexDesc.Height = height;
		blurredTexDesc.MipLevels = 1;
		blurredTexDesc.ArraySize = 1;
		blurredTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		blurredTexDesc.SampleDesc.Count = 1;
		blurredTexDesc.SampleDesc.Quality = 0;
		blurredTexDesc.Usage = D3D11_USAGE_DEFAULT;
		blurredTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		blurredTexDesc.CPUAccessFlags = 0;
		blurredTexDesc.MiscFlags = 0;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		ID3D11Texture2D* blurredTex = 0;

		HRESULT HR = device->CreateTexture2D(&blurredTexDesc, 0, &blurredTex);
		assert(SUCCEEDED(HR));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		HR = device->CreateShaderResourceView(blurredTex, &srvDesc, m_srv[0].GetAddressOf());
		assert(SUCCEEDED(HR));

		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;
		m_viewport.Width = (float)width;
		m_viewport.Height = (float)height;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = blurredTexDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		HR = device->CreateRenderTargetView(blurredTex, &rtvDesc, m_rtv[0].GetAddressOf());
		assert(SUCCEEDED(HR));

		D3D11_TEXTURE2D_DESC textureDescDepth = {};
		ID3D11Texture2D* depthTex2D = nullptr;
		textureDescDepth.Width = (UINT)width;
		textureDescDepth.Height = (UINT)height;
		textureDescDepth.MipLevels = 1;
		textureDescDepth.ArraySize = 1;
		textureDescDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		textureDescDepth.SampleDesc.Count = 1;
		textureDescDepth.Usage = D3D11_USAGE_DEFAULT;
		textureDescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		textureDescDepth.CPUAccessFlags = 0;
		textureDescDepth.MiscFlags = 0;

		HR = device->CreateTexture2D(&textureDescDepth, 0, &depthTex2D);
		assert(SUCCEEDED(HR));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		HR = device->CreateDepthStencilView(depthTex2D, &depthStencilViewDesc, m_dsv.GetAddressOf());
		assert(SUCCEEDED(HR));
		RELEASE_COM(blurredTex);
		RELEASE_COM(depthTex2D);
		TextureHandler::getInstance().eraseSRV("volLight");
		TextureHandler::getInstance().addTextureSRV("volLight", m_srv[0]);


		break;

	}
	case RENDERPASS::GAUSSIAN_HOR:
	{
		UINT width = (UINT)w / 4;
		UINT height = (UINT)h / 4;
		g_w = width;
		g_h = height;
		D3D11_TEXTURE2D_DESC blurredTexDesc;
		blurredTexDesc.Width = width;
		blurredTexDesc.Height = height;
		blurredTexDesc.MipLevels = 1;
		blurredTexDesc.ArraySize = 1;
		blurredTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		blurredTexDesc.SampleDesc.Count = 1;
		blurredTexDesc.SampleDesc.Quality = 0;
		blurredTexDesc.Usage = D3D11_USAGE_DEFAULT;
		blurredTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		blurredTexDesc.CPUAccessFlags = 0;
		blurredTexDesc.MiscFlags = 0;


		D3D11_TEXTURE2D_DESC blurredTexDescRTV;
		blurredTexDescRTV.Width = width;
		blurredTexDescRTV.Height = height;
		blurredTexDescRTV.MipLevels = 1;
		blurredTexDescRTV.ArraySize = 1;
		blurredTexDescRTV.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		blurredTexDescRTV.SampleDesc.Count = 1;
		blurredTexDescRTV.SampleDesc.Quality = 0;
		blurredTexDescRTV.Usage = D3D11_USAGE_DEFAULT;
		blurredTexDescRTV.BindFlags =  D3D11_BIND_RENDER_TARGET;
		blurredTexDescRTV.CPUAccessFlags = 0;
		blurredTexDescRTV.MiscFlags = 0;


		ID3D11Texture2D* blurredTexRTV = 0;
		HRESULT HR = device->CreateTexture2D(&blurredTexDescRTV, 0, &blurredTexRTV);
		assert(SUCCEEDED(HR));

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		ID3D11Texture2D* blurredTex = 0;
		 HR = device->CreateTexture2D(&blurredTexDesc, 0, &blurredTex);
		assert(SUCCEEDED(HR));
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		HR = device->CreateShaderResourceView(blurredTex, &srvDesc, m_srv[0].GetAddressOf());
		assert(SUCCEEDED(HR));
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;
		HR = device->CreateUnorderedAccessView(blurredTex, &uavDesc, m_uav[0].GetAddressOf());
		assert(SUCCEEDED(HR));
		D3D11_TEXTURE2D_DESC textureDescDepth = {};
		ID3D11Texture2D* depthTex2D = nullptr;
		textureDescDepth.Width = (UINT)width;
		textureDescDepth.Height = (UINT)height;
		textureDescDepth.MipLevels = 1;
		textureDescDepth.ArraySize = 1;
		textureDescDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		textureDescDepth.SampleDesc.Count = 1;
		textureDescDepth.Usage = D3D11_USAGE_DEFAULT;
		textureDescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		textureDescDepth.CPUAccessFlags = 0;
		textureDescDepth.MiscFlags = 0;

		 HR = device->CreateTexture2D(&textureDescDepth, 0, &depthTex2D);
		assert(SUCCEEDED(HR));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		HR = device->CreateDepthStencilView(depthTex2D, &depthStencilViewDesc, m_dsv.GetAddressOf());
		assert(SUCCEEDED(HR));
		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;
		m_viewport.Width = (float)width;
		m_viewport.Height = (float)height;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = blurredTexDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		HR = device->CreateRenderTargetView(blurredTexRTV, &rtvDesc, m_rtv[0].GetAddressOf());
		assert(SUCCEEDED(HR));
		RELEASE_COM(blurredTex);
		RELEASE_COM(depthTex2D);
		RELEASE_COM(blurredTexRTV);
		TextureHandler::getInstance().eraseSRV("GaussianHorSRV");
		TextureHandler::getInstance().addTextureSRV("GaussianHorSRV", m_srv[0]);
		TextureHandler::getInstance().eraseUAV("GaussianHorUAV");
		TextureHandler::getInstance().addTextureUAV("GaussianHorUAV", m_uav[0]);

		break;
	}
	case RENDERPASS::GAUSSIAN_VER:
	{
		UINT width = (UINT)w / 4;
		UINT height = (UINT)h / 4;
		g_w = width;
		g_h = height;
		D3D11_TEXTURE2D_DESC blurredTexDesc;
		blurredTexDesc.Width = width;
		blurredTexDesc.Height = height;
		blurredTexDesc.MipLevels = 1;
		blurredTexDesc.ArraySize = 1;
		blurredTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		blurredTexDesc.SampleDesc.Count = 1;
		blurredTexDesc.SampleDesc.Quality = 0;
		blurredTexDesc.Usage = D3D11_USAGE_DEFAULT;
		blurredTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		blurredTexDesc.CPUAccessFlags = 0;
		blurredTexDesc.MiscFlags = 0;

		D3D11_TEXTURE2D_DESC blurredTexDescRTV;
		blurredTexDescRTV.Width = width;
		blurredTexDescRTV.Height = height;
		blurredTexDescRTV.MipLevels = 1;
		blurredTexDescRTV.ArraySize = 1;
		blurredTexDescRTV.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		blurredTexDescRTV.SampleDesc.Count = 1;
		blurredTexDescRTV.SampleDesc.Quality = 0;
		blurredTexDescRTV.Usage = D3D11_USAGE_DEFAULT;
		blurredTexDescRTV.BindFlags = D3D11_BIND_RENDER_TARGET;
		blurredTexDescRTV.CPUAccessFlags = 0;
		blurredTexDescRTV.MiscFlags = 0;


		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		ID3D11Texture2D* blurredTex = 0;
		ID3D11Texture2D* blurredTexRTV = 0;
		HRESULT HR = device->CreateTexture2D(&blurredTexDesc, 0, &blurredTex);
		assert(SUCCEEDED(HR));
		 HR = device->CreateTexture2D(&blurredTexDescRTV, 0, &blurredTexRTV);
		assert(SUCCEEDED(HR));
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		HR = device->CreateShaderResourceView(blurredTex, &srvDesc, &m_srv[0]);
		assert(SUCCEEDED(HR));
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;
	
		
		HR = device->CreateUnorderedAccessView(blurredTex, &uavDesc, &m_uav[0]);
		assert(SUCCEEDED(HR));
		D3D11_TEXTURE2D_DESC textureDescDepth = {};
		ID3D11Texture2D* depthTex2D = nullptr;
		textureDescDepth.Width = (UINT)width;
		textureDescDepth.Height = (UINT)height;
		textureDescDepth.MipLevels = 1;
		textureDescDepth.ArraySize = 1;
		textureDescDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		textureDescDepth.SampleDesc.Count = 1;
		textureDescDepth.Usage = D3D11_USAGE_DEFAULT;
		textureDescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		textureDescDepth.CPUAccessFlags = 0;
		textureDescDepth.MiscFlags = 0;

		 HR = device->CreateTexture2D(&textureDescDepth, 0, &depthTex2D);
		assert(SUCCEEDED(HR));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		HR = device->CreateDepthStencilView(depthTex2D, &depthStencilViewDesc, m_dsv.GetAddressOf());
		assert(SUCCEEDED(HR));
		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;
		m_viewport.Width = (float)width;
		m_viewport.Height = (float)height;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = blurredTexDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		HR = device->CreateRenderTargetView(blurredTexRTV, &rtvDesc, m_rtv[0].GetAddressOf());
		assert(SUCCEEDED(HR));
		RELEASE_COM(depthTex2D);
		RELEASE_COM(blurredTex);
		RELEASE_COM(blurredTexRTV);
		TextureHandler::getInstance().eraseSRV("GaussianVerSRV");
		TextureHandler::getInstance().addTextureSRV("GaussianVerSRV", m_srv[0]);
		TextureHandler::getInstance().eraseUAV("GaussianVerUAV");
		TextureHandler::getInstance().addTextureUAV("GaussianVerUAV", m_uav[0]);
		break;
	}
	case RENDERPASS::UP_SAMPLE_EMISSIVE_PASS:
	{

		UINT width = (UINT)w;
		UINT height = (UINT)h;
		g_w = width;
		g_h = height;
		D3D11_TEXTURE2D_DESC blurredTexDesc;
		blurredTexDesc.Width = width;
		blurredTexDesc.Height = height;
		blurredTexDesc.MipLevels = 1;
		blurredTexDesc.ArraySize = 1;
		blurredTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		blurredTexDesc.SampleDesc.Count = 1;
		blurredTexDesc.SampleDesc.Quality = 0;
		blurredTexDesc.Usage = D3D11_USAGE_DEFAULT;
		blurredTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		blurredTexDesc.CPUAccessFlags = 0;
		blurredTexDesc.MiscFlags = 0;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		ID3D11Texture2D* blurredTex = 0;

		HRESULT HR = device->CreateTexture2D(&blurredTexDesc, 0, &blurredTex);
		assert(SUCCEEDED(HR));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		HR = device->CreateShaderResourceView(blurredTex, &srvDesc, m_srv[0].GetAddressOf());
		assert(SUCCEEDED(HR));

		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;
		m_viewport.Width = (float)width;
		m_viewport.Height = (float)height;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = blurredTexDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		HR = device->CreateRenderTargetView(blurredTex, &rtvDesc, m_rtv[0].GetAddressOf());
		assert(SUCCEEDED(HR));

		D3D11_TEXTURE2D_DESC textureDescDepth = {};
		ID3D11Texture2D* depthTex2D = nullptr;
		textureDescDepth.Width = (UINT)width;
		textureDescDepth.Height = (UINT)height;
		textureDescDepth.MipLevels = 1;
		textureDescDepth.ArraySize = 1;
		textureDescDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		textureDescDepth.SampleDesc.Count = 1;
		textureDescDepth.Usage = D3D11_USAGE_DEFAULT;
		textureDescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		textureDescDepth.CPUAccessFlags = 0;
		textureDescDepth.MiscFlags = 0;

		HR = device->CreateTexture2D(&textureDescDepth, 0, &depthTex2D);
		assert(SUCCEEDED(HR));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		HR = device->CreateDepthStencilView(depthTex2D, &depthStencilViewDesc, m_dsv.GetAddressOf());
		assert(SUCCEEDED(HR));
		RELEASE_COM(blurredTex);
		RELEASE_COM(depthTex2D);
		TextureHandler::getInstance().eraseSRV("upSampleEmissive");
		TextureHandler::getInstance().addTextureSRV("upSampleEmissive", m_srv[0]);


		break;

	}
	case RENDERPASS::UP_SAMPLE_RAY_MARCH_PASS:
	{

		UINT width = (UINT)w;
		UINT height = (UINT)h;
		g_w = width;
		g_h = height;
		D3D11_TEXTURE2D_DESC blurredTexDesc;
		blurredTexDesc.Width = width;
		blurredTexDesc.Height = height;
		blurredTexDesc.MipLevels = 1;
		blurredTexDesc.ArraySize = 1;
		blurredTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		blurredTexDesc.SampleDesc.Count = 1;
		blurredTexDesc.SampleDesc.Quality = 0;
		blurredTexDesc.Usage = D3D11_USAGE_DEFAULT;
		blurredTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		blurredTexDesc.CPUAccessFlags = 0;
		blurredTexDesc.MiscFlags = 0;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		ID3D11Texture2D* blurredTex = 0;

		HRESULT HR = device->CreateTexture2D(&blurredTexDesc, 0, &blurredTex);
		assert(SUCCEEDED(HR));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		HR = device->CreateShaderResourceView(blurredTex, &srvDesc, m_srv[0].GetAddressOf());
		assert(SUCCEEDED(HR));

		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;
		m_viewport.Width = (float)width;
		m_viewport.Height = (float)height;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = blurredTexDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		HR = device->CreateRenderTargetView(blurredTex, &rtvDesc, m_rtv[0].GetAddressOf());
		assert(SUCCEEDED(HR));

		D3D11_TEXTURE2D_DESC textureDescDepth = {};
		ID3D11Texture2D* depthTex2D = nullptr;
		textureDescDepth.Width = (UINT)width;
		textureDescDepth.Height = (UINT)height;
		textureDescDepth.MipLevels = 1;
		textureDescDepth.ArraySize = 1;
		textureDescDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		textureDescDepth.SampleDesc.Count = 1;
		textureDescDepth.Usage = D3D11_USAGE_DEFAULT;
		textureDescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		textureDescDepth.CPUAccessFlags = 0;
		textureDescDepth.MiscFlags = 0;

		HR = device->CreateTexture2D(&textureDescDepth, 0, &depthTex2D);
		assert(SUCCEEDED(HR));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		HR = device->CreateDepthStencilView(depthTex2D, &depthStencilViewDesc, m_dsv.GetAddressOf());
		assert(SUCCEEDED(HR));
		RELEASE_COM(blurredTex);
		RELEASE_COM(depthTex2D);
		TextureHandler::getInstance().eraseSRV("upSampleRay");
		TextureHandler::getInstance().addTextureSRV("upSampleRay", m_srv[0]);


		break;

	}
	}

	
	
}
