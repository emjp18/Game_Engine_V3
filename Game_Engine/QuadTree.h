#pragma once
#include "KD.h"
class QuadTree :
    public KD
{
private:
    QUADTREE_NODE* m_root = nullptr;
    std::vector<Shape*> m_newShapes;
    Shape* createGrassShape(Shape* base, ID3D11Device* device);
    std::vector<Shape*> m_grassShapes;
    float satRandF();
    float randF(float a, float b);
    void Barycentric(Vector3 p, Vector3 a, Vector3 b, Vector3 c, float& u, float& v, float& w);
public:
    virtual ~QuadTree();
    const std::vector<Shape*>& getNewShapes() { return m_newShapes; }
    const std::vector<Shape*>& getGrassShapes() { return m_grassShapes; }
    // Inherited via KD
    virtual KD_NODE* getRoot() override;
    virtual void buildHiearchy(ID3D11Device* device) override;
    void visualizeBoundingBoxes();
private:
    
    virtual void release(KD_NODE* node) override;
    virtual void divide(ID3D11Device* device, KD_NODE* node) override;
    virtual unsigned int countTrianglesInsideNode(const AABB& bounds, Shape* shape) override;
    virtual bool isInsideAABB(const AABB& bounds, Shape* shape, int triangle) override;
    virtual Shape* separate(const AABB& bounds, Shape* shape, ID3D11Device* device) override;
};

