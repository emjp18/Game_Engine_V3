#pragma once
#ifndef RESOURCEHANDLER_H
#define RESOURCEHANDLER_H
#include "SceneHandler.h"
class ResourceHandler
{
private:
	SCENE m_scene;
	SceneHandler* m_sceneHandler = nullptr;
public:
	ResourceHandler(SCENE scene, size_t x, size_t y);
	~ResourceHandler();
	void restart(SCENE scene, size_t x, size_t y);
	const std::vector<Shape*>& getShapes();
	const std::vector<Shape*>& getOcShapes();
	const std::vector<Shape*>& getQuadShapes();
	const std::vector<Light*>& getLights();
	Shape* getBSP();
	Shape* getBB();
	void update();
	Light* getDirLight();
};

#endif