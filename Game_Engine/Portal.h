#pragma once
#include "LODHandler.h"

class Portal :
    public LODHandler
{
private:
    std::unordered_map<std::string, PORTAL_ROOM*> m_nodes;
    std::vector< PORTAL_ROOM*> m_nodesV;
    std::unordered_map<std::string, PORTAL*> m_portalM;
    std::vector< PORTAL*> m_portalV;
    std::unordered_map<std::string, PORTAL_ROOM*> m_visibleRooms;
public:
    ~Portal();
    bool isVisible(const Vector4& Plane, PORTAL* portal);
    bool isVisible(const Vector4& Plane, PORTAL* portal, const Vector3& camPos);
    void createHiearchy(std::vector<Shape*>& portalShapes);
    const std::unordered_map<std::string, PORTAL_ROOM*>& getNodes() { return m_nodes; }
    const std::unordered_map<std::string, PORTAL_ROOM*>& getVisibleRooms() { return m_visibleRooms; }
    const std::unordered_map<std::string, PORTAL*>& getPortalM() { return m_portalM; }
    const  std::vector< PORTAL*>& getPortalV() { return m_portalV; }
    PORTAL_ROOM* getRoom(unsigned int nr) { return m_nodesV[nr]; }
    int findClosestRoom(const Vector3& point);
    PORTAL_ROOM* getRoom(std::string name) { return m_nodes[name]; }
    const Vector3& getRoomCenter(std::string name);
};

