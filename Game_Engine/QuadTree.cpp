#include "stdafx.h"
#include "QuadTree.h"

Shape* QuadTree::createGrassShape(Shape* base, ID3D11Device* device)
{
	RENDERPASS rp = RENDERPASS::BASIC_PASS_TRANSPARENT;
	rp |= RENDERPASS::DEPTH_SHADOW_DIR;

	Shape* grass = new Shape(device, DC_TYPE::GRASS, rp);

	grass->setTranslation(base->getLocalTranformations().trans);
	grass->setRotationQuat(base->getLocalTranformations().rotQ);
	grass->setScale(base->getLocalTranformations().scale);
	grass->updateCBfr();

	

	grass->g_renderClockWise = true;
	std::vector<VERTEX> patches = base->getVertices();
	
	std::vector<GRASS_STRAND> strandsS;
	std::vector<VERTEX> strands;
	for (int i = 0; i < base->getIndices().size() / 3; i++)
	{
		int index = i * 3;
		int index1 = index + 1;
		int index2 = index + 2;

		Vector3 a = patches[base->getIndices()[index]].pos;
		Vector3 b = patches[base->getIndices()[index1]].pos;
		Vector3 c = patches[base->getIndices()[index2]].pos;

		Vector3 aN = patches[base->getIndices()[index]].normal;
		Vector3 bN = patches[base->getIndices()[index1]].normal;
		Vector3 cN = patches[base->getIndices()[index2]].normal;

		Vector3 aT = patches[base->getIndices()[index]].tangent;
		Vector3 bT = patches[base->getIndices()[index1]].tangent;
		Vector3 cT = patches[base->getIndices()[index2]].tangent;

		Vector3 aB = patches[base->getIndices()[index]].biNormal;
		Vector3 bB = patches[base->getIndices()[index1]].biNormal;
		Vector3 cB = patches[base->getIndices()[index2]].biNormal;

		Vector2 aU = patches[base->getIndices()[index]].uv;
		Vector2 bU = patches[base->getIndices()[index1]].uv;
		Vector2 cU = patches[base->getIndices()[index2]].uv;

		VERTEX s;
		Vector3 center = (a + b + c) / 3.0f;
		Vector3 centerN = (aN + bN + cN) / 3.0f;
		Vector3 centerT = (aT + bT + cT) / 3.0f;
		Vector3 centerB = (aB + bB + cB) / 3.0f;
		Vector2 centerU = (aU + bU + cU) / 3.0f;

		

		s.pos = center;
		s.biNormal = centerB;
		s.normal = centerN;
		s.tangent = centerT;
		s.uv = centerU;
		strands.push_back(s);
	
	}


	std::vector<unsigned int> strandsI;
	std::vector<VERTEX> strandsSorted;
	int c = 0;
	for (int i = 0; i < strands.size(); i++)
	{
		auto it = std::find(strandsSorted.begin(), strandsSorted.end(), strands[i]);
		if (it == strandsSorted.end())
		{
			strandsSorted.push_back(strands[i]);
			strandsI.push_back(c++);
		}
		else
		{

			strandsI.push_back((unsigned int)std::distance(strandsSorted.begin(), it));
		}

	}


	grass->setVertices(device, strandsSorted);
	grass->setIndices(device, strandsI);
	


	
	auto baseI = base->getIndices();



	std::vector<XMFLOAT3> bc(1,XMFLOAT3(0,0,0));
	

	//}
	DC_GRASS* dc = dynamic_cast<DC_GRASS*>(grass->getDrawCall());
	dc->g_strands = strandsS;
	dc->g_strandsIndices = baseI;
	dc->g_strandsBC = bc;
	return grass;
}

QuadTree::~QuadTree()
{
	
	release(m_root);
	RELEASE(m_root);
	
}

KD_NODE* QuadTree::getRoot()
{
    return m_root;
}

void QuadTree::buildHiearchy(ID3D11Device* device)
{
	if (m_shapes.empty())
	{
		return;
	}
	if (m_worldBounds.diagonal == 0.0f)
	{
		return;
	}
	m_root = new QUADTREE_NODE;

	m_root->bounds = m_worldBounds;
	divide(device, m_root);

	for (auto shape : m_shapes)
	{
		
		RELEASE(shape);
	}
}

void QuadTree::release(KD_NODE* node)
{
	int i;
	QUADTREE_NODE* quadNode = dynamic_cast<QUADTREE_NODE*>(node);
	if (quadNode)
	{
		// Recursively go down the tree and release the bottom nodes first.
		for (i = 0; i < 4; i++)
		{
			if (quadNode->children[i] != 0)
			{
				release(quadNode->children[i]);
			}
		}

		for (i = 0; i < node->shapes.size(); i++)
		{
			RELEASE(node->shapes[i]);
		}
		node->shapes.clear();
		RELEASE(quadNode->grass);
		// Release the four child nodes.
		for (i = 0; i < 4; i++)
		{
			if (quadNode->children[i])
			{
				RELEASE(quadNode->children[i]);

			}
		}
	}
}

