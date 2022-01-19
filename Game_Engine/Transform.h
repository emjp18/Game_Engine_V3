#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "common.h"
class Transform
{
private:
	Matrix m_world;
	Matrix m_parentOffset;
protected:
	std::vector<Transform*> m_parent;
	TRANSFORM_TYPE m_transformType = TRANSFORM_TYPE::STATIC;
	std::vector<Vector3> m_scale;
	std::vector<Quaternion> m_rotationQ;
	std::vector<Vector3> m_rotationEuler;
	std::vector<Vector3> m_translation;
	unsigned int m_instanceCount = 1;
	TRANSFORM_VALUES_LOCAL m_localTransformations;
	TRANSFORM_VALUES_GLOBAL m_globalTransformations;
	virtual void setInstanceCount(unsigned int count);
	bool m_useS = true;
	bool m_useR = true;
	bool m_useT = true;
	bool m_byPassWorld = false;
	bool m_useOffset = false;
public:
	Transform();
	virtual ~Transform();
	virtual Matrix& getWorld(unsigned int instance = 0, bool isParent = false);
	virtual Matrix& getInverseWorld(unsigned int instance = 0, bool isParent = false);
	virtual void setScale(const Vector3& scale, unsigned int instance = 0);
	virtual void setTranslation(const Vector3& position, unsigned int instance = 0);
	virtual void setRotationQuat(const Quaternion& rotation, unsigned int instance = 0);
	virtual void setRotationEuler(const Vector3& rotation, unsigned int instance = 0);
	virtual const TRANSFORM_VALUES_LOCAL& getLocalTranformations(unsigned int instance = 0);
	virtual const TRANSFORM_VALUES_GLOBAL& getGlobalTransformations(unsigned int instance = 0);
	virtual const TRANSFORM_TYPE& getTransformStatus() { return m_transformType; }
	virtual void setTransformStatus(const TRANSFORM_TYPE& status) { m_transformType = status; }
	virtual void setParent(Transform* parent, unsigned int instance = 0) { m_parent[instance] = parent; }
	virtual Transform* getParent(unsigned int instance = 0) { return m_parent[instance]; }
	virtual unsigned int getInstanceCount() { return m_instanceCount; }
	virtual void setBooleans(bool s, bool r, bool t) { m_useS = s; m_useR = r; m_useT = t; }
	virtual void setWorldImmediately(const Matrix& world) { m_world = world; m_byPassWorld = true; }
	virtual void setParentOffset(const Matrix& m) {m_parentOffset = m; m_useOffset = true; }
};

#endif