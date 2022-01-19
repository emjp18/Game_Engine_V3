#pragma once
#ifndef SHAPE_H
#define SHAPE_H
#include "Camera.h"
#include "BoundingBoxHandler.h"
#include "Skeleton.h"
struct DC
{
    
    SHADER_BIND_FLAG g_shaders;
    std::unordered_map<unsigned int, std::string> g_srvs;
    RENDERPASS g_renderPassTypes;
    RENDER_TYPE g_renderType;
    virtual void pureVirtual() = 0;
};

struct DC_BASIC : DC
{
    WRL::ComPtr<ID3D11Buffer> g_vertexAndInstanceBfr[2] = { nullptr, nullptr };
    WRL::ComPtr<ID3D11Buffer> g_indexBfr = nullptr;
    CB_OBJECT g_objectData;
    unsigned int g_instanceCount = 0;
    unsigned int g_indexCount = 0;
    D3D11_PRIMITIVE_TOPOLOGY g_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    // Inherited via DRAW_CALL
    virtual void pureVirtual() override;
};
struct DC_GRASS : DC
{
    WRL::ComPtr<ID3D11Buffer> g_vertexBfr = nullptr;
    WRL::ComPtr<ID3D11Buffer> g_indexBfr = nullptr;
    CB_OBJECT g_objectData;
    std::vector<GRASS_STRAND> g_strands;
    std::vector<unsigned int> g_strandsIndices;
    std::vector<XMFLOAT3> g_strandsBC;
    unsigned int g_indexCount = 0;
    D3D11_PRIMITIVE_TOPOLOGY g_topology = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
    // Inherited via DRAW_CALL
    virtual void pureVirtual() override;
};
struct DC_COMPUTE : DC
{
    XMUINT3 g_dispatchThreads;
    std::unordered_map<unsigned int, std::string> uavs;
    // Inherited via DRAW_CALL
    virtual void pureVirtual() override;
};
struct DC_QUAD : DC
{
   
    bool vertexShaderOne = false;
    WRL::ComPtr<ID3D11Buffer> g_vertexBfr = nullptr;
    WRL::ComPtr<ID3D11Buffer> g_indexBfr = nullptr;
    unsigned int g_indexCount = 6;
    D3D11_PRIMITIVE_TOPOLOGY g_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    // Inherited via DRAW_CALL
    virtual void pureVirtual() override;
};
struct DC_SKELETON : DC
{
    std::string g_currentTrack = G_S_IDLE;
    std::unordered_map<std::string, AnimationHandler*> g_animations;
    CB_OBJECT g_objectData;
    WRL::ComPtr<ID3D11Buffer> g_vertexBfr = nullptr;
    WRL::ComPtr<ID3D11Buffer> g_indexBfr = nullptr;
    unsigned int g_indexCount = 0;
    D3D11_PRIMITIVE_TOPOLOGY g_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    // Inherited via DRAW_CALL
    virtual void pureVirtual() override;
    ~DC_SKELETON();
};

struct DC_PARTICLE : DC
{
    CB_PARTICLE g_particleData;
    CB_OBJECT g_objectData;
    D3D11_PRIMITIVE_TOPOLOGY g_topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
    WRL::ComPtr<ID3D11Buffer> g_vertexBfr = nullptr;
    WRL::ComPtr<ID3D11Buffer> g_extravertexBfr0 = nullptr;
    WRL::ComPtr<ID3D11Buffer> g_extravertexBfr1 = nullptr;
    bool g_particleFirstRun = true;
    // Inherited via DRAW_CALL
    virtual void pureVirtual() override;
};

