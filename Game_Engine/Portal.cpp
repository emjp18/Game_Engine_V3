#include "stdafx.h"
#include "Portal.h"



Portal::~Portal()
{
    for (auto m : m_nodes)
    {
        
        RELEASE(m.second);
    }
    for (auto p : m_portalV)
    {
        RELEASE(p)
    }
}

bool Portal::isVisible(const Vector4& Plane, PORTAL* p)
{
   
    Vector3 N = XMFLOAT3(Plane.x, Plane.y, Plane.z);

    int eps = 0;
   
    int numInFront = 0, numBehind = 0;
    int numVerts = 4;
    for (int i = 0; i < numVerts; i++) {
        Vector3 point = p->polygon.vertices[i].pos;
        float signedD = (point.Dot(N)) - Plane.w;
        if (signedD > FLT_EPSILON)
        {
            eps = 1;
        }
        else if (signedD < -FLT_EPSILON)
        {
            eps = -1;
        }
        else
        {
            eps = 0;
        }
        switch (eps) {
        default:
        {
            break;
        }
            
        case 1:
        {
            numInFront++;
            break;
        }
            
        case -1:
        {
            numBehind++;
            break;
        }
            
        }
    }
    // If vertices on both sides of the plane, the polygon is straddling
    if (numBehind != 0 && numInFront != 0)
    {
        return true;
    }
       
    // If one or more vertices in front of the plane and no vertices behind
    // the plane, the polygon lies in front of the plane
    if (numInFront != 0)
    {
        return true;
    }
        
    // Ditto, the polygon lies behind the plane if no vertices in front of
    // the plane, and one or more vertices behind the plane
    if (numBehind != 0)
    {
        return false;
    }
        
    // All vertices lie on the plane so the polygon is coplanar with the plane
    return false;
   
    
    //const int SIDE_BEHIND = 0;
    //const int SIDE_FRONT = 1;
    //const int SIDE_ON = 2;

    //
    //int numPoints = 4;

    //float dist = 0;
    //int sides[4];
    //float dists[4];
    //int counts[3];

    //for (int i = 0; i < numPoints; i++)
    //{
    //    Vector3 point = p->polygon.vertices[i].pos;
    //    dists[i] = dist = N.Dot(point)-Plane.w;
    //    if (dist > FLT_EPSILON) sides[i] = SIDE_FRONT;
    //    else if (dist < -FLT_EPSILON) sides[i] = SIDE_BEHIND;
    //    else sides[i] = SIDE_ON;
    //    counts[sides[i]]++;
    //}

    //// no point is clipped
    //if (counts[SIDE_BEHIND] == 0) return true;
    //if (counts[SIDE_FRONT] + counts[SIDE_ON] == 0) return false;

    //return true;
}



void Portal::createHiearchy(std::vector<Shape*>& portalShapes)
{
	std::ifstream file("../Resources/Textfiles/PortalCulling.txt");

	assert(file.is_open());
    std::unordered_map<std::string, std::vector<std::string>> childNames;
    
	while (file)
	{
		std::string line;
		std::getline(file, line);
		std::stringstream ss(line);
		std::string word;
		std::string path;
		if (line == "")
		{
			break;
		}
		while (ss)
		{
			ss >> word;

           
            for (int name = 0; name < portalShapes.size(); name++)
            {
                std::vector<std::string> childNames1;
                std::vector<std::string> portalNames;
                if (word == portalShapes[name]->getName()) //strcmp is not needed
                {
                    PORTAL_ROOM* node = new PORTAL_ROOM;
                    node->name = portalShapes[name]->getName();
                    node->shape = portalShapes[name];
                    
                    int nrOfObjects = 0;
                    ss >> word;
                    nrOfObjects = std::stoi(word);
                  
                    for (int object = 0; object < nrOfObjects; object++)
                    {
                        ss >> word;
                        for (int childName = 0; childName < portalShapes.size(); childName++)
                        {
                            if (word == portalShapes[childName]->getName())
                            {
                                node->objects.push_back(portalShapes[childName]);
                            }
                        }
                    }
                    int nrOfPortals = 0;
                    ss >> word;
                    nrOfPortals = std::stoi(word);
                    
                    for (int portal = 0; portal < nrOfPortals; portal++)
                    {
                        ss >> word;
                        if (std::find(portalNames.begin(), portalNames.end(), word) == portalNames.end())
                        {
                            for (int childName = 0; childName < portalShapes.size(); childName++)
                            {
                                if (word == portalShapes[childName]->getName())
                                {
                                    PORTAL* portal = new PORTAL;
                                    portal->name = word;
                                    portal->polygon.vertices[0] = portalShapes[childName]->getVertices()[0];
                                    portal->polygon.vertices[1] = portalShapes[childName]->getVertices()[1];
                                    portal->polygon.vertices[2] = portalShapes[childName]->getVertices()[2];
                                    portal->polygon.vertices[3] = portalShapes[childName]->getVertices()[3];
                                    portal->rooms.push_back(node->name);
                                    m_portalV.push_back(portal);
                                    m_portalM.insert({ word , portal });
                                    portalShapes[childName]->g_render = false;
                                    node->portals.push_back(portal);
                                    portal->plane = createPlane(portal->polygon.vertices[0].pos, portal->polygon.vertices[1].pos, portal->polygon.vertices[2].pos, portal->polygon.vertices[3].pos);
                                    portal->shape = portalShapes[childName];
                                    portal->shape->generateBounds();
                                }
                            }
                            portalNames.push_back(word);
                        }
                        else
                        {
                            auto it = std::find(portalNames.begin(), portalNames.end(), word);
                            std::string n = portalNames[std::distance(portalNames.begin(), it)];
                            m_portalM[n]->rooms.push_back(node->name);
                            node->portals.push_back(m_portalM[n]);
                        }
                      
                    }
                    int nrOfChildren = 0;
                    ss >> word;
                    nrOfChildren = std::stoi(word);
                    
                    for (int child = 0; child < nrOfChildren; child++)
                    {
                        ss >> word;
                        
                        childNames1.push_back(word);
                    }

                    
                    childNames.insert({ portalShapes[name]->getName() ,childNames1 });
                   
                    m_nodes.insert({ portalShapes[name]->getName(), node });
                    break;
                }
            }
			break;
		}
	}
	file.close();
    
    for (auto node : m_nodes)
    {
        node.second->shape->generateBounds();
        if (childNames.find(node.first) != childNames.end())
        {
            for (int child = 0; child < childNames[node.first].size(); child++)
            {
                node.second->children.push_back(m_nodes[childNames[node.first][child]]);
                m_nodes[childNames[node.first][child]]->parent = node.second;
            }
            
        }
        m_nodesV.push_back(node.second);
    }

   

}



const Vector3& Portal::getRoomCenter(std::string name)
{
    return m_nodes[name]->shape->getBB().getBounds().aabb.center;
}
