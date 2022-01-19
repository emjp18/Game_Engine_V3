#pragma once
#ifndef SCENEHANDLER_H
#define SCENEHANDLER_H
#include "Parser.h"
class SceneHandler
{
private:
	float m_maxX;
	float m_maxZ;
	float m_minX;
	float m_minZ;
	Shape* m_bspShape = nullptr;
	std::vector<Shape*> m_ocShapes;
	std::vector<Shape*> m_quadShapes;
	std::vector<XMFLOAT2> m_patchBoundsY;
	WRL::ComPtr<ID3D11ShaderResourceView> m_hmSRV;
	UINT m_numPatchVerPerRow;
	UINT m_numPatchVerPerCol;
	UINT m_numPatchVer;
	UINT m_numPatchQuadFace;
	int m_hmW = HMW;
	int m_hmH = HMH;
	std::vector<float> m_hm;
	std::vector<Shape*> m_moveAndRotateShapes;
	std::vector<Shape*> m_allSceneShapes;
	std::vector<Light*> m_allSceneLights;
	Light* m_dirLight = nullptr;
	Light* m_shadowPoint = nullptr;
	Shape* m_terrainShape = nullptr;
	Shape* m_playerShape = nullptr;
	Shape* m_playerClothShape = nullptr;
	SCENE m_scene;
	Timer m_timer;
	bool m_hideMouse = false;
	bool m_freeMouse = true;
	void moveAndRotate(Shape* shape);
	void moveAndRotateLights();
	void visualizeBoundingBoxes(std::vector<Shape*> shapesWithBounds, bool move);
	Shape* createParticleSystem(RENDER_TYPE type);
	Shape* createSkySphere(bool fromDDS = false);
	Shape* createTerrain();
	void buildHMSRV();
	bool inBounds(int i, int j);
	float average(int i, int j);
	void smooth();
	unsigned short float_to_half(const float x);
	unsigned int as_uint(const float x);
	float getTerrainwidth() { return (float)(m_hmW - 1) * (float)CELL_SPACING; }
	float getTerrainDepth() { return  (float)(m_hmH - 1) * (float)CELL_SPACING; }
	void calcAllPatchBoundsY();
	void calcPatchBoundsY(UINT i, UINT j);
	void loadPortalCullingTest();
	size_t m_currResX;
	size_t m_currResY;
	float satRandF();
	float randF(float a, float b);
	void Barycentric(Vector3 p, Vector3 a, Vector3 b, Vector3 c, float& u, float& v, float& w);
	Shape* m_bb = nullptr;
public:
	SceneHandler(SCENE scene, size_t x, size_t y);
	~SceneHandler();
	const std::vector<Shape*>& getShapes() { return m_allSceneShapes; }
	const std::vector<Shape*>& getOcShapes() { return m_ocShapes; m_ocShapes.clear();  }
	const std::vector<Shape*>& getQuadShapes() { return m_quadShapes; m_quadShapes.clear(); }
	const std::vector<Light*>& getLights() { return m_allSceneLights; }
	Shape* getPlayer() { return m_playerShape; }
	Shape* getTerrain() { return m_terrainShape; }
	void update();
	Light* getDirLight() { return m_dirLight; }
	Shape* getBSP() { return m_bspShape; }
	Shape* getBB() { return m_bb; }
};

#endif