#include "stdafx.h"
#include "UpdateDrawClass.h"

void UpdateDrawClass::resetRTVtoMain(ID3D11DeviceContext* context, const D3D11_VIEWPORT& viewPort)
{
	context->RSSetViewports(1, &viewPort);
	context->OMSetRenderTargets(1, &m_mainRTV, m_mainDSV);
	context->ClearRenderTargetView(m_mainRTV, m_DEFAULT_BG_COLOR);
	context->ClearDepthStencilView(m_mainDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetDepthStencilState(m_mainDSS, 1);
	context->OMSetBlendState(m_blendOff, m_mask, 0xffffffff);
}

void UpdateDrawClass::bindNull(SHADER_BIND_FLAG flag, std::vector<int> slotsSRV, std::vector<int> slotsUAV, ID3D11DeviceContext* context)
{
	int flag1 = (int)flag;
	if ((flag1 & (int)SHADER_BIND_FLAG::VERTEX_SHADER) != 0)
	{
		for (auto slot : slotsSRV)
		{
			context->VSSetShaderResources(slot, 1, &m_nullSRV);

		}
	}
	if ((flag1 & (int)SHADER_BIND_FLAG::PIXEL_SHADER) != 0)
	{
		for (auto slot : slotsSRV)
		{
			context->PSSetShaderResources(slot, 1, &m_nullSRV);
		}
	}
	if ((flag1 & (int)SHADER_BIND_FLAG::COMPUTE_SHADER) != 0)
	{
		for (auto slot : slotsSRV)
		{

			context->CSSetShaderResources(slot, 1, &m_nullSRV);

		}
		for (auto slot : slotsUAV)
		{

			context->CSSetUnorderedAccessViews(slot, 1, &m_nullUAV, nullptr);
		}
	}
	if ((flag1 & (int)SHADER_BIND_FLAG::HULL_SHADER) != 0)
	{
		for (auto slot : slotsSRV)
		{
			context->HSSetShaderResources(slot, 1, &m_nullSRV);
		}
	}
	if ((flag1 & (int)SHADER_BIND_FLAG::DOMAIN_SHADER) != 0)
	{
		for (auto slot : slotsSRV)
		{
			context->DSSetShaderResources(slot, 1, &m_nullSRV);
		}
	}
	if ((flag1 & (int)SHADER_BIND_FLAG::GEOMETRY_SHADER) != 0)
	{
		for (auto slot : slotsSRV)
		{
			context->GSSetShaderResources(slot, 1, &m_nullSRV);
		}
	}
}

void UpdateDrawClass::renderParticleSys(DC_PARTICLE* system, ID3D11DeviceContext* context)
{
	ShaderHandler& sh = ShaderHandler::getInstance();

	context->IASetPrimitiveTopology(system->g_topology);

	context->IASetInputLayout(sh.getParticleInputLayout());

	m_objectBfr.mapBfr(context, &system->g_objectData, sizeof(CB_OBJECT));
	m_particleBfr.mapBfr(context, &system->g_particleData, sizeof(CB_PARTICLE));
	context->VSSetConstantBuffers(CB_OBJECT_SLOT, 1, m_objectBfr.getBfr());
	context->GSSetConstantBuffers(CB_OBJECT_SLOT, 1, m_objectBfr.getBfr());
	context->VSSetConstantBuffers(CB_PARTICLE_SLOT, 1, m_particleBfr.getBfr());
	context->GSSetConstantBuffers(CB_PARTICLE_SLOT, 1, m_particleBfr.getBfr());
	switch (system->g_renderType)
	{
	default:
	{
		break;
	}
	case RENDER_TYPE::PARTICLE_FIRE_TYPE:
	{
		context->OMSetBlendState(m_blendParticle, m_mask, 0xffffffff);

		if (system->g_particleFirstRun)
		{
			context->IASetVertexBuffers(0, 1, system->g_vertexBfr.GetAddressOf(), &m_pStride, &m_offset);
		}
		else
		{
			context->IASetVertexBuffers(0, 1, system->g_extravertexBfr0.GetAddressOf(), &m_pStride, &m_offset);
		}

		context->SOSetTargets(1, system->g_extravertexBfr1.GetAddressOf(), &m_offset);

		context->VSSetShader(sh.getVS(sh.g_particleMainVS).Get(), nullptr, 0);
		context->GSSetShader(sh.getGS(sh.g_particleFireOneGS).Get(), nullptr, 0);

		if (system->g_particleFirstRun)
		{

			context->Draw(1, 0);
			system->g_particleFirstRun = false;
		}
		else
		{
			context->DrawAuto();
		}
		ID3D11Buffer* bufferArray[1] = { 0 };
		context->SOSetTargets(1, bufferArray, &m_offset);
		std::swap(system->g_extravertexBfr0, system->g_extravertexBfr1);
		context->VSSetShader(sh.getVS(sh.g_particleFireVS).Get(), nullptr, 0);
		context->GSSetShader(sh.getGS(sh.g_particleFireTwoGS).Get(), nullptr, 0);
		context->PSSetShader(sh.getPS(sh.g_particlePS).Get(), nullptr, 0);

		context->IASetVertexBuffers(0, 1, system->g_extravertexBfr0.GetAddressOf(), &m_pStride, &m_offset);

		context->DrawAuto();
		break;
	}
	case RENDER_TYPE::PARTICLE_RAIN_TYPE:
	{
		context->OMSetBlendState(m_blendOff, m_mask, 0xffffffff);



		if (system->g_particleFirstRun)
		{
			context->IASetVertexBuffers(0, 1, system->g_vertexBfr.GetAddressOf(), &m_pStride, &m_offset);
		}
		else
		{
			context->IASetVertexBuffers(0, 1, system->g_extravertexBfr0.GetAddressOf(), &m_pStride, &m_offset);
		}
		context->SOSetTargets(1, system->g_extravertexBfr1.GetAddressOf(), &m_offset);

		context->VSSetShader(sh.getVS(sh.g_particleMainVS).Get(), nullptr, 0);
		context->GSSetShader(sh.getGS(sh.g_particleRainOneGS).Get(), nullptr, 0);
		if (system->g_particleFirstRun)
		{
			context->Draw(1, 0);
			system->g_particleFirstRun = false;
		}
		else
		{
			context->DrawAuto();
		}
		ID3D11Buffer* bufferArray[1] = { 0 };
		context->SOSetTargets(1, bufferArray, &m_offset);
		std::swap(system->g_extravertexBfr0, system->g_extravertexBfr1);
		context->VSSetShader(sh.getVS(sh.g_particleRainVS).Get(), nullptr, 0);
		context->GSSetShader(sh.getGS(sh.g_particleRainTwoGS).Get(), nullptr, 0);
		context->PSSetShader(sh.getPS(sh.g_particlePS).Get(), nullptr, 0);

		context->IASetVertexBuffers(0, 1, system->g_extravertexBfr0.GetAddressOf(), &m_pStride, &m_offset);

		context->DrawAuto();
		break;
	}
	case RENDER_TYPE::PARTICLE_TEXT_TYPE:
	{
		context->OMSetBlendState(m_blendOn, m_mask, 0xffffffff);

		context->IASetVertexBuffers(0, 1, system->g_vertexBfr.GetAddressOf(), &m_pStride, &m_offset);
		context->VSSetShader(sh.getVS(sh.g_particleMainVS).Get(), nullptr, 0);
		context->GSSetShader(sh.getGS(sh.g_particleTextGS).Get(), nullptr, 0);

		context->PSSetShader(sh.getPS(sh.g_particlePS).Get(), nullptr, 0);
		context->Draw(4, 0);


		break;
	}
	case RENDER_TYPE::PARTICLE_LEAVES_TYPE:
	{
		context->OMSetBlendState(m_blendOn, m_mask, 0xffffffff);



		if (system->g_particleFirstRun)
		{
			context->IASetVertexBuffers(0, 1, system->g_vertexBfr.GetAddressOf(), &m_pStride, &m_offset);
		}
		else
		{
			context->IASetVertexBuffers(0, 1, system->g_extravertexBfr0.GetAddressOf(), &m_pStride, &m_offset);
		}
		context->SOSetTargets(1, system->g_extravertexBfr1.GetAddressOf(), &m_offset);

		context->VSSetShader(sh.getVS(sh.g_particleMainVS).Get(), nullptr, 0);
		context->GSSetShader(sh.getGS(sh.g_particleLeafOneGS).Get(), nullptr, 0);
		if (system->g_particleFirstRun)
		{
			context->Draw(1, 0);
			system->g_particleFirstRun = false;
		}
		else
		{
			context->DrawAuto();
		}
		ID3D11Buffer* bufferArray[1] = { 0 };
		context->SOSetTargets(1, bufferArray, &m_offset);
		std::swap(system->g_extravertexBfr0, system->g_extravertexBfr1);
		context->VSSetShader(sh.getVS(sh.g_particleRainVS).Get(), nullptr, 0);
		context->GSSetShader(sh.getGS(sh.g_particleLeafTwoGS).Get(), nullptr, 0);
		context->PSSetShader(sh.getPS(sh.g_particlePS).Get(), nullptr, 0);

		context->IASetVertexBuffers(0, 1, system->g_extravertexBfr0.GetAddressOf(), &m_pStride, &m_offset);

		context->DrawAuto();
		break;
	}
	}
}

void UpdateDrawClass::updateDynamicCubeMap(Vector3& t)
{
	m_camTarget[0].x += t.x;

	m_camTarget[1].x += t.x;

	m_camTarget[2].y += t.y;

	m_camTarget[3].y += t.y;

	m_camTarget[4].z += t.z;

	m_camTarget[5].z += t.z;

	for (int i = 0; i < 6; i++)
	{
		m_dynamicCubeCameras[i]->updateViewMatrix(t, m_camTarget[i], m_camUp[i]);
	}
}

UpdateDrawClass::UpdateDrawClass()
{

	

}

UpdateDrawClass::~UpdateDrawClass()
{
	RELEASE(m_mainCam);
	RELEASE(m_CSLightCullingShape);
	RELEASE(m_dynamicCubeCameras[0]);
	RELEASE(m_dynamicCubeCameras[1]);
	RELEASE(m_dynamicCubeCameras[2]);
	RELEASE(m_dynamicCubeCameras[3]);
	RELEASE(m_dynamicCubeCameras[4]);
	RELEASE(m_dynamicCubeCameras[5]);
	RELEASE(m_portal);
	RELEASE(m_quadTree);
	RELEASE(m_ocTree);
	RELEASE(m_binaryTree);
}

void UpdateDrawClass::bindResources(Shape* shape, RENDERPASS currentPass, ID3D11DeviceContext* context)
{
	if (shape==nullptr)
	{
		return;
	}
	DC_COMPUTE* tempC = dynamic_cast<DC_COMPUTE*>(shape->getDrawCall());
	DC_BASIC* tempB = dynamic_cast<DC_BASIC*>(shape->getDrawCall());
	DC_QUAD* tempQ = dynamic_cast<DC_QUAD*>(shape->getDrawCall());
	DC_SKELETON* tempSkeleton = dynamic_cast<DC_SKELETON*>(shape->getDrawCall());
	DC_PARTICLE* tempParticle = dynamic_cast<DC_PARTICLE*>(shape->getDrawCall());
	DC_GRASS* tempGrass = dynamic_cast<DC_GRASS*>(shape->getDrawCall());
	ShaderHandler& sh = ShaderHandler::getInstance();
	TextureHandler& t = TextureHandler::getInstance();
	int flag = (int)(shape->getDrawCall()->g_shaders);

	switch (currentPass)
	{
	default:
	{
		
		break;
	}
	case RENDERPASS::EDGE_DETECTION_PASS:
	{
		if (tempSkeleton)
		{
			m_objectBfr.mapBfr(context, &tempSkeleton->g_objectData, sizeof(CB_OBJECT));
			context->VSSetConstantBuffers(CB_OBJECT_SLOT, 1, m_objectBfr.getBfr());
			context->VSSetShader(sh.getVS(sh.g_celSkeletonVS).Get(), nullptr, 0);
			
		}
		else if (tempB)
		{
			if (shape->isInstanced())
			{
				context->VSSetShader(sh.getVS(sh.g_celInstanceVS).Get(), nullptr, 0);
			}
			else
			{
				m_objectBfr.mapBfr(context, &tempB->g_objectData, sizeof(CB_OBJECT));
				context->VSSetConstantBuffers(CB_OBJECT_SLOT, 1, m_objectBfr.getBfr());
				context->VSSetShader(sh.getVS(sh.g_celBasicVS).Get(), nullptr, 0);
			}
			
		}
		context->PSSetShader(sh.getPS(sh.g_celEdgePS).Get(), nullptr, 0);
		break;
	}
	case RENDERPASS::BILATERA_HOR:
	{
		context->VSSetShader(sh.getVS(sh.g_basicQuadVS).Get(), nullptr, 0);
		context->PSSetShader(sh.getPS(sh.g_bilateralHorPS).Get(), nullptr, 0);
		break;
	}
	case RENDERPASS::FXAA:
	{
		context->VSSetShader(sh.getVS(sh.g_basicQuadVS).Get(), nullptr, 0);
		context->PSSetShader(sh.getPS(sh.g_fxaa).Get(), nullptr, 0);
		break;
	}
	case RENDERPASS::BILATERA_VER:
	{
		context->VSSetShader(sh.getVS(sh.g_basicQuadVS).Get(), nullptr, 0);
		context->PSSetShader(sh.getPS(sh.g_bilateralVerPS).Get(), nullptr, 0);
		break;
	}
	case RENDERPASS::DOWN_SAMPLE_EMISSIVE_PASS:
	{
		context->VSSetShader(sh.getVS(sh.g_basicQuadVS).Get(), nullptr, 0);
		context->PSSetShader(sh.getPS(sh.g_fullQuadPS).Get(), nullptr, 0);
		break;
	}
	case RENDERPASS::DOWN_SAMPLE_RAY_MARCH_PASS:
	{
		context->VSSetShader(sh.getVS(sh.g_basicQuadVS).Get(), nullptr, 0);
		context->PSSetShader(sh.getPS(sh.g_fullQuadPS).Get(), nullptr, 0);
		break;
	}
	case RENDERPASS::RAY_MARCH:
	{
		context->VSSetShader(sh.getVS(sh.g_basicQuadVS).Get(), nullptr, 0);
		context->PSSetShader(sh.getPS(sh.g_rayMarchPS).Get(), nullptr, 0);
		break;
	}
	case RENDERPASS::UP_SAMPLE_EMISSIVE_PASS:
	{
		context->VSSetShader(sh.getVS(sh.g_basicQuadVS).Get(), nullptr, 0);
		context->PSSetShader(sh.getPS(sh.g_fullQuadPS).Get(), nullptr, 0);
		break;
	}
	case RENDERPASS::UP_SAMPLE_RAY_MARCH_PASS:
	{
		context->VSSetShader(sh.getVS(sh.g_basicQuadVS).Get(), nullptr, 0);
		context->PSSetShader(sh.getPS(sh.g_fullQuadPS).Get(), nullptr, 0);
		break;
	}
	case RENDERPASS::BASIC_PASS_SINGLE:
	{
		bindResourcesBasic(shape, context);
		break;
	}
	case RENDERPASS::BASIC_PASS_TRANSPARENT:
	{
		bindResourcesBasic(shape, context);
		break;
	}
	case RENDERPASS::GAUSSIAN_HOR:
	{
		
		context->CSSetShader(sh.getCS(sh.g_computeHorCS).Get(), nullptr, 0);
		
		break;
	}
	case RENDERPASS::GAUSSIAN_VER:
	{
		context->CSSetShader(sh.getCS(sh.g_computeVerCS).Get(), nullptr, 0);
		break;
	}
	case RENDERPASS::BASIC_PASS_QUAD:
	{
		context->VSSetShader(sh.getVS(sh.g_basicQuadVS).Get(), nullptr, 0);
		if (shape->getDrawCall()->g_renderType == RENDER_TYPE::HEAT_MAP)
		{
			context->PSSetShader(sh.getPS(sh.g_heatMap).Get(), nullptr, 0);
		}
		else
		{
			context->PSSetShader(sh.getPS(sh.g_addFramesPS).Get(), nullptr, 0);
		}
		
		break;
	}
	
	case RENDERPASS::CALC_FRUSTUM_PASS:
	{
		break;
	}
	case RENDERPASS::DYNAMIC_CUBE_MAP_PASS:
	{
		bindResourcesBasic(shape, context);
		switch (shape->getDrawCall()->g_renderType)
		{
		default:
		{
			context->GSSetShader(sh.getGS(sh.g_depthGS).Get(), nullptr, 0);
			break;
		}
		case RENDER_TYPE::BASIC:
		{
			context->GSSetShader(sh.getGS(sh.g_depthGS).Get(), nullptr, 0);
			break;
		}
		case RENDER_TYPE::DYNAMIC_SKY:
		{
			context->GSSetShader(sh.getGS(sh.g_depthGS).Get(), nullptr, 0);
			break;
		}
		
		case RENDER_TYPE::SKY:
		{
			context->GSSetShader(sh.getGS(sh.g_depthSkyGS).Get(), nullptr, 0);
			break;
		}
		case RENDER_TYPE::TERRAIN:
		{
			context->GSSetShader(sh.getGS(sh.g_terrainDepthGS).Get(), nullptr, 0);
			break;
		}
		case RENDER_TYPE::OC_TREE:
		{
			context->GSSetShader(sh.getGS(sh.g_depthGS).Get(), nullptr, 0);
			break;
		}

		}
		
		break;
	}
	case RENDERPASS::DEPTH_LIGHT_PASS:
	{
		
		bindResourcesBasic(shape, context);
		break;
	}
	case RENDERPASS::DEPTH_SHADOW_DIR:
	{
	
		bindResourcesBasic(shape, context);
		break;
	}
	case RENDERPASS::DEPTH_SHADOW_POINT:
	{
		m_shadowPointBfr.mapBfr(context, &m_shadowPointConstants, sizeof(CB_SHADOW_POINT));
		context->VSSetConstantBuffers(CB_SHADOW_POINT_SLOT, 1, m_shadowPointBfr.getBfr());
		context->GSSetConstantBuffers(CB_SHADOW_POINT_SLOT, 1, m_shadowPointBfr.getBfr());
		context->PSSetConstantBuffers(CB_SHADOW_POINT_SLOT, 1, m_shadowPointBfr.getBfr());
		bindResourcesBasic(shape, context);
		if (shape->getDrawCall()->g_renderType == RENDER_TYPE::TERRAIN)
		{
			context->GSSetShader(sh.getGS(sh.g_terrainDepthGS).Get(), nullptr, 0);
		}
		else
		{
			context->GSSetShader(sh.getGS(sh.g_depthGS).Get(), nullptr, 0);
		}
		
		break;
	}
	case RENDERPASS::LIGHT_CULL_PASS:
	{
		

		context->CSSetShader(sh.getCS(sh.g_lightCS).Get(), nullptr, 0);
		break;
	}
	}

	for (std::pair<unsigned int, std::string> tex : shape->getDrawCall()->g_srvs)
	{

		if ((flag & (int)SHADER_BIND_FLAG::VERTEX_SHADER) != 0)
		{
			if (std::find(m_nullSlotsSRV.begin(), m_nullSlotsSRV.end(), tex.first) != m_nullSlotsSRV.end())
			{
				continue;
			}
			context->VSSetShaderResources(tex.first, 1, t.getTextureSRV(tex.second));
			context->VSSetSamplers(WRAP_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::WRAP).GetAddressOf());
			context->VSSetSamplers(COMPARISON_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::COMPARISON).GetAddressOf());
			context->VSSetSamplers(CLAMP_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::CLAMP).GetAddressOf());
			context->VSSetSamplers(A_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::ANISOTROPIC).GetAddressOf());
			context->VSSetSamplers(TERRAIN_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::TERRAIN).GetAddressOf());
		}
		if ((flag & (int)SHADER_BIND_FLAG::HULL_SHADER) != 0)
		{
			if (std::find(m_nullSlotsSRV.begin(), m_nullSlotsSRV.end(), tex.first) != m_nullSlotsSRV.end())
			{
				continue;
			}
			context->HSSetShaderResources(tex.first, 1, t.getTextureSRV(tex.second));
			context->HSSetSamplers(WRAP_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::WRAP).GetAddressOf());
			context->HSSetSamplers(COMPARISON_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::COMPARISON).GetAddressOf());
			context->HSSetSamplers(CLAMP_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::CLAMP).GetAddressOf());
			context->HSSetSamplers(A_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::ANISOTROPIC).GetAddressOf());
			context->HSSetSamplers(TERRAIN_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::TERRAIN).GetAddressOf());
		}
		if ((flag & (int)SHADER_BIND_FLAG::DOMAIN_SHADER) != 0)
		{
			if (std::find(m_nullSlotsSRV.begin(), m_nullSlotsSRV.end(), tex.first) != m_nullSlotsSRV.end())
			{
				continue;
			}
			context->DSSetShaderResources(tex.first, 1, t.getTextureSRV(tex.second));
			context->DSSetSamplers(WRAP_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::WRAP).GetAddressOf());
			context->DSSetSamplers(COMPARISON_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::COMPARISON).GetAddressOf());
			context->DSSetSamplers(CLAMP_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::CLAMP).GetAddressOf());
			context->DSSetSamplers(A_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::ANISOTROPIC).GetAddressOf());
			context->DSSetSamplers(TERRAIN_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::TERRAIN).GetAddressOf());
		}
		if ((flag & (int)SHADER_BIND_FLAG::GEOMETRY_SHADER) != 0)
		{
			if (std::find(m_nullSlotsSRV.begin(), m_nullSlotsSRV.end(), tex.first) != m_nullSlotsSRV.end())
			{
				continue;
			}
			context->GSSetShaderResources(tex.first, 1, t.getTextureSRV(tex.second));
			context->GSSetSamplers(WRAP_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::WRAP).GetAddressOf());
			context->GSSetSamplers(COMPARISON_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::COMPARISON).GetAddressOf());
			context->GSSetSamplers(CLAMP_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::CLAMP).GetAddressOf());
			context->GSSetSamplers(A_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::ANISOTROPIC).GetAddressOf());
			context->GSSetSamplers(TERRAIN_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::TERRAIN).GetAddressOf());
		}
		if ((flag & (int)SHADER_BIND_FLAG::PIXEL_SHADER) != 0)
		{
			
			if (std::find(m_nullSlotsSRV.begin(), m_nullSlotsSRV.end(), tex.first) != m_nullSlotsSRV.end())
			{
				continue;
			}
			
			context->PSSetShaderResources(tex.first, 1, t.getTextureSRV(tex.second));
			context->PSSetSamplers(WRAP_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::WRAP).GetAddressOf());
			context->PSSetSamplers(COMPARISON_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::COMPARISON).GetAddressOf());
			context->PSSetSamplers(CLAMP_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::CLAMP).GetAddressOf());
			context->PSSetSamplers(A_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::ANISOTROPIC).GetAddressOf());
			context->PSSetSamplers(TERRAIN_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::TERRAIN).GetAddressOf());
		}
		if ((flag & (int)SHADER_BIND_FLAG::COMPUTE_SHADER) != 0)
		{

			if (std::find(m_nullSlotsSRV.begin(), m_nullSlotsSRV.end(), tex.first) != m_nullSlotsSRV.end())
			{
				continue;
			}
			
			context->CSSetShaderResources(tex.first, 1, t.getTextureSRV(tex.second));
			context->CSSetSamplers(WRAP_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::WRAP).GetAddressOf());
			context->CSSetSamplers(COMPARISON_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::COMPARISON).GetAddressOf());
			context->CSSetSamplers(CLAMP_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::CLAMP).GetAddressOf());
			context->CSSetSamplers(A_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::ANISOTROPIC).GetAddressOf());
			context->CSSetSamplers(TERRAIN_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::TERRAIN).GetAddressOf());
		}

	}

	if (tempC)
	{
		for (std::pair<unsigned int, std::string> tex : tempC->uavs)
		{
			if ((flag & (int)SHADER_BIND_FLAG::COMPUTE_SHADER) != 0)
			{
				if (std::find(m_nullSlotsUAV.begin(), m_nullSlotsUAV.end(), tex.first) != m_nullSlotsUAV.end())
				{
					continue;
				}
				context->CSSetUnorderedAccessViews(tex.first, 1, t.getTextureUAV(tex.second), 0);
				context->CSSetSamplers(WRAP_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::WRAP).GetAddressOf());
				context->CSSetSamplers(COMPARISON_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::COMPARISON).GetAddressOf());
				context->CSSetSamplers(CLAMP_SAMPLER, 1, t.getSampler(SAMPLER_TYPE::CLAMP).GetAddressOf());
			}
		}
	}
}

