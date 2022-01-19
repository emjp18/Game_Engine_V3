#pragma once
#include "KD.h"
class OcTree :
    public KD
{
private:
    OCTREE_NODE* m_root = nullptr;
    std::vector<Shape*> m_newShapes;
   /* std::vector<Shape*> m_bbShapes;*/
    std::vector<AABB> m_bb;
    Shape* m_visBB = nullptr;
    int id = 0;
public:
    virtual ~OcTree();
    const  std::vector<Shape*>& getNewShapes() { return m_newShapes; }
    // Inherited via KD
    virtual void release(KD_NODE* node) override;
    virtual void divide(ID3D11Device* device, KD_NODE* node) override;
    virtual unsigned int countTrianglesInsideNode(const AABB& bounds, Shape* shape) override;
    virtual bool isInsideAABB(const AABB& bounds, Shape* shape, int triangle) override;
    virtual Shape* separate(const AABB& bounds, Shape* shape, ID3D11Device* device) override;
    virtual KD_NODE* getRoot() override;
    virtual void buildHiearchy(ID3D11Device* device) override;
    virtual Shape* visualizeBoundingBoxes(Shape* bb, ID3D11Device* device, ID3D11DeviceContext* context);
};

