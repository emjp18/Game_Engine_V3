#include "stdafx.h"
#include "ResourceHandler.h"

ResourceHandler::ResourceHandler(SCENE scene, size_t x, size_t y)
{
	m_sceneHandler = new SceneHandler(scene,x,y);
	m_scene = scene;
}

ResourceHandler::~ResourceHandler()
{
	RELEASE(m_sceneHandler);
}

void ResourceHandler::restart(SCENE scene, size_t x, size_t y)
{
	RELEASE(m_sceneHandler);
	m_sceneHandler = new SceneHandler(scene,x,y );
}

const std::vector<Shape*>& ResourceHandler::getShapes()
{
	return m_sceneHandler->getShapes();
}

const std::vector<Shape*>& ResourceHandler::getOcShapes()
{
	return m_sceneHandler->getOcShapes();
}

const std::vector<Shape*>& ResourceHandler::getQuadShapes()
{
	return m_sceneHandler->getQuadShapes();
}

const std::vector<Light*>& ResourceHandler::getLights()
{
	return m_sceneHandler->getLights();
}

Shape* ResourceHandler::getBSP()
{
	return m_sceneHandler->getBSP();
}

Shape* ResourceHandler::getBB()
{
	return m_sceneHandler->getBB();
}

void ResourceHandler::update()
{
	m_sceneHandler->update();
}

Light* ResourceHandler::getDirLight()
{
	return m_sceneHandler->getDirLight();
}
