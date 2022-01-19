#include "stdafx.h"
#include "Parser.h"

Parser::Parser()
{
}

Parser::~Parser()
{
}

bool Parser::importMeshes(char* path)
{
	m_vertices.clear();
	m_vertexIndices.clear();
	m_meshInfo.clear();
	
	bool couldLoadPath = m_xImporter.loadMesh(path);
	unsigned short nrOfMeshes = m_xImporter.getMeshCount();
	m_sceneInfo = m_xImporter.getScene(X::SCENE_TYPE::MESH);

	if (nrOfMeshes > 0)
	{
		std::vector<X::MESH> meshes = m_xImporter.getMeshes();
		m_vertices.resize(nrOfMeshes);
		m_vertexIndices.resize(nrOfMeshes);
		for (unsigned short i = 0; i < nrOfMeshes; i++)
		{
			std::vector<X::VERTEX> vertices = m_xImporter.getVertices(i);
			std::vector<unsigned int> indices = m_xImporter.getIndices(i);
			m_vertices[i].resize(meshes[i].vertexCount);
			m_vertexIndices[i].resize(meshes[i].indexCount);
			m_meshInfo.push_back(meshes[i]);


			for (unsigned int j = 0; j < meshes[i].vertexCount; j++)
			{

				m_vertices[i][j].pos.x = vertices[j].posX;
				m_vertices[i][j].pos.y = vertices[j].posY;
				m_vertices[i][j].pos.z = vertices[j].posZ;
				m_vertices[i][j].uv.x = vertices[j].u;
				m_vertices[i][j].uv.y = vertices[j].v;
				m_vertices[i][j].normal.x = vertices[j].normalX;
				m_vertices[i][j].normal.y = vertices[j].normalY;
				m_vertices[i][j].normal.z = vertices[j].normalZ;
				m_vertices[i][j].tangent.x = vertices[j].tangentX;
				m_vertices[i][j].tangent.y = vertices[j].tangentY;
				m_vertices[i][j].tangent.z = vertices[j].tangentZ;
				m_vertices[i][j].biNormal.x = vertices[j].biNormalX;
				m_vertices[i][j].biNormal.y = vertices[j].biNormalY;
				m_vertices[i][j].biNormal.z = vertices[j].biNormalZ;
				if (vertices[j].jointNr[0] == -1)
				{
					m_vertices[i][j].jointNr.x = 0;
				}
				else
				{
					m_vertices[i][j].jointNr.x = vertices[j].jointNr[0];
				}
				if (vertices[j].jointNr[1] == -1)
				{
					m_vertices[i][j].jointNr.y = 0;
				}
				else
				{
					m_vertices[i][j].jointNr.y = vertices[j].jointNr[1];
				}
				if (vertices[j].jointNr[2] == -1)
				{
					m_vertices[i][j].jointNr.z = 0;
				}
				else
				{
					m_vertices[i][j].jointNr.z = vertices[j].jointNr[2];
				}
				if (vertices[j].jointNr[3] == -1)
				{
					m_vertices[i][j].jointNr.w = 0;
				}
				else
				{
					m_vertices[i][j].jointNr.w = vertices[j].jointNr[3];
				}



				m_vertices[i][j].skinWeight.x = vertices[j].skinWeight[0];
				m_vertices[i][j].skinWeight.y = vertices[j].skinWeight[1];
				m_vertices[i][j].skinWeight.z = vertices[j].skinWeight[2];
				m_vertices[i][j].skinWeight.w = vertices[j].skinWeight[3];



			}
			for (unsigned int j = 0; j < meshes[i].indexCount; j++)
			{
				m_vertexIndices[i][j] = indices[j];

			}



		}
	}

	m_xImporter.clearAll();
	return couldLoadPath;
}

bool Parser::importLights(char* path)
{
	return false;
}

