#pragma once
#ifndef RENDERPASS_H
#define RENDERPASS_H
#include "Shape.h"
#include "TextureHandler.h"
class RenderPass
{
private:
	std::vector<Shape*> m_shapes;
	WRL::ComPtr<ID3D11DepthStencilView> m_dsv = nullptr;
	WRL::ComPtr<ID3D11RenderTargetView> m_rtv[4] = { nullptr,nullptr, nullptr, nullptr };
	WRL::ComPtr<ID3D11ShaderResourceView> m_srv[4] = { nullptr,nullptr, nullptr, nullptr };
	WRL::ComPtr<ID3D11UnorderedAccessView> m_uav[3] = { nullptr,nullptr, nullptr };
	D3D11_VIEWPORT m_viewport = {};
	RENDERPASS m_type;
public:
	RenderPass();
	RenderPass(RENDERPASS type, size_t w, size_t h, ID3D11Device* device);
	void setShape(Shape* shape) { m_shapes.push_back(shape); }
	const WRL::ComPtr<ID3D11DepthStencilView>& getDSV() { return m_dsv; }
	const WRL::ComPtr<ID3D11RenderTargetView>& getRTV(int nr = 0) { return m_rtv[nr]; }
	const WRL::ComPtr<ID3D11ShaderResourceView>& getSRV(int nr = 0) { return m_srv[nr]; }
	const WRL::ComPtr<ID3D11UnorderedAccessView>& getUAV(int nr = 0) { return m_uav[nr]; }
	const D3D11_VIEWPORT& getViewPort() { return m_viewport; }
	const std::vector<Shape*>& getShapes() { return m_shapes; }
	size_t g_w;
	size_t g_h;
};

#endif