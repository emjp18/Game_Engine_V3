#pragma once
#ifndef RENDERPASSHANDLER_H
#define RENDERPASSHANDLER_H
#include "RenderPass.h"
class RenderPassHandler
{
private:
	std::map<RENDERPASS, RenderPass> m_renderPasses;
	std::vector< RENDERPASS> m_existing;
public:
	RenderPassHandler();
	~RenderPassHandler();
	static RenderPassHandler& getInstance() { static RenderPassHandler instance; return instance; }
	RenderPassHandler(const RenderPassHandler&) = delete;
	RenderPassHandler& operator=(const RenderPassHandler&) = delete;
	RenderPassHandler(RenderPassHandler&&) = delete;
	RenderPassHandler& operator=(RenderPassHandler&&) = delete;
	void init(ID3D11Device* device, size_t w, size_t h, const SCENE& scene);
	void reCreate(ID3D11Device* device, size_t w, size_t h, const SCENE& scene);
	RenderPass& getRenderPass(RENDERPASS type);
	const std::map<RENDERPASS, RenderPass>& getRenderPass() { return m_renderPasses; }
	bool renderPassExists(RENDERPASS type);
};

#endif