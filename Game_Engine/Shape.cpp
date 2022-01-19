#include "stdafx.h"
#include "Shape.h"

void DC_BASIC::pureVirtual()
{
	return;
}

Shape::~Shape()
{

}

Shape::Shape(ID3D11Device* device, const DC_TYPE& drawType , const RENDERPASS& renderpasses , const RENDER_TYPE& renderType)
{
	m_dcType = drawType;
	m_shaders = SHADER_BIND_FLAG::VERTEX_SHADER;
	m_numPatchVerPerRow = ((m_hmH - 1) / CELLS_PER_PATCH_HM) + 1;
	m_numPatchVerPerCol = ((m_hmW - 1) / CELLS_PER_PATCH_HM) + 1;
	m_numPatchVer = m_numPatchVerPerRow * m_numPatchVerPerCol;
	m_numPatchQuadFace = (m_numPatchVerPerRow - 1) * (m_numPatchVerPerCol - 1);
	
	
	switch (renderType)
	{
	default:
	{
		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_srvs.insert({ SRV_ALBEDO_SLOT, "grey" });
		m_srvs.insert({ SRV_NORMAL_SLOT, "blue" });
		m_srvs.insert({ SRV_METAL_SLOT, "black" });
		m_srvs.insert({ SRV_AO_SLOT, "white" });
		m_srvs.insert({ SRV_ROUGH_SLOT, "grey" });
		m_srvs.insert({ SRV_EMISSIVE_SLOT, "black" });
		m_srvs.insert({ SRV_LIGHT_SLOT, "light" });
		m_srvs.insert({ SRV_SHADOW_POINT_SLOT, "shadowPoint" });
		m_srvs.insert({ SRV_SHADOW_DIR_SLOT, "shadowDir" });
		m_srvs.insert({ SRV_LIGHT_GRID, "lightGrid" });
		m_srvs.insert({ SRV_LIGHT_INDEX, "indexList" });
		break;
	}
	case RENDER_TYPE::BASIC:
	{
		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_srvs.insert({ SRV_ALBEDO_SLOT, "grey" });
		m_srvs.insert({ SRV_NORMAL_SLOT, "blue" });
		m_srvs.insert({ SRV_METAL_SLOT, "black" });
		m_srvs.insert({ SRV_AO_SLOT, "white" });
		m_srvs.insert({ SRV_ROUGH_SLOT, "grey" });
		m_srvs.insert({ SRV_EMISSIVE_SLOT, "black" });
		m_srvs.insert({ SRV_LIGHT_SLOT, "light" });
		m_srvs.insert({ SRV_SHADOW_POINT_SLOT, "shadowPoint" }); 
		m_srvs.insert({ SRV_SHADOW_DIR_SLOT, "shadowDir" });
		m_srvs.insert({ SRV_LIGHT_GRID, "lightGrid" });
		m_srvs.insert({ SRV_LIGHT_INDEX, "indexList" });
		break;
	}
	case RENDER_TYPE::BOUNDING_BOX:
	{
		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_srvs.insert({ SRV_ALBEDO_SLOT, "blue" });
		m_srvs.insert({ SRV_NORMAL_SLOT, "blue" });
		m_srvs.insert({ SRV_METAL_SLOT, "black" });
		m_srvs.insert({ SRV_AO_SLOT, "white" });
		m_srvs.insert({ SRV_ROUGH_SLOT, "grey" });
		m_srvs.insert({ SRV_EMISSIVE_SLOT, "black" });
		m_srvs.insert({ SRV_LIGHT_SLOT, "light" });
		m_srvs.insert({ SRV_SHADOW_POINT_SLOT, "shadowPoint" });
		m_srvs.insert({ SRV_SHADOW_DIR_SLOT, "shadowDir" });
		m_srvs.insert({ SRV_LIGHT_GRID, "lightGrid" });
		m_srvs.insert({ SRV_LIGHT_INDEX, "indexList" });
		break;
	}
	case RENDER_TYPE::PARTICLE_FIRE_TYPE:
	{
		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_shaders |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
		m_srvs.insert({ SRV_RANDOM_1D, "random1D" });
		m_srvs.erase(SRV_ALBEDO_SLOT);
		m_srvs.insert({ SRV_ALBEDO_SLOT, "../Resources/Textures/fire.png" });
		g_renderClockWise = true;
		break;
	}
	case RENDER_TYPE::PARTICLE_RAIN_TYPE:
	{
		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_shaders |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
		m_srvs.insert({ SRV_RANDOM_1D, "random1D" });
		m_srvs.insert({ SRV_ALBEDO_SLOT, "white" });
		break;
	}
	case RENDER_TYPE::PARTICLE_TEXT_TYPE:
	{
		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_shaders |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
		m_srvs.erase(SRV_ALBEDO_SLOT);
		m_srvs.insert({ SRV_ALBEDO_SLOT, "../Resources/Textures/Claw.png" });
		m_srvs.insert({ SRV_RANDOM_1D, "random1D" });
		g_renderClockWise = true;
		break;
	}
	case RENDER_TYPE::PARTICLE_LEAVES_TYPE:
	{
		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_shaders |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
		m_srvs.erase(SRV_ALBEDO_SLOT);
		m_srvs.insert({ SRV_ALBEDO_SLOT, "../Resources/Textures/oakLeaf.png" });
		m_srvs.insert({ SRV_RANDOM_1D, "random1D" });
		g_renderClockWise = true;
		break;
	}
	
	case RENDER_TYPE::TERRAIN:
	{


		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_shaders |= SHADER_BIND_FLAG::HULL_SHADER;
		m_shaders |= SHADER_BIND_FLAG::DOMAIN_SHADER;
		m_srvs.insert({ SRV_ALBEDO_SLOT, "grey" });
		m_srvs.insert({ SRV_NORMAL_SLOT, "blue" });
		m_srvs.insert({ SRV_METAL_SLOT, "black" });
		m_srvs.insert({ SRV_AO_SLOT, "white" });
		m_srvs.insert({ SRV_ROUGH_SLOT, "grey" });
		m_srvs.insert({ SRV_EMISSIVE_SLOT, "black" });
		m_srvs.insert({ SRV_LIGHT_SLOT, "light" });
		m_srvs.insert({ SRV_SHADOW_POINT_SLOT, "shadowPoint" });
		m_srvs.insert({ SRV_SHADOW_DIR_SLOT, "shadowDir" });
		m_srvs.insert({ SRV_LIGHT_GRID, "lightGrid" });
		m_srvs.insert({ SRV_LIGHT_INDEX, "indexList" });
		m_srvs.insert({ SRV_NOISE_SLOT, "../Resources/Textures/noise.png" });
		m_srvs.insert({ SRV_TERRAIN_MAP, "terrain" });
		m_srvs.insert({ SRV_TERRAIN_LAYERS_MAP, "terrainLayers" });
		m_srvs.insert({ SRV_BLEND_MAP, "../Resources/Textures/blendTerrain.png" });
		break;
	}
	case RENDER_TYPE::DYNAMIC_SKY:
	{
		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_shaders |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
		m_srvs.insert({ SRV_ALBEDO_SLOT, "grey" });
		m_srvs.insert({ SRV_NORMAL_SLOT, "blue" });
		m_srvs.insert({ SRV_METAL_SLOT, "black" });
		m_srvs.insert({ SRV_AO_SLOT, "white" });
		m_srvs.insert({ SRV_ROUGH_SLOT, "grey" });
		m_srvs.insert({ SRV_EMISSIVE_SLOT, "black" });
		m_srvs.insert({ SRV_LIGHT_SLOT, "light" });
		m_srvs.insert({ SRV_SHADOW_POINT_SLOT, "shadowPoint" });
		m_srvs.insert({ SRV_SHADOW_DIR_SLOT, "shadowDir" });
		m_srvs.insert({ SRV_LIGHT_GRID, "lightGrid" });
		m_srvs.insert({ SRV_LIGHT_INDEX, "indexList" });
		m_srvs.insert({ SRV_DYNAMIC_CUBE, "dynamicCube" });
		break;
	}
	case RENDER_TYPE::SKY:
	{
		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_shaders |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
		m_srvs.insert({ SRV_ALBEDO_SLOT, "grey" });
		m_srvs.insert({ SRV_NORMAL_SLOT, "blue" });
		m_srvs.insert({ SRV_METAL_SLOT, "black" });
		m_srvs.insert({ SRV_AO_SLOT, "white" });
		m_srvs.insert({ SRV_ROUGH_SLOT, "grey" });
		m_srvs.insert({ SRV_EMISSIVE_SLOT, "black" });
		m_srvs.insert({ SRV_LIGHT_SLOT, "light" });
		m_srvs.insert({ SRV_SHADOW_POINT_SLOT, "shadowPoint" });
		m_srvs.insert({ SRV_SHADOW_DIR_SLOT, "shadowDir" });
		m_srvs.insert({ SRV_LIGHT_GRID, "lightGrid" });
		m_srvs.insert({ SRV_LIGHT_INDEX, "indexList" });
		m_srvs.insert({ SRV_TEXTURE_CUBE, "sky" });
		break;
	}
	}

	int flag = (int)(renderpasses);

	if ((flag & (int)RENDERPASS::LIGHT_CULL_PASS) != 0)
	{
		m_shaders &= ~SHADER_BIND_FLAG::VERTEX_SHADER;
		m_shaders &= ~SHADER_BIND_FLAG::PIXEL_SHADER;
		m_shaders = SHADER_BIND_FLAG::COMPUTE_SHADER;
		m_srvs.clear();
		m_uavs.clear();
		m_srvs.insert({ SRV_LIGHT_DEPTH, "depthLight" });
		m_srvs.insert({ SRV_LIGHT_SLOT, "light" });
		m_srvs.erase(SRV_LOADED_HEAT);
		m_srvs.insert({ SRV_LOADED_HEAT,"../Resources/Textures/LightCountHeatMap.png" });;
		m_uavs.insert({ UAV_LIGHT_GRID, "lightGrid" });
		m_uavs.erase(UAV_LIGHT_INDEX);
		m_uavs.insert({ UAV_LIGHT_INDEX, "indexList" });
		m_uavs.insert({ UAV_LIGHT_HEAT, "heat" });
		m_uavs.insert({ UAV_LIGHT_COUNTER, "lightCounter" });
		
	}
	if ((flag & (int)RENDERPASS::GAUSSIAN_HOR) != 0)
	{
		m_shaders &= ~SHADER_BIND_FLAG::VERTEX_SHADER;
		m_shaders &= ~SHADER_BIND_FLAG::PIXEL_SHADER;
		m_shaders = SHADER_BIND_FLAG::COMPUTE_SHADER;
		m_srvs.erase(SRV_DOWN_SAMPLE);
		m_uavs.erase(UAV_GAUSSIAN_HOR);
		m_srvs.insert({ SRV_DOWN_SAMPLE, "downSampleEmissive" });
		m_uavs.insert({ UAV_GAUSSIAN_HOR, "GaussianHorUAV" });
	}
	if ((flag & (int)RENDERPASS::GAUSSIAN_VER) != 0)
	{
		m_shaders &= ~SHADER_BIND_FLAG::VERTEX_SHADER;
		m_shaders &= ~SHADER_BIND_FLAG::PIXEL_SHADER;
		m_shaders = SHADER_BIND_FLAG::COMPUTE_SHADER;
		m_srvs.erase(SRV_GAUSSIAN_HOR);
		m_uavs.erase(UAV_GAUSSIAN_VER);
		m_srvs.insert({ SRV_GAUSSIAN_HOR, "GaussianHorSRV" });
		m_uavs.insert({ UAV_GAUSSIAN_VER, "GaussianVerUAV" });
	}
	if ((flag & (int)RENDERPASS::DEPTH_SHADOW_POINT) != 0)
	{
		m_shaders |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
	}
	if ((flag & (int)RENDERPASS::BASIC_PASS_QUAD) != 0)
	{
		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_srvs.erase(SRV_VOL_LIGHT_QUAD);
		m_srvs.erase(SRV_EMISSIVE_QUAD);
		m_srvs.erase(SRV_FRAME_QUAD);
		m_srvs.insert({ SRV_VOL_LIGHT_QUAD, "upSampleRay" });
		m_srvs.insert({ SRV_EMISSIVE_QUAD, "upSampleEmissive" }); 
		m_srvs.insert({ SRV_FRAME_QUAD,"normalFrame"  });
		
		
	}
	if ((flag & (int)RENDERPASS::DOWN_SAMPLE_EMISSIVE_PASS) != 0)
	{
		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_srvs.erase(SRV_ALBEDO_QUAD);
		m_srvs.insert({ SRV_ALBEDO_QUAD, "emissive" });
	}
	if ((flag & (int)RENDERPASS::FXAA) != 0)
	{
		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_srvs.erase(SRV_ALBEDO_QUAD);
		m_srvs.insert({ SRV_ALBEDO_QUAD, "normalFrame" });
		
	}
	if ((flag & (int)RENDERPASS::DOWN_SAMPLE_RAY_MARCH_PASS) != 0)
	{
		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_srvs.erase(SRV_ALBEDO_QUAD);
		m_srvs.insert({ SRV_ALBEDO_QUAD, "normalFrame" });
	}
	if ((flag & (int)RENDERPASS::UP_SAMPLE_EMISSIVE_PASS) != 0)
	{
		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_srvs.erase(SRV_ALBEDO_QUAD);
		m_srvs.insert({ SRV_ALBEDO_QUAD, "GaussianVerSRV" });
		
	}
	if ((flag & (int)RENDERPASS::UP_SAMPLE_RAY_MARCH_PASS) != 0)
	{
		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_srvs.erase(SRV_ALBEDO_QUAD);
		m_srvs.insert({ SRV_ALBEDO_QUAD,"bilateralVer" });
	}
	if ((flag & (int)RENDERPASS::BILATERA_HOR) != 0)
	{
		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_srvs.erase(SRV_ALBEDO_QUAD);
		m_srvs.insert({ SRV_ALBEDO_QUAD,"volLight" });
	}
	if ((flag & (int)RENDERPASS::BILATERA_VER) != 0)
	{
		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_srvs.erase(SRV_ALBEDO_QUAD);
		m_srvs.insert({ SRV_ALBEDO_QUAD,"bilateralHor" });
	}
	if ((flag & (int)RENDERPASS::RAY_MARCH) != 0)
	{
		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_srvs.erase(SRV_DOWN_SAMPLE);
		m_srvs.insert({ SRV_DOWN_SAMPLE, "downSampleRay" });
		m_srvs.erase(SRV_SHADOW_DIR_SLOT); 
		m_srvs.insert({ SRV_SHADOW_DIR_SLOT, "shadowDir" });
		m_srvs.erase(SRV_LIGHT_DEPTH); 
		m_srvs.insert({ SRV_LIGHT_DEPTH, "depthLight" });
		m_srvs.erase(SRV_WORLD_POS);
		m_srvs.insert({ SRV_WORLD_POS, "worldPos" });
	}
	switch (drawType)
	{
	default:
	{
		break;
	}
	case DC_TYPE::BASIC:
	{
		std::shared_ptr<DC> drawCall(new DC_BASIC);
		m_drawCall = drawCall;
		m_drawCall.get()->g_renderPassTypes = renderpasses;
		m_drawCall.get()->g_renderType = renderType;
		m_drawCall.get()->g_shaders = m_shaders;
		m_drawCall.get()->g_srvs = m_srvs;
		auto basic = dynamic_cast<DC_BASIC*>(m_drawCall.get());
		Matrix id = XMMatrixIdentity();
		basic->g_objectData.g_inverseWorld = id;
		basic->g_objectData.g_world = id;
		if (basic->g_renderType == RENDER_TYPE::TERRAIN)
		{
			basic->g_topology = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
		}
		break;
		
	}
	case DC_TYPE::GRASS:
	{
		m_shaders |= SHADER_BIND_FLAG::PIXEL_SHADER;
		m_shaders |= SHADER_BIND_FLAG::HULL_SHADER;
		m_shaders |= SHADER_BIND_FLAG::DOMAIN_SHADER;
		m_shaders |= SHADER_BIND_FLAG::GEOMETRY_SHADER;
		m_srvs.erase(SRV_ALBEDO_SLOT);
		m_srvs.erase(SRV_NORMAL_SLOT);
		m_srvs.erase(SRV_AO_SLOT);
		m_srvs.erase(SRV_ROUGH_SLOT);
		m_srvs.insert({ SRV_ALBEDO_SLOT, "../Resources/Textures/GrassAlbedo.png" });
		m_srvs.insert({ SRV_NORMAL_SLOT, "../Resources/Textures/GrassNormal.png" });
		m_srvs.insert({ SRV_METAL_SLOT, "black" });
		m_srvs.insert({ SRV_AO_SLOT, "../Resources/Textures/GrassAO.png" });
		m_srvs.insert({ SRV_ROUGH_SLOT, "../Resources/Textures/GrassRough.png" });
		m_srvs.insert({ SRV_EMISSIVE_SLOT, "black" });
		m_srvs.insert({ SRV_LIGHT_SLOT, "light" });
		m_srvs.insert({ SRV_SHADOW_POINT_SLOT, "shadowPoint" });
		m_srvs.insert({ SRV_SHADOW_DIR_SLOT, "shadowDir" });
		m_srvs.insert({ SRV_LIGHT_GRID, "lightGrid" });
		m_srvs.insert({ SRV_LIGHT_INDEX, "indexList" });
		m_srvs.insert({ SRV_NOISE_SLOT, "../Resources/Textures/noise.png" });
		m_srvs.insert({ SRV_TERRAIN_MAP, "terrain" });
		m_srvs.insert({ SRV_GRASS_STRAND, "grassStrands" });
		m_srvs.insert({ SRV_GRASS_INDEX, "grassStrandsIndices" });
		m_srvs.insert({ SRV_GRASS_BC, "grassStrandsBC" });
		std::shared_ptr<DC> drawCall(new DC_GRASS);
		m_drawCall = drawCall;
		m_drawCall.get()->g_renderPassTypes = renderpasses;
		m_drawCall.get()->g_renderType = renderType;
		m_drawCall.get()->g_shaders = m_shaders;
		m_drawCall.get()->g_srvs = m_srvs;
		auto grassDC = dynamic_cast<DC_GRASS*>(m_drawCall.get());
		Matrix id = XMMatrixIdentity();
		grassDC->g_objectData.g_inverseWorld = id;
		grassDC->g_objectData.g_world = id;
	
		break;

	}
	case DC_TYPE::COMPUTE_SHADER:
	{
		
		std::shared_ptr<DC> drawCall(new DC_COMPUTE);
		m_drawCall = drawCall;
		m_drawCall.get()->g_renderPassTypes = renderpasses;
		m_drawCall.get()->g_renderType = renderType;
		m_drawCall.get()->g_shaders = m_shaders;
		m_drawCall.get()->g_srvs = m_srvs;
		auto compute = dynamic_cast<DC_COMPUTE*>(m_drawCall.get());
		compute->uavs = m_uavs;
		
		break;
	}
	case DC_TYPE::FULLSCREEN_QUAD:
	{
		std::shared_ptr<DC> drawCall(new DC_QUAD);
		m_drawCall = drawCall;
		m_drawCall.get()->g_renderPassTypes = renderpasses;
		m_drawCall.get()->g_renderType = renderType;
		m_drawCall.get()->g_shaders = m_shaders;
		m_drawCall.get()->g_srvs = m_srvs;;
		m_drawCall.get()->g_srvs.erase(SRV_LIGHT_INDEX);
		m_drawCall.get()->g_srvs.erase(SRV_LIGHT_GRID);
		m_drawCall.get()->g_srvs.insert({ SRV_LIGHT_INDEX, "indexList" });
		m_drawCall.get()->g_srvs.insert({ SRV_LIGHT_GRID, "lightGrid" });

		

		
		std::vector<VERTEX> vertices;
		VERTEX vertex;
		vertex.pos = XMFLOAT3(-1, -1, 0);
		vertex.uv = XMFLOAT2(0, 1);
		vertices.push_back(vertex);
		vertex.pos = XMFLOAT3(1, -1, 0);
		vertex.uv = XMFLOAT2(1, 1);
		vertices.push_back(vertex);
		vertex.pos = XMFLOAT3(1, 1, 0);
		vertex.uv = XMFLOAT2(1, 0);
		vertices.push_back(vertex);
		vertex.pos = XMFLOAT3(-1, 1, 0);
		vertex.uv = XMFLOAT2(0, 0);
		vertices.push_back(vertex);
		/*-size, -size, 0} , {0 ,1 },
 size, -size, 0 }, { 1,1 },
 size, size, 0 }, { 1,0 }, 
 -size, size, 0 }, { 0,0 },
3,2,1, 3,1,0*/
		std::vector<unsigned int> indices = {0,1,3,1,2,3  };
		createVertexBuffer(device, vertices.data(), sizeof(VERTEX) * 4);
		createIndexBuffer(device, indices.data(), sizeof(unsigned int) * 6);
		
		break;
	}
	case DC_TYPE::MORPH:
	{
		break;
	}
	case DC_TYPE::SKELETON:
	{
		m_srvs.insert({ SRV_SKELETON_SLOT, "skeleton" });
		std::shared_ptr<DC> drawCall(new DC_SKELETON);
		m_drawCall = drawCall;
		m_drawCall.get()->g_renderPassTypes = renderpasses;
		m_drawCall.get()->g_renderType = renderType;
		m_drawCall.get()->g_shaders = m_shaders;
		m_drawCall.get()->g_srvs = m_srvs;
		auto skeleton = dynamic_cast<DC_SKELETON*>(m_drawCall.get());
		Matrix id = XMMatrixIdentity();
		skeleton->g_objectData.g_inverseWorld = id;
		skeleton->g_objectData.g_world = id;
		break;
	}
	case DC_TYPE::SKELETON_AND_MORPH:
	{
		break;
	}
	case DC_TYPE::PARTICLE:
	{
		std::shared_ptr<DC> drawCall(new DC_PARTICLE);
		m_drawCall = drawCall;
		m_drawCall.get()->g_renderPassTypes = renderpasses;
		m_drawCall.get()->g_renderType = renderType;
		m_drawCall.get()->g_shaders = m_shaders;
		m_drawCall.get()->g_srvs = m_srvs;
		DC_PARTICLE* temp = dynamic_cast<DC_PARTICLE*>(m_drawCall.get());
		temp->g_particleData.g_particleConstants.x = 100;
		temp->g_particleData.g_particleConstants.y = 100;
		break;
	}
	}
}

