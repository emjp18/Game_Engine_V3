#pragma once
#ifndef TEXTUREANDLER_H
#define TEXTUREANDLER_H
#include "common.h"
class TextureHandler
{
private:
	std::unordered_map<std::string, WRL::ComPtr < ID3D11ShaderResourceView>> m_srvPool;
	std::unordered_map<std::string, WRL::ComPtr < ID3D11UnorderedAccessView>> m_uavPool;
	WRL::ComPtr < ID3D11SamplerState> m_samplerWrap;
	WRL::ComPtr < ID3D11SamplerState> m_samplerClamp;
	WRL::ComPtr < ID3D11SamplerState> m_samplerComparison;
	WRL::ComPtr < ID3D11SamplerState> m_samplerAnisotropic;
	WRL::ComPtr < ID3D11SamplerState> m_samplerTerrain;
	void createDummyTextures(ID3D11Device* device);
	void createSamplers(ID3D11Device* device);
	void createRandom1DTexture(ID3D11Device* device);
	float satRandF();
	float randF(float a, float b);
public:
	TextureHandler();
	~TextureHandler();
	static TextureHandler& getInstance() { static TextureHandler instance; return instance; }
	TextureHandler(const TextureHandler&) = delete;
	TextureHandler& operator=(const TextureHandler&) = delete;
	TextureHandler(TextureHandler&&) = delete;
	TextureHandler& operator=(TextureHandler&&) = delete;
	WRL::ComPtr < ID3D11SamplerState> getSampler(SAMPLER_TYPE type);
	ID3D11ShaderResourceView** getTextureSRV(std::string path) { return m_srvPool[path].GetAddressOf(); }
	ID3D11UnorderedAccessView** getTextureUAV(std::string path) { return m_uavPool[path].GetAddressOf(); }
	void init(ID3D11Device* device);
	void addTextureSRV(std::string path, ID3D11Device*);
	void addTextureUAV(std::string name,const WRL::ComPtr < ID3D11UnorderedAccessView>&);
	void addTextureSRV(std::string name,const WRL::ComPtr < ID3D11ShaderResourceView>&);
	void addTextureUAV(std::string name, ID3D11Texture2D* tex, ID3D11Device*, D3D11_UNORDERED_ACCESS_VIEW_DESC&);
	void addTextureSRV(std::string name, ID3D11Texture2D* tex, ID3D11Device*, D3D11_SHADER_RESOURCE_VIEW_DESC&);
	void addTextureSRV(std::string name, ID3D11Texture1D* tex, ID3D11Device*, D3D11_SHADER_RESOURCE_VIEW_DESC&);
	void eraseSRV(std::string path);
	void eraseUAV(std::string path);
};

#endif