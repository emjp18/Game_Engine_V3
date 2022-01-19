#include "stdafx.h"
#include "OcTree.h"

OcTree::~OcTree()
{
	release(m_root);
	RELEASE(m_root);
	RELEASE(m_visBB);
}

void OcTree::release(KD_NODE* node)
{
	int i;
	OCTREE_NODE* ocNode = dynamic_cast<OCTREE_NODE*>(node);
	if (ocNode)
	{
		// Recursively go down the tree and release the bottom nodes first.
		for (i = 0; i <8; i++)
		{
			if (ocNode->children[i] != 0)
			{
				release(ocNode->children[i]);
			}
		}

		for (i = 0; i < node->shapes.size(); i++)
		{
			RELEASE(node->shapes[i]);
		}

		
		for (i = 0; i < 8; i++)
		{
			if (ocNode->children[i])
			{
				RELEASE(ocNode->children[i]);

			}
		}
	}
}

void OcTree::divide(ID3D11Device* device, KD_NODE* node)
{
	OCTREE_NODE* ocNode = dynamic_cast<OCTREE_NODE*>(node);
	int count = 0;
	for (auto shape : m_shapes)
	{
		count += countTrianglesInsideNode(node->bounds, shape); //Get all the triangles of all the shapes.
	}
	if (count == 0)
	{
		return;
	}
	if (count > MAX_TRIANGLES_OC)
	{

		for (int i = 0; i < 8; i++)
		{
			
			float step = node->bounds.diagonal / 4.0f;

			float offsetX = ((i & 1) ? step : -step);
			float offsetY = ((i & 2) ? step : -step);
			float offsetZ = ((i & 4) ? step : -step);
			AABB aabb;
			aabb.center.x = node->bounds.center.x + offsetX;
			aabb.center.y = node->bounds.center.y + offsetY;
			aabb.center.z = node->bounds.center.z + offsetZ;
			aabb.diagonal = node->bounds.diagonal / 2.0f;
			aabb.min.x = aabb.center.x - (aabb.diagonal / 2.0f);
			aabb.max.x = aabb.center.x + (aabb.diagonal / 2.0f);
			aabb.min.y = aabb.center.y - (aabb.diagonal / 2.0f);
			aabb.max.y = aabb.center.y + (aabb.diagonal / 2.0f);
			aabb.min.z = aabb.center.z - (aabb.diagonal / 2.0f);
			aabb.max.z = aabb.center.z + (aabb.diagonal / 2.0f);
			m_bb.push_back(aabb);
			count = 0;
			for (auto shape : m_shapes)
			{
				if (shape)
				{
					int triangles = countTrianglesInsideNode(aabb, shape);
					count += triangles;
				}
			}

			if (count > 0)
			{
				ocNode->children[i] = new OCTREE_NODE;
				ocNode->children[i]->parent = ocNode;
				ocNode->children[i]->bounds = aabb;
				ocNode->children[i]->ID = id;
				id += 1;
				divide(device, ocNode->children[i]);
			}
		}

		return;

	}


	for (auto shape : m_shapes)
	{

		Shape* newShape = separate(node->bounds, shape, device);
		if (newShape)
		{
			node->shapes.push_back(newShape);
			ocNode->trianglesInNode += (int)newShape->getIndices().size() / 3;
			
		}
		
	}
}

