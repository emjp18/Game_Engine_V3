#pragma once
#include "LODHandler.h"
struct BINARY_NODE
{
	BINARY_NODE* frontchild = nullptr;
	BINARY_NODE* backchild = nullptr;
	std::vector<POLYGON_TRIS*> polygons;
	PLANE* plane = nullptr;
	Shape* shape = nullptr;
	AABB aabb;
	BOUNDING_SPHERE bs;

};
class BSP :
    public LODHandler
{
private:
	Shape* shape = nullptr;
	std::vector<XMFLOAT4*> m_planes;
	std::vector<POLYGON_TRIS*> m_polygons;
	std::vector<POLYGON_TRIS*> m_allpolygons;
	std::vector< Shape*> m_allShapes;
	POLYGON_TRIS** m_poly = nullptr;
	POLYGON_TRIS** m_frontpoly = nullptr;
	POLYGON_TRIS** m_backpoly = nullptr;
	BoundingBoxHandler m_boundingBoxHandler;
	void release(BINARY_NODE* node);
	bool shouldSwitchNormal(Vector3& a, Vector3& b, Vector3& c, bool orientationShouldBeNegative);
	void calcNormals(POLYGON_TRIS* triangle);
	void buildTree(BINARY_NODE*& node, std::vector<POLYGON_TRIS*>& polygons, int& depth);
	void split(POLYGON_TRIS* poly, PLANE* part, std::vector<POLYGON_TRIS*>& front, std::vector<POLYGON_TRIS*>& back);
	
	void splitShape(BINARY_NODE* node, ID3D11Device* device);
	VERTEX& interpolate(VERTEX& a, VERTEX& b, float t);
	float findT(Vector3& posA, Vector3& posB, Vector3& result);
public:
	BSP();
	~BSP();
	void init(Shape* shapeToSplit, ID3D11Device* device);
	int classifyCamera(Vector3& point, PLANE* plane);
	BINARY_NODE* g_root = nullptr;
	const std::vector< Shape*>& getAllShapes() { return m_allShapes; }

};