class Shape :
    public Transform
{
private:
    //Terrain
    UINT m_numPatchVerPerRow;
    UINT m_numPatchVerPerCol;
    UINT m_numPatchVer;
    UINT m_numPatchQuadFace;
    int m_hmW = HMW;
    int m_hmH = HMH;
    std::vector<float> m_hm;
    float m_maxX;
    float m_maxZ;
    float m_minX;
    float m_minZ;
    //
   
    DC_TYPE m_dcType;
    std::unordered_map<unsigned int, std::string> m_srvs;
    std::unordered_map<unsigned int, std::string> m_uavs;
    SHADER_BIND_FLAG m_shaders;
    bool m_isInstanced = false;
    BoundingBoxHandler m_boundingBoxHandler;
    std::shared_ptr<DC> m_drawCall = nullptr;
    std::string m_name = "No name";
    BB_CULL_TYPE m_BBCullType = BB_CULL_TYPE::BB_CULL_AABB;
    std::vector<TERRAIN_PATCH> m_terrainPatches;
    std::vector<PARTICLE> m_particles;
    std::vector<VERTEX> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<INSTANCE_DATA> m_instanceData;
    void createVertexBuffer(ID3D11Device* device, void* data, UINT size, bool dynamic = false,  bool secondVB = false, bool streamOut = false);
    void createIndexBuffer(ID3D11Device* device, void* data, UINT size);
    void createInstanceBuffer(ID3D11Device* device, void* data, UINT size);
    bool rayCast(float x, float z, float& h, Vector3& v0, Vector3& v1, Vector3& v2);
    float getTerrainwidth() { return (float)(m_hmW - 1) * (float)CELL_SPACING; }
    float getTerrainDepth() { return (float)(m_hmH - 1) * (float)CELL_SPACING; }
   
public:
    Shape(ID3D11Device* device, const DC_TYPE& drawType = DC_TYPE::BASIC, const RENDERPASS& renderpasses = RENDERPASS::BASIC_PASS_SINGLE, const RENDER_TYPE& renderType = RENDER_TYPE::BASIC);
    ~Shape();
    DC* getDrawCall() { return m_drawCall.get(); }
    void setTerrainPatches(ID3D11Device* device, std::vector<TERRAIN_PATCH>& patches);
    void setVertices(ID3D11Device* device, std::vector<VERTEX>& vertices, bool dynamic = false);
    void setParticles(ID3D11Device* device, std::vector<PARTICLE>& particles, bool dynamic = false,bool secondVB = false, bool so = false);
    void setIndices(ID3D11Device* device, const std::vector<unsigned int>& indices);
    void setInstances(ID3D11Device* device, const std::vector<INSTANCE_DATA>& instanceData);
    void setTerrain(const std::vector<float>& hm) { m_hm = hm; }
    bool g_cull = false;
    bool g_render = true;
    bool g_renderBack = false;
    bool g_renderWireFrame = false;
    bool g_canMove = true;
    std::string getName() { return m_name; }
    BB_CULL_TYPE getBB_type() { return m_BBCullType; }
    void setBBCullType(BB_CULL_TYPE type) { m_BBCullType = type; }
    bool getSurfaceHeight(float x, float z, float& h);
    float getSurfaceHeight(float x, float z);
    bool draw(Camera* camera, ID3D11DeviceContext* context);
    BoundingBoxHandler& getBB() { return m_boundingBoxHandler; }
    void setName(std::string name) { m_name = name; }
    void updateInstances(ID3D11DeviceContext* context);
    bool isInstanced() { return m_isInstanced; }
    bool g_renderClockWise = false;
    void generateBounds(); //do tihs after you have set the buffers.
    bool inBoundsPosX(float x, float z);
    bool inBoundsPosZ(float x, float z);
    bool inBoundsNegX(float x, float z);
    bool inBoundsNegZ(float x, float z);
    void setMaxX(float m) { m_maxX = m; }
    void setMaxZ(float m) { m_maxZ = m; }
    void setMinX(float m) { m_minX = m; }
    void setMinZ(float m) { m_minZ = m; }
    const std::vector<VERTEX>& getVertices() { return m_vertices; }
    const std::vector<unsigned int>& getIndices() { return m_indices; }
    DC_TYPE getDCType() { return m_dcType; }
    const std::vector<TERRAIN_PATCH>& getPatches() { return m_terrainPatches; }
    void updateCBfr(unsigned int instance = 0);
    
};

#endif