void UpdateDrawClass::bindResourcesBasic(Shape* shape, ID3D11DeviceContext* context)
{
	DC_COMPUTE* tempC = dynamic_cast<DC_COMPUTE*>(shape->getDrawCall());
	DC_BASIC* tempB = dynamic_cast<DC_BASIC*>(shape->getDrawCall());
	DC_QUAD* tempQ = dynamic_cast<DC_QUAD*>(shape->getDrawCall());
	DC_SKELETON* tempSkeleton = dynamic_cast<DC_SKELETON*>(shape->getDrawCall());
	DC_PARTICLE* tempParticle = dynamic_cast<DC_PARTICLE*>(shape->getDrawCall());
	DC_GRASS* tempGrass = dynamic_cast<DC_GRASS*>(shape->getDrawCall());
	ShaderHandler& sh = ShaderHandler::getInstance();
	TextureHandler& t = TextureHandler::getInstance();
	if (tempGrass)
	{
		m_objectBfr.mapBfr(context, &tempGrass->g_objectData, sizeof(CB_OBJECT));
		context->VSSetShader(sh.getVS(sh.g_grassVS).Get(), nullptr, 0);
		context->HSSetShader(sh.getHS(sh.g_grassHS).Get(), nullptr, 0);
		context->DSSetShader(sh.getDS(sh.g_grassDS).Get(), nullptr, 0);
		context->GSSetShader(sh.getGS(sh.g_grassGS).Get(), nullptr, 0);
		context->VSSetConstantBuffers(CB_GRASS_SLOT, 1, m_grassBfr.getBfr());
		context->HSSetConstantBuffers(CB_GRASS_SLOT, 1, m_grassBfr.getBfr());
		context->DSSetConstantBuffers(CB_GRASS_SLOT, 1, m_grassBfr.getBfr());
		context->GSSetConstantBuffers(CB_GRASS_SLOT, 1, m_grassBfr.getBfr());
		context->VSSetConstantBuffers(CB_OBJECT_SLOT, 1, m_objectBfr.getBfr());
		context->HSSetConstantBuffers(CB_OBJECT_SLOT, 1, m_objectBfr.getBfr());
		context->DSSetConstantBuffers(CB_OBJECT_SLOT, 1, m_objectBfr.getBfr());
		context->GSSetConstantBuffers(CB_OBJECT_SLOT, 1, m_objectBfr.getBfr());
		m_strandsBfr.mapBfr(context, tempGrass->g_strands.data(), sizeof(GRASS_STRAND) * tempGrass->g_strands.size());
		m_strandsBCBfr.mapBfr(context, tempGrass->g_strandsBC.data(), sizeof(XMFLOAT3) * tempGrass->g_strandsBC.size());
		m_strandsIBfr.mapBfr(context, tempGrass->g_strandsIndices.data(), sizeof(unsigned int) * tempGrass->g_strandsIndices.size());
		context->PSSetShader(sh.getPS(sh.g_grassPS).Get(), nullptr, 0);
	}
	if (tempB)
	{
		if (shape->isInstanced())
		{
			context->VSSetShader(sh.getVS(sh.g_basicInstanceVS).Get(), nullptr, 0);
		}
		else
		{

			switch (shape->getDrawCall()->g_renderType)
			{
			default:
			{
				context->VSSetShader(sh.getVS(sh.g_basicVS).Get(), nullptr, 0);
				m_objectBfr.mapBfr(context, &tempB->g_objectData, sizeof(CB_OBJECT));
				context->VSSetConstantBuffers(CB_OBJECT_SLOT, 1, m_objectBfr.getBfr());
				break;
			}
			case RENDER_TYPE::BASIC:
			{
				context->VSSetShader(sh.getVS(sh.g_basicVS).Get(), nullptr, 0);
				m_objectBfr.mapBfr(context, &tempB->g_objectData, sizeof(CB_OBJECT));
				context->VSSetConstantBuffers(CB_OBJECT_SLOT, 1, m_objectBfr.getBfr());
				break;
			}
			case RENDER_TYPE::DYNAMIC_SKY:
			{
				context->VSSetShader(sh.getVS(sh.g_basicVS).Get(), nullptr, 0);
				m_objectBfr.mapBfr(context, &tempB->g_objectData, sizeof(CB_OBJECT));
				context->VSSetConstantBuffers(CB_OBJECT_SLOT, 1, m_objectBfr.getBfr());
				break;
			}
		
			case RENDER_TYPE::SKY:
			{
				context->VSSetShader(sh.getVS(sh.g_skyVS).Get(), nullptr, 0);
				m_objectBfr.mapBfr(context, &tempB->g_objectData, sizeof(CB_OBJECT));
				context->VSSetConstantBuffers(CB_OBJECT_SLOT, 1, m_objectBfr.getBfr());
				break;
			}
			case RENDER_TYPE::TERRAIN:
			{
				context->VSSetShader(sh.getVS(sh.g_terrainVS).Get(), nullptr, 0);
				context->HSSetShader(sh.getHS(sh.g_terrainHS).Get(), nullptr, 0);
				context->DSSetShader(sh.getDS(sh.g_terrainDS).Get(), nullptr, 0);
				break;
			}
			case RENDER_TYPE::OC_TREE:
			{
				context->VSSetShader(sh.getVS(sh.g_basicVS).Get(), nullptr, 0);
				m_objectBfr.mapBfr(context, &tempB->g_objectData, sizeof(CB_OBJECT));
				context->VSSetConstantBuffers(CB_OBJECT_SLOT, 1, m_objectBfr.getBfr());
				break;
			}
			}


		}

		switch (shape->getDrawCall()->g_renderType)
		{
		default:
		{
			context->PSSetShader(sh.getPS(sh.g_PS).Get(), nullptr, 0);
			break;
		}
		case RENDER_TYPE::BASIC:
		{
			context->PSSetShader(sh.getPS(sh.g_PS).Get(), nullptr, 0);
			break;
		}
		case RENDER_TYPE::DYNAMIC_SKY:
		{
			context->PSSetShader(sh.getPS(sh.g_dynamicSkyPS).Get(), nullptr, 0);
			break;
		}
		
		case RENDER_TYPE::SKY:
		{
			context->PSSetShader(sh.getPS(sh.g_skyPS).Get(), nullptr, 0);
			break;
		}
		case RENDER_TYPE::TERRAIN:
		{
			context->PSSetShader(sh.getPS(sh.g_terrainPS).Get(), nullptr, 0);
			break;
		}
		case RENDER_TYPE::OC_TREE:
		{
			context->PSSetShader(sh.getPS(sh.g_PS).Get(), nullptr, 0);
			break;
		}
		}


	}
	else if (tempQ)
	{
		context->VSSetShader(sh.getVS(sh.g_basicQuadVS).Get(), nullptr, 0);

#if DEBUG_HEAT_MAP
		context->PSSetShader(sh.getPS(sh.g_heatMap).Get(), nullptr, 0);
#else
		context->PSSetShader(sh.getPS(sh.g_fullQuadPS).Get(), nullptr, 0);
#endif // DEBUG_HEAT_MAP


	}
	else if (tempSkeleton)
	{
		Skeleton* s = dynamic_cast<Skeleton*>(tempSkeleton->g_animations[tempSkeleton->g_currentTrack]);
		m_skeletonBfr.mapBfr(context, s->getBones(), sizeof(XMFLOAT4X4)* MAX_BONES);
		context->VSSetShader(sh.getVS(sh.g_skeleton).Get(), nullptr, 0);
		m_objectBfr.mapBfr(context, &tempSkeleton->g_objectData, sizeof(CB_OBJECT));
		context->VSSetConstantBuffers(CB_OBJECT_SLOT, 1, m_objectBfr.getBfr());


		context->PSSetShader(sh.getPS(sh.g_PS).Get(), nullptr, 0);
	}
}