bool Shape::rayCast(float x, float z, float& h, Vector3& v0, Vector3& v1, Vector3& v2)
{
	// Starting position of the ray that is being cast.
	Vector3 ray = XMFLOAT3(x, 0.0f, z);
	// The direction the ray is being cast.
	Vector3 rayDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
	// Calculate the two edges from the three points given.
	Vector3 edge1, edge2, normal;
	edge1 = v0 - v2;
	edge2 = v1 - v2;
	// Calculate the sufrace normal of the triangle from the two edges.
	edge1.Cross(edge2, normal);
	/*normal.x = (edge1.y * edge2.z) - (edge1.z * edge2.y);
	normal.y = (edge1.z * edge2.x) - (edge1.x * edge2.z);
	normal.z = (edge1.x * edge2.y) - (edge1.y * edge2.x);*/
	//float area = normal.Length();
	//normal *= 1.0f / area;
	normal.Normalize();
	// Find the distance from the origin to the plane.
	float d = ((-normal.x * v0.x) + (-normal.y * v0.y) + (-normal.z * v0.z));
	// Get the denominator of the equation.
	float denominator = normal.Dot(rayDirection);
	// Make sure the result doesn't get too close to zero to prevent divide by zero.
	if (std::fabs(denominator) < 0.0001f)
	{
		return false;
	}
	// Get the numerator of the equation.

	float numerator = -(normal.Dot(ray) + d);

	// Calculate where we intersect the triangle.
	float t = numerator / denominator;

	// Find the intersection vector.
	Vector3 Q = ray + (rayDirection * t);



	Vector3 edge3, edgeNormal;
	// Find the three edges of the triangle.
	edge3 = v1 - v0;

	// Calculate the normal for the first edge.
	edgeNormal.x = (edge1.y * normal.z) - (edge1.z * normal.y);
	edgeNormal.y = (edge1.z * normal.x) - (edge1.x * normal.z);
	edgeNormal.z = (edge1.x * normal.y) - (edge1.y * normal.x);
	//edgeNormal.Normalize();
	// Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
	Vector3 temp = Q - v0;
	float determinant = edgeNormal.Dot(temp);
	//float determinant = ((edgeNormal.x * temp.x) + (edgeNormal.y * temp.y) + (edgeNormal.z * temp.z));

	// Check if it is outside.
	if (determinant > 0.001f)
	{
		return false;
	}

	// Calculate the normal for the second edge.
	edgeNormal.x = (edge2.y * normal.z) - (edge2.z * normal.y);
	edgeNormal.y = (edge2.z * normal.x) - (edge2.x * normal.z);
	edgeNormal.z = (edge2.x * normal.y) - (edge2.y * normal.x);
	//edgeNormal.Normalize();
	// Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
	temp = Q - v1;
	determinant = edgeNormal.Dot(temp);
	//determinant = ((edgeNormal.x * temp.x) + (edgeNormal.y * temp.y) + (edgeNormal.z * temp.z));

	// Check if it is outside.
	if (determinant > 0.001f)
	{
		return false;
	}

	// Calculate the normal for the third edge.
	edgeNormal.x = (edge3.y * normal.z) - (edge3.z * normal.y);
	edgeNormal.y = (edge3.z * normal.x) - (edge3.x * normal.z);
	edgeNormal.z = (edge3.x * normal.y) - (edge3.y * normal.x);
	//edgeNormal.Normalize();
	// Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
	temp = Q - v2;
	determinant = edgeNormal.Dot(temp);
	//determinant = ((edgeNormal.x * temp.x) + (edgeNormal.y * temp.y) + (edgeNormal.z * temp.z));

	// Check if it is outside.
	if (determinant > 0.001f)
	{
		return false;
	}

	// Now we have our height.
	h = Q.y;

	return true;
}

