#pragma once
#ifndef UPDATEDRAWCLASS_H
#define UPDATEDRAWCLASS_H
#include "GuiHandler.h"
#include "PlayerCharacter.h"
#include "Camera.h"
#include "ShaderHandler.h"
#include "SBfr.h"
#include "CBfr.h"
#include "Light.h"
#include "RenderPassHandler.h"
#include "Portal.h"
#include "QuadTree.h"
#include "OcTree.h"
#include "BSP.h"
class UpdateDrawClass
{
private:
	BSP* m_binaryTree = nullptr;
	std::vector<Shape*> m_ocTreeShapes;
	OcTree* m_ocTree = nullptr;
	QuadTree* m_quadTree = nullptr;
	Portal* m_portal = nullptr;
	std::vector<Shape*> m_quadTreeShapes;
	std::vector<Shape*> m_portalShapes;
	PlayerCharacter m_player;
	UINT m_vStride = sizeof(VERTEX);
	UINT m_pStride = sizeof(PARTICLE);
	UINT m_offset = 0;
	UINT m_vInStride[2] = { sizeof(VERTEX),sizeof(INSTANCE_DATA) };
	UINT m_offsetTwo[2] = { 0 ,0 };
	UINT m_tStride = sizeof(TERRAIN_PATCH);
	Camera* m_mainCam = nullptr;
	std::vector<Shape*> m_shapes;
	Shape* m_dynamicSkyShape = nullptr;
	Vector3 m_camUp[6];
	Vector3 m_camTarget[6];
	Camera* m_dynamicCubeCameras[6] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };
	Timer m_timer;
	Light* m_shadowPointLight = nullptr;
	std::vector<LIGHT_DATA> m_lightData;
	Shape* m_playerShape = nullptr;
	Shape* m_clothShape = nullptr;
	Shape* m_skyShape = nullptr;
	Shape* m_terrainShape = nullptr;
	bool m_hasDynamicLight = false;
	std::vector<Light*> m_lights;
	Light* m_directionalLight;
	const float m_DEFAULT_BG_COLOR[4] = { 0.6f, 0.6f, 0.600f, 1.0f };
	const float m_mask[4] = { 0.0f,0.0f,0.0f,0.0f };
	 ID3D11BlendState* m_blendOn = nullptr;
	 ID3D11BlendState* m_blendOff = nullptr;
	 ID3D11BlendState* m_blendParticle = nullptr;
	ID3D11RasterizerState* m_shadowRS = nullptr;
	ID3D11RasterizerState* m_mainRS = nullptr;
	ID3D11RasterizerState* m_cullNoneRS = nullptr;
	ID3D11RasterizerState* m_clockWiseRS = nullptr;
	ID3D11RasterizerState* m_wireframeRS = nullptr;
	ID3D11DepthStencilState* m_depthDisableDSS = nullptr;
	ID3D11DepthStencilState* m_mainDSS = nullptr;
	ID3D11DepthStencilView* m_mainDSV = nullptr;
	ID3D11RenderTargetView* m_mainRTV = nullptr;
	ID3D11DepthStencilView* m_nullDSV = nullptr;
	ID3D11UnorderedAccessView* m_nullUAV = nullptr;
	ID3D11ShaderResourceView* m_nullSRV = nullptr;
	ID3D11RenderTargetView* m_nullRTV = nullptr;
	SHADER_BIND_FLAG m_nullShaderBindflag;
	std::vector<int> m_nullSlotsSRV;
	std::vector<int> m_nullSlotsUAV;
	CB_SCENE m_sceneConstants;
	CB_SHADOW_POINT m_shadowPointConstants;
	CBfr m_shadowPointBfr;
	CBfr m_objectBfr;
	CBfr m_sceneBfr;
	CBfr m_grassBfr;
	CBfr m_particleBfr;
	SBfr m_lightBfr;
	SBfr m_lightCounterBfr;
	SBfr m_lightIndexBfr;
	SBfr m_skeletonBfr;
	SBfr m_strandsBCBfr;
	SBfr m_strandsIBfr;
	SBfr m_strandsBfr;
	Shape* m_CSLightCullingShape = nullptr;
	void resetRTVtoMain(ID3D11DeviceContext* context, const D3D11_VIEWPORT& viewPort);
	void bindNull(SHADER_BIND_FLAG flag, std::vector<int> slotsSRV, std::vector<int> slotsUAV, ID3D11DeviceContext* context);
	void renderParticleSys(DC_PARTICLE* system, ID3D11DeviceContext* context);
	void updateDynamicCubeMap(Vector3& t);
	void bindResources(Shape* shape, RENDERPASS currentPass, ID3D11DeviceContext* context);
	void bindResourcesBasic(Shape* shape, ID3D11DeviceContext* context);
	void renderPortalCulling();
	void initPortalCulling();
	void initBinaryTree(Shape* shape, ID3D11Device* device);
	void renderBinaryTree(BINARY_NODE* node);
	void renderPortal();
	void initQuadTree(ID3D11Device* device);
	void initOcTree(ID3D11Device* device, ID3D11DeviceContext* context);
	void renderQuadTree(QUADTREE_NODE* node);
	void renderOcTree(OCTREE_NODE* node);
	void findChildrenOcTree(OCTREE_NODE* node);
	int m_fireSize = 10;
	int m_leafSize = 10;
	int m_smokeSize = 10;
	int m_textSize = 10;
	float m_mouseSensitivity = 0.006f;
	float m_walkSpeed = 5.50f;
	float m_turnSpeed = 10.0f;
	float m_runSpeed = 10.0f;
	Shape* m_boundingBoxesVis = nullptr;
	bool m_renderBoundingBoxes = false;
	bool m_renderBoundingBoxesWireFrame = false;
	int m_currInstanceCount = 0;
	std::unordered_map<int, int> m_triangleCount;
