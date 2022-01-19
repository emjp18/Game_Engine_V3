#include "stdafx.h"
#include "TextureHandler.h"

WRL::ComPtr<ID3D11SamplerState> TextureHandler::getSampler(SAMPLER_TYPE type)
{
	switch (type)
	{
	default:
	{
		return m_samplerWrap;
		break;
	}
	case SAMPLER_TYPE::WRAP:
	{
		return m_samplerWrap;
		break;
	}
	case SAMPLER_TYPE::CLAMP:
	{
		return m_samplerClamp;
		break;
	}
	case SAMPLER_TYPE::COMPARISON:
	{
		return m_samplerComparison;
		break;
	}
	case SAMPLER_TYPE::ANISOTROPIC:
	{
		return m_samplerAnisotropic;
		break;
	}
	case SAMPLER_TYPE::TERRAIN:
	{
		return m_samplerTerrain;
		break;
	}
	}
}

void TextureHandler::init(ID3D11Device* device)
{
	srand((unsigned)time(NULL));
	createSamplers(device);
	createDummyTextures(device);
}

void TextureHandler::addTextureSRV(std::string path, ID3D11Device* device)
{
	if (m_srvPool.find(path) == m_srvPool.end())
	{
		std::wstring pathWString(path.begin(), path.end());
		WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;
		HRESULT hr = DirectX::CreateWICTextureFromFile(device, pathWString.c_str(), nullptr, srv.GetAddressOf());
		assert(SUCCEEDED(hr));
		m_srvPool.insert({ path, srv });
		
	}
}

void TextureHandler::addTextureUAV(std::string name,const WRL::ComPtr<ID3D11UnorderedAccessView>& uav)
{
	if (m_uavPool.find(name) == m_uavPool.end())
	{
		m_uavPool.insert({ name, uav });
		
	}
}

void TextureHandler::addTextureSRV(std::string name,const WRL::ComPtr<ID3D11ShaderResourceView>& srv)
{
	if (m_srvPool.find(name) == m_srvPool.end())
	{
		m_srvPool.insert({ name, srv });
		
	}
}

void TextureHandler::addTextureUAV(std::string name, ID3D11Texture2D* tex, ID3D11Device* device, D3D11_UNORDERED_ACCESS_VIEW_DESC& desc)
{
	if (m_uavPool.find(name) == m_uavPool.end())
	{
		WRL::ComPtr<ID3D11UnorderedAccessView> uav = nullptr;
		device->CreateUnorderedAccessView(tex, &desc, uav.GetAddressOf());
		m_uavPool.insert({ name, uav });
		
	}
}

void TextureHandler::addTextureSRV(std::string name, ID3D11Texture2D* tex, ID3D11Device* device, D3D11_SHADER_RESOURCE_VIEW_DESC& desc)
{
	if (m_srvPool.find(name) == m_srvPool.end())
	{
		WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;
		device->CreateShaderResourceView(tex, &desc, srv.GetAddressOf());
		m_srvPool.insert({ name, srv });
	}
}

void TextureHandler::addTextureSRV(std::string name, ID3D11Texture1D* tex, ID3D11Device* device, D3D11_SHADER_RESOURCE_VIEW_DESC& desc)
{
	if (m_srvPool.find(name) == m_srvPool.end())
	{
		WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;
		device->CreateShaderResourceView(tex, &desc, srv.GetAddressOf());
		m_srvPool.insert({ name, srv });
		
	}
}

void TextureHandler::eraseSRV(std::string path)
{
	m_srvPool.erase(path);
}

void TextureHandler::eraseUAV(std::string path)
{
	m_uavPool.erase(path);
}

void TextureHandler::createDummyTextures(ID3D11Device* device)
{
	WRL::ComPtr<ID3D11ShaderResourceView> dummyNormalsrv = nullptr;
	WRL::ComPtr<ID3D11ShaderResourceView> dummyMetalsrv = nullptr;
	WRL::ComPtr<ID3D11ShaderResourceView> dummyAOsrv = nullptr;
	WRL::ComPtr<ID3D11ShaderResourceView> dummyRoughsrv = nullptr;
	WRL::ComPtr<ID3D11ShaderResourceView> dummyGrasssrv = nullptr;
	ID3D11Texture2D* tex2D = nullptr;
	XMFLOAT4 s_pixel = XMFLOAT4(0, 0, 1, 1);
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = &s_pixel;
	initData.SysMemPitch = sizeof(XMFLOAT4);
	initData.SysMemSlicePitch = 0;

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = desc.Height = desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;


	HRESULT hr = device->CreateTexture2D(&desc, &initData, &tex2D);

	assert(SUCCEEDED(hr));

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = desc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(tex2D, &SRVDesc, dummyNormalsrv.GetAddressOf());

	assert(SUCCEEDED(hr));

	RELEASE_COM(tex2D);

	s_pixel = XMFLOAT4(0, 0, 0, 1);

	initData.pSysMem = &s_pixel;
	initData.SysMemPitch = sizeof(XMFLOAT4);
	initData.SysMemSlicePitch = 0;

	hr = device->CreateTexture2D(&desc, &initData, &tex2D);

	assert(SUCCEEDED(hr));

	hr = device->CreateShaderResourceView(tex2D, &SRVDesc, dummyMetalsrv.GetAddressOf());

	assert(SUCCEEDED(hr));

	RELEASE_COM(tex2D);

	s_pixel = XMFLOAT4(1, 1, 1, 1);

	initData.pSysMem = &s_pixel;
	initData.SysMemPitch = sizeof(XMFLOAT4);
	initData.SysMemSlicePitch = 0;

	hr = device->CreateTexture2D(&desc, &initData, &tex2D);

	assert(SUCCEEDED(hr));

	hr = device->CreateShaderResourceView(tex2D, &SRVDesc, dummyAOsrv.GetAddressOf());

	assert(SUCCEEDED(hr));

	RELEASE_COM(tex2D);

	s_pixel = XMFLOAT4(0.5f, 0.5f, 0.5f, 1);

	initData.pSysMem = &s_pixel;
	initData.SysMemPitch = sizeof(XMFLOAT4);
	initData.SysMemSlicePitch = 0;

	hr = device->CreateTexture2D(&desc, &initData, &tex2D);

	assert(SUCCEEDED(hr));

	hr = device->CreateShaderResourceView(tex2D, &SRVDesc, dummyRoughsrv.GetAddressOf());

	assert(SUCCEEDED(hr));

	RELEASE_COM(tex2D);
	s_pixel = XMFLOAT4(0, 1, 0, 1);

	initData.pSysMem = &s_pixel;
	initData.SysMemPitch = sizeof(XMFLOAT4);
	initData.SysMemSlicePitch = 0;

	hr = device->CreateTexture2D(&desc, &initData, &tex2D);

	assert(SUCCEEDED(hr));

	hr = device->CreateShaderResourceView(tex2D, &SRVDesc, dummyGrasssrv.GetAddressOf());

	assert(SUCCEEDED(hr));

	RELEASE_COM(tex2D);
	getInstance().addTextureSRV("blue", dummyNormalsrv);
	getInstance().addTextureSRV("black", dummyMetalsrv);
	getInstance().addTextureSRV("white", dummyAOsrv);
	getInstance().addTextureSRV("grey", dummyRoughsrv);
	getInstance().addTextureSRV("green", dummyGrasssrv);

	getInstance().addTextureSRV("../Resources/Textures/noise.png", device);

	createRandom1DTexture(device);
}

