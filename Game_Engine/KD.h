#pragma once
#include "LODHandler.h"
class KD :
    public LODHandler
{
private:
protected:
    AABB m_worldBounds;
    std::vector<Shape*> m_shapes;
   
    virtual void release(KD_NODE* node) = 0;
    virtual void divide(ID3D11Device* device, KD_NODE* node) = 0;
    virtual unsigned int countTrianglesInsideNode(const AABB& bounds, Shape* shape) = 0;
    virtual bool isInsideAABB(const AABB& bounds, Shape* shape, int triangle) = 0;
    virtual Shape* separate(const AABB& bounds, Shape* shape, ID3D11Device* device) = 0;
public:
    virtual void generateWorldBounds();
    virtual void setShapes(const std::vector<Shape*>& shapes) { m_shapes = shapes; }
    virtual void setWorldBounds(const AABB& bounds) { m_worldBounds = bounds; }
    virtual KD_NODE* getRoot() = 0;
    virtual void buildHiearchy(ID3D11Device* device) = 0;

    
 
};

