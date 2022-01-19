#pragma once
#ifndef PARSER_H
#define PARSER_H
#include "Renderer.h"
class Parser
{
private:
	XFileFormatLibr m_xImporter;
	X::SCENE m_sceneInfo;
	std::vector<X::MESH> m_meshInfo;
	std::vector<std::vector<VERTEX>> m_vertices;
	std::vector<std::vector<unsigned int>> m_vertexIndices;
	float m_fps = 0.0f;
	float m_trackLength = 0.0f;
	std::unordered_map<std::string, Matrix> m_offsets;
	std::vector<BONE> m_bones;
	std::vector<std::vector<BONE_KEY>> m_boneKeys;
public:
	Parser();
	~Parser();
	static Parser& getInstance() { static Parser instance; return instance; }
	Parser(const Parser&) = delete;
	Parser& operator=(const Parser&) = delete;
	Parser(Parser&&) = delete;
	Parser& operator=(Parser&&) = delete;
	bool importMeshes(char* path);
	bool importLights(char* path);
	bool importSkeletonTrack(char* path);
	bool importMorphTargets(char* path);
	void importTextures(std::string name, Shape* shape);
	const X::SCENE& getSceneInfo() const { return m_sceneInfo; }
	const X::MESH& getMeshInfo(unsigned short meshNr) { return m_meshInfo[meshNr]; }
	std::vector<VERTEX>& getVertices(unsigned short meshNr) { return m_vertices[meshNr]; }
	std::vector<unsigned int>& getIndices(unsigned short meshNr) { return m_vertexIndices[meshNr]; }
	float getFPS() { return m_fps; }
	float getAniLength() { return m_trackLength; }
	const std::unordered_map<std::string, Matrix>& getOffsetMatrices() { return m_offsets; }
	const std::vector<BONE_KEY>& getBoneKeys(int nr) { return m_boneKeys[nr]; }
	const std::vector<BONE>& getBones() { return m_bones; }
	
};

#endif