public:
	UpdateDrawClass();
	~UpdateDrawClass();
	void initBfrs(ID3D11Device* device, size_t currResX, size_t currResY, SCENE scene, bool firstTime);
	void setPointers(ID3D11BlendState* blendOn, ID3D11BlendState* blendOff, ID3D11BlendState* blendParticle,
		ID3D11RasterizerState* shadowRS, ID3D11RasterizerState* mainRS, ID3D11RasterizerState* cullNoneRS, ID3D11RasterizerState* clockWiseRS,
		ID3D11RasterizerState* wireframeRS, ID3D11DepthStencilState* disableDepthDSS, ID3D11DepthStencilState* mainDSS, ID3D11DepthStencilView* mainDSV, ID3D11RenderTargetView* mainRTV) {
		m_blendOn = blendOn; m_blendOff = blendOff; m_blendParticle = blendParticle; m_shadowRS = shadowRS; m_mainRS = mainRS; m_cullNoneRS = cullNoneRS; m_clockWiseRS = clockWiseRS; 
		m_wireframeRS = wireframeRS; m_depthDisableDSS = disableDepthDSS; m_mainDSS = mainDSS; m_mainDSV = mainDSV; m_mainRTV = mainRTV;
	}
	Shape* getCSCullingShape() { return m_CSLightCullingShape; }
	void setShapes(const std::vector<Shape*>& shapes, const SCENE& scene, ID3D11Device* device, ID3D11DeviceContext* context,const std::vector<Shape*>& shapesQuad, const std::vector<Shape*>& shapesOc, Shape* bspShape = nullptr, Shape* bb = nullptr);
	void setLights(const std::vector<Light*>& lights,ID3D11Device* device, Light* dirLight = nullptr);
	void renderBackground(ID3D11DeviceContext* context, const D3D11_VIEWPORT& viewPort);
	void renderMain(ID3D11DeviceContext* context,const D3D11_VIEWPORT& viewPort, SCENE scene);
	void renderDebugLight(ID3D11DeviceContext* context, const D3D11_VIEWPORT& viewPort);
	void changeRes(size_t currResX, size_t currResY, ID3D11Device* device);
	void restart();
	void updateTransform(ID3D11DeviceContext* context);
	Camera* getCamera() { return m_mainCam; }
	void updateIMGUI(int fireSize, int smokeSize, int leafSize, int textSize, float mouseSense, float runSpeed, float walkSpeed, float turnSpeed, bool bb, bool bbwf);
	void updateIMGUI(int& triangleCount, int& currInstanceCount);
	
	
};

#endif