void UpdateDrawClass::initPortalCulling()
{
	
	m_portal = new Portal;
	m_portal->createHiearchy(m_portalShapes);
	
}

void UpdateDrawClass::initBinaryTree(Shape* shape, ID3D11Device* device)
{
	m_binaryTree = new BSP;
	m_binaryTree->init(shape, device);
	for (auto shape : m_binaryTree->getAllShapes())
	{
		int flag = (int)shape->getDrawCall()->g_renderPassTypes;
		if ((flag & (int)RENDERPASS::DEPTH_LIGHT_PASS) != 0)
		{
			RenderPassHandler::getInstance().getRenderPass(RENDERPASS::DEPTH_LIGHT_PASS).setShape(shape);
		}
		if ((flag & (int)RENDERPASS::BASIC_PASS_SINGLE) != 0)
		{
			RenderPassHandler::getInstance().getRenderPass(RENDERPASS::BASIC_PASS_SINGLE).setShape(shape);
		}
		if ((flag & (int)RENDERPASS::BASIC_PASS_QUAD) != 0)
		{
			RenderPassHandler::getInstance().getRenderPass(RENDERPASS::BASIC_PASS_QUAD).setShape(shape);
		}
		if ((flag & (int)RENDERPASS::DEPTH_SHADOW_DIR) != 0)
		{
			RenderPassHandler::getInstance().getRenderPass(RENDERPASS::DEPTH_SHADOW_DIR).setShape(shape);
		}
		if ((flag & (int)RENDERPASS::DEPTH_SHADOW_POINT) != 0)
		{
			RenderPassHandler::getInstance().getRenderPass(RENDERPASS::DEPTH_SHADOW_POINT).setShape(shape);
		}
		if ((flag & (int)RENDERPASS::DYNAMIC_CUBE_MAP_PASS) != 0)
		{
			RenderPassHandler::getInstance().getRenderPass(RENDERPASS::DYNAMIC_CUBE_MAP_PASS).setShape(shape);
		}
		if ((flag & (int)RENDERPASS::GAUSSIAN_VER) != 0)
		{
			RenderPassHandler::getInstance().getRenderPass(RENDERPASS::GAUSSIAN_VER).setShape(shape);
		}
		if ((flag & (int)RENDERPASS::GAUSSIAN_HOR) != 0)
		{
			RenderPassHandler::getInstance().getRenderPass(RENDERPASS::GAUSSIAN_HOR).setShape(shape);
		}
	}

}

void UpdateDrawClass::renderBinaryTree(BINARY_NODE* node)
{
	
	if (!node)
	{
		return;
	}

	if (node->shape && m_mainCam->cull(node->aabb))
	{
		if (node->shape)
		{
			node->shape->g_render = false;

		}
		return;
	}
	
	Vector4 eye4 = m_mainCam->getTransAsV4();
	Vector3 eye = XMFLOAT3(eye4.x, eye4.y, eye4.z);

	int result = m_binaryTree->classifyCamera(eye, node->plane);

	if (result > 0)
	{
		renderBinaryTree(node->backchild);

		if (node->shape)
		{
			node->shape->g_render = true;

		}
		renderBinaryTree(node->frontchild);

	}
	else if (result < 0)
	{
		renderBinaryTree(node->frontchild);

		if (node->shape)
		{
			node->shape->g_render = true;

		}
		renderBinaryTree(node->backchild);
	}
	else // result is 0
	{
		// the eye point is on the partition plane...
		renderBinaryTree(node->frontchild);
		renderBinaryTree(node->backchild);
	}
}





void UpdateDrawClass::renderPortal()
{
	for (auto node : m_portal->getNodes())
	{
		for (auto portal : node.second->portals)
		{
			bool visible = true;
			
			for (int plane = 0; plane< 6; plane++)
			{
				if (plane == 4)
				{
					continue;
				}
				visible = m_portal->isVisible(m_mainCam->getFrustumPlanes()[plane], portal);
				if (!visible)
				{
					break;
				}
			}
			if (visible)
			{
				for (auto o : node.second->objects)
				{
					o->g_render = true;
				}
				
			}
			else
			{
				for (auto o : node.second->objects)
				{
					o->g_render = false;
				}
			}
		}
	}
}

void UpdateDrawClass::initQuadTree(ID3D11Device* device)
{
	m_quadTree = new QuadTree;
	m_quadTreeShapes[0]->generateBounds();
	m_quadTree->setWorldBounds(m_quadTreeShapes[0]->getBB().getBounds().aabb);
	std::vector<Shape*> shapes;
	shapes.push_back(m_quadTreeShapes[0]);
	m_quadTree->setShapes(shapes);
	m_quadTree->buildHiearchy(device);
	for (auto shape : m_quadTree->getNewShapes())
	{
		int flag = (int)shape->getDrawCall()->g_renderPassTypes;
		if ((flag & (int)RENDERPASS::DEPTH_LIGHT_PASS) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::DEPTH_LIGHT_PASS))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::DEPTH_LIGHT_PASS).setShape(shape);
			}
			
		}
		if ((flag & (int)RENDERPASS::BASIC_PASS_SINGLE) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::BASIC_PASS_SINGLE))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::BASIC_PASS_SINGLE).setShape(shape);
			}
		}
		if ((flag & (int)RENDERPASS::BASIC_PASS_QUAD) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::BASIC_PASS_QUAD))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::BASIC_PASS_QUAD).setShape(shape);
			}
		}
		if ((flag & (int)RENDERPASS::DEPTH_SHADOW_DIR) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::DEPTH_SHADOW_DIR))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::DEPTH_SHADOW_DIR).setShape(shape);
			}
		}
		if ((flag & (int)RENDERPASS::DEPTH_SHADOW_POINT) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::DEPTH_SHADOW_POINT))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::DEPTH_SHADOW_POINT).setShape(shape);
			}
		}
		if ((flag & (int)RENDERPASS::DYNAMIC_CUBE_MAP_PASS) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::DYNAMIC_CUBE_MAP_PASS))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::DYNAMIC_CUBE_MAP_PASS).setShape(shape);
			}
		}
		if ((flag & (int)RENDERPASS::GAUSSIAN_VER) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::GAUSSIAN_VER))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::GAUSSIAN_VER).setShape(shape);
			}
		}
		if ((flag & (int)RENDERPASS::BASIC_PASS_TRANSPARENT) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::BASIC_PASS_TRANSPARENT))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::BASIC_PASS_TRANSPARENT).setShape(shape);
			}

		}
		if ((flag & (int)RENDERPASS::GAUSSIAN_HOR) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::GAUSSIAN_HOR))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::GAUSSIAN_HOR).setShape(shape);
			}
		}
	}
	
	for (auto shape : m_quadTree->getGrassShapes())
	{
		int flag = (int)shape->getDrawCall()->g_renderPassTypes;
		if ((flag & (int)RENDERPASS::DEPTH_LIGHT_PASS) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::DEPTH_LIGHT_PASS))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::DEPTH_LIGHT_PASS).setShape(shape);
			}

		}
		if ((flag & (int)RENDERPASS::BASIC_PASS_TRANSPARENT) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::BASIC_PASS_TRANSPARENT))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::BASIC_PASS_TRANSPARENT).setShape(shape);
			}

		}
		if ((flag & (int)RENDERPASS::BASIC_PASS_SINGLE) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::BASIC_PASS_SINGLE))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::BASIC_PASS_SINGLE).setShape(shape);
			}
		}
		if ((flag & (int)RENDERPASS::BASIC_PASS_QUAD) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::BASIC_PASS_QUAD))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::BASIC_PASS_QUAD).setShape(shape);
			}
		}
		if ((flag & (int)RENDERPASS::DEPTH_SHADOW_DIR) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::DEPTH_SHADOW_DIR))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::DEPTH_SHADOW_DIR).setShape(shape);
			}
		}
		if ((flag & (int)RENDERPASS::DEPTH_SHADOW_POINT) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::DEPTH_SHADOW_POINT))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::DEPTH_SHADOW_POINT).setShape(shape);
			}
		}
		if ((flag & (int)RENDERPASS::DYNAMIC_CUBE_MAP_PASS) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::DYNAMIC_CUBE_MAP_PASS))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::DYNAMIC_CUBE_MAP_PASS).setShape(shape);
			}
		}
		if ((flag & (int)RENDERPASS::GAUSSIAN_VER) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::GAUSSIAN_VER))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::GAUSSIAN_VER).setShape(shape);
			}
		}
		if ((flag & (int)RENDERPASS::GAUSSIAN_HOR) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::GAUSSIAN_HOR))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::GAUSSIAN_HOR).setShape(shape);
			}
		}
	}
}