void QuadTree::divide(ID3D11Device* device, KD_NODE* node)
{
	QUADTREE_NODE* quadNode = dynamic_cast<QUADTREE_NODE*>(node);
	int count = 0;
	for (auto shape : m_shapes)
	{
		count += countTrianglesInsideNode(node->bounds, shape); //Get all the triangles of all the shapes.
	}
	if (count == 0)
	{
		return;
	}
	if (count > MAX_TRIANGLES_QUAD)
	{

		for (int i = 0; i < 4; i++)
		{

			float offsetX = (((i % 2) < 1) ? -1.0f : 1.0f) * (node->bounds.diagonal / 4.0f);
			float offsetZ = (((i % 4) < 2) ? -1.0f : 1.0f) * (node->bounds.diagonal / 4.0f);
			AABB aabb;
			aabb.center.x = node->bounds.center.x + offsetX;
			aabb.center.z = node->bounds.center.z + offsetZ;
			aabb.diagonal = node->bounds.diagonal / 2.0f;
			aabb.min.x = aabb.center.x - (aabb.diagonal / 2.0f);
			aabb.max.x = aabb.center.x + (aabb.diagonal / 2.0f);
			//Not using Y.
			aabb.min.z = aabb.center.z - (aabb.diagonal / 2.0f);
			aabb.max.z = aabb.center.z + (aabb.diagonal / 2.0f);

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
				quadNode->children[i] = new QUADTREE_NODE;
				quadNode->children[i]->parent = quadNode;
				quadNode->children[i]->bounds = aabb;
				
				divide(device, quadNode->children[i]);
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
			QUADTREE_NODE* qNode = dynamic_cast<QUADTREE_NODE*>(node);
			qNode->grass = createGrassShape(newShape, device);
			m_grassShapes.push_back(qNode->grass);
		}
		
	}
}

unsigned int QuadTree::countTrianglesInsideNode(const AABB& bounds, Shape* shape)
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
		vertexIndex++;
		pos = shape->getVertices()[shape->getIndices()[vertexIndex]].pos;
		pos = XMVector3TransformCoord(pos, shape->getWorld());
		float x2 = pos.x;
		float z2 = pos.z;
		vertexIndex++;
		pos = shape->getVertices()[shape->getIndices()[vertexIndex]].pos;
		pos = XMVector3TransformCoord(pos, shape->getWorld());
		float x3 = pos.x;
		float z3 = pos.z;


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

		count++;
	}
	return count;
}

bool QuadTree::isInsideAABB(const AABB& bounds, Shape* shape, int triangle)
{
	float radius = bounds.diagonal / 2.0f;

	int vertexIndex = triangle * 3;
	Vector3 pos = shape->getVertices()[shape->getIndices()[vertexIndex]].pos;
	pos = XMVector3TransformCoord(pos, shape->getWorld());
	float x1 = pos.x;
	float z1 = pos.z;
	vertexIndex++;
	pos = shape->getVertices()[shape->getIndices()[vertexIndex]].pos;
	pos = XMVector3TransformCoord(pos, shape->getWorld());
	float x2 = pos.x;
	float z2 = pos.z;
	vertexIndex++;
	pos = shape->getVertices()[shape->getIndices()[vertexIndex]].pos;
	pos = XMVector3TransformCoord(pos, shape->getWorld());
	float x3 = pos.x;
	float z3 = pos.z;


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

Shape* QuadTree::separate(const AABB& bounds, Shape* shape, ID3D11Device* device)
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
float QuadTree::satRandF()
{
	return (float)rand() / (float)RAND_MAX;
}
float QuadTree::randF(float a, float b)
{
	return a + satRandF() * (b - a);
}

void QuadTree::Barycentric(Vector3 p, Vector3 a, Vector3 b, Vector3 c, float& u, float& v, float& w)
{
	Vector3 v0 = b - a, v1 = c - a, v2 = p - a;
	float d00 = v0.Dot(v0);
	float d01 = v0.Dot(v1);
	float d11 = v1.Dot(v1);
	float d20 = v2.Dot(v0);
	float d21 = v2.Dot(v1);
	float denom = d00 * d11 - d01 * d01;
	v = (d11 * d20 - d01 * d21) / denom;
	w = (d00 * d21 - d01 * d20) / denom;
	u = 1.0f - v - w;
}
