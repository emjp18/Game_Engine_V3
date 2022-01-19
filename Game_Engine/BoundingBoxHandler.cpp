#include "stdafx.h"
#include "BoundingBoxHandler.h"

BoundingBoxHandler::BoundingBoxHandler()
{
	m_bounds.resize(1);
}
void BoundingBoxHandler::calcBounds(const std::vector<TERRAIN_PATCH>& vertices, const Matrix& world, const Quaternion& rot, unsigned int instance)
{
	Vector3 minVertex = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
	Vector3 maxVertex = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (unsigned int vertex = 0; vertex < vertices.size(); vertex++)
	{
		Vector3 v = vertices[vertex].pos;
		v = XMVector3TransformCoord(v, world);
		minVertex.x = fmin(minVertex.x, v.x);
		minVertex.y = fmin(minVertex.y, v.y);
		minVertex.z = fmin(minVertex.z, v.z);


		maxVertex.x = fmax(maxVertex.x, v.x);
		maxVertex.y = fmax(maxVertex.y, v.y);
		maxVertex.z = fmax(maxVertex.z, v.z);

	}

	Vector3 center = (maxVertex + minVertex) * 0.5;

	float diameter = (maxVertex - minVertex).Length();

	float r = diameter / 2.0f;


	Matrix rotM = XMMatrixRotationQuaternion(rot);

	Vector3 uX;
	Vector3 uY;
	Vector3 uZ;

	uX.x = rotM._11;
	uX.y = rotM._21;
	uX.z = rotM._31;

	uY.x = rotM._12;
	uY.y = rotM._22;
	uY.z = rotM._32;

	uZ.x = rotM._13;
	uZ.y = rotM._23;
	uZ.z = rotM._33;

	float eX, eY, eZ;

	eX = fabsf(maxVertex.x - minVertex.x) * 0.5f;
	eY = fabsf(maxVertex.y - minVertex.y) * 0.5f;
	eZ = fabsf(maxVertex.z - minVertex.z) * 0.5f;

	OBB obb;
	obb.center = center;
	obb.extendsX = eX;
	obb.extendsY = eY;
	obb.extendsZ = eZ;
	obb.max = maxVertex;
	obb.min = minVertex;
	obb.uX = uX;
	obb.uY = uY;
	obb.uZ = uZ;

	AABB aabb;
	aabb.center = center;
	aabb.diagonal = diameter;
	aabb.max = maxVertex;
	aabb.min = minVertex;

	BOUNDING_SPHERE bs;
	bs.center = center;
	bs.radius = r;



	m_bounds[instance].aabb = aabb;
	m_bounds[instance].boundingSphere = bs;
	m_bounds[instance].obb = obb;
}
void BoundingBoxHandler::calcBounds(const std::vector<VERTEX>& vertices, const Matrix& world, const Quaternion& rot, unsigned int instance)
{
	Vector3 minVertex = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
	Vector3 maxVertex = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (unsigned int vertex = 0; vertex < vertices.size(); vertex++)
	{
		Vector3 v = vertices[vertex].pos;
		v = XMVector3TransformCoord(v, world);
		minVertex.x = fmin(minVertex.x, v.x);
		minVertex.y = fmin(minVertex.y, v.y);
		minVertex.z = fmin(minVertex.z, v.z);


		maxVertex.x = fmax(maxVertex.x, v.x);
		maxVertex.y = fmax(maxVertex.y, v.y);
		maxVertex.z = fmax(maxVertex.z, v.z);

	}

	Vector3 center = (maxVertex + minVertex) * 0.5;

	float diameter = (maxVertex - minVertex).Length();

	float r = diameter / 2.0f;


	Matrix rotM = XMMatrixRotationQuaternion(rot);

	Vector3 uX;
	Vector3 uY;
	Vector3 uZ;

	uX.x = rotM._11;
	uX.y = rotM._21;
	uX.z = rotM._31;

	uY.x = rotM._12;
	uY.y = rotM._22;
	uY.z = rotM._32;

	uZ.x = rotM._13;
	uZ.y = rotM._23;
	uZ.z = rotM._33;

	float eX, eY, eZ;

	eX = fabsf(maxVertex.x - minVertex.x) * 0.5f;
	eY = fabsf(maxVertex.y - minVertex.y) * 0.5f;
	eZ = fabsf(maxVertex.z - minVertex.z) * 0.5f;

	OBB obb;
	obb.center = center;
	obb.extendsX = eX;
	obb.extendsY = eY;
	obb.extendsZ = eZ;
	obb.max = maxVertex;
	obb.min = minVertex;
	obb.uX = uX;
	obb.uY = uY;
	obb.uZ = uZ;

	AABB aabb;
	aabb.center = center;
	aabb.diagonal = diameter;
	aabb.max = maxVertex;
	aabb.min = minVertex;

	BOUNDING_SPHERE bs;
	bs.center = center;
	bs.radius = r;

	

	m_bounds[instance].aabb = aabb;
	m_bounds[instance].boundingSphere = bs;
	m_bounds[instance].obb = obb;
}