void UpdateDrawClass::initOcTree(ID3D11Device* device, ID3D11DeviceContext* context)
{
	m_ocTree = new OcTree;
	std::vector<Bounds> bounds;
	for (auto shape : m_ocTreeShapes)
	{
		shape->generateBounds();
		bounds.push_back(shape->getBB().getBounds());
	}
	BoundingBoxHandler bb;
	bb.calcBounds(bounds);
	m_ocTree->setWorldBounds(bb.getBounds().aabb);
	m_ocTree->setShapes(m_ocTreeShapes);
	m_ocTree->buildHiearchy(device);
	if (m_boundingBoxesVis)
	{
		Shape* sBB = m_ocTree->visualizeBoundingBoxes(m_boundingBoxesVis, device, context);
		m_boundingBoxesVis = sBB;
	}
	
	if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::BASIC_PASS_SINGLE))
	{
		RenderPassHandler::getInstance().getRenderPass(RENDERPASS::BASIC_PASS_SINGLE).setShape(m_boundingBoxesVis);
	}
	for (auto shape : m_ocTree->getNewShapes())
	{
		int flag = (int)shape->getDrawCall()->g_renderPassTypes;
		if ((flag & (int)RENDERPASS::DEPTH_LIGHT_PASS) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::DEPTH_LIGHT_PASS))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::DEPTH_LIGHT_PASS).setShape(shape);
			}

		}
	
		if ((flag & (int)RENDERPASS::BASIC_PASS_SINGLE) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::BASIC_PASS_SINGLE))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::BASIC_PASS_SINGLE).setShape(shape);
			}
		}
		if ((flag & (int)RENDERPASS::BASIC_PASS_QUAD) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::BASIC_PASS_QUAD))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::BASIC_PASS_QUAD).setShape(shape);
			}
		}
		if ((flag & (int)RENDERPASS::DEPTH_SHADOW_DIR) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::DEPTH_SHADOW_DIR))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::DEPTH_SHADOW_DIR).setShape(shape);
			}
		}
		if ((flag & (int)RENDERPASS::DEPTH_SHADOW_POINT) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::DEPTH_SHADOW_POINT))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::DEPTH_SHADOW_POINT).setShape(shape);
			}
		}
		if ((flag & (int)RENDERPASS::DYNAMIC_CUBE_MAP_PASS) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::DYNAMIC_CUBE_MAP_PASS))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::DYNAMIC_CUBE_MAP_PASS).setShape(shape);
			}
		}
	
		if ((flag & (int)RENDERPASS::BASIC_PASS_TRANSPARENT) != 0)
		{
			if (RenderPassHandler::getInstance().renderPassExists(RENDERPASS::BASIC_PASS_TRANSPARENT))
			{
				RenderPassHandler::getInstance().getRenderPass(RENDERPASS::BASIC_PASS_TRANSPARENT).setShape(shape);
			}

		}
		
		
	}
}

void UpdateDrawClass::renderQuadTree(QUADTREE_NODE* node)
{
	if (node)
	{
		

		if (m_mainCam->cull(node->bounds))
		{
			for (int s = 0; s < node->shapes.size(); s++)
			{

				node->shapes[s]->g_render = false;
			}
			if (node->grass)
			{
				node->grass->g_render = false;
			}
			
			return;

		}

		int count = 0;
		for (int i = 0; i < 4; i++)
		{
			if (node->children[i] != 0)
			{
				count++;
				renderQuadTree(node->children[i]);
			}
		}
		if (count != 0)
		{
			return;
		}

		for (int s = 0; s < node->shapes.size(); s++)
		{
			
			node->shapes[s]->g_render = true;
		}
		node->grass->g_render = true;
	}
}

void UpdateDrawClass::renderOcTree(OCTREE_NODE* node)
{
	
	if (node)
	{


		if (m_mainCam->cull(node->bounds))
		{
			for (int s = 0; s < node->shapes.size(); s++)
			{

				node->shapes[s]->g_render = false;
			}
			if (m_triangleCount.find(node->ID) != m_triangleCount.end())
			{
				m_triangleCount.erase(node->ID);
			}
			findChildrenOcTree(node);
			return;

		}

		int count = 0;
		for (int i = 0; i < 8; i++)
		{
			if (node->children[i] != 0)
			{
				count++;
				renderOcTree(node->children[i]);
			}
		}
		if (count != 0)
		{
			return;
		}

		for (int s = 0; s < node->shapes.size(); s++)
		{

			node->shapes[s]->g_render = true;
		}
		if (m_triangleCount.find(node->ID) == m_triangleCount.end())
		{
			m_triangleCount.insert({ node->ID, node->trianglesInNode });
		}
		
	}
}

void UpdateDrawClass::findChildrenOcTree(OCTREE_NODE* node)
{
	if (m_triangleCount.find(node->ID) != m_triangleCount.end())
	{
		m_triangleCount.erase(node->ID);
	}
	for (int i = 0; i < 8; i++)
	{
		if (node->children[i] != 0)
		{
			findChildrenOcTree(node->children[i]);
			
		}
	}
}

void UpdateDrawClass::initBfrs(ID3D11Device* device, size_t currResX, size_t currResY, SCENE scene, bool firstTime)
{
	RELEASE(m_mainCam);
	m_mainCam = new Camera;
	m_mainCam->updateProjectionMatrix((float)currResX, (float)currResY, 60.0f);
	m_sceneConstants.g_cameraPosition = m_mainCam->getTransAsV4();
	m_sceneConstants.g_vp = m_mainCam->getVP();
	m_sceneConstants.g_lightCount.x = 0;
	if (m_directionalLight)
	{
		m_sceneConstants.g_dirLightColor = m_directionalLight->getData().color;
		m_sceneConstants.g_dirLightDirection = m_directionalLight->getData().direction;
		m_sceneConstants.g_dirLightOtherConstants = m_directionalLight->getData().otherConstants;
		m_sceneConstants.g_lvpt = m_directionalLight->getProjectiveTexCoord();
	}
	if (scene == SCENE::PARTICLE)
	{
		m_sceneConstants.g_booleans.x = 1;
	}
	else
	{
		m_sceneConstants.g_booleans.x = 0;
	}
	m_sceneConstants.g_screenDimenstions.x = (UINT)currResX;
	m_sceneConstants.g_screenDimenstions.y = (UINT)currResY;
	m_sceneConstants.g_screenDimenstions.z = (UINT)(1.0f/(float)currResX);
	m_sceneConstants.g_screenDimenstions.w = (UINT)(1.0f/(float)currResY);
	m_sceneConstants.g_inverseProjection = m_mainCam->getProjectionMatrix().Invert();
	m_sceneConstants.g_view = m_mainCam->getViewMatrix();
	m_sceneConstants.g_projection = m_mainCam->getProjectionMatrix();
	m_mainCam->setTransformStatus(TRANSFORM_TYPE::DYNAMIC);
	m_player.setCam(m_mainCam);
	CB_GRASS grassConstants;
	grassConstants.g_grassConstants.x = 100;
	grassConstants.g_grassConstants.y = 100;
	if (firstTime)
	{
		m_grassBfr.createBfr(device, sizeof(CB_GRASS), &grassConstants);
	}
	else
	{
		m_grassBfr.reCreateBfr(device, sizeof(CB_GRASS), &grassConstants);
	}


	m_dynamicCubeCameras[0] = new Camera;
	m_dynamicCubeCameras[0]->updateProjectionMatrix(DYNAMIC_CUBE_MAP_RES, DYNAMIC_CUBE_MAP_RES, 90);
	m_dynamicCubeCameras[1] = new Camera;
	m_dynamicCubeCameras[1]->updateProjectionMatrix(DYNAMIC_CUBE_MAP_RES, DYNAMIC_CUBE_MAP_RES, 90);
	m_dynamicCubeCameras[2] = new Camera;
	m_dynamicCubeCameras[2]->updateProjectionMatrix(DYNAMIC_CUBE_MAP_RES, DYNAMIC_CUBE_MAP_RES, 90);
	m_dynamicCubeCameras[3] = new Camera;
	m_dynamicCubeCameras[3]->updateProjectionMatrix(DYNAMIC_CUBE_MAP_RES, DYNAMIC_CUBE_MAP_RES, 90);
	m_dynamicCubeCameras[4] = new Camera;
	m_dynamicCubeCameras[4]->updateProjectionMatrix(DYNAMIC_CUBE_MAP_RES, DYNAMIC_CUBE_MAP_RES, 90);
	m_dynamicCubeCameras[5] = new Camera;
	m_dynamicCubeCameras[5]->updateProjectionMatrix(DYNAMIC_CUBE_MAP_RES, DYNAMIC_CUBE_MAP_RES, 90);
	m_camUp[0] = XMFLOAT3(0, 1, 0);
	m_camUp[1] = XMFLOAT3(0, 1, 0);
	m_camUp[2] = XMFLOAT3(0, 0, -1);
	m_camUp[3] = XMFLOAT3(0, 0, 1);
	m_camUp[4] = XMFLOAT3(0, 1, 0);
	m_camUp[5] = XMFLOAT3(0, 1, 0);
	m_camTarget[0] = XMFLOAT3(1, 0, 0);
	m_camTarget[1] = XMFLOAT3(-1, 0, 0);
	m_camTarget[2] = XMFLOAT3(0, 1, 0);
	m_camTarget[3] = XMFLOAT3(0, -1, 0);
	m_camTarget[4] = XMFLOAT3(0, 0, 1);
	m_camTarget[5] = XMFLOAT3(0, 0, -1);


	CB_OBJECT temp;
	Matrix id = XMMatrixIdentity();
	temp.g_inverseWorld = id;
	temp.g_world = id;
	if (firstTime)
	{
		m_objectBfr.createBfr(device, sizeof(CB_OBJECT), &temp);
		m_sceneBfr.createBfr(device, sizeof(CB_SCENE), &m_sceneConstants);
	}
	else
	{
		m_objectBfr.reCreateBfr(device, sizeof(CB_OBJECT), &temp);
		m_sceneBfr.reCreateBfr(device, sizeof(CB_SCENE), &m_sceneConstants);
	}
	

	UINT screenWidth = (UINT)currResX;
	UINT screenHeight = (UINT)currResY;
	int lightCullingBlockSize = LIGHT_CULLING_BLOCK_SIZE;
	XMUINT4 numThreadGroups = XMUINT4(UINT(std::ceil((float)screenWidth / (float)lightCullingBlockSize)), UINT(std::ceil((float)screenHeight / (float)lightCullingBlockSize)), 1, 1);
	XMUINT4 numThreads = XMUINT4(numThreadGroups.x * lightCullingBlockSize, numThreadGroups.y * lightCullingBlockSize, 1, 1);
	

	TextureHandler& t = TextureHandler::getInstance();
	D3D11_TEXTURE2D_DESC textureDescTemp = {};
	ID3D11Texture2D* depthTex2D = nullptr;
	textureDescTemp.Width = (UINT)numThreadGroups.x;
	textureDescTemp.Height = (UINT)numThreadGroups.y;
	textureDescTemp.MipLevels = 1;
	textureDescTemp.ArraySize = 1;
	textureDescTemp.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDescTemp.SampleDesc.Count = 1;
	textureDescTemp.Usage = D3D11_USAGE_DEFAULT;
	textureDescTemp.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	textureDescTemp.CPUAccessFlags = 0;
	textureDescTemp.MiscFlags = 0;
	HRESULT HR = device->CreateTexture2D(&textureDescTemp, NULL, &depthTex2D);
	assert(SUCCEEDED(HR));
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDescTemp = {};
	uavDescTemp.Format = textureDescTemp.Format;
	uavDescTemp.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDescTemp = {};
	srvDescTemp.Format = textureDescTemp.Format;
	srvDescTemp.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDescTemp.Texture2D.MostDetailedMip = 0;
	srvDescTemp.Texture2D.MipLevels = textureDescTemp.MipLevels;;
	t.addTextureSRV("heat", depthTex2D, device, srvDescTemp);
	t.addTextureUAV("heat", depthTex2D, device, uavDescTemp);
	RELEASE_COM(depthTex2D);

	int size = numThreadGroups.x * numThreadGroups.y * numThreadGroups.z * 200;

	std::vector<UINT> lightIndices;
	lightIndices.resize(size);
	for (auto lightI : lightIndices)
	{
		lightI = 0;
	}
	if (firstTime)
	{
		m_lightIndexBfr.createBfr(device, sizeof(UINT), lightIndices.data(), false, (UINT)lightIndices.size(), true, true);
	}
	else
	{
		m_lightIndexBfr.reCreateBfr(device, sizeof(UINT), lightIndices.data(), false, (UINT)lightIndices.size(), true, true);
	}
	
	
	t.addTextureUAV("indexList", m_lightIndexBfr.getUAV());
	t.addTextureSRV("indexList", m_lightIndexBfr.getSRV());
	unsigned int counter = 0;
	std::vector<unsigned int> counterV;
	counterV.push_back(counter);
	if (firstTime)
	{
		m_lightCounterBfr.createBfr(device, sizeof(unsigned int), counterV.data(), false, (UINT)counterV.size(), false, true);
	}
	else
	{
		m_lightCounterBfr.reCreateBfr(device, sizeof(unsigned int), counterV.data(), false, (UINT)counterV.size(), false, true);
	}


	t.addTextureUAV("lightCounter", m_lightCounterBfr.getUAV());
	ID3D11Texture2D* tex2D = nullptr;;
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = numThreadGroups.x;
	desc.Height = numThreadGroups.y;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32_UINT;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	device->CreateTexture2D(&desc, NULL, &tex2D);
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc1 = {};
	uavDesc1.Format = desc.Format;
	uavDesc1.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	t.addTextureUAV("lightGrid", tex2D, device, uavDesc1);
	t.addTextureSRV("lightGrid", tex2D, device, srvDesc);
	RELEASE_COM(tex2D);
	std::vector<LIGHT_DATA> tempLights(MAX_LIGHTS);
	if (firstTime)
	{
		m_lightBfr.createBfr(device, sizeof(LIGHT_DATA), tempLights.data(), true, MAX_LIGHTS);
	}
	else
	{
		m_lightBfr.reCreateBfr(device, sizeof(LIGHT_DATA), tempLights.data(), true, MAX_LIGHTS);
	}
	
	t.addTextureSRV("light", m_lightBfr.getSRV());
	m_CSLightCullingShape = new Shape(device, DC_TYPE::COMPUTE_SHADER, RENDERPASS::LIGHT_CULL_PASS);
	DC_COMPUTE* tempC = dynamic_cast<DC_COMPUTE*>(m_CSLightCullingShape->getDrawCall());
	tempC->g_dispatchThreads.x = numThreadGroups.x;
	tempC->g_dispatchThreads.y = numThreadGroups.y;
	tempC->g_dispatchThreads.z = numThreadGroups.z;

	std::vector<XMFLOAT4X4> tempMatrices(MAX_BONES);
	if (firstTime)
	{
		m_skeletonBfr.createBfr(device, sizeof(XMFLOAT4X4), tempMatrices.data(), true, (UINT)tempMatrices.size());
	}
	else
	{
		m_skeletonBfr.reCreateBfr(device, sizeof(XMFLOAT4X4), tempMatrices.data(), true, (UINT)tempMatrices.size());
	}
	

	t.addTextureSRV("skeleton", m_skeletonBfr.getSRV());
	if (firstTime)
	{
		m_shadowPointBfr.createBfr(device, sizeof(CB_SHADOW_POINT), &m_shadowPointConstants);
	}
	else
	{
		m_shadowPointBfr.reCreateBfr(device, sizeof(CB_SHADOW_POINT), &m_shadowPointConstants);
	}


	GRASS_STRAND s;


	std::vector<GRASS_STRAND> strands(3000, s);

	std::vector<unsigned int> baseI(6000, 0);

	std::vector<XMFLOAT3> bc((2000) * 64, XMFLOAT3(0, 0, 0));

	if (firstTime)
	{
		m_strandsBfr.createBfr(device, sizeof(GRASS_STRAND), strands.data(), true, (UINT)strands.size());
		m_strandsBCBfr.createBfr(device, sizeof(XMFLOAT3), bc.data(), true, (UINT)bc.size());
		m_strandsIBfr.createBfr(device, sizeof(unsigned int), baseI.data(), true, (UINT)baseI.size());
	}
	else
	{
		m_strandsBfr.reCreateBfr(device, sizeof(GRASS_STRAND), strands.data(), true, (UINT)strands.size());
		m_strandsBCBfr.reCreateBfr(device, sizeof(XMFLOAT3), bc.data(), true, (UINT)bc.size());
		m_strandsIBfr.reCreateBfr(device, sizeof(unsigned int), baseI.data(), true, (UINT)baseI.size());
	}

	TextureHandler::getInstance().addTextureSRV("grassStrands", m_strandsBfr.getSRV());

	
	
	TextureHandler::getInstance().addTextureSRV("grassStrandsBC", m_strandsBCBfr.getSRV());

	
	
	TextureHandler::getInstance().addTextureSRV("grassStrandsIndices", m_strandsIBfr.getSRV());

	TextureHandler::getInstance().addTextureSRV("../Resources/Textures/GrassAlbedo.png", device);
	TextureHandler::getInstance().addTextureSRV("../Resources/Textures/GrassNormal.png", device);
	TextureHandler::getInstance().addTextureSRV("../Resources/Textures/GrassRough.png", device);
	TextureHandler::getInstance().addTextureSRV("../Resources/Textures/GrassAO.png", device);

	CB_PARTICLE particleTemp;
	particleTemp.g_particleConstants.x = 100;
	particleTemp.g_particleConstants.y = 100;
	if (firstTime)
	{
		m_particleBfr.createBfr(device, sizeof(CB_PARTICLE), &particleTemp, true);
	}
	else
	{
		m_particleBfr.reCreateBfr(device, sizeof(CB_PARTICLE), &particleTemp, true);
	}
	




	m_timer.start();
}