unsigned int OcTree::countTrianglesInsideNode(const AABB& bounds, Shape* shape)
{
	int triCount = (int)shape->getIndices().size() / 3;
	unsigned int count = 0;

	for (int tri = 0; tri < triCount; tri++)
	{


		float radius = bounds.diagonal / 2.0f;

		int vertexIndex = tri * 3;
		Vector3 pos = shape->getVertices()[shape->getIndices()[vertexIndex]].pos;
		pos = XMVector3TransformCoord(pos, shape->getWorld());
		float x1 = pos.x;
		float z1 = pos.z;
		float y1 = pos.y;
		vertexIndex++;
		pos = shape->getVertices()[shape->getIndices()[vertexIndex]].pos;
		pos = XMVector3TransformCoord(pos, shape->getWorld());
		float x2 = pos.x;
		float z2 = pos.z;
		float y2 = pos.y;
		vertexIndex++;
		pos = shape->getVertices()[shape->getIndices()[vertexIndex]].pos;
		pos = XMVector3TransformCoord(pos, shape->getWorld());
		float x3 = pos.x;
		float z3 = pos.z;
		float y3 = pos.y;

		float minimumX = fmin(x1, fmin(x2, x3));
		if (minimumX > (bounds.center.x + radius))
		{
			continue;
		}


		float maximumX = fmax(x1, fmax(x2, x3));
		if (maximumX < (bounds.center.x - radius))
		{
			continue;
		}


		float minimumZ = fmin(z1, fmin(z2, z3));
		if (minimumZ > (bounds.center.z + radius))
		{
			continue;
		}


		float maximumZ = fmax(z1, fmax(z2, z3));
		if (maximumZ < (bounds.center.z - radius))
		{
			continue;
		}
		float minimumY = fmin(y1, fmin(y2, y3));
		if (minimumY > (bounds.center.y + radius))
		{
			continue;
		}


		float maximumY = fmax(y1, fmax(y2, y3));
		if (maximumY < (bounds.center.y - radius))
		{
			continue;
		}
		count++;
	}
	return count;
}

bool OcTree::isInsideAABB(const AABB& bounds, Shape* shape, int triangle)
{
	float radius = bounds.diagonal / 2.0f;

	int vertexIndex = triangle * 3;
	Vector3 pos = shape->getVertices()[shape->getIndices()[vertexIndex]].pos;
	pos = XMVector3TransformCoord(pos, shape->getWorld());
	float x1 = pos.x;
	float z1 = pos.z;
	float y1 = pos.y;
	vertexIndex++;
	pos = shape->getVertices()[shape->getIndices()[vertexIndex]].pos;
	pos = XMVector3TransformCoord(pos, shape->getWorld());
	float x2 = pos.x;
	float z2 = pos.z;
	float y2 = pos.y;
	vertexIndex++;
	pos = shape->getVertices()[shape->getIndices()[vertexIndex]].pos;
	pos = XMVector3TransformCoord(pos, shape->getWorld());
	float x3 = pos.x;
	float z3 = pos.z;
	float y3 = pos.y;

	float minimumX = fmin(x1, fmin(x2, x3));
	if (minimumX > (bounds.center.x + radius))
	{
		return false;
	}


	float maximumX = fmax(x1, fmax(x2, x3));
	if (maximumX < (bounds.center.x - radius))
	{
		return false;
	}

	float minimumY = fmin(y1, fmin(y2, y3));
	if (minimumY > (bounds.center.y + radius))
	{
		return false;
	}

	float maximumY = fmax(y1, fmax(y2, y3));
	if (maximumY < (bounds.center.y - radius))
	{
		return false;
	}

	float minimumZ = fmin(z1, fmin(z2, z3));
	if (minimumZ > (bounds.center.z + radius))
	{
		return false;
	}


	float maximumZ = fmax(z1, fmax(z2, z3));
	if (maximumZ < (bounds.center.z - radius))
	{
		return false;
	}
	return true;
}