void TextureHandler::createSamplers(ID3D11Device* device)
{
	HRESULT hr;
	WRL::ComPtr<ID3D11SamplerState> sampleWrap;
	WRL::ComPtr<ID3D11SamplerState> sampleClamp;
	WRL::ComPtr<ID3D11SamplerState> sampleComparison;
	WRL::ComPtr<ID3D11SamplerState> sampleA;
	WRL::ComPtr<ID3D11SamplerState> sampleTerrain;
	D3D11_SAMPLER_DESC desc = {};
	desc.MaxAnisotropy = 16;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = 0.0f;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	hr = device->CreateSamplerState(&desc, sampleWrap.GetAddressOf());
	assert(SUCCEEDED(hr));


	desc.MaxAnisotropy = 4;
	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = 0.0f;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	hr = device->CreateSamplerState(&desc, sampleA.GetAddressOf());
	assert(SUCCEEDED(hr));



	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.MipLODBias = 0.0f;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxAnisotropy = 16;
	hr = device->CreateSamplerState(&desc, sampleClamp.GetAddressOf());
	assert(SUCCEEDED(hr));

	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.MipLODBias = 0.0f;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxAnisotropy = 16;
	hr = device->CreateSamplerState(&desc, sampleTerrain.GetAddressOf());
	assert(SUCCEEDED(hr));


	desc.MaxAnisotropy = 16;
	desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.MipLODBias = 0.0f;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	desc.MinLOD = 0;
	desc.ComparisonFunc = D3D11_COMPARISON_LESS;
	desc.BorderColor[0] = 0;
	desc.BorderColor[1] = 0;
	desc.BorderColor[2] = 0;
	desc.BorderColor[3] = 0;

	hr = device->CreateSamplerState(&desc, sampleComparison.GetAddressOf());
	assert(SUCCEEDED(hr));

	getInstance().m_samplerWrap = sampleWrap;
	getInstance().m_samplerClamp = sampleClamp;
	getInstance().m_samplerComparison = sampleComparison;
	getInstance().m_samplerAnisotropic = sampleA;
	getInstance().m_samplerTerrain = sampleTerrain;
}

void TextureHandler::createRandom1DTexture(ID3D11Device* device)
{
	DirectX::XMFLOAT4* randomValues = new DirectX::XMFLOAT4[1024];
	for (int i = 0; i < 1024; ++i)
	{
		randomValues[i].x = randF(-1.0f, 1.0f);
		randomValues[i].y = randF(-1.0f, 1.0f);
		randomValues[i].z = randF(-1.0f, 1.0f);
		randomValues[i].w = randF(-1.0f, 1.0f);
	}
	D3D11_SUBRESOURCE_DATA initData = {};

	initData.pSysMem = randomValues;
	initData.SysMemPitch = 1024 * sizeof(DirectX::XMFLOAT4);
	initData.SysMemSlicePitch = 0;

	D3D11_TEXTURE1D_DESC texDesc = {};
	texDesc.Width = 1024;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.MipLevels = 1;

	ID3D11Texture1D* tex1D = nullptr;

	HRESULT hr = device->CreateTexture1D(&texDesc, &initData, &tex1D);

	assert(SUCCEEDED(hr));


	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip = 0;

	WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;

	hr = device->CreateShaderResourceView(tex1D, &viewDesc, srv.GetAddressOf());

	assert(SUCCEEDED(hr));
	delete[] randomValues;
	RELEASE_COM(tex1D);

	getInstance().addTextureSRV("random1D", srv);
}

float TextureHandler::satRandF()
{
	return (float)rand() / (float)RAND_MAX;
}

float TextureHandler::randF(float a, float b)
{
	return a + satRandF() * (b - a);
}

TextureHandler::TextureHandler()
{
}

TextureHandler::~TextureHandler()
{
}