void UpdateDrawClass::setShapes(const std::vector<Shape*>& shapes, const SCENE& scene, ID3D11Device* device, ID3D11DeviceContext* context, const std::vector<Shape*>& shapesQuad, const std::vector<Shape*>& shapesOc, Shape* bsp, Shape* bb)
{
	m_shapes = shapes;
	bool initPortal = false;
	bool createQuadTree = false;
	bool createOcTree = false;
	m_dynamicSkyShape = nullptr;
	m_skyShape = nullptr;
	m_terrainShape = nullptr;
	m_boundingBoxesVis = bb;
	m_player.setType(PLAYER_TYPE::FIRST_PERSON);
	for (auto shape : shapes)
	{

		switch (shape->getDrawCall()->g_renderType)
		{
		default:
		{
			break;
		}
		case RENDER_TYPE::PLAYER_CHARACTER:
		{
			m_mainCam->setParent(shape);
			m_player.setCharacterShape(shape);
			m_player.setType(PLAYER_TYPE::THIRD_PERSON);
			m_playerShape = shape;
			break;
		}
		case RENDER_TYPE::CLOTH:
		{
			m_player.setClothShape(shape);
			break;
		}
		case RENDER_TYPE::SKY:
		{
			m_skyShape = shape;
			m_skyShape->g_renderBack = true;
			break;
		}
		case RENDER_TYPE::DYNAMIC_SKY:
		{
			m_dynamicSkyShape = shape;
			break;
		}
		case RENDER_TYPE::TERRAIN:
		{
			m_terrainShape = shape;
			break;
		}
		case RENDER_TYPE::PORTAL:
		{
			m_portalShapes.push_back(shape);

			initPortal = true;
			break;
		}
		
		}
		
		
	}
	for (auto shape : shapesOc)
	{

		switch (shape->getDrawCall()->g_renderType)
		{
		default:
		{
			break;
		}
		case RENDER_TYPE::OC_TREE:
		{
			m_ocTreeShapes.push_back(shape);
			createOcTree = true;;
			break;
		}
		}
	}
	for (auto shape : shapesQuad)
	{

		switch (shape->getDrawCall()->g_renderType)
		{
		default:
		{
			break;
		}
		case RENDER_TYPE::QUAD_TREE:
		{
			m_quadTreeShapes.push_back(shape);
			createQuadTree = true;;
			break;
		}
		}
	}
	if (bsp)
	{
		initBinaryTree(bsp, device);
	}
	if (initPortal)
	{
		initPortalCulling();
	}
	if (createQuadTree)
	{
		initQuadTree(device);
	}
	if (createOcTree)
	{
		initOcTree(device, context);
	}
	m_portalShapes.clear();
	m_ocTreeShapes.clear();
	m_quadTreeShapes.clear();
}

void UpdateDrawClass::setLights(const std::vector<Light*>& lights, ID3D11Device* device, Light* dirLight)
{
	m_directionalLight = dirLight;
	m_lights = lights;
	if (!lights.empty())
	{
		m_lightData.clear();
		for (auto l : lights)
		{
			m_lightData.push_back(l->getData());
			if (l->getTransformStatus() == TRANSFORM_TYPE::DYNAMIC)
			{
				m_hasDynamicLight = true;
			}
			if (l->getData().otherConstants.w == (float)(int)LIGHT_SHADOW_TYPE::HAS_SHADOW && l->getType() == LIGHT_TYPE::POINT)
			{
				m_shadowPointLight = l;
				m_shadowPointLight->setParent(m_mainCam);
			}
		}
		m_lightBfr.reCreateBfr(device, sizeof(LIGHT_DATA), m_lightData.data(), true, (UINT)lights.size());
		TextureHandler::getInstance().eraseSRV("light");
		TextureHandler::getInstance().addTextureSRV("light", m_lightBfr.getSRV());
		m_sceneConstants.g_lightCount.x = (UINT)lights.size();
	}

	if (dirLight)
	{
		m_sceneConstants.g_dirLightColor = dirLight->getData().color;
		m_sceneConstants.g_dirLightDirection = dirLight->getData().direction;
		m_sceneConstants.g_dirLightOtherConstants = dirLight->getData().otherConstants;
		m_sceneConstants.g_lvpt = m_directionalLight->getProjectiveTexCoord();
	}
}