Shape* OcTree::separate(const AABB& bounds, Shape* shape, ID3D11Device* device)
{
	std::vector<VERTEX> vertices;
	std::vector<unsigned int> indices;
	int index = 0;


	for (int i = 0; i < (int)shape->getIndices().size() / 3; i++)
	{
		if (isInsideAABB(bounds, shape, i))
		{
			int vertexIndex = i * 3;
			for (int j = 0; j < 3; j++)
			{

				VERTEX v = shape->getVertices()[shape->getIndices()[vertexIndex]];
				auto it = std::find(vertices.begin(), vertices.end(), v);

				if (it == vertices.end())
				{
					vertices.push_back(v);
					indices.push_back(index++);
				}
				else
				{
					indices.push_back((unsigned int)std::distance(vertices.begin(), it));
				}
				vertexIndex++;
			}


		}
	}
	if (vertices.size() == 0)
	{
		return nullptr;
	}
	Vector3 s, t;
	Quaternion r;
	Transform* parent = shape->getParent();
	if (parent)
	{
		TRANSFORM_VALUES_GLOBAL transform = shape->getGlobalTransformations();
		s = transform.scale;
		t = transform.trans;
		r = transform.rotQ;
	}
	else
	{
		TRANSFORM_VALUES_LOCAL transform = shape->getLocalTranformations();
		s = transform.scale;
		t = transform.trans;
		r = transform.rotQ;
	}

	RENDERPASS rp = shape->getDrawCall()->g_renderPassTypes;
	SHADER_BIND_FLAG shaders = shape->getDrawCall()->g_shaders;
	RENDER_TYPE rt = shape->getDrawCall()->g_renderType;
	auto srv = shape->getDrawCall()->g_srvs;
	DC_TYPE type = shape->getDCType();
	Shape* newShape = nullptr;


	newShape = new Shape(device, type, rp, rt);
	newShape->getDrawCall()->g_srvs = srv;
	newShape->getDrawCall()->g_shaders = shaders;
	newShape->setVertices(device, vertices);
	newShape->setIndices(device, indices);
	newShape->setScale(s);
	newShape->setTranslation(t);
	newShape->setRotationQuat(r);
	newShape->setParent(parent);
	newShape->generateBounds();
	newShape->g_cull = true;
	m_newShapes.push_back(newShape);
	return newShape;
}

KD_NODE* OcTree::getRoot()
{
    return m_root;
}

void OcTree::buildHiearchy(ID3D11Device* device)
{
	if (m_shapes.empty())
	{
		return;
	}
	if (m_worldBounds.diagonal == 0.0f)
	{
		return;
	}
	m_root = new OCTREE_NODE;

	m_root->bounds = m_worldBounds;
	divide(device, m_root);

	for (auto shape : m_shapes)
	{

		RELEASE(shape);
	}
	m_shapes.clear();
}

Shape* OcTree::visualizeBoundingBoxes(Shape* bb, ID3D11Device* device, ID3D11DeviceContext* context)
{
	Shape* newBBShape = new Shape(device, DC_TYPE::BASIC, RENDERPASS::BASIC_PASS_SINGLE, RENDER_TYPE::BOUNDING_BOX);
	auto v = bb->getVertices();
	auto i = bb->getIndices();
	newBBShape->setVertices(device, v);
	newBBShape->setIndices(device, i);
	int count = bb->getInstanceCount() + (int)m_bb.size()+ (int)m_newShapes.size();
	std::vector<INSTANCE_DATA> instanceBB(count);
	newBBShape->setInstances(device, instanceBB);
	

	for (unsigned int i = 0; i < bb->getInstanceCount(); i++)
	{
		
		float x = (bb->getBB().getBounds(i).aabb.max.x - bb->getBB().getBounds(i).aabb.min.x);
		float y = (bb->getBB().getBounds(i).aabb.max.y - bb->getBB().getBounds(i).aabb.min.y);
		float z = (bb->getBB().getBounds(i).aabb.max.z - bb->getBB().getBounds(i).aabb.min.z);
		Vector3 basicScale(x, y, z);
		basicScale *= 0.5;
		
		newBBShape->setScale(bb->getLocalTranformations(i).scale, i);
		newBBShape->setTranslation(bb->getBB().getBounds(i).aabb.center, i);

		
	}

	for (int i = 0; i < m_newShapes.size(); i++)
	{

		float x = (m_newShapes[i]->getBB().getBounds().aabb.max.x - m_newShapes[i]->getBB().getBounds().aabb.min.x);
		float y = (m_newShapes[i]->getBB().getBounds().aabb.max.y - m_newShapes[i]->getBB().getBounds().aabb.min.y);
		float z = (m_newShapes[i]->getBB().getBounds().aabb.max.z - m_newShapes[i]->getBB().getBounds().aabb.min.z);
		Vector3 basicScale(x, y, z);

		basicScale *= 0.5;
		newBBShape->setScale(basicScale, i+ bb->getInstanceCount());
		newBBShape->setTranslation(m_newShapes[i]->getBB().getBounds().aabb.center, i + bb->getInstanceCount());
	}
	newBBShape->updateInstances(context);
	newBBShape->generateBounds();
	newBBShape->g_cull = true;

	RELEASE(bb);
	m_visBB = newBBShape;
	return newBBShape;
}