void BoundingBoxHandler::updateBounds(const Vector3& center, const Quaternion& rotation, unsigned int instance)
{
	m_bounds[instance].aabb.center = center;
	m_bounds[instance].boundingSphere.center = center;
	m_bounds[instance].obb.center = center;

	Matrix rotM = XMMatrixRotationQuaternion(rotation);

	m_bounds[instance].obb.uX.x = rotM._11;
	m_bounds[instance].obb.uX.y = rotM._21;
	m_bounds[instance].obb.uX.z = rotM._31;

	m_bounds[instance].obb.uY.x = rotM._12;
	m_bounds[instance].obb.uY.y = rotM._22;
	m_bounds[instance].obb.uY.z = rotM._32;

	m_bounds[instance].obb.uZ.x = rotM._13;
	m_bounds[instance].obb.uZ.y = rotM._23;
	m_bounds[instance].obb.uZ.z = rotM._33;

	m_bounds[instance].obb.extendsX = fabsf(m_bounds[instance].obb.max.x - m_bounds[instance].obb.min.x) * 0.5f;
	m_bounds[instance].obb.extendsY = fabsf(m_bounds[instance].obb.max.y - m_bounds[instance].obb.min.y) * 0.5f;
	m_bounds[instance].obb.extendsZ = fabsf(m_bounds[instance].obb.max.z - m_bounds[instance].obb.min.z) * 0.5f;
}

void BoundingBoxHandler::calcBounds(const std::vector<Bounds>& bounds, unsigned int instance)
{

	Vector3 minVertex = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
	Vector3 maxVertex = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (unsigned int vertex = 0; vertex < bounds.size(); vertex++)
	{
		Vector3 max = bounds[vertex].aabb.max;
		Vector3 min = bounds[vertex].aabb.min;
	
		minVertex.x = fmin(minVertex.x, min.x);
		minVertex.y = fmin(minVertex.y, min.y);
		minVertex.z = fmin(minVertex.z, min.z);


		maxVertex.x = fmax(maxVertex.x, max.x);
		maxVertex.y = fmax(maxVertex.y, max.y);
		maxVertex.z = fmax(maxVertex.z, max.z);

	}

	Vector3 center = (maxVertex + minVertex) * 0.5;

	float diameter = (maxVertex - minVertex).Length();

	float r = diameter / 2.0f;


	Matrix rotM = XMMatrixRotationQuaternion(Quaternion::Identity);

	Vector3 uX;
	Vector3 uY;
	Vector3 uZ;

	uX.x = rotM._11;
	uX.y = rotM._21;
	uX.z = rotM._31;

	uY.x = rotM._12;
	uY.y = rotM._22;
	uY.z = rotM._32;

	uZ.x = rotM._13;
	uZ.y = rotM._23;
	uZ.z = rotM._33;

	float eX, eY, eZ;

	eX = fabsf(maxVertex.x - minVertex.x) * 0.5f;
	eY = fabsf(maxVertex.y - minVertex.y) * 0.5f;
	eZ = fabsf(maxVertex.z - minVertex.z) * 0.5f;

	OBB obb;
	obb.center = center;
	obb.extendsX = eX;
	obb.extendsY = eY;
	obb.extendsZ = eZ;
	obb.max = maxVertex;
	obb.min = minVertex;
	obb.uX = uX;
	obb.uY = uY;
	obb.uZ = uZ;

	AABB aabb;
	aabb.center = center;
	aabb.diagonal = diameter;
	aabb.max = maxVertex;
	aabb.min = minVertex;

	BOUNDING_SPHERE bs;
	bs.center = center;
	bs.radius = r;



	m_bounds[instance].aabb = aabb;
	m_bounds[instance].boundingSphere = bs;
	m_bounds[instance].obb = obb;
}