void UpdateDrawClass::renderBackground(ID3D11DeviceContext* context, const D3D11_VIEWPORT& viewPort)
{
	context->RSSetViewports(1, &viewPort);
	context->RSSetState(m_mainRS);
	context->OMSetBlendState(m_blendOff, m_mask, 0xffffffff);
	context->OMSetRenderTargets(1, &m_mainRTV, m_mainDSV);
	context->ClearRenderTargetView(m_mainRTV, m_DEFAULT_BG_COLOR);
	context->ClearDepthStencilView(m_mainDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetDepthStencilState(m_mainDSS, 1);
}

void UpdateDrawClass::renderMain(ID3D11DeviceContext* context, const D3D11_VIEWPORT& viewPort, SCENE scene)
{
	
	m_timer.run();
	m_sceneConstants.g_time.y =  0.005f;
	m_sceneConstants.g_time.x = (float)m_timer.getElapsedDeltaTime();
	m_sceneConstants.g_cameraPosition = m_mainCam->getTransAsV4();
	m_sceneConstants.g_vp = m_mainCam->getVP();
	m_sceneConstants.g_view = m_mainCam->getViewMatrix();
	m_sceneConstants.g_inverseView = m_mainCam->getViewMatrix().Invert();
	if (m_directionalLight)
	{
		if (m_directionalLight->getTransformStatus() == TRANSFORM_TYPE::DYNAMIC && m_directionalLight->getData().otherConstants.w == 1)
		{
			m_sceneConstants.g_lvpt = m_directionalLight->getProjectiveTexCoord();
		}
		m_sceneConstants.g_shadowVP = m_directionalLight->getView() * m_directionalLight->getProjection();
	}
	
	m_sceneConstants.g_frustumPlanes[0] = m_mainCam->getFrustumPlanes()[0];
	m_sceneConstants.g_frustumPlanes[1] = m_mainCam->getFrustumPlanes()[1];
	m_sceneConstants.g_frustumPlanes[2] = m_mainCam->getFrustumPlanes()[2];
	m_sceneConstants.g_frustumPlanes[3] = m_mainCam->getFrustumPlanes()[3];
	m_sceneConstants.g_frustumPlanes[4] = m_mainCam->getFrustumPlanes()[4];
	m_sceneConstants.g_frustumPlanes[5] = m_mainCam->getFrustumPlanes()[5];
	m_sceneBfr.mapBfr(context, &m_sceneConstants, sizeof(CB_SCENE));
	context->VSSetConstantBuffers(CB_SCENE_SLOT, 1, m_sceneBfr.getBfr());
	context->GSSetConstantBuffers(CB_SCENE_SLOT, 1, m_sceneBfr.getBfr());
	context->PSSetConstantBuffers(CB_SCENE_SLOT, 1, m_sceneBfr.getBfr());
	context->CSSetConstantBuffers(CB_SCENE_SLOT, 1, m_sceneBfr.getBfr());
	context->HSSetConstantBuffers(CB_SCENE_SLOT, 1, m_sceneBfr.getBfr());
	context->DSSetConstantBuffers(CB_SCENE_SLOT, 1, m_sceneBfr.getBfr());
	if (m_portal)
	{
		renderPortal();
	}
	if (m_quadTree)
	{
		QUADTREE_NODE* root = dynamic_cast<QUADTREE_NODE*>(m_quadTree->getRoot());
		renderQuadTree(root);
	}
	if (m_ocTree)
	{
		OCTREE_NODE* root = dynamic_cast<OCTREE_NODE*>(m_ocTree->getRoot());
		renderOcTree(root);
	}
	if (m_binaryTree)
	{
		renderBinaryTree(m_binaryTree->g_root);
	}
	
	for (std::pair<RENDERPASS, RenderPass> rp : RenderPassHandler::getInstance().getRenderPass())
	{
		switch (rp.first)
		{
		default:
		{
			break;
		}
		case RENDERPASS::DOWN_SAMPLE_EMISSIVE_PASS:
		{
			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
			context->RSSetViewports(1, &rp.second.getViewPort());
			context->OMSetRenderTargets(1, rp.second.getRTV().GetAddressOf(), rp.second.getDSV().Get());
			context->ClearRenderTargetView(rp.second.getRTV().Get(), m_DEFAULT_BG_COLOR);
			context->ClearDepthStencilView(rp.second.getDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			break;
		}
		case RENDERPASS::DOWN_SAMPLE_RAY_MARCH_PASS:
		{
			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
			context->RSSetViewports(1, &rp.second.getViewPort());
			context->OMSetRenderTargets(1, rp.second.getRTV().GetAddressOf(), rp.second.getDSV().Get());
			context->ClearRenderTargetView(rp.second.getRTV().Get(), m_DEFAULT_BG_COLOR);
			context->ClearDepthStencilView(rp.second.getDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			break;
		}
		case RENDERPASS::GAUSSIAN_HOR:
		{
			resetRTVtoMain(context, viewPort);
			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
			m_nullSlotsSRV.push_back(SRV_SHADOW_POINT_SLOT);
			m_nullSlotsSRV.push_back(SRV_SHADOW_DIR_SLOT);
			m_nullSlotsSRV.push_back(SRV_LIGHT_GRID);
			m_nullSlotsSRV.push_back(SRV_LIGHT_INDEX);
			m_nullSlotsUAV.push_back(UAV_GAUSSIAN_VER);
			m_nullShaderBindflag = SHADER_BIND_FLAG::COMPUTE_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::PIXEL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::VERTEX_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::HULL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::DOMAIN_SHADER;
			bindNull(m_nullShaderBindflag, m_nullSlotsSRV, m_nullSlotsUAV, context);
			context->RSSetViewports(1, &rp.second.getViewPort());

			break;
		}
		case RENDERPASS::GAUSSIAN_VER:
		{
			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
			m_nullSlotsUAV.push_back(UAV_GAUSSIAN_HOR);
			m_nullSlotsSRV.push_back(SRV_GAUSSIAN_VER);
			m_nullSlotsSRV.push_back(SRV_SHADOW_POINT_SLOT);
			m_nullSlotsSRV.push_back(SRV_SHADOW_DIR_SLOT);
			m_nullSlotsSRV.push_back(SRV_LIGHT_GRID);
			m_nullSlotsSRV.push_back(SRV_LIGHT_INDEX);
			m_nullShaderBindflag = SHADER_BIND_FLAG::COMPUTE_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::PIXEL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::VERTEX_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::HULL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::DOMAIN_SHADER;
			bindNull(m_nullShaderBindflag, m_nullSlotsSRV, m_nullSlotsUAV, context);
			context->RSSetViewports(1, &rp.second.getViewPort());

			break;
		}
		case RENDERPASS::UP_SAMPLE_EMISSIVE_PASS:
		{

			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
			m_nullSlotsSRV.push_back(SRV_UP_SAMPLE);
			m_nullSlotsUAV.push_back(UAV_GAUSSIAN_VER);
			m_nullShaderBindflag = SHADER_BIND_FLAG::COMPUTE_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::PIXEL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::VERTEX_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::HULL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::DOMAIN_SHADER;
			bindNull(m_nullShaderBindflag, m_nullSlotsSRV, m_nullSlotsUAV, context);
			context->RSSetViewports(1, &rp.second.getViewPort());
			context->OMSetRenderTargets(1, rp.second.getRTV().GetAddressOf(), rp.second.getDSV().Get());
			context->ClearRenderTargetView(rp.second.getRTV().Get(), m_DEFAULT_BG_COLOR);
			context->ClearDepthStencilView(rp.second.getDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			break;
		}
		case RENDERPASS::UP_SAMPLE_RAY_MARCH_PASS:
		{

			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
			m_nullSlotsSRV.push_back(SRV_UP_SAMPLE);
			m_nullShaderBindflag = SHADER_BIND_FLAG::COMPUTE_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::PIXEL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::VERTEX_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::HULL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::DOMAIN_SHADER;
			bindNull(m_nullShaderBindflag, m_nullSlotsSRV, m_nullSlotsUAV, context);
			context->RSSetViewports(1, &rp.second.getViewPort());
			context->OMSetRenderTargets(1, rp.second.getRTV().GetAddressOf(), rp.second.getDSV().Get());
			context->ClearRenderTargetView(rp.second.getRTV().Get(), m_DEFAULT_BG_COLOR);
			context->ClearDepthStencilView(rp.second.getDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			break;
		}
		case RENDERPASS::BASIC_PASS_QUAD:
		{
			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
			resetRTVtoMain(context, viewPort);
			break;
		}
		case RENDERPASS::EDGE_DETECTION_PASS:
		{
			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
			m_nullSlotsSRV.push_back(SRV_FRAME_QUAD);
			m_nullSlotsSRV.push_back(SRV_EMISSIVE_QUAD);
			m_nullSlotsSRV.push_back(SRV_N_VIEW);
			m_nullSlotsSRV.push_back(SRV_WORLD_POS);
			m_nullSlotsUAV.push_back(UAV_LIGHT_HEAT);
			m_nullSlotsUAV.push_back(UAV_LIGHT_INDEX);
			m_nullSlotsUAV.push_back(UAV_LIGHT_GRID);
			m_nullShaderBindflag = SHADER_BIND_FLAG::COMPUTE_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::PIXEL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::VERTEX_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::HULL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::DOMAIN_SHADER;
			bindNull(m_nullShaderBindflag, m_nullSlotsSRV, m_nullSlotsUAV, context);
			RenderPass basicRender = RenderPassHandler::getInstance().getRenderPass().at(RENDERPASS::BASIC_PASS_SINGLE);

			ID3D11RenderTargetView* rtvarr[4] = { basicRender.getRTV(0).Get(),basicRender.getRTV(1).Get() ,basicRender.getRTV(2).Get(),basicRender.getRTV(3).Get() };
			context->RSSetViewports(1, &basicRender.getViewPort());
			context->OMSetRenderTargets(4, &rtvarr[0], basicRender.getDSV().Get());
			context->ClearRenderTargetView(rtvarr[0], m_DEFAULT_BG_COLOR);
			context->ClearRenderTargetView(rtvarr[1], m_DEFAULT_BG_COLOR);
			context->ClearRenderTargetView(rtvarr[2], m_DEFAULT_BG_COLOR);
			context->ClearRenderTargetView(rtvarr[3], m_DEFAULT_BG_COLOR);
			context->ClearDepthStencilView(basicRender.getDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			break;
		}
		case RENDERPASS::FXAA:
		{
			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
			resetRTVtoMain(context, viewPort);
			break;
		}
		case RENDERPASS::RAY_MARCH:
		{
			
			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
			m_nullSlotsSRV.push_back(SRV_VOL_LIGHT_QUAD);
			bindNull(m_nullShaderBindflag, m_nullSlotsSRV, m_nullSlotsUAV, context);
			context->RSSetViewports(1, &rp.second.getViewPort());
			context->OMSetRenderTargets(1, rp.second.getRTV().GetAddressOf(), rp.second.getDSV().Get());
			context->ClearRenderTargetView(rp.second.getRTV().Get(), m_DEFAULT_BG_COLOR);
			context->ClearDepthStencilView(rp.second.getDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			break;
		}
		case RENDERPASS::BILATERA_HOR:
		{

			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();

			context->RSSetViewports(1, &rp.second.getViewPort());
			context->OMSetRenderTargets(1, rp.second.getRTV().GetAddressOf(), rp.second.getDSV().Get());
			context->ClearRenderTargetView(rp.second.getRTV().Get(), m_DEFAULT_BG_COLOR);
			context->ClearDepthStencilView(rp.second.getDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			break;
		}
		case RENDERPASS::BILATERA_VER:
		{
			
			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
			context->RSSetViewports(1, &rp.second.getViewPort());
			context->OMSetRenderTargets(1, rp.second.getRTV().GetAddressOf(), rp.second.getDSV().Get());
			context->ClearRenderTargetView(rp.second.getRTV().Get(), m_DEFAULT_BG_COLOR);
			context->ClearDepthStencilView(rp.second.getDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			break;
		}
		case RENDERPASS::BASIC_PASS_SINGLE:
		{
		
			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
		

			
			if (scene != SCENE::PARTICLE)
			{
				m_nullSlotsSRV.push_back(SRV_FRAME_QUAD);
				m_nullSlotsSRV.push_back(SRV_EMISSIVE_QUAD);
				m_nullSlotsSRV.push_back(SRV_N_VIEW);
				m_nullSlotsSRV.push_back(SRV_WORLD_POS);
				m_nullSlotsUAV.push_back(UAV_LIGHT_HEAT);
				m_nullSlotsUAV.push_back(UAV_LIGHT_INDEX);
				m_nullSlotsUAV.push_back(UAV_LIGHT_GRID);
				m_nullShaderBindflag = SHADER_BIND_FLAG::COMPUTE_SHADER;
				m_nullShaderBindflag |= SHADER_BIND_FLAG::PIXEL_SHADER;
				m_nullShaderBindflag |= SHADER_BIND_FLAG::VERTEX_SHADER;
				m_nullShaderBindflag |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
				m_nullShaderBindflag |= SHADER_BIND_FLAG::HULL_SHADER;
				m_nullShaderBindflag |= SHADER_BIND_FLAG::DOMAIN_SHADER;
				bindNull(m_nullShaderBindflag, m_nullSlotsSRV, m_nullSlotsUAV, context);
				ID3D11RenderTargetView* rtvarr[4] = { rp.second.getRTV(0).Get(),rp.second.getRTV(1).Get() ,rp.second.getRTV(2).Get(),rp.second.getRTV(3).Get() };
				context->RSSetViewports(1, &rp.second.getViewPort());
				context->OMSetRenderTargets(4, &rtvarr[0], rp.second.getDSV().Get());
				context->ClearRenderTargetView(rtvarr[0], m_DEFAULT_BG_COLOR);
				context->ClearRenderTargetView(rtvarr[1], m_DEFAULT_BG_COLOR);
				context->ClearRenderTargetView(rtvarr[2], m_DEFAULT_BG_COLOR);
				context->ClearRenderTargetView(rtvarr[3], m_DEFAULT_BG_COLOR);
				context->ClearDepthStencilView(rp.second.getDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			}
			
			break;
		}
		case RENDERPASS::BASIC_PASS_TRANSPARENT:
		{

			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();




			break;
		}
		case RENDERPASS::DEPTH_LIGHT_PASS:
		{
			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
			m_nullSlotsSRV.push_back(SRV_LIGHT_DEPTH);
			m_nullShaderBindflag = SHADER_BIND_FLAG::COMPUTE_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::PIXEL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::VERTEX_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::HULL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::DOMAIN_SHADER;
			bindNull(m_nullShaderBindflag, m_nullSlotsSRV, m_nullSlotsUAV, context);
			context->RSSetViewports(1, &rp.second.getViewPort());
			context->OMSetRenderTargets(1, rp.second.getRTV().GetAddressOf(), rp.second.getDSV().Get());
			context->ClearDepthStencilView(rp.second.getDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			break;
		}
		case RENDERPASS::CALC_FRUSTUM_PASS:
		{
			break;
		}
		case RENDERPASS::DYNAMIC_CUBE_MAP_PASS:
		{
			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
			m_nullSlotsSRV.push_back(SRV_DYNAMIC_CUBE);
			m_nullSlotsUAV.push_back(UAV_LIGHT_HEAT);
			m_nullSlotsUAV.push_back(UAV_LIGHT_INDEX);
			m_nullSlotsUAV.push_back(UAV_LIGHT_GRID);
			m_nullSlotsSRV.push_back(SRV_FRAME_QUAD);
			m_nullSlotsSRV.push_back(SRV_EMISSIVE_QUAD);
			
			m_nullShaderBindflag = SHADER_BIND_FLAG::COMPUTE_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::PIXEL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::VERTEX_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::HULL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::DOMAIN_SHADER;
			bindNull(m_nullShaderBindflag, m_nullSlotsSRV, m_nullSlotsUAV, context);
			for (int i = 0; i < 6; i++)
			{
				m_shadowPointConstants.g_vpPL[i] = m_dynamicCubeCameras[i]->getVP();
			}
			
			m_shadowPointBfr.mapBfr(context, &m_shadowPointConstants, sizeof(CB_SHADOW_POINT));
			context->VSSetConstantBuffers(CB_SHADOW_POINT_SLOT, 1, m_shadowPointBfr.getBfr());
			context->GSSetConstantBuffers(CB_SHADOW_POINT_SLOT, 1, m_shadowPointBfr.getBfr());
			context->PSSetConstantBuffers(CB_SHADOW_POINT_SLOT, 1, m_shadowPointBfr.getBfr());
			context->RSSetViewports(1, &rp.second.getViewPort());
			context->OMSetRenderTargets(1, rp.second.getRTV(0).GetAddressOf(), rp.second.getDSV().Get());
			context->ClearRenderTargetView(rp.second.getRTV(0).Get(), m_DEFAULT_BG_COLOR);
			context->ClearDepthStencilView(rp.second.getDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			break;
		}
		case RENDERPASS::DEPTH_SHADOW_DIR:
		{
			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
			m_nullSlotsSRV.push_back(SRV_SHADOW_DIR_SLOT);
			m_nullShaderBindflag = SHADER_BIND_FLAG::COMPUTE_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::PIXEL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::VERTEX_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::HULL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::DOMAIN_SHADER;
			bindNull(m_nullShaderBindflag, m_nullSlotsSRV, m_nullSlotsUAV, context);
			
			Matrix vp = m_directionalLight->getView(0) * m_directionalLight->getProjection();
			m_sceneConstants.g_vp = vp;
			m_sceneBfr.mapBfr(context, &m_sceneConstants, sizeof(CB_SCENE));
			
			context->RSSetViewports(1, &rp.second.getViewPort());
			context->OMSetRenderTargets(1, rp.second.getRTV().GetAddressOf(), rp.second.getDSV().Get());
			context->ClearDepthStencilView(rp.second.getDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			break;
		}
		case RENDERPASS::DEPTH_SHADOW_POINT:
		{
			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
			m_nullSlotsSRV.push_back(SRV_SHADOW_POINT_SLOT);
			m_nullShaderBindflag = SHADER_BIND_FLAG::COMPUTE_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::PIXEL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::VERTEX_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::HULL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::DOMAIN_SHADER;
			bindNull(m_nullShaderBindflag, m_nullSlotsSRV, m_nullSlotsUAV, context);
			for (int i = 0; i < 6; i++)
			{
				Matrix vp = m_shadowPointLight->getView(i) * m_shadowPointLight->getProjection();
				m_shadowPointConstants.g_vpPL[i] = vp;
			}

		

			context->RSSetViewports(1, &rp.second.getViewPort());
			context->OMSetRenderTargets(1, rp.second.getRTV().GetAddressOf(), rp.second.getDSV().Get());
			context->ClearDepthStencilView(rp.second.getDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			break;
		}
		case RENDERPASS::LIGHT_CULL_PASS:
		{
			
			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
			
			m_nullSlotsSRV.push_back(SRV_LIGHT_INDEX);
			m_nullSlotsSRV.push_back(SRV_LIGHT_GRID);
			m_nullShaderBindflag = SHADER_BIND_FLAG::VERTEX_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::HULL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::DOMAIN_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::PIXEL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
			bindNull(m_nullShaderBindflag, m_nullSlotsSRV, m_nullSlotsUAV, context);
			context->OMSetRenderTargets(1, &m_mainRTV, m_mainDSV);
			context->ClearDepthStencilView(m_mainDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			m_sceneConstants.g_vp = m_mainCam->getVP();
			m_sceneBfr.mapBfr(context, &m_sceneConstants, sizeof(CB_SCENE));
			break;
		}
		}
		for (auto s : rp.second.getShapes())
		{
			if (s)
			{
				if (s->draw(m_mainCam, context))
				{
					
					DC_BASIC* dc0 = dynamic_cast<DC_BASIC*>(s->getDrawCall());
					DC_COMPUTE* dc1 = dynamic_cast<DC_COMPUTE*>(s->getDrawCall());
					DC_QUAD* dcQ = dynamic_cast<DC_QUAD*>(s->getDrawCall());
					DC_SKELETON* dcS = dynamic_cast<DC_SKELETON*>(s->getDrawCall());
					DC_PARTICLE* dcP = dynamic_cast<DC_PARTICLE*>(s->getDrawCall());
					DC_GRASS* tempGrass = dynamic_cast<DC_GRASS*>(s->getDrawCall());
					if (s->getName() == "lamp5") //not very general coding 
					{
						m_currInstanceCount = dc0->g_instanceCount;
					}

					if (s->g_renderBack)
					{
						context->RSSetState(m_cullNoneRS);
					}
					else if (s->g_renderWireFrame)
					{
						context->RSSetState(m_wireframeRS);
					}
					else if (s->g_renderClockWise)
					{
						context->RSSetState(m_clockWiseRS);
					}
					else
					{
						context->RSSetState(m_mainRS);
					}
					if (rp.first == RENDERPASS::DEPTH_SHADOW_DIR)
					{
						context->RSSetState(m_shadowRS);
					}
					if (rp.first == RENDERPASS::EDGE_DETECTION_PASS)
					{
						context->RSSetState(m_clockWiseRS);
					}
					if (rp.first == RENDERPASS::BASIC_PASS_TRANSPARENT)
					{
						context->OMSetBlendState(m_blendOn, m_mask, 0xffffffff);
					}
					else
					{
						context->OMSetBlendState(m_blendOff, m_mask, 0xffffffff);
					}

					bindResources(s, rp.first, context);
					
					ShaderHandler& sh = ShaderHandler::getInstance();
					if (dc0)
					{

						context->IASetPrimitiveTopology(dc0->g_topology);
						if (s->isInstanced())
						{

							context->IASetInputLayout(sh.getDefaultInstanceInputLayout());
							context->IASetVertexBuffers(0, 2, dc0->g_vertexAndInstanceBfr->GetAddressOf(), m_vInStride, m_offsetTwo);
							context->IASetIndexBuffer(dc0->g_indexBfr.Get(), DXGI_FORMAT_R32_UINT, 0);
							context->DrawIndexedInstanced(dc0->g_indexCount, dc0->g_instanceCount, 0, 0, 0);
							sh.bindNull(context);
						}
						else
						{
							switch (s->getDrawCall()->g_renderType)
							{
							default:
							{
								context->IASetInputLayout(sh.getDefaultInputLayout());
								context->IASetVertexBuffers(0, 1, dc0->g_vertexAndInstanceBfr[0].GetAddressOf(), &m_vStride, &m_offset);
								context->IASetIndexBuffer(dc0->g_indexBfr.Get(), DXGI_FORMAT_R32_UINT, 0);
								context->DrawIndexed(dc0->g_indexCount, 0, 0);
								sh.bindNull(context);
								break;
							}
							case RENDER_TYPE::BASIC:
							{
								context->IASetInputLayout(sh.getDefaultInputLayout());
								context->IASetVertexBuffers(0, 1, dc0->g_vertexAndInstanceBfr[0].GetAddressOf(), &m_vStride, &m_offset);
								context->IASetIndexBuffer(dc0->g_indexBfr.Get(), DXGI_FORMAT_R32_UINT, 0);
								context->DrawIndexed(dc0->g_indexCount, 0, 0);
								sh.bindNull(context);
								break;
							}

						
							case RENDER_TYPE::TERRAIN:
							{
								context->IASetInputLayout(sh.getTerrainInputLayout());
								context->IASetVertexBuffers(0, 1, dc0->g_vertexAndInstanceBfr[0].GetAddressOf(), &m_tStride, &m_offset);
								context->IASetIndexBuffer(dc0->g_indexBfr.Get(), DXGI_FORMAT_R32_UINT, 0);
								context->DrawIndexed(dc0->g_indexCount, 0, 0);
								sh.bindNull(context);
								break;
							}
							case RENDER_TYPE::SKY:
							{
								context->IASetInputLayout(sh.getDefaultInputLayout());
								context->IASetVertexBuffers(0, 1, dc0->g_vertexAndInstanceBfr[0].GetAddressOf(), &m_vStride, &m_offset);
								context->IASetIndexBuffer(dc0->g_indexBfr.Get(), DXGI_FORMAT_R32_UINT, 0);
								context->DrawIndexed(dc0->g_indexCount, 0, 0);
								sh.bindNull(context);
								break;
							}
							case RENDER_TYPE::DYNAMIC_SKY:
							{
								context->IASetInputLayout(sh.getDefaultInputLayout());
								context->IASetVertexBuffers(0, 1, dc0->g_vertexAndInstanceBfr[0].GetAddressOf(), &m_vStride, &m_offset);
								context->IASetIndexBuffer(dc0->g_indexBfr.Get(), DXGI_FORMAT_R32_UINT, 0);
								context->DrawIndexed(dc0->g_indexCount, 0, 0);
								sh.bindNull(context);
								break;
							}

							}


						}

					}
					if (tempGrass)
					{
						context->IASetPrimitiveTopology(tempGrass->g_topology);
						context->IASetInputLayout(sh.getDefaultInputLayout());
						context->IASetVertexBuffers(0, 1, tempGrass->g_vertexBfr.GetAddressOf(), &m_vStride, &m_offset);
						context->IASetIndexBuffer(tempGrass->g_indexBfr.Get(), DXGI_FORMAT_R32_UINT, 0);
						context->DrawIndexed(tempGrass->g_indexCount, 0, 0);
						sh.bindNull(context);
					}
					if (dc1)
					{

						context->Dispatch(dc1->g_dispatchThreads.x, dc1->g_dispatchThreads.y, dc1->g_dispatchThreads.z);
						sh.bindNull(context);

					}
					if (dcQ)
					{
						context->IASetPrimitiveTopology(dcQ->g_topology);
						context->IASetInputLayout(sh.getDefaultInputLayout());
						context->IASetVertexBuffers(0, 1, dcQ->g_vertexBfr.GetAddressOf(), &m_vStride, &m_offset);
						context->IASetIndexBuffer(dcQ->g_indexBfr.Get(), DXGI_FORMAT_R32_UINT, 0);
						context->DrawIndexed(dcQ->g_indexCount, 0, 0);
						sh.bindNull(context);
					}
					if (dcS)
					{
						

						context->IASetPrimitiveTopology(dcS->g_topology);
						context->IASetInputLayout(sh.getDefaultInputLayout());
						context->IASetVertexBuffers(0, 1, dcS->g_vertexBfr.GetAddressOf(), &m_vStride, &m_offset);
						context->IASetIndexBuffer(dcS->g_indexBfr.Get(), DXGI_FORMAT_R32_UINT, 0);
						context->DrawIndexed(dcS->g_indexCount, 0, 0);
						sh.bindNull(context);
					}
					if (dcP)
					{
						renderParticleSys(dcP, context);
						sh.bindNull(context);
					}
				}
			}
			
			
		}

	}
}

void UpdateDrawClass::renderDebugLight(ID3D11DeviceContext* context, const D3D11_VIEWPORT& viewPort)
{
	m_timer.run();
	m_sceneConstants.g_time.y = 0.005f;
	m_sceneConstants.g_time.x = (float)m_timer.getElapsedDeltaTime();
	m_sceneConstants.g_cameraPosition = m_mainCam->getTransAsV4();
	m_sceneConstants.g_vp = m_mainCam->getVP();
	m_sceneConstants.g_view = m_mainCam->getViewMatrix();
	m_sceneConstants.g_frustumPlanes[0] = m_mainCam->getFrustumPlanes()[0];
	m_sceneConstants.g_frustumPlanes[1] = m_mainCam->getFrustumPlanes()[1];
	m_sceneConstants.g_frustumPlanes[2] = m_mainCam->getFrustumPlanes()[2];
	m_sceneConstants.g_frustumPlanes[3] = m_mainCam->getFrustumPlanes()[3];
	m_sceneConstants.g_frustumPlanes[4] = m_mainCam->getFrustumPlanes()[4];
	m_sceneConstants.g_frustumPlanes[5] = m_mainCam->getFrustumPlanes()[5];
	m_sceneBfr.mapBfr(context, &m_sceneConstants, sizeof(CB_SCENE));
	context->VSSetConstantBuffers(CB_SCENE_SLOT, 1, m_sceneBfr.getBfr());
	context->GSSetConstantBuffers(CB_SCENE_SLOT, 1, m_sceneBfr.getBfr());
	context->PSSetConstantBuffers(CB_SCENE_SLOT, 1, m_sceneBfr.getBfr());
	context->CSSetConstantBuffers(CB_SCENE_SLOT, 1, m_sceneBfr.getBfr());
	context->HSSetConstantBuffers(CB_SCENE_SLOT, 1, m_sceneBfr.getBfr());
	context->DSSetConstantBuffers(CB_SCENE_SLOT, 1, m_sceneBfr.getBfr());

	for (std::pair<RENDERPASS, RenderPass> rp : RenderPassHandler::getInstance().getRenderPass())
	{
		switch (rp.first)
		{
		default:
		{
			break;
		}
		case RENDERPASS::DEPTH_LIGHT_PASS:
		{
			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
			m_nullSlotsSRV.push_back(SRV_LIGHT_DEPTH);
			m_nullShaderBindflag = SHADER_BIND_FLAG::COMPUTE_SHADER;
			bindNull(m_nullShaderBindflag, m_nullSlotsSRV, m_nullSlotsUAV, context);
			context->RSSetViewports(1, &rp.second.getViewPort());
			context->OMSetRenderTargets(1, rp.second.getRTV().GetAddressOf(), rp.second.getDSV().Get());
			context->ClearDepthStencilView(rp.second.getDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			break;
		}
		case RENDERPASS::LIGHT_CULL_PASS:
		{

			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
			m_nullSlotsSRV.push_back(SRV_ALBEDO_QUAD);
			m_nullSlotsSRV.push_back(SRV_LIGHT_INDEX);
			m_nullSlotsSRV.push_back(SRV_LIGHT_GRID);
			m_nullShaderBindflag = SHADER_BIND_FLAG::VERTEX_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::HULL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::DOMAIN_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::PIXEL_SHADER;
			m_nullShaderBindflag |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
			bindNull(m_nullShaderBindflag, m_nullSlotsSRV, m_nullSlotsUAV, context);
			resetRTVtoMain(context, viewPort);
			m_sceneConstants.g_vp = m_mainCam->getVP();
			m_sceneBfr.mapBfr(context, &m_sceneConstants, sizeof(CB_SCENE));
			
			break;
		}
		case RENDERPASS::BASIC_PASS_QUAD:
		{
			m_nullSlotsUAV.clear();
			m_nullSlotsSRV.clear();
			m_nullSlotsUAV.push_back(UAV_LIGHT_INDEX);
			m_nullSlotsUAV.push_back(UAV_LIGHT_GRID);
			m_nullSlotsUAV.push_back(UAV_LIGHT_HEAT);
			m_nullSlotsUAV.push_back(UAV_LIGHT_COUNTER);
			m_nullShaderBindflag = SHADER_BIND_FLAG::COMPUTE_SHADER;
			bindNull(m_nullShaderBindflag, m_nullSlotsSRV, m_nullSlotsUAV, context);
			break;
		}
		}
		for (auto s : rp.second.getShapes())
		{
			if (s)
			{
				if (s->draw(m_mainCam, context))
				{
					DC_BASIC* dc0 = dynamic_cast<DC_BASIC*>(s->getDrawCall());
					DC_COMPUTE* dc1 = dynamic_cast<DC_COMPUTE*>(s->getDrawCall());
					DC_QUAD* dcQ = dynamic_cast<DC_QUAD*>(s->getDrawCall());
					DC_SKELETON* dcS = dynamic_cast<DC_SKELETON*>(s->getDrawCall());
					DC_PARTICLE* dcP = dynamic_cast<DC_PARTICLE*>(s->getDrawCall());
					
					if (s->g_renderBack)
					{
						context->RSSetState(m_cullNoneRS);
					}
					else if (s->g_renderWireFrame)
					{
						context->RSSetState(m_wireframeRS);
					}
					else if (s->g_renderClockWise)
					{
						context->RSSetState(m_clockWiseRS);
					}
					else
					{
						context->RSSetState(m_mainRS);
					}
					bindResources(s, rp.first, context);

					ShaderHandler& sh = ShaderHandler::getInstance();
					if (dc0)
					{

						context->IASetPrimitiveTopology(dc0->g_topology);
						if (s->isInstanced())
						{

							context->IASetInputLayout(sh.getDefaultInstanceInputLayout());
							context->IASetVertexBuffers(0, 2, dc0->g_vertexAndInstanceBfr->GetAddressOf(), m_vInStride, m_offsetTwo);
							context->IASetIndexBuffer(dc0->g_indexBfr.Get(), DXGI_FORMAT_R32_UINT, 0);
							context->DrawIndexedInstanced(dc0->g_indexCount, dc0->g_instanceCount, 0, 0, 0);
							ShaderHandler::getInstance().bindNull(context);
						}
						else
						{
							switch (s->getDrawCall()->g_renderType)
							{
							default:
							{
								context->IASetInputLayout(sh.getDefaultInputLayout());
								context->IASetVertexBuffers(0, 1, dc0->g_vertexAndInstanceBfr[0].GetAddressOf(), &m_vStride, &m_offset);
								context->IASetIndexBuffer(dc0->g_indexBfr.Get(), DXGI_FORMAT_R32_UINT, 0);
								context->DrawIndexed(dc0->g_indexCount, 0, 0);
								ShaderHandler::getInstance().bindNull(context);
								break;
							}
							case RENDER_TYPE::BASIC:
							{
								context->IASetInputLayout(sh.getDefaultInputLayout());
								context->IASetVertexBuffers(0, 1, dc0->g_vertexAndInstanceBfr[0].GetAddressOf(), &m_vStride, &m_offset);
								context->IASetIndexBuffer(dc0->g_indexBfr.Get(), DXGI_FORMAT_R32_UINT, 0);
								context->DrawIndexed(dc0->g_indexCount, 0, 0);
								ShaderHandler::getInstance().bindNull(context);
							}
							case RENDER_TYPE::TERRAIN:
							{
								context->IASetInputLayout(sh.getTerrainInputLayout());
								context->IASetVertexBuffers(0, 1, dc0->g_vertexAndInstanceBfr[0].GetAddressOf(), &m_tStride, &m_offset);
								context->IASetIndexBuffer(dc0->g_indexBfr.Get(), DXGI_FORMAT_R32_UINT, 0);
								context->DrawIndexed(dc0->g_indexCount, 0, 0);
								sh.bindNull(context);
								break;
							}
							

							}


						}

					}
					if (dc1)
					{

						context->Dispatch(dc1->g_dispatchThreads.x, dc1->g_dispatchThreads.y, dc1->g_dispatchThreads.z);
						ShaderHandler::getInstance().bindNull(context);

					}
					if (dcQ)
					{
						context->IASetPrimitiveTopology(dcQ->g_topology);
						context->IASetInputLayout(sh.getDefaultInputLayout());
						context->IASetVertexBuffers(0, 1, dcQ->g_vertexBfr.GetAddressOf(), &m_vStride, &m_offset);
						context->IASetIndexBuffer(dcQ->g_indexBfr.Get(), DXGI_FORMAT_R32_UINT, 0);
						context->DrawIndexed(dcQ->g_indexCount, 0, 0);
						ShaderHandler::getInstance().bindNull(context);
					}
					
					
				}
				
			}
		}
	}
}

void UpdateDrawClass::changeRes(size_t currResX, size_t currResY, ID3D11Device* device)
{
	m_mainCam->changeResolution(currResX, currResY);
	UINT screenWidth = (UINT)currResX;
	UINT screenHeight = (UINT)currResY;
	int lightCullingBlockSize = LIGHT_CULLING_BLOCK_SIZE;
	XMUINT4 numThreads = XMUINT4(UINT(std::ceil((float)screenWidth / (float)lightCullingBlockSize)), UINT(std::ceil((float)screenHeight / (float)lightCullingBlockSize)), 1, 1);
	XMUINT4 numThreadGroups = XMUINT4(UINT(std::ceil((float)numThreads.x / (float)lightCullingBlockSize)), UINT(std::ceil((float)numThreads.y / (float)lightCullingBlockSize)), 1, 1);
	UINT count = numThreads.x * numThreads.y * numThreads.z;

	TextureHandler& t = TextureHandler::getInstance();
	D3D11_TEXTURE2D_DESC textureDescTemp = {};
	ID3D11Texture2D* depthTex2D = nullptr;
	textureDescTemp.Width = (UINT)numThreads.x;
	textureDescTemp.Height = (UINT)numThreads.y;
	textureDescTemp.MipLevels = 1;
	textureDescTemp.ArraySize = 1;
	textureDescTemp.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDescTemp.SampleDesc.Count = 1;
	textureDescTemp.Usage = D3D11_USAGE_DEFAULT;
	textureDescTemp.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	textureDescTemp.CPUAccessFlags = 0;
	textureDescTemp.MiscFlags = 0;
	HRESULT HR = device->CreateTexture2D(&textureDescTemp, NULL, &depthTex2D);
	assert(SUCCEEDED(HR));
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDescTemp = {};
	uavDescTemp.Format = textureDescTemp.Format;
	uavDescTemp.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDescTemp = {};
	srvDescTemp.Format = textureDescTemp.Format;
	srvDescTemp.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDescTemp.Texture2D.MostDetailedMip = 0;
	srvDescTemp.Texture2D.MipLevels = textureDescTemp.MipLevels;
	t.eraseSRV("heat");
	t.addTextureSRV("heat", depthTex2D, device, srvDescTemp);
	t.eraseUAV("heat");
	t.addTextureUAV("heat", depthTex2D, device, uavDescTemp);
	RELEASE_COM(depthTex2D);

	DC_COMPUTE* tempC = dynamic_cast<DC_COMPUTE*>(m_CSLightCullingShape->getDrawCall());
	tempC->g_dispatchThreads.x = numThreadGroups.x * lightCullingBlockSize;
	tempC->g_dispatchThreads.y = numThreadGroups.y * lightCullingBlockSize;
	tempC->g_dispatchThreads.z = numThreadGroups.z * lightCullingBlockSize;

	m_sceneConstants.g_screenDimenstions.x = screenWidth;
	m_sceneConstants.g_screenDimenstions.y = screenHeight;
	m_sceneConstants.g_inverseProjection = m_mainCam->getProjectionMatrix().Invert();
	m_sceneConstants.g_projection = m_mainCam->getProjectionMatrix();
}

void UpdateDrawClass::restart()
{
	m_shapes.clear();
	m_lights.clear();
	
	m_directionalLight = nullptr; //released elsewhere
	m_boundingBoxesVis = nullptr;
	m_playerShape = nullptr;
	m_timer.reset();
	if (m_quadTree)
	{
		RELEASE(m_quadTree);
	
	}
	if (m_ocTree)
	{
		RELEASE(m_ocTree);
	}
	if (m_portal)
	{
		RELEASE(m_portal);
	}
	if (m_binaryTree)
	{
		RELEASE(m_binaryTree);
	}
}

void UpdateDrawClass::updateTransform(ID3D11DeviceContext* context)
{
	for (size_t i = 0; i < m_shapes.size(); i++)
	{
		if (m_shapes[i]->getTransformStatus() == TRANSFORM_TYPE::DYNAMIC && !m_shapes[i]->isInstanced())
		{
			
			m_shapes[i]->updateCBfr();

		}
		else if (m_shapes[i]->getTransformStatus() == TRANSFORM_TYPE::DYNAMIC && m_shapes[i]->isInstanced())
		{
			m_shapes[i]->updateInstances(context);
		}
		DC_PARTICLE* part = dynamic_cast<DC_PARTICLE*>(m_shapes[i]->getDrawCall());
		if (m_shapes[i]->getDrawCall()->g_renderType == RENDER_TYPE::PARTICLE_FIRE_TYPE)
		{
			if (m_shapes[i]->getName() == "smoke")
			{
				part->g_particleData.g_particleConstants.x = (float)m_smokeSize;
				part->g_particleData.g_particleConstants.y = (float)m_smokeSize;
			}
			else
			{
				part->g_particleData.g_particleConstants.x = (float)m_fireSize;
				part->g_particleData.g_particleConstants.y = (float)m_fireSize;
			}
		}
		else if (m_shapes[i]->getDrawCall()->g_renderType == RENDER_TYPE::PARTICLE_LEAVES_TYPE)
		{
			part->g_particleData.g_particleConstants.x = (float)m_leafSize;
			part->g_particleData.g_particleConstants.y = (float)m_leafSize;
		}
		else if (m_shapes[i]->getDrawCall()->g_renderType == RENDER_TYPE::PARTICLE_TEXT_TYPE)
		{
			part->g_particleData.g_particleConstants.x = (float)m_textSize;
			part->g_particleData.g_particleConstants.y = (float)m_textSize;
		}
		
	}
	if (m_hasDynamicLight)
	{
		m_lightData.clear();
		for (size_t i = 0; i < m_lights.size(); i++)
		{
			m_lightData.push_back(m_lights[i]->getData());

		}
		m_lightBfr.mapBfr(context, m_lightData.data(), sizeof(LIGHT_DATA) * m_lightData.size());
	}
	if (m_directionalLight)
	{
		if (m_directionalLight->getTransformStatus() == TRANSFORM_TYPE::DYNAMIC)
		{
			m_sceneConstants.g_dirLightDirection = m_directionalLight->getData().direction;
		}
	}
	if (m_dynamicSkyShape)
	{
		Vector3 t = m_dynamicSkyShape->getLocalTranformations().trans;
		updateDynamicCubeMap(t);
	}
	if (m_terrainShape && m_playerShape)
	{

		Vector3 currTrans = m_playerShape->getLocalTranformations().trans; //Need to change this if player is parented to something.
		float y = currTrans.y;
		if (currTrans.x>m_terrainShape->getBB().getBounds().aabb.min.x&& currTrans.x < m_terrainShape->getBB().getBounds().aabb.max.x
			&& currTrans.z>m_terrainShape->getBB().getBounds().aabb.min.z && currTrans.z < m_terrainShape->getBB().getBounds().aabb.max.z)
		{
			
			y = m_terrainShape->getSurfaceHeight(currTrans.x, currTrans.z) + 20;
			m_playerShape->setTranslation({ currTrans.x,y,currTrans.z });

		}
		else
		{
			
			
			if (currTrans.x < m_terrainShape->getBB().getBounds().aabb.min.x)
			{
				currTrans.x = currTrans.x+10;
			}
			if (currTrans.x > m_terrainShape->getBB().getBounds().aabb.max.x)
			{
				currTrans.x = currTrans.x- 10;
			}
			if (currTrans.z < m_terrainShape->getBB().getBounds().aabb.min.z)
			{
				currTrans.z = currTrans.z+10;
			}
			if (currTrans.z > m_terrainShape->getBB().getBounds().aabb.max.z)
			{
				currTrans.z = currTrans.z- 10;
			}
			m_playerShape->setTranslation({ currTrans.x,y,currTrans.z });
		}
		
	}
		
		
	m_player.updateIMGUI(m_mouseSensitivity, m_walkSpeed, m_turnSpeed, m_runSpeed);
	m_player.updateCam();
	if (m_skyShape)
	{
		m_skyShape->setTranslation(m_mainCam->getLocalTranformations().trans);
	}
}

void UpdateDrawClass::updateIMGUI(int fireSize, int smokeSize, int leafSize, int textSize, float mouseSense, float runSpeed, float walkSpeed, float turnSpeed, bool bb, bool bbwf)
{
	m_fireSize = fireSize;
	m_smokeSize = smokeSize;
	m_leafSize = leafSize;
	m_textSize = textSize;
	m_mouseSensitivity = mouseSense;
	m_turnSpeed = turnSpeed;
	m_runSpeed = runSpeed;
	m_walkSpeed = walkSpeed;
	if (m_boundingBoxesVis)
	{
		m_boundingBoxesVis->g_render = bb;
		m_boundingBoxesVis->g_renderWireFrame = bbwf;
	}
}

void UpdateDrawClass::updateIMGUI(int& triangleCount, int& currInstanceCount)
{
	int c = 0;
	for (auto pair : m_triangleCount)
	{
		c += pair.second;
	}
	triangleCount = c;
	currInstanceCount = m_currInstanceCount;
}