void Shape::setIndices(ID3D11Device* device, const std::vector<unsigned int>& indices)
{
	m_indices = indices;
	createIndexBuffer(device, m_indices.data(), sizeof(unsigned int) * (UINT)indices.size());
	DC_BASIC* defaultTemp = nullptr;
	defaultTemp = dynamic_cast<DC_BASIC*>(m_drawCall.get());
	DC_SKELETON* skeletonTemp = nullptr;
	skeletonTemp = dynamic_cast<DC_SKELETON*>(m_drawCall.get());
	DC_GRASS* grassTemp = dynamic_cast<DC_GRASS*>(m_drawCall.get());
	if (defaultTemp)
	{
		defaultTemp->g_indexCount = (unsigned int)indices.size();
	}
	else if (skeletonTemp)
	{
		skeletonTemp->g_indexCount = (unsigned int)indices.size();
	}
	else if (grassTemp)
	{
		grassTemp->g_indexCount = (unsigned int)indices.size();
	}
	
}

void Shape::setTerrainPatches(ID3D11Device* device, std::vector<TERRAIN_PATCH>& patches)
{
	m_terrainPatches = patches;
	createVertexBuffer(device, patches.data(), sizeof(TERRAIN_PATCH) * (UINT)patches.size(), false);
	
}

