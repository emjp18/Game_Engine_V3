#pragma once
#include "common.h"
#include "Shape.h"

struct POLYGON_TRIS
{
    VERTEX vertices[3];
    bool negaticeFace = true;
};
struct POLYGON_QUAD
{
    VERTEX vertices[4];
};
struct PORTAL
{
    std::string name;
    POLYGON_QUAD polygon;
    std::vector<std::string> rooms;
    Vector4 plane;
    Shape* shape;
};
struct PORTAL_ROOM
{
    Shape* shape = nullptr;
    std::string name;;
    std::vector<PORTAL_ROOM*> children;
    PORTAL_ROOM* parent = nullptr;
    std::vector<Shape*> objects;
    std::vector< PORTAL*> portals;
};
struct KD_NODE
{
    AABB bounds;
    std::vector<Shape*> shapes;
    virtual void pureVirtual() = 0;
};

struct QUADTREE_NODE : KD_NODE
{

    QUADTREE_NODE* parent = nullptr;
    QUADTREE_NODE* children[4] = { nullptr, nullptr,nullptr,nullptr };
    Shape* grass = nullptr;
    
    // Inherited via KD_NODE
    virtual void pureVirtual() override;

};
struct OCTREE_NODE : KD_NODE
{
    OCTREE_NODE* parent = nullptr;
    OCTREE_NODE* children[8] = { nullptr, nullptr,nullptr,nullptr,nullptr, nullptr,nullptr,nullptr };
    int trianglesInNode = 0;
    int ID = -1;
    // Inherited via KD_NODE
    virtual void pureVirtual() override;
};
class LODHandler
{
protected:
    Vector3 m_i;
    VERTEX m_v;
    CLASSIFICATION classify(POLYGON_TRIS* triangle, PLANE* plane);
    int classify(Vector3& point, PLANE* plane);
    int classify2(Vector3& point, PLANE* plane);
    PLANE* getPlaneFromTriangles(std::vector<POLYGON_TRIS*>& tri);
    PLANE* getPlaneFromTriangle(POLYGON_TRIS* tri);
    Vector3& rayPlaneInters(Ray& ray, PLANE* plane, float& t);
    Vector4 createPlane(const Vector3& zero, const Vector3& one, const Vector3& two, const Vector3& three);
    Vector4 createPlane(const Vector3& zero, const Vector3& one, const Vector3& two);
    Vector4 createPlane(const Vector3& zero, const Vector3& normal);
public:
	
};