bool Parser::importSkeletonTrack(char* path)
{
	m_boneKeys.clear();
	m_bones.clear();
	m_fps = 0.0f;
	m_offsets.clear();
	m_trackLength = 0.0f;

	bool couldLoadPath  = m_xImporter.loadSkeleton(path);
	m_sceneInfo = m_xImporter.getScene(X::SCENE_TYPE::SKELETON);
	std::vector<X::SkeletonOffset> off = m_xImporter.getOffsets();
	std::vector<std::string> names;
	for (unsigned short j = 0; j < (unsigned short)off.size(); j++)
	{
		if (std::find(names.begin(),names.end(), (std::string)off[j].jointName) != names.end())
		{
			continue;
		}
		Matrix offMat;


		offMat = XMMatrixSet(off[j].m[0], off[j].m[1], off[j].m[2], off[j].m[3],
			off[j].m[4], off[j].m[5], off[j].m[6], off[j].m[7],
			off[j].m[8], off[j].m[9], off[j].m[10], off[j].m[11],
			off[j].m[12], off[j].m[13], off[j].m[14], off[j].m[15]);

		m_offsets.insert({ off[j].jointName, offMat });
		BONE bone;
		bone.name = off[j].jointName;
		bone.nr = off[j].jointNr;
		bone.parentName = off[j].jointParentName;
		std::vector<X::SkeletonKeyFrame> keyFrameData = m_xImporter.getSkeletonKeyFrameData(j);
		std::vector<BONE_KEY> keys;
		keys.resize(keyFrameData.size());
		for (int k = 0; k < keys.size(); k++)
		{

			keys[k].s = XMFLOAT3(keyFrameData[k].s);
			keys[k].r = XMFLOAT4(keyFrameData[k].r);
			keys[k].t = XMFLOAT3(keyFrameData[k].t);

		}
		names.push_back(bone.name);
		m_bones.push_back(bone);
		m_boneKeys.push_back(keys);
		
	}
	
	for (int i = 0; i<m_bones.size(); i++)
	{
		
		for (int j = 0; j < m_bones.size(); j++)
		{
			if (m_bones[i].name == m_bones[j].name)
			{
				continue;
			}
			if (m_bones[j].parentName == m_bones[i].name)
			{
				m_bones[i].childrenNames.push_back(m_bones[j].name);
				m_bones[i].childrenNrs.push_back(m_bones[j].nr);
				m_bones[j].parentName = m_bones[i].name;
				m_bones[j].parentNr = m_bones[i].nr;
				
			}
			
		}
		
	}

	for (int i = 0; i < m_bones.size(); i++)
	{
		if (m_bones[i].parentName == "RootNode")//should be deafault from FBX SDK
		{
			;
			m_bones[i].parentNr = -1;
			m_bones[i].root = true;
			m_bones[i].nr = 0;
		}
		if (m_bones[i].childrenNrs.size() == 0)
		{
			m_bones[i].leaf = true;
			
		}
		

	}
	m_fps = m_xImporter.getFPS(X::SCENE_TYPE::SKELETON);
	m_trackLength = m_xImporter.getLength(X::SCENE_TYPE::SKELETON);
	
	m_xImporter.clearAll();

	return couldLoadPath;
}

bool Parser::importMorphTargets(char* path)
{
	return false;
}

void Parser::importTextures(std::string fileName, Shape* shape)
{
	std::ifstream file("../Resources/Textfiles/Textures.txt");

	assert(file.is_open());
	TextureHandler& t =  TextureHandler::getInstance();
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

			if (word != fileName) //strcmp is not needed
			{
				continue;
			}
			else
			{
				ss >> word;

				if (word != "NULL") //the default textures should already be set. ALBEDO, NORMAL, ROUGH, METAL, AO, EMISSIVE
				{
					path = "../Resources/Textures/" + fileName + "Albedo.png"; //All textures should follow the same naming convention.
					t.addTextureSRV(path,Renderer::getDevice());
					shape->getDrawCall()->g_srvs.erase(SRV_ALBEDO_SLOT);
					shape->getDrawCall()->g_srvs.insert({ SRV_ALBEDO_SLOT, path });

				}
				ss >> word;
				if (word != "NULL")
				{
					path = "../Resources/Textures/" + fileName + "Normal.png";
					t.addTextureSRV(path, Renderer::getDevice());
					shape->getDrawCall()->g_srvs.erase(SRV_NORMAL_SLOT);
					shape->getDrawCall()->g_srvs.insert({ SRV_NORMAL_SLOT, path });

				}
				ss >> word;
				if (word != "NULL")
				{
					path = "../Resources/Textures/" + fileName + "Rough.png";
					t.addTextureSRV(path, Renderer::getDevice());
					shape->getDrawCall()->g_srvs.erase(SRV_ROUGH_SLOT);
					shape->getDrawCall()->g_srvs.insert({ SRV_ROUGH_SLOT, path });

				}
				ss >> word;
				if (word != "NULL")
				{
					path = "../Resources/Textures/" + fileName + "Metal.png";
					t.addTextureSRV(path, Renderer::getDevice());
					shape->getDrawCall()->g_srvs.erase(SRV_METAL_SLOT);
					shape->getDrawCall()->g_srvs.insert({ SRV_METAL_SLOT, path });

				}
				ss >> word;
				if (word != "NULL")
				{
					path = "../Resources/Textures/" + fileName + "AO.png";
					t.addTextureSRV(path, Renderer::getDevice());
					shape->getDrawCall()->g_srvs.erase(SRV_AO_SLOT);
					shape->getDrawCall()->g_srvs.insert({ SRV_AO_SLOT, path });

				}
				ss >> word;
				if (word != "NULL")
				{
					path = "../Resources/Textures/" + fileName + "Emissive.png";
					t.addTextureSRV(path, Renderer::getDevice());
					shape->getDrawCall()->g_srvs.erase(SRV_EMISSIVE_SLOT);
					shape->getDrawCall()->g_srvs.insert({ SRV_EMISSIVE_SLOT, path });

				}
				

			}
			break;
		}
	}
	file.close();
}


