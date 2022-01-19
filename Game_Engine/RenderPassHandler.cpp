#include "stdafx.h"
#include "RenderPassHandler.h"

RenderPassHandler::RenderPassHandler()
{
}

RenderPassHandler::~RenderPassHandler()
{
}

void RenderPassHandler::init(ID3D11Device* device, size_t w, size_t h, const SCENE& scene)
{
	switch(scene)
	{
		default:
		{
		/*	RenderPass rp0(RENDERPASS::DEPTH_LIGHT_PASS, w, h, device);
			m_renderPasses.insert({ RENDERPASS::DEPTH_LIGHT_PASS, rp0 });
			RenderPass rp1(RENDERPASS::DEPTH_SHADOW_DIR, (size_t)w, (size_t)h, device);
			m_renderPasses.insert({ RENDERPASS::DEPTH_SHADOW_DIR, rp1 });
			RenderPass rp2(RENDERPASS::DEPTH_SHADOW_POINT, (size_t)w, (size_t)h, device);
			m_renderPasses.insert({ RENDERPASS::DEPTH_SHADOW_POINT, rp2 });
			RenderPass rp3(RENDERPASS::LIGHT_CULL_PASS, w, h, device);
			m_renderPasses.insert({ RENDERPASS::LIGHT_CULL_PASS, rp3 });
			RenderPass rp4(RENDERPASS::DYNAMIC_CUBE_MAP_PASS, w, h, device);
			m_renderPasses.insert({ RENDERPASS::DYNAMIC_CUBE_MAP_PASS, rp4 });
			RenderPass rp5(RENDERPASS::EDGE_DETECTION_PASS, w, h, device);
			m_renderPasses.insert({ RENDERPASS::EDGE_DETECTION_PASS, rp5 });
			RenderPass rp6(RENDERPASS::BASIC_PASS_SINGLE, w, h, device);
			m_renderPasses.insert({ RENDERPASS::BASIC_PASS_SINGLE, rp6 });
			RenderPass rp7(RENDERPASS::DOWN_SAMPLE_EMISSIVE_PASS, w, h, device);
			m_renderPasses.insert({ RENDERPASS::DOWN_SAMPLE_EMISSIVE_PASS, rp7 });
			RenderPass rp8(RENDERPASS::DOWN_SAMPLE_RAY_MARCH_PASS, w, h, device);
			m_renderPasses.insert({ RENDERPASS::DOWN_SAMPLE_RAY_MARCH_PASS, rp8 });
			RenderPass rp9(RENDERPASS::GAUSSIAN_HOR, w, h, device);
			m_renderPasses.insert({ RENDERPASS::GAUSSIAN_HOR, rp9 });
			RenderPass rp10(RENDERPASS::GAUSSIAN_VER, w, h, device);
			m_renderPasses.insert({ RENDERPASS::GAUSSIAN_VER, rp10 });
			RenderPass rp11(RENDERPASS::RAY_MARCH, w, h, device);
			m_renderPasses.insert({ RENDERPASS::RAY_MARCH, rp11 });
			RenderPass rp12(RENDERPASS::BILATERA_HOR, w, h, device);
			m_renderPasses.insert({ RENDERPASS::BILATERA_HOR, rp12 });
			RenderPass rp13(RENDERPASS::BILATERA_VER, w, h, device);
			m_renderPasses.insert({ RENDERPASS::BILATERA_VER, rp13 });
			RenderPass rp14(RENDERPASS::UP_SAMPLE_EMISSIVE_PASS, w, h, device);
			m_renderPasses.insert({ RENDERPASS::UP_SAMPLE_EMISSIVE_PASS, rp14 });
			RenderPass rp15(RENDERPASS::UP_SAMPLE_RAY_MARCH_PASS, w, h, device);
			m_renderPasses.insert({ RENDERPASS::UP_SAMPLE_RAY_MARCH_PASS, rp15 });
			RenderPass rp16(RENDERPASS::FXAA, w, h, device);
			m_renderPasses.insert({ RENDERPASS::FXAA, rp16 });
			RenderPass rp17(RENDERPASS::BASIC_PASS_QUAD, w, h, device);
			m_renderPasses.insert({ RENDERPASS::BASIC_PASS_QUAD, rp17 });*/
			break;
		}
	case SCENE::DEBUG_LIGHT_CULL:
	{
		m_existing.push_back(RENDERPASS::DEPTH_LIGHT_PASS);
		m_existing.push_back(RENDERPASS::LIGHT_CULL_PASS);
		m_existing.push_back(RENDERPASS::BASIC_PASS_QUAD);


		RenderPass rp0(RENDERPASS::DEPTH_LIGHT_PASS, w, h, device);
		m_renderPasses.insert({ RENDERPASS::DEPTH_LIGHT_PASS, rp0 });
		RenderPass rp3(RENDERPASS::LIGHT_CULL_PASS, w, h, device);
		m_renderPasses.insert({ RENDERPASS::LIGHT_CULL_PASS, rp3 });
		RenderPass rp4(RENDERPASS::BASIC_PASS_QUAD, w, h, device);
		m_renderPasses.insert({ RENDERPASS::BASIC_PASS_QUAD, rp4 });
		break;
	}
	case SCENE::MENU:
	{
		break;
	
	}
	case SCENE::HEAT:
	{
		m_existing.push_back(RENDERPASS::DEPTH_LIGHT_PASS);
		m_existing.push_back(RENDERPASS::LIGHT_CULL_PASS);
		m_existing.push_back(RENDERPASS::BASIC_PASS_QUAD);
		RenderPass rp0(RENDERPASS::DEPTH_LIGHT_PASS, w, h, device);
		m_renderPasses.insert({ RENDERPASS::DEPTH_LIGHT_PASS, rp0 });
		RenderPass rp1(RENDERPASS::LIGHT_CULL_PASS, (size_t)w, (size_t)h, device);
		m_renderPasses.insert({ RENDERPASS::LIGHT_CULL_PASS, rp1 });
		RenderPass rp2(RENDERPASS::BASIC_PASS_QUAD, (size_t)w, (size_t)h, device);
		m_renderPasses.insert({ RENDERPASS::BASIC_PASS_QUAD, rp2 });
		break;

	}
	case SCENE::LOD:
	{
		m_existing.push_back(RENDERPASS::DEPTH_LIGHT_PASS);
		m_existing.push_back(RENDERPASS::LIGHT_CULL_PASS);
		m_existing.push_back(RENDERPASS::BASIC_PASS_SINGLE);
		m_existing.push_back(RENDERPASS::BASIC_PASS_QUAD);
		m_existing.push_back(RENDERPASS::DEPTH_SHADOW_DIR);
		m_existing.push_back(RENDERPASS::UP_SAMPLE_EMISSIVE_PASS);
		m_existing.push_back(RENDERPASS::DOWN_SAMPLE_EMISSIVE_PASS);
		m_existing.push_back(RENDERPASS::GAUSSIAN_HOR);
		m_existing.push_back(RENDERPASS::GAUSSIAN_VER);
		m_existing.push_back(RENDERPASS::DEPTH_SHADOW_POINT);


		RenderPass rp0(RENDERPASS::DEPTH_LIGHT_PASS, w, h, device);
		m_renderPasses.insert({ RENDERPASS::DEPTH_LIGHT_PASS, rp0 });
		RenderPass rp1(RENDERPASS::DEPTH_SHADOW_DIR, (size_t)w, (size_t)h, device);
		m_renderPasses.insert({ RENDERPASS::DEPTH_SHADOW_DIR, rp1 });
		RenderPass rp2(RENDERPASS::DEPTH_SHADOW_POINT, (size_t)w, (size_t)h, device);
		m_renderPasses.insert({ RENDERPASS::DEPTH_SHADOW_POINT, rp2 });
		RenderPass rp3(RENDERPASS::LIGHT_CULL_PASS, w, h, device);
		m_renderPasses.insert({ RENDERPASS::LIGHT_CULL_PASS, rp3 });
		RenderPass rp6(RENDERPASS::BASIC_PASS_SINGLE, w, h, device);
		m_renderPasses.insert({ RENDERPASS::BASIC_PASS_SINGLE, rp6 });;
		RenderPass rp7(RENDERPASS::DOWN_SAMPLE_EMISSIVE_PASS, w, h, device);
		m_renderPasses.insert({ RENDERPASS::DOWN_SAMPLE_EMISSIVE_PASS, rp7 });
		RenderPass rp9(RENDERPASS::GAUSSIAN_HOR, w, h, device);
		m_renderPasses.insert({ RENDERPASS::GAUSSIAN_HOR, rp9 });
		RenderPass rp10(RENDERPASS::GAUSSIAN_VER, w, h, device);
		m_renderPasses.insert({ RENDERPASS::GAUSSIAN_VER, rp10 });
		RenderPass rp14(RENDERPASS::UP_SAMPLE_EMISSIVE_PASS, w, h, device);
		m_renderPasses.insert({ RENDERPASS::UP_SAMPLE_EMISSIVE_PASS, rp14 });
		RenderPass rp16(RENDERPASS::BASIC_PASS_QUAD, w, h, device);
		m_renderPasses.insert({ RENDERPASS::BASIC_PASS_QUAD, rp16 });
		break;

	}
	case SCENE::PARTICLE:
	{

		m_existing.push_back(RENDERPASS::DEPTH_LIGHT_PASS);
		m_existing.push_back(RENDERPASS::LIGHT_CULL_PASS);
		m_existing.push_back(RENDERPASS::DYNAMIC_CUBE_MAP_PASS);
		m_existing.push_back(RENDERPASS::EDGE_DETECTION_PASS);
		m_existing.push_back(RENDERPASS::BASIC_PASS_SINGLE);
		m_existing.push_back(RENDERPASS::BASIC_PASS_TRANSPARENT);
		m_existing.push_back(RENDERPASS::FXAA);
		
		
		RenderPass rp0(RENDERPASS::DEPTH_LIGHT_PASS, w, h, device);
		m_renderPasses.insert({ RENDERPASS::DEPTH_LIGHT_PASS, rp0 });
		RenderPass rp3(RENDERPASS::LIGHT_CULL_PASS, w, h, device);
		m_renderPasses.insert({ RENDERPASS::LIGHT_CULL_PASS, rp3 });
		RenderPass rp4(RENDERPASS::DYNAMIC_CUBE_MAP_PASS, w, h, device);
		m_renderPasses.insert({ RENDERPASS::DYNAMIC_CUBE_MAP_PASS, rp4 });
		RenderPass rp5(RENDERPASS::EDGE_DETECTION_PASS, w, h, device);
		m_renderPasses.insert({ RENDERPASS::EDGE_DETECTION_PASS, rp5 });
		RenderPass rp6(RENDERPASS::BASIC_PASS_SINGLE, w, h, device);
		m_renderPasses.insert({ RENDERPASS::BASIC_PASS_SINGLE, rp6 });
		RenderPass rp7(RENDERPASS::BASIC_PASS_TRANSPARENT, w, h, device);
		m_renderPasses.insert({ RENDERPASS::BASIC_PASS_TRANSPARENT, rp7 });
		RenderPass rp16(RENDERPASS::FXAA, w, h, device);
		m_renderPasses.insert({ RENDERPASS::FXAA, rp16 });
		
		break;

	}
	case SCENE::ANIMATION:
	{

		m_existing.push_back(RENDERPASS::DEPTH_LIGHT_PASS);
		m_existing.push_back(RENDERPASS::LIGHT_CULL_PASS);
		m_existing.push_back(RENDERPASS::BASIC_PASS_SINGLE);
		m_existing.push_back(RENDERPASS::BASIC_PASS_QUAD);
		


		RenderPass rp0(RENDERPASS::DEPTH_LIGHT_PASS, w, h, device);
		m_renderPasses.insert({ RENDERPASS::DEPTH_LIGHT_PASS, rp0 });
		RenderPass rp3(RENDERPASS::LIGHT_CULL_PASS, w, h, device);
		m_renderPasses.insert({ RENDERPASS::LIGHT_CULL_PASS, rp3 });
		
		RenderPass rp6(RENDERPASS::BASIC_PASS_SINGLE, w, h, device);
		m_renderPasses.insert({ RENDERPASS::BASIC_PASS_SINGLE, rp6 });
		RenderPass rp7(RENDERPASS::BASIC_PASS_QUAD, w, h, device);
		m_renderPasses.insert({ RENDERPASS::BASIC_PASS_QUAD, rp7 });

		break;

	}
	}
	
}

void RenderPassHandler::reCreate(ID3D11Device* device, size_t w, size_t h, const SCENE& scene)
{
	m_existing.clear();
	m_renderPasses.clear();
	init(device, w, h, scene);
}

RenderPass& RenderPassHandler::getRenderPass(RENDERPASS type)
{
	
	return m_renderPasses[type];
	
}

bool RenderPassHandler::renderPassExists(RENDERPASS type)
{
	if (std::find(m_existing.begin(), m_existing.end(), type) == m_existing.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}
