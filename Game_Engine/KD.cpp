#include "stdafx.h"
#include "KD.h"

void KD::generateWorldBounds()
{
	if (m_shapes.empty())
	{
		return;
	}
	std::vector<Bounds> bounds;
	for (auto shape : m_shapes)
	{
		shape->generateBounds();
		bounds.push_back(shape->getBB().getBounds());
	}
	BoundingBoxHandler bh;
	bh.calcBounds(bounds);
	m_worldBounds = bh.getBounds().aabb;
}
