#pragma once
#ifndef BOUNDINGBOXANDLER_H
#define BOUNDINGBOXANDLER_H
#include "common.h"
class BoundingBoxHandler
{
private:
	unsigned int m_instances = 1;
	std::vector<Bounds> m_bounds;
public:
	BoundingBoxHandler();
	void setInstances(unsigned int nr) { m_instances = nr;  m_bounds.resize(nr); }
	void calcBounds(const std::vector<VERTEX>& vertices, const Matrix& world, const Quaternion& rot, unsigned int instance = 0);
	void calcBounds(const std::vector<TERRAIN_PATCH>& vertices, const Matrix& world, const Quaternion& rot, unsigned int instance = 0);
	void updateBounds(const Vector3& center, const Quaternion& rotation, unsigned int instance = 0);
	const Bounds& getBounds(unsigned int instance = 0) { return m_bounds[instance]; }
	void calcBounds(const std::vector<Bounds>&, unsigned int instance = 0);
};

#endif