void Shape::setVertices(ID3D11Device* device, std::vector<VERTEX>& vertices, bool dynamic)
{
	m_vertices = vertices;
	createVertexBuffer(device, m_vertices.data(), sizeof(VERTEX) * (UINT)vertices.size(), dynamic);
	

}
void Shape::createVertexBuffer(ID3D11Device* device, void* data, UINT size, bool dynamic, bool secondVB, bool streamOut)
{
	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_DEFAULT;
	if (dynamic)
	{
		vbd.Usage = D3D11_USAGE_DYNAMIC;
	}
	vbd.ByteWidth = size;
	if (streamOut)
	{
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	}
	else
	{
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	}
	vbd.MiscFlags = 0;
	vbd.CPUAccessFlags = 0;
	if (dynamic)
	{
		vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	D3D11_SUBRESOURCE_DATA vData = {};
	vData.pSysMem = data;
	if (streamOut)
	{
		DC_PARTICLE* particleTemp = nullptr;
		particleTemp = dynamic_cast<DC_PARTICLE*>(m_drawCall.get());
		if (particleTemp)
		{
			if (secondVB)
			{
				HRESULT hr = device->CreateBuffer(&vbd, &vData, particleTemp->g_extravertexBfr1.GetAddressOf());
				assert(SUCCEEDED(hr));
			}
			else
			{
				HRESULT hr = device->CreateBuffer(&vbd, &vData, particleTemp->g_extravertexBfr0.GetAddressOf());
				assert(SUCCEEDED(hr));
			}

		}

	}
	else
	{
		
		DC_BASIC* defaultTemp = nullptr;
		defaultTemp = dynamic_cast<DC_BASIC*>(m_drawCall.get());
		
		
		if (defaultTemp)
		{
			HRESULT hr = device->CreateBuffer(&vbd, &vData, defaultTemp->g_vertexAndInstanceBfr[0].GetAddressOf());
			assert(SUCCEEDED(hr));
		}
		else
		{
			DC_SKELETON* skeletonTemp = nullptr;
			skeletonTemp = dynamic_cast<DC_SKELETON*>(m_drawCall.get());
			DC_QUAD* defaultQuad = nullptr;
			defaultQuad = dynamic_cast<DC_QUAD*>(m_drawCall.get());
			DC_PARTICLE* defaultP = nullptr;
			defaultP = dynamic_cast<DC_PARTICLE*>(m_drawCall.get());
			DC_GRASS* defaultGrass = nullptr;
			defaultGrass = dynamic_cast<DC_GRASS*>(m_drawCall.get());
			if (defaultQuad)
			{
				HRESULT hr = device->CreateBuffer(&vbd, &vData, defaultQuad->g_vertexBfr.GetAddressOf());
				assert(SUCCEEDED(hr));
			}
			else if (skeletonTemp)
			{
				HRESULT hr = device->CreateBuffer(&vbd, &vData, skeletonTemp->g_vertexBfr.GetAddressOf());
				assert(SUCCEEDED(hr));
			}
			else if (defaultP)
			{
				HRESULT hr = device->CreateBuffer(&vbd, &vData, defaultP->g_vertexBfr.GetAddressOf());
				assert(SUCCEEDED(hr));
			}
			else if (defaultGrass)
			{
				HRESULT hr = device->CreateBuffer(&vbd, &vData, defaultGrass->g_vertexBfr.GetAddressOf());
				assert(SUCCEEDED(hr));
			}
		}
	}
}
void Shape::createIndexBuffer(ID3D11Device* device, void* data, UINT size)
{
	D3D11_BUFFER_DESC indexBDesc = {};


	indexBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBDesc.ByteWidth = size;
	indexBDesc.CPUAccessFlags = 0;
	indexBDesc.MiscFlags = 0;
	indexBDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA indexdata = {};
	indexdata.pSysMem = data;

	DC_BASIC* defaultTemp = nullptr;
	defaultTemp = dynamic_cast<DC_BASIC*>(m_drawCall.get());
	if(defaultTemp)
	{
		HRESULT hr = device->CreateBuffer(&indexBDesc, &indexdata, defaultTemp->g_indexBfr.GetAddressOf());
		assert(SUCCEEDED(hr));

	}
	else
	{
		DC_SKELETON* skeletonTemp = nullptr;
		DC_GRASS* grassTemp = nullptr;
		skeletonTemp = dynamic_cast<DC_SKELETON*>(m_drawCall.get());
		DC_QUAD* defaultQuad = nullptr;
		defaultQuad = dynamic_cast<DC_QUAD*>(m_drawCall.get());
		grassTemp = dynamic_cast<DC_GRASS*>(m_drawCall.get());
		if (defaultQuad)
		{
			HRESULT hr = device->CreateBuffer(&indexBDesc, &indexdata, defaultQuad->g_indexBfr.GetAddressOf());
			assert(SUCCEEDED(hr));
		}
		else if (skeletonTemp)
		{
			HRESULT hr = device->CreateBuffer(&indexBDesc, &indexdata, skeletonTemp->g_indexBfr.GetAddressOf());
			assert(SUCCEEDED(hr));
		}
		else if (grassTemp)
		{
			HRESULT hr = device->CreateBuffer(&indexBDesc, &indexdata, grassTemp->g_indexBfr.GetAddressOf());
			assert(SUCCEEDED(hr));
		}
	}
	
}
void Shape::createInstanceBuffer(ID3D11Device* device, void* data, UINT size)
{
	DC_BASIC* defaultTemp = nullptr;
	defaultTemp = dynamic_cast<DC_BASIC*>(m_drawCall.get());
	if (defaultTemp)
	{
		D3D11_BUFFER_DESC instBuffDesc = {};

		instBuffDesc.StructureByteStride = 0;
		instBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
		instBuffDesc.ByteWidth = size;
		instBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		instBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		instBuffDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA instData = {};
		instData.SysMemSlicePitch = 0;
		instData.SysMemPitch = 0;
		instData.pSysMem = data;
		HRESULT hr = device->CreateBuffer(&instBuffDesc, &instData, defaultTemp->g_vertexAndInstanceBfr[1].GetAddressOf());

		assert(SUCCEEDED(hr));
	}
}


void Shape::setParticles(ID3D11Device* device, std::vector<PARTICLE>& particles, bool dynamic, bool secondVB, bool so)
{
	m_particles = particles;
	createVertexBuffer(device, particles.data(), sizeof(PARTICLE) * (UINT)particles.size(), dynamic, secondVB, so);
}

void Shape::updateInstances(ID3D11DeviceContext* context)
{
	if (m_isInstanced)
	{
		D3D11_MAPPED_SUBRESOURCE mappedData = {};
		DC_BASIC* dc = dynamic_cast<DC_BASIC*>(m_drawCall.get());
		context->Map(dc->g_vertexAndInstanceBfr[1].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		INSTANCE_DATA* dataView = reinterpret_cast<INSTANCE_DATA*>(mappedData.pData);
		for (unsigned int i = 0; i < m_instanceCount; i++)
		{
			Matrix w = getWorld(i);
			Matrix iW = getInverseWorld(i);
			m_instanceData[i].world = w;
			m_instanceData[i].inverseWorld = w;
			dataView[i].world = w;
			dataView[i].inverseWorld = iW;
		}
		context->Unmap(dc->g_vertexAndInstanceBfr[1].Get(), 0);
	}
}

void Shape::generateBounds()
{
	if (m_isInstanced)
	{
		g_cull = true;
		m_boundingBoxHandler.setInstances((unsigned int)m_instanceData.size());
		for (unsigned int i = 0; i < m_instanceData.size(); i++)
		{
			if (!m_vertices.empty())
			{
				m_boundingBoxHandler.calcBounds(m_vertices, getWorld(i), getGlobalTransformations().rotQ, i);
			}
			else
			{
				m_boundingBoxHandler.calcBounds(m_terrainPatches, getWorld(), getGlobalTransformations().rotQ);
			}
		}
	}
	else
	{
		if (!m_vertices.empty())
		{
			m_boundingBoxHandler.calcBounds(m_vertices, getWorld(), getGlobalTransformations().rotQ);
		}
		else
		{
			m_boundingBoxHandler.calcBounds(m_terrainPatches, getWorld(), getGlobalTransformations().rotQ);
		}
		
		
	}
}

bool Shape::inBoundsPosX(float x, float z)
{
	return (x < getTerrainwidth());
}

bool Shape::inBoundsPosZ(float x, float z)
{
	return (z < getTerrainDepth());
}

bool Shape::inBoundsNegX(float x, float z)
{
	return (x > -getTerrainwidth());
}

bool Shape::inBoundsNegZ(float x, float z)
{
	return (z > -getTerrainDepth());
}

void Shape::updateCBfr(unsigned int instance)
{
	switch (m_dcType)
	{
	default:
	{
		break;
	}
	case DC_TYPE::BASIC:
	{
		
		DC_BASIC* dc = dynamic_cast<DC_BASIC*>(getDrawCall());
		Vector3 wp = getGlobalTransformations().trans;
		dc->g_objectData.g_inverseWorld = getInverseWorld(instance);
		dc->g_objectData.g_world = getWorld(instance);
		dc->g_objectData.g_particleWP = XMFLOAT4(wp.x, wp.y, wp.z, 1);
		break;
	}
	case DC_TYPE::COMPUTE_SHADER:
	{
		break;
	}
	case DC_TYPE::FULLSCREEN_QUAD:
	{
		break;
	}
	case DC_TYPE::MORPH:
	{
		
		break;
	}
	case DC_TYPE::PARTICLE:
	{
		DC_PARTICLE* dc = dynamic_cast<DC_PARTICLE*>(getDrawCall());
		Vector3 wp = getGlobalTransformations().trans;
		dc->g_objectData.g_inverseWorld = getInverseWorld(instance);
		dc->g_objectData.g_world = getWorld(instance);
		dc->g_objectData.g_particleWP = XMFLOAT4(wp.x, wp.y, wp.z, 1);
		break;
	}
	case DC_TYPE::SKELETON:
	{
		DC_SKELETON* dc = dynamic_cast<DC_SKELETON*>(getDrawCall());
		Vector3 wp = getGlobalTransformations().trans;
		dc->g_objectData.g_inverseWorld = getInverseWorld(instance);
		dc->g_objectData.g_world = getWorld(instance);
		dc->g_objectData.g_particleWP = XMFLOAT4(wp.x, wp.y, wp.z, 1);
		break;
	}
	case DC_TYPE::SKELETON_AND_MORPH:
	{
		break;
	}
	}
	
	
}



bool Shape::draw(Camera* camera, ID3D11DeviceContext* context)
{
	if (!g_render)
	{
		return false;
	}
	if (g_cull)
	{
		
		if (m_isInstanced)
		{
			D3D11_MAPPED_SUBRESOURCE mappedData = {};
			DC_BASIC* dc = dynamic_cast<DC_BASIC*>(m_drawCall.get());
			context->Map(dc->g_vertexAndInstanceBfr[1].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
			INSTANCE_DATA* dataView = reinterpret_cast<INSTANCE_DATA*>(mappedData.pData);
			int count = 0;
			if (m_BBCullType == BB_CULL_TYPE::BB_CULL_OBB)
			{
				for (unsigned int i = 0; i < m_instanceCount; i++)
				{
					if (m_transformType == TRANSFORM_TYPE::DYNAMIC)
					{
						const TRANSFORM_VALUES_GLOBAL& transforms = getGlobalTransformations(i);
						m_boundingBoxHandler.updateBounds(transforms.trans, transforms.rotQ, i);
					}
					if (!camera->cull(m_boundingBoxHandler.getBounds(i).obb)) //this is true if it should be culled
					{

						dataView[count++] = m_instanceData[i];
					}
				}

			}
			else if (m_BBCullType == BB_CULL_TYPE::BB_CULL_SPHERE)
			{
				for (unsigned int i = 0; i < m_instanceCount; i++)
				{
					if (m_transformType == TRANSFORM_TYPE::DYNAMIC)
					{
						const TRANSFORM_VALUES_GLOBAL& transforms = getGlobalTransformations(i);
						m_boundingBoxHandler.updateBounds(transforms.trans, transforms.rotQ, i);
					}
					if (!camera->cull(m_boundingBoxHandler.getBounds(i).boundingSphere))
					{

						dataView[count++] = m_instanceData[i];

					}
				}
			}
			else
			{
				for (unsigned int i = 0; i < m_instanceCount; i++)
				{
					if (m_transformType == TRANSFORM_TYPE::DYNAMIC)
					{
						const TRANSFORM_VALUES_GLOBAL& transforms = getGlobalTransformations(i);
						m_boundingBoxHandler.updateBounds(transforms.trans, transforms.rotQ, i);
					}
					if (!camera->cull(m_boundingBoxHandler.getBounds(i).aabb))
					{

						dataView[count++] = m_instanceData[i];

					}
				}
			}
			dc->g_instanceCount = count;
			
			context->Unmap(dc->g_vertexAndInstanceBfr[1].Get(), 0);
			if (count > 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (m_transformType == TRANSFORM_TYPE::DYNAMIC)
			{
				const TRANSFORM_VALUES_GLOBAL& transforms = getGlobalTransformations();
				m_boundingBoxHandler.updateBounds(transforms.trans, transforms.rotQ);
			}
			if (m_BBCullType == BB_CULL_TYPE::BB_CULL_OBB)
			{
				if (camera->cull(m_boundingBoxHandler.getBounds().obb))
				{
					return false;
				}
				else
				{
					return true;
				}
			}
			else if (m_BBCullType == BB_CULL_TYPE::BB_CULL_SPHERE)
			{
				if (camera->cull(m_boundingBoxHandler.getBounds().boundingSphere))
				{
					return false;
				}
				else
				{
					return true;
				}
			}
			else
			{
				if (camera->cull(m_boundingBoxHandler.getBounds().aabb))
				{
					return false;
				}
				else
				{
					return true;
				}
			}
		}

	}
	else
	{
		return true;
	}
}

void Shape::setInstances(ID3D11Device* device, const std::vector<INSTANCE_DATA>& instanceData)
{
	assert(m_vertices.size() != 0);
	m_instanceData = instanceData;
	createInstanceBuffer(device, m_instanceData.data(), sizeof(INSTANCE_DATA) * (UINT)instanceData.size());

	
	m_isInstanced = true;
	setInstanceCount((unsigned int)instanceData.size());
	DC_BASIC* temp = dynamic_cast<DC_BASIC*>(m_drawCall.get());
	temp->g_instanceCount = (unsigned int)instanceData.size();
}

bool Shape::getSurfaceHeight(float x, float z, float& height)
{
	for (int tri = 0; tri < (int)m_indices.size() / 3; tri++)
	{

		int index = tri * 3;
		Vector3 v0 = m_vertices[m_indices[index + 0]].pos;
		Vector3 v1 = m_vertices[m_indices[index + 1]].pos;
		Vector3 v2 = m_vertices[m_indices[index + 2]].pos;

		v0 = XMVector3TransformCoord(v0, getWorld());
		v1 = XMVector3TransformCoord(v1, getWorld());
		v2 = XMVector3TransformCoord(v2, getWorld());

		if (rayCast(x, z, height, v0, v1, v2))
		{

			return true;
		}

	}

	return false;
}

float Shape::getSurfaceHeight(float x, float z)
{
	
	float c = (x + 0.5f * getTerrainwidth()) / (float)CELL_SPACING;
	float d = (z - 0.5f * getTerrainDepth()) / (float)-CELL_SPACING;
	int row = (int)floorf(d);
	int col = (int)floorf(c);
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = m_hm[row * m_hmW + col];
	float B = m_hm[row * m_hmW + col + 1];
	float C = m_hm[(row + 1) * m_hmW+ col];
	float D = m_hm[(row + 1) * m_hmW+ col + 1];
	float s = c - (float)col;
	float t = d - (float)row;
	if (s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		return A + s * uy + t * vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f - s) * uy + (1.0f - t) * vy;
	}

	
}

void DC_COMPUTE::pureVirtual()
{
	return;
}

void DC_QUAD::pureVirtual()
{
}

void DC_SKELETON::pureVirtual()
{
}

DC_SKELETON::~DC_SKELETON()
{
	for (auto map : g_animations)
	{
		RELEASE(map.second);
	}
}

void DC_PARTICLE::pureVirtual()
{
	return;
}

void DC_GRASS::pureVirtual()
{
}
