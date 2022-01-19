#include "stdafx.h"
#include "SceneHandler.h"
void SceneHandler::moveAndRotate(Shape* shape)
{
	for (unsigned int i = 0; i < shape->getInstanceCount(); i++)
	{
		
		Vector3 NewLocation = shape->getLocalTranformations(i).trans;
		Vector3 NewRotation = shape->getLocalTranformations(i).rotE;
		float RunningTime = (float)m_timer.getElapsedDeltaTime();
		float DeltaHeight = (sinf(RunningTime + (float)m_timer.getDeltaTime()) - sinf(RunningTime));
		NewLocation.y += DeltaHeight * 5.0f;       //Scale our height by a factor of 20
		float DeltaRotation = (float)m_timer.getDeltaTime() * 0.5f;    //Rotate by 20 degrees per second
		NewRotation.z += DeltaRotation;
		shape->setTranslation(NewLocation,i);
		shape->setRotationEuler(NewRotation,i);
	}
	
}
void SceneHandler::moveAndRotateLights()
{
	for (unsigned int i = 0; i < m_allSceneLights.size(); i++)
	{
		Vector3 NewLocation = m_allSceneLights[i]->getLocalTranformations().trans;
		float RunningTime = (float)m_timer.getElapsedDeltaTime();
		float DeltaHeight = (sinf(RunningTime + (float)m_timer.getDeltaTime()) - sinf(RunningTime));
		NewLocation.y += DeltaHeight * 1.50f;       //Scale our height by a factor of 20
		m_allSceneLights[i]->setTranslation(NewLocation);
		
		
	}
}
void SceneHandler::visualizeBoundingBoxes(std::vector<Shape*> shapesWithBounds, bool move)
{
	RENDERPASS rp = RENDERPASS::BASIC_PASS_SINGLE;
	char path[100];
	std::string box = "../Resources/Models/cubeMESH.x";
	strcpy_s(path, box.c_str());
	Parser& p = Parser::getInstance();
	bool succededLoading = (p.importMeshes(path));
	assert(succededLoading);
	Shape* mainBoxShape = new Shape(Renderer::getDevice(), DC_TYPE::BASIC, rp, RENDER_TYPE::BOUNDING_BOX);
	mainBoxShape->setName(p.getMeshInfo(0).meshName);
	mainBoxShape->setVertices(Renderer::getDevice(), p.getVertices(0));
	mainBoxShape->setIndices(Renderer::getDevice(), p.getIndices(0));
	int count = 0;
	for (int i = 0; i < shapesWithBounds.size(); i++ )
	{
		count += shapesWithBounds[i]->getInstanceCount();
	}
	std::vector<INSTANCE_DATA> instanceBB(count);
	
	
	mainBoxShape->setInstances(Renderer::getDevice(), instanceBB);
	
	

	for (unsigned int i = 0; i < (unsigned int)shapesWithBounds.size(); i++)
	{

		for (unsigned int j = 0; j < (size_t)shapesWithBounds[i]->getInstanceCount(); j++)
		{
			float x = (shapesWithBounds[i]->getBB().getBounds(j).aabb.max.x - shapesWithBounds[i]->getBB().getBounds(j).aabb.min.x);
			float y = (shapesWithBounds[i]->getBB().getBounds(j).aabb.max.y - shapesWithBounds[i]->getBB().getBounds(j).aabb.min.y);
			float z = (shapesWithBounds[i]->getBB().getBounds(j).aabb.max.z - shapesWithBounds[i]->getBB().getBounds(j).aabb.min.z);

			Vector3 basicScale(x, y, z);
			basicScale *= 0.5; //the unit cube is 2 times the size it should be. Fn blender.
			mainBoxShape->setScale(basicScale, j);
			mainBoxShape->setTranslation(shapesWithBounds[i]->getBB().getBounds(j).aabb.center, j);
			
		}
		


	}
	mainBoxShape->updateInstances(Renderer::getContext());
	mainBoxShape->generateBounds();

	
	if (move)
	{
		
		mainBoxShape->setTransformStatus(TRANSFORM_TYPE::DYNAMIC);

	}
	m_bb = mainBoxShape;
}
Shape* SceneHandler::createParticleSystem(RENDER_TYPE type)
{
	switch (type)
	{
	default:
	{
		return nullptr;
		break;
	}
	case RENDER_TYPE::PARTICLE_FIRE_TYPE:
	{
		Shape* particleSys = new Shape(Renderer::getDevice(), DC_TYPE::PARTICLE, RENDERPASS::BASIC_PASS_TRANSPARENT, type);

		std::vector<PARTICLE> emitter(1);

		emitter[0].Age = 0;
		emitter[0].Type = (UINT)PARTICLE_TYPE::PT_EMITTER;
		emitter[0].Size = XMFLOAT2(60, 30);
		emitter[0].InitialPos = XMFLOAT3(0, 0, 0);
		emitter[0].InitialVel = DirectX::XMFLOAT3(1, 0, 0);
		particleSys->setParticles(Renderer::getDevice(), emitter);

		emitter.resize(100);

		particleSys->setParticles(Renderer::getDevice(), emitter,false, false, true);

		particleSys->setParticles(Renderer::getDevice(), emitter, false, true,true);

		return particleSys;
		break;
	}
	case RENDER_TYPE::PARTICLE_RAIN_TYPE:
	{
		Shape* particleSys = new Shape(Renderer::getDevice(), DC_TYPE::PARTICLE, RENDERPASS::BASIC_PASS_SINGLE, type);

		std::vector<PARTICLE> emitter(1);

		emitter[0].Age = 0;
		emitter[0].Type = (UINT)PARTICLE_TYPE::PT_EMITTER;
		emitter[0].Size = XMFLOAT2(60, 30);
		emitter[0].InitialPos = XMFLOAT3(0, 0, 0);
		emitter[0].InitialVel = DirectX::XMFLOAT3(1, 0, 0);
		particleSys->setParticles(Renderer::getDevice(), emitter);

		emitter.resize(300);

		particleSys->setParticles(Renderer::getDevice(), emitter, false, false, true);

		particleSys->setParticles(Renderer::getDevice(), emitter, false, true, true);


		return particleSys;
		break;
	}
	case RENDER_TYPE::PARTICLE_TEXT_TYPE:
	{
		Shape* particleSys = new Shape(Renderer::getDevice(), DC_TYPE::PARTICLE, RENDERPASS::BASIC_PASS_SINGLE, type);

		std::vector<PARTICLE> emitter(4);

		emitter[0].Age = 0;
		emitter[0].Type = (UINT)PARTICLE_TYPE::PT_EMITTER;
		emitter[0].Size = XMFLOAT2(60, 30);
		emitter[0].InitialPos = XMFLOAT3(0, 0, 0);
		emitter[0].InitialVel = DirectX::XMFLOAT3(1, 0, 0);
		particleSys->setParticles(Renderer::getDevice(), emitter);

		return particleSys;
		break;
	}
	case RENDER_TYPE::PARTICLE_LEAVES_TYPE:
	{
		Shape* particleSys = new Shape(Renderer::getDevice(), DC_TYPE::PARTICLE, RENDERPASS::BASIC_PASS_TRANSPARENT, type);

		std::vector<PARTICLE> emitter(1);

		emitter[0].Age = 0;
		emitter[0].Type = (UINT)PARTICLE_TYPE::PT_EMITTER;
		emitter[0].Size = XMFLOAT2(60, 30);
		emitter[0].InitialPos = XMFLOAT3(0, 0, 0);
		emitter[0].InitialVel = DirectX::XMFLOAT3(1, 0, 0);
		particleSys->setParticles(Renderer::getDevice(), emitter);

		emitter.resize(300);

		particleSys->setParticles(Renderer::getDevice(), emitter, false, false, true);

		particleSys->setParticles(Renderer::getDevice(), emitter, false, true, true);


		return particleSys;
		break;
	}
	}
}
Shape* SceneHandler::createSkySphere(bool dds)
{
	/*0 posX
1 negX
2 posY
3 negY
4 posZ
5 negZ
*/
	if (dds)
	{
		std::string path = "../Resources/Textures/textureCube.dds";
		WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView = NULL;

		std::wstring pathWString(path.begin(), path.end());
		ID3D11Texture2D* SMTexture = nullptr;

		HRESULT hr = DirectX::CreateDDSTextureFromFileEx(Renderer::getDevice(),
			Renderer::getContext(),
			pathWString.c_str(), 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
			NULL, D3D11_RESOURCE_MISC_TEXTURECUBE, NULL,
			(ID3D11Resource**)&SMTexture, nullptr
			, nullptr);

		assert(SUCCEEDED(hr));

		D3D11_TEXTURE2D_DESC SMTextureDesc = {};

		SMTexture->GetDesc(&SMTextureDesc);

		SMTextureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc = {};
		SMViewDesc.Format = SMTextureDesc.Format;
		SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
		SMViewDesc.TextureCube.MostDetailedMip = 0;

		hr = Renderer::getDevice()->CreateShaderResourceView(SMTexture, &SMViewDesc, shaderResourceView.GetAddressOf());

		assert(SUCCEEDED(hr));
		RELEASE_COM(SMTexture);
		TextureHandler::getInstance().addTextureSRV("sky", shaderResourceView);

		
		
	}
	else
	{
		std::vector<std::string> paths = { "../Resources/Textures/xPos.png",  "../Resources/Textures/xNeg.png",
	 "../Resources/Textures/yPos.png",
	 "../Resources/Textures/yNeg.png",
 "../Resources/Textures/zPos.png", "../Resources/Textures/zNeg.png" };


		ID3D11Texture2D* cubeTexture = NULL;
		WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView = NULL;

		//Description of each face
		D3D11_TEXTURE2D_DESC texDesc = {};
		
		D3D11_TEXTURE2D_DESC texDesc1 = {};
		//The Shader Resource view description
		D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc = {};
	
		

		ID3D11Texture2D* tex[6] = { nullptr, nullptr, nullptr,nullptr, nullptr, nullptr };
		for (int i = 0; i < 6; i++)
		{
			std::wstring pathWString(paths[i].begin(), paths[i].end());

			HRESULT hr = DirectX::CreateWICTextureFromFileEx(Renderer::getDevice(), pathWString.c_str(), 0, D3D11_USAGE_STAGING, 0, D3D11_CPU_ACCESS_READ| D3D11_CPU_ACCESS_WRITE, 0,
				WIC_LOADER_FLAGS::WIC_LOADER_DEFAULT,
				(ID3D11Resource**)&tex[i], 0);
			assert(SUCCEEDED(hr));
		}

		tex[0]->GetDesc(&texDesc1);

		texDesc.Width = texDesc1.Width;
		texDesc.Height = texDesc1.Height;
		texDesc.MipLevels = texDesc1.MipLevels;
		texDesc.ArraySize = 6;
		texDesc.Format = texDesc1.Format;
		texDesc.CPUAccessFlags = 0;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		SMViewDesc.Format = texDesc.Format;
		SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		SMViewDesc.TextureCube.MipLevels = texDesc.MipLevels;
		SMViewDesc.TextureCube.MostDetailedMip = 0;


		Renderer::getDevice()->CreateTexture2D(&texDesc, NULL, &cubeTexture);
		for (int i = 0; i < 6; i++)
		{
			
			for (UINT mipLevel = 0; mipLevel < texDesc.MipLevels; ++mipLevel)
			{
				D3D11_MAPPED_SUBRESOURCE mappedTex2D;
				HRESULT hr = (Renderer::getContext()->Map(tex[i], mipLevel, D3D11_MAP_READ, 0, &mappedTex2D));
				assert(SUCCEEDED(hr));
				Renderer::getContext()->UpdateSubresource(cubeTexture,
					D3D11CalcSubresource(mipLevel, i, texDesc.MipLevels),
					0, mappedTex2D.pData, mappedTex2D.RowPitch, mappedTex2D.DepthPitch);

				Renderer::getContext()->Unmap(tex[i], mipLevel);
			}
			

			
		}

		for (int i = 0; i < 6; i++)
		{
			RELEASE_COM(tex[i]);
		}

		
		Renderer::getDevice()->CreateShaderResourceView(cubeTexture, &SMViewDesc, shaderResourceView.GetAddressOf());




		RELEASE_COM(cubeTexture);

		TextureHandler::getInstance().addTextureSRV("sky", shaderResourceView);
	}


	char meshPathChar[100] = "../Resources/Models/skySphereMESH.x";
	Parser::getInstance().importMeshes(meshPathChar);

	RENDERPASS rp = RENDERPASS::BASIC_PASS_SINGLE;
	rp |= RENDERPASS::DYNAMIC_CUBE_MAP_PASS;
	Shape* skyShape = new Shape(Renderer::getDevice(),DC_TYPE::BASIC,rp, RENDER_TYPE::SKY);
	skyShape->setTransformStatus(TRANSFORM_TYPE::DYNAMIC);
	skyShape->setVertices(Renderer::getDevice(), Parser::getInstance().getVertices(0));
	skyShape->setIndices(Renderer::getDevice(), Parser::getInstance().getIndices(0));
	skyShape->setScale({ 1.0f, 0.8f, 1.0f }); // squish it
	skyShape->getDrawCall()->g_srvs.erase(SRV_TEXTURE_CUBE);
	skyShape->getDrawCall()->g_srvs.insert({ SRV_TEXTURE_CUBE, "sky" });
	return skyShape;
}
Shape* SceneHandler::createTerrain()
{
	int w = m_hmW;
	int h = m_hmH;
	std::vector<unsigned char> mapData(w * h);
	std::ifstream file("../Resources/Models/heightMap.data",std::ios::binary);
	assert(file.is_open());
	file.read((char*)&mapData[0], (std::streamsize)mapData.size());
	file.close();
	float hScale = 50;
	m_hm.resize(w * h,0);
	
	for (int i = 0; i < w * h; i++)
	{
		
		
		m_hm[i] = ((float)(mapData[i] / 255.0f) * hScale);
	}
	
	
	smooth();
	buildHMSRV();
	calcAllPatchBoundsY();
	std::vector<TERRAIN_PATCH> patches(m_numPatchVerPerCol*m_numPatchVerPerRow);
	
	float halfW = 0.5f * getTerrainwidth();
	float halfD = 0.5f * getTerrainDepth();
	float patchW =getTerrainwidth()/(float)(m_numPatchVerPerCol-1);
	float patchD =getTerrainDepth() / (float)(m_numPatchVerPerRow - 1);
	float depthU = 1.0f/ (float)(m_numPatchVerPerCol - 1);
	float depthV = 1.0f / (float)(m_numPatchVerPerRow - 1);
	m_maxX = -INFINITY;
	m_maxZ = -INFINITY;
	m_minX = INFINITY;
	m_minZ = INFINITY;
	
	for (UINT i = 0; i < m_numPatchVerPerRow; ++i)
	{
		float z = halfD - i * patchD;
		for (UINT j = 0; j < m_numPatchVerPerCol; ++j)
		{
			float x = -halfW + j * patchW;
			patches[i * m_numPatchVerPerCol + j].pos = XMFLOAT3(x, 0, z);
			patches[i * m_numPatchVerPerCol + j].uv.x = j * depthU;
			patches[i * m_numPatchVerPerCol + j].uv.y = i * depthV;
			m_minX = fminf(m_minX, x);
			m_minZ = fminf(m_minZ, z);
			m_maxX = fmaxf(m_maxX, x);
			m_maxZ = fmaxf(m_maxZ, z);
			
		}
	}
	for (UINT i = 0; i < m_numPatchVerPerRow-1; i++)
	{
		
		for (UINT j = 0; j < m_numPatchVerPerCol-1; j++)
		{
			UINT id = i * (m_numPatchVerPerCol - 1) + j;
			patches[i * m_numPatchVerPerCol + j].boundsY = m_patchBoundsY[id];
		}
	}

	std::vector<unsigned int> indices(m_numPatchQuadFace*4); 
	int k = 0;

	for (UINT i = 0; i < m_numPatchVerPerRow - 1; ++i)
	{

		for (UINT j = 0; j < m_numPatchVerPerCol - 1; ++j)
		{
			indices[k] = i * m_numPatchVerPerCol + j;
			indices[k+1] = i * m_numPatchVerPerCol + j+1;

			indices[k + 2] = (i+1) * m_numPatchVerPerCol + j;
			indices[k + 3] = (i+1) * m_numPatchVerPerCol + j + 1;

			
			k += 4;
		}
	}

	
	
	RENDERPASS rp = RENDERPASS::DEPTH_LIGHT_PASS;
	rp |= RENDERPASS::DEPTH_SHADOW_DIR;
	rp |= RENDERPASS::DYNAMIC_CUBE_MAP_PASS;
	rp |= RENDERPASS::BASIC_PASS_SINGLE;
	rp |= RENDERPASS::DEPTH_SHADOW_POINT;

	Shape* terrainShape = new Shape(Renderer::getDevice(),DC_TYPE::BASIC, rp, RENDER_TYPE::TERRAIN);
	
	terrainShape->setTerrainPatches(Renderer::getDevice(), patches);
	terrainShape->setIndices(Renderer::getDevice(), indices);
	

	terrainShape->setTerrain(m_hm);
	terrainShape->setMaxX(m_maxX);
	terrainShape->setMaxZ(m_maxZ);
	terrainShape->setMinX(m_minX);
	terrainShape->setMinZ(m_minZ);
	m_terrainShape = terrainShape;
	terrainShape->generateBounds();
	return terrainShape;
}
bool SceneHandler::inBounds(int i, int j)
{
	return (i >= 0 && i < m_hmH&& j >= 0 && j < m_hmW);
}
void SceneHandler::buildHMSRV()
{
	D3D11_TEXTURE2D_DESC tex2DDesc = {};
	tex2DDesc.Width = (UINT)m_hmW;
	tex2DDesc.Height = (UINT)m_hmH;
	tex2DDesc.MipLevels = 1;
	tex2DDesc.ArraySize = 1;
	tex2DDesc.Format = DXGI_FORMAT_R32_FLOAT; //16 is used in the example but I don't know how to half the float.
	tex2DDesc.SampleDesc.Count = 1;
	tex2DDesc.SampleDesc.Quality = 0;
	tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
	tex2DDesc.CPUAccessFlags = 0;
	tex2DDesc.MiscFlags = 0;
	tex2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	D3D11_SUBRESOURCE_DATA subData= {};
	subData.pSysMem = &m_hm[0];
	subData.SysMemPitch = m_hmW * sizeof(FLOAT32);
	subData.SysMemSlicePitch = 0;

	ID3D11Texture2D* tex2D = nullptr;
	HRESULT hr = Renderer::getDevice()->CreateTexture2D(&tex2DDesc, &subData, &tex2D);
	assert(SUCCEEDED(hr));
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = tex2DDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1; 

	hr = Renderer::getDevice()->CreateShaderResourceView(tex2D, &srvDesc, m_hmSRV.GetAddressOf());
	assert(SUCCEEDED(hr));
	RELEASE_COM(tex2D);
	TextureHandler::getInstance().addTextureSRV("terrain", m_hmSRV);

	ID3D11Texture2D* tex[3] = { nullptr, nullptr, nullptr };

	std::string filenames[3] = { "../Resources/Textures/grass.png","../Resources/Textures/stone.png","../Resources/Textures/dirt.png" };
	
	UINT size = 3;

	
	for (UINT i = 0; i < size; ++i)
	{
		std::wstring pathWString(filenames[i].begin(), filenames[i].end());
		HRESULT hr = DirectX::CreateWICTextureFromFileEx(Renderer::getDevice(), pathWString.c_str(), 0, D3D11_USAGE_STAGING, 0, D3D11_CPU_ACCESS_READ| D3D11_CPU_ACCESS_WRITE, 0,
			WIC_LOADER_FLAGS::WIC_LOADER_DEFAULT,
			(ID3D11Resource**) &tex[i], 0);
		assert(SUCCEEDED(hr));

	
	}

	D3D11_TEXTURE2D_DESC texElementDesc = {};
	tex[0]->GetDesc(&texElementDesc);

	D3D11_TEXTURE2D_DESC texArrayDesc = {};
	texArrayDesc.Width = texElementDesc.Width;
	texArrayDesc.Height = texElementDesc.Height;
	texArrayDesc.MipLevels = texElementDesc.MipLevels;
	texArrayDesc.ArraySize = size;
	texArrayDesc.Format = texElementDesc.Format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;

	ID3D11Texture2D* texArray = 0;
	hr = Renderer::getDevice()->CreateTexture2D(&texArrayDesc, 0, &texArray);
	assert(SUCCEEDED(hr));


	for (UINT texElement = 0; texElement < size; ++texElement)
	{
		// for each mipmap level...
		for (UINT mipLevel = 0; mipLevel < texElementDesc.MipLevels; ++mipLevel)
		{
			D3D11_MAPPED_SUBRESOURCE mappedTex2D;
			hr = (Renderer::getContext()->Map(tex[texElement], mipLevel, D3D11_MAP_READ, 0, &mappedTex2D));
			assert(SUCCEEDED(hr));
			Renderer::getContext()->UpdateSubresource(texArray,
				D3D11CalcSubresource(mipLevel, texElement, texElementDesc.MipLevels),
				0, mappedTex2D.pData, mappedTex2D.RowPitch, mappedTex2D.DepthPitch);

			Renderer::getContext()->Unmap(tex[texElement], mipLevel);
		}
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = size;

	WRL::ComPtr<ID3D11ShaderResourceView> texArraySRV;
	hr = (Renderer::getDevice()->CreateShaderResourceView(texArray, &viewDesc, &texArraySRV));
	assert(SUCCEEDED(hr));

	TextureHandler::getInstance().addTextureSRV("terrainLayers", texArraySRV);
	RELEASE_COM(texArray);

	for (UINT i = 0; i < size; ++i)
		RELEASE_COM(tex[i]);



	TextureHandler::getInstance().addTextureSRV("../Resources/Textures/blendTerrain.png",Renderer::getDevice());

}
unsigned int SceneHandler::as_uint(const float x) {
	return *(unsigned int*)&x;
}
unsigned short SceneHandler::float_to_half(const float x) { // IEEE-754 16-bit floating-point format (without infinity): 1-5-10, exp-15, +-131008.0, +-6.1035156E-5, +-5.9604645E-8, 3.311 digits
	const unsigned int b = as_uint(x) + 0x00001000; // round-to-nearest-even: add last bit after truncated mantissa
	const unsigned int e = (b & 0x7F800000) >> 23; // exponent
	const unsigned int m = b & 0x007FFFFF; // mantissa; in line below: 0x007FF000 = 0x00800000-0x00001000 = decimal indicator flag - initial rounding
	return (b & 0x80000000) >> 16 | (e > 112) * ((((e - 112) << 10) & 0x7C00) | m >> 13) | ((e < 113) & (e > 101)) * ((((0x007FF000 + m) >> (125 - e)) + 1) >> 1) | (e > 143) * 0x7FFF; // sign : normalized : denormalized : saturate
}

float SceneHandler::average(int i, int j)
{
	float avg = 0.0f;
	float num = 0.0f;

	for (int m = i - 1; m <= i + 1; ++m)
	{
		for (int n = j - 1; n <= j + 1; ++n)
		{
			if (inBounds(m, n))
			{
				avg += m_hm[m * m_hmW + n];
				num += 1.0f;
			}
		}
	}


	return avg / num;

}
void SceneHandler::smooth()
{
	std::vector<float> dest(m_hm.size());
	for (int i = 0; i < m_hmH; i++)
	{
		for (int j = 0; j < m_hmW; j++)
		{
			dest[i * m_hmW + j] = average(i, j);
		}
	}
	m_hm = dest;
	float min = INFINITY;

	float desiredMin = -50;
	
	for (UINT i = 0; i < m_hm.size(); i++)
	{
		min = fminf(min, m_hm[i]);
		float movedistance = min - desiredMin;
		m_hm[i] -= movedistance;
	}
	

}
float SceneHandler::satRandF()
{
	return (float)rand() / (float)RAND_MAX;
}
SceneHandler::SceneHandler(SCENE scene, size_t x, size_t y)
{
	m_currResX = x;
	m_currResY = y;
	m_numPatchVerPerRow = ((m_hmH - 1) / CELLS_PER_PATCH_HM) + 1;
	m_numPatchVerPerCol = ((m_hmW - 1) / CELLS_PER_PATCH_HM) + 1;
	m_numPatchVer = m_numPatchVerPerRow * m_numPatchVerPerCol;
	m_numPatchQuadFace = (m_numPatchVerPerRow-1) * (m_numPatchVerPerCol-1);
	m_timer.reset();
	m_timer.start();
	m_scene = scene;
	
	switch (scene)
	{
	default:
	{
		break;
	case SCENE::MAIN_TEST:
	{
		InputHandler::getInstance().SetMouseMode(Mouse::Mode::MODE_RELATIVE);
		InputHandler::getInstance().ConfineMouse();
		char path[100];
		std::string box = "../Resources/Models/BoxMESH.x";
		strcpy_s(path, box.c_str());
		Parser& p = Parser::getInstance();
		bool succededLoading = (p.importMeshes(path));
		assert(succededLoading);
		
		RENDERPASS rp = RENDERPASS::DEPTH_LIGHT_PASS;
		rp |= RENDERPASS::DEPTH_SHADOW_DIR;
		rp |= RENDERPASS::DYNAMIC_CUBE_MAP_PASS;
		rp |= RENDERPASS::BASIC_PASS_SINGLE;
#if CEL_SHADING
		rp |= RENDERPASS::EDGE_DETECTION_PASS;
#endif
		
#if POINT_SHADOWS
		rp |= RENDERPASS::DEPTH_SHADOW_POINT;
#endif // POINT_SHADOWS

		
		Shape* shape = new Shape(Renderer::getDevice(), DC_TYPE::BASIC, rp);
		shape->setName(p.getMeshInfo(0).meshName);
		shape->setVertices(Renderer::getDevice(), p.getVertices(0));
		shape->setIndices(Renderer::getDevice(), p.getIndices(0));
		shape->setTransformStatus(TRANSFORM_TYPE::DYNAMIC);
		
		std::vector<INSTANCE_DATA> instance(10);
	
		
		shape->setInstances(Renderer::getDevice(), instance);
		
		for (int i = 0; i < 10; i++)
		{
			
			shape->setScale({ (float)i * 2 + 1, (float)i * 2 + 1, (float)i * 2 + 1 }, i);
			shape->setTranslation({ (float)i * 20, (float)40, (float)i * 20 }, i);
			/*Light* light = new Light(LIGHT_TYPE::POINT,XMFLOAT4((float)i * 20, (float)40, (float)i * 20,1) , XMFLOAT4(0, 0, 1, 1), 2000);
		
			m_allSceneLights.push_back(light);*/
		}
		shape->updateInstances(Renderer::getContext());
		m_moveAndRotateShapes.push_back(shape);
		
		shape->generateBounds();
	
	
		m_allSceneShapes.push_back(shape);
		
		float charScale = 0.4444444444f; //used to be 0.0444...
		std::string character = "../Resources/Models/mainCharIdleMESH.x";
		std::string characterIdle = "../Resources/Animations/Skeletons/mainCharIdleSKELETON.x";
		std::string characterRun = "../Resources/Animations/Skeletons/mainCharRunSKELETON.x";
		std::string cloth = "../Resources/Models/coatIdleMESH.x";
		std::string clothIdle = "../Resources/Animations/Skeletons/coatIdleSKELETON.x";
		std::string clothRun = "../Resources/Animations/Skeletons/coatRunSKELETON.x";
		strcpy_s(path, character.c_str());
		succededLoading = (p.importMeshes(path));
		assert(succededLoading);
		Shape* shapeChar = new Shape(Renderer::getDevice(), DC_TYPE::SKELETON, rp, RENDER_TYPE::PLAYER_CHARACTER);
		shapeChar->setScale({ charScale ,charScale ,charScale });
		DC_SKELETON* dcSkel = dynamic_cast<DC_SKELETON*>(shapeChar->getDrawCall());
		shapeChar->updateCBfr();
		shapeChar->setName(p.getMeshInfo(0).meshName);
		shapeChar->setVertices(Renderer::getDevice(), p.getVertices(0));
		shapeChar->setIndices(Renderer::getDevice(), p.getIndices(0));
		shapeChar->setTransformStatus(TRANSFORM_TYPE::DYNAMIC);
		m_playerShape = shapeChar;
		p.importTextures("mainChar", m_playerShape);
		DC_SKELETON* skeletonTemp = nullptr;
		skeletonTemp = dynamic_cast<DC_SKELETON*>(shapeChar->getDrawCall());
		Skeleton* idleTrack = nullptr;
		Skeleton* runTrack = nullptr;
		strcpy_s(path, characterIdle.c_str());
		succededLoading = (p.importSkeletonTrack(path));
		assert(succededLoading);
		idleTrack = new Skeleton(G_S_IDLE);
		for (int i = 0; i < p.getBones().size(); i++)
		{
			idleTrack->setBone(p.getBones()[i], p.getBoneKeys(i));
		}
		idleTrack->setTimeKeyCountAndOffsets(p.getFPS(), p.getAniLength(), p.getOffsetMatrices());
		strcpy_s(path, characterRun.c_str());
		succededLoading = (p.importSkeletonTrack(path));
		assert(succededLoading);
		runTrack = new Skeleton(G_S_RUN);
		for (int i = 0; i < p.getBones().size(); i++)
		{
			runTrack->setBone(p.getBones()[i], p.getBoneKeys(i));
		}
		runTrack->setTimeKeyCountAndOffsets(p.getFPS(), p.getAniLength(), p.getOffsetMatrices());
		skeletonTemp->g_animations.insert({ G_S_IDLE, idleTrack});
		skeletonTemp->g_animations.insert({ G_S_RUN, runTrack });
		
		m_allSceneShapes.push_back(shapeChar);
		Skeleton* combined = runTrack->combineTracks(idleTrack, "clavicle_r", G_S_RUN_G_S_IDLE_COMBINED);
		skeletonTemp->g_animations.insert({ G_S_RUN_G_S_IDLE_COMBINED, combined });


		strcpy_s(path, cloth.c_str());
		succededLoading = (p.importMeshes(path));
		assert(succededLoading);
		Shape* shapeCloth = new Shape(Renderer::getDevice(), DC_TYPE::SKELETON, rp, RENDER_TYPE::CLOTH);
		shapeCloth->setName(p.getMeshInfo(0).meshName);
		shapeCloth->setVertices(Renderer::getDevice(), p.getVertices(0));
		shapeCloth->setIndices(Renderer::getDevice(), p.getIndices(0));
		shapeCloth->setTransformStatus(TRANSFORM_TYPE::DYNAMIC);
		shapeCloth->setParent(shapeChar);
		strcpy_s(path, clothIdle.c_str());
		succededLoading = (p.importSkeletonTrack(path));
		assert(succededLoading);
		Skeleton* idleTrackCloth = new Skeleton(G_S_IDLE);
		for (int i = 0; i < p.getBones().size(); i++)
		{
			idleTrackCloth->setBone(p.getBones()[i], p.getBoneKeys(i));
		}
		idleTrackCloth->setTimeKeyCountAndOffsets(p.getFPS(), p.getAniLength(), p.getOffsetMatrices());
	
		skeletonTemp = dynamic_cast<DC_SKELETON*>(shapeCloth->getDrawCall());
		skeletonTemp->g_animations.insert({ G_S_IDLE,idleTrackCloth });
	
		strcpy_s(path, clothRun.c_str());
		succededLoading = (p.importSkeletonTrack(path));
		assert(succededLoading);
		Skeleton* runTrackCloth = new Skeleton(G_S_RUN);
		
		for (int i = 0; i < p.getBones().size(); i++)
		{
			runTrackCloth->setBone(p.getBones()[i], p.getBoneKeys(i));
		}
		runTrackCloth->setTimeKeyCountAndOffsets(p.getFPS(), p.getAniLength(), p.getOffsetMatrices());
		
		skeletonTemp->g_animations.insert({ G_S_RUN,runTrackCloth });
		
		m_allSceneShapes.push_back(shapeCloth);
		m_playerClothShape = shapeCloth;
		p.importTextures("coat", m_playerClothShape);
		shapeCloth->g_renderBack = true;
		
		Skeleton* comb = runTrackCloth->combineTracks(idleTrackCloth, "clavicle_r", G_S_RUN_G_S_IDLE_COMBINED);
		skeletonTemp->g_animations.insert({ G_S_RUN_G_S_IDLE_COMBINED, comb });
		
		m_dirLight = new Light(LIGHT_TYPE::DIRECTIONAL, XMFLOAT4(0, 0, 0, 1), XMFLOAT4(1, 1, 1, 1), 0.2f, XMFLOAT4(1, 0, 0, 0),XMConvertToRadians(90), LIGHT_SHADOW_TYPE::HAS_SHADOW);
		m_dirLight->setTransformStatus(TRANSFORM_TYPE::DYNAMIC);
		BOUNDING_SPHERE tempBS;
		tempBS.center = XMFLOAT3(0, 0, 0);
		tempBS.radius = 500;
		m_dirLight->setShadow(tempBS);

		Light* spotLight = new Light(LIGHT_TYPE::SPOT, XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 1, 0, 1), 20, XMFLOAT4(1, 0, 0, 0), XMConvertToRadians(45), LIGHT_SHADOW_TYPE::NO_SHADOW);
		spotLight->setParent(m_playerShape);
		m_allSceneLights.push_back(spotLight);
		spotLight->setTransformStatus(TRANSFORM_TYPE::DYNAMIC);
#if POINT_SHADOWS
		Light* playerLight = new Light(LIGHT_TYPE::POINT, XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 1, 0, 1), 20000,
			XMFLOAT4(1, 0, 0, 0), XMConvertToRadians(90), LIGHT_SHADOW_TYPE::HAS_SHADOW, 10);
		playerLight->setTransformStatus(TRANSFORM_TYPE::DYNAMIC);
		playerLight->setParent(m_playerShape);
		m_allSceneLights.push_back(playerLight);
#endif // POINT_SHADOWS


		Shape* fire =createParticleSystem(RENDER_TYPE::PARTICLE_FIRE_TYPE);
		Shape* rain = createParticleSystem(RENDER_TYPE::PARTICLE_RAIN_TYPE);
		Shape* text = createParticleSystem(RENDER_TYPE::PARTICLE_TEXT_TYPE);
		Shape* leaf = createParticleSystem(RENDER_TYPE::PARTICLE_LEAVES_TYPE);
		fire->setTranslation({ 0,-5,0 });
		fire->updateCBfr();

		TextureHandler::getInstance().addTextureSRV("../Resources/Textures/fire.png", Renderer::getDevice());
		dynamic_cast<DC_PARTICLE*>(fire->getDrawCall())->g_srvs.insert({ SRV_ALBEDO_SLOT,"../Resources/Textures/fire.png" });

		m_allSceneShapes.push_back(fire);
		m_allSceneShapes.push_back(rain);
		TextureHandler::getInstance().addTextureSRV("../Resources/Textures/oakLeaf.png", Renderer::getDevice());
		dynamic_cast<DC_PARTICLE*>(leaf->getDrawCall())->g_srvs.insert({ SRV_ALBEDO_SLOT,"../Resources/Textures/oakLeaf.png" });
		
		
		m_allSceneShapes.push_back(leaf);
		m_allSceneShapes.push_back(text);
		TextureHandler::getInstance().addTextureSRV("../Resources/Textures/Claw.png", Renderer::getDevice());
		dynamic_cast<DC_PARTICLE*>(text->getDrawCall())->g_srvs.insert({ SRV_ALBEDO_SLOT,"../Resources/Textures/Claw.png" });

		leaf->setTranslation({ 10,0,5 });
		leaf->updateCBfr();


		m_allSceneShapes.push_back(createSkySphere());


		char meshPathChar[100] = "../Resources/Models/skySphereMESH.x";
		Parser::getInstance().importMeshes(meshPathChar);

		Shape* skyShape = new Shape(Renderer::getDevice(), DC_TYPE::BASIC, RENDERPASS::BASIC_PASS_SINGLE, RENDER_TYPE::DYNAMIC_SKY);
		skyShape->setTransformStatus(TRANSFORM_TYPE::DYNAMIC);
		skyShape->setVertices(Renderer::getDevice(), Parser::getInstance().getVertices(0));
		skyShape->setIndices(Renderer::getDevice(), Parser::getInstance().getIndices(0));
		
		skyShape->setScale({ 0.01f, 0.01f, 0.01f });
		skyShape->setTranslation({ 0,0,100 });
		
		m_allSceneShapes.push_back(skyShape);
		m_moveAndRotateShapes.push_back(skyShape);
		m_allSceneShapes.push_back(createTerrain());
		



		char path1[100] = "../Resources/Models/BuildingsMESH.x";
		succededLoading = (p.importMeshes(path1));

		assert(succededLoading);

		
		rp |= RENDERPASS::DEPTH_LIGHT_PASS;
		rp |= RENDERPASS::DEPTH_SHADOW_DIR;
		rp |= RENDERPASS::DYNAMIC_CUBE_MAP_PASS;
		rp |= RENDERPASS::BASIC_PASS_SINGLE;
#if CEL_SHADING
		rp |= RENDERPASS::EDGE_DETECTION_PASS;
#endif
		Shape* ocshape = nullptr;

		for (int i = 0; i < p.getSceneInfo().meshCount; i++)
		{
			ocshape = new Shape(Renderer::getDevice(), DC_TYPE::BASIC, rp, RENDER_TYPE::OC_TREE);
			ocshape->setVertices(Renderer::getDevice(), p.getVertices(i));
			ocshape->setIndices(Renderer::getDevice(), p.getIndices(i));
			ocshape->setName(p.getMeshInfo(i).meshName);
			m_ocShapes.push_back(ocshape);
			p.importTextures(ocshape->getName(), ocshape);
			
		}

		char path2[100] = "../Resources/Models/churchMESH.x";
		succededLoading = (p.importMeshes(path2));
		assert(succededLoading);

		for (int i = 0; i < p.getSceneInfo().meshCount; i++)
		{
			ocshape = new Shape(Renderer::getDevice(), DC_TYPE::BASIC, rp, RENDER_TYPE::OC_TREE);
			ocshape->setVertices(Renderer::getDevice(), p.getVertices(i));
			ocshape->setIndices(Renderer::getDevice(), p.getIndices(i));
			ocshape->setName(p.getMeshInfo(i).meshName);
			m_ocShapes.push_back(ocshape);
			p.importTextures(ocshape->getName(), ocshape);

		}

		rp = RENDERPASS::BASIC_PASS_QUAD;
	

		Shape* quadShape = new Shape(Renderer::getDevice(), DC_TYPE::FULLSCREEN_QUAD, rp);
		m_allSceneShapes.push_back(quadShape);
		
		rp |= RENDERPASS::DOWN_SAMPLE_EMISSIVE_PASS;
		quadShape = new Shape(Renderer::getDevice(), DC_TYPE::FULLSCREEN_QUAD, rp);
		m_allSceneShapes.push_back(quadShape);

		rp = RENDERPASS::DOWN_SAMPLE_RAY_MARCH_PASS;

		quadShape = new Shape(Renderer::getDevice(), DC_TYPE::FULLSCREEN_QUAD, rp);
		m_allSceneShapes.push_back(quadShape);

		rp = RENDERPASS::UP_SAMPLE_EMISSIVE_PASS;

		quadShape = new Shape(Renderer::getDevice(), DC_TYPE::FULLSCREEN_QUAD, rp);
		m_allSceneShapes.push_back(quadShape);

		rp = RENDERPASS::UP_SAMPLE_RAY_MARCH_PASS;

		quadShape = new Shape(Renderer::getDevice(), DC_TYPE::FULLSCREEN_QUAD, rp);
		m_allSceneShapes.push_back(quadShape);

		rp = RENDERPASS::RAY_MARCH;

		quadShape = new Shape(Renderer::getDevice(), DC_TYPE::FULLSCREEN_QUAD, rp);
		m_allSceneShapes.push_back(quadShape);

		rp = RENDERPASS::BILATERA_HOR;

		quadShape = new Shape(Renderer::getDevice(), DC_TYPE::FULLSCREEN_QUAD, rp);
		m_allSceneShapes.push_back(quadShape);

		rp = RENDERPASS::BILATERA_VER;

		quadShape = new Shape(Renderer::getDevice(), DC_TYPE::FULLSCREEN_QUAD, rp);
		m_allSceneShapes.push_back(quadShape);

		rp = RENDERPASS::GAUSSIAN_HOR;

		
		
		Shape* computeblur = new Shape(Renderer::getDevice(), DC_TYPE::COMPUTE_SHADER, rp);

		DC_COMPUTE* dcCompute = dynamic_cast<DC_COMPUTE*>(computeblur->getDrawCall());
		UINT numGroupsX = (UINT)ceilf(((float)m_currResX / 4.0f) / 256.0f);

		dcCompute->g_dispatchThreads.x = numGroupsX;
		dcCompute->g_dispatchThreads.y = (UINT)((float)m_currResY / 4.0f);
		dcCompute->g_dispatchThreads.z = 1;
		m_allSceneShapes.push_back(computeblur);
		rp = RENDERPASS::GAUSSIAN_VER;
		Shape* computeBlur2 = new Shape(Renderer::getDevice(), DC_TYPE::COMPUTE_SHADER, rp);
		dcCompute = dynamic_cast<DC_COMPUTE*>(computeBlur2->getDrawCall());

		UINT numGroupsY = (UINT)ceilf(((float)m_currResY / 4.0f) / 256.0f);
		dcCompute->g_dispatchThreads.x = (UINT)((float)m_currResX / 4.0f);
		dcCompute->g_dispatchThreads.y = numGroupsY;
		dcCompute->g_dispatchThreads.z = 1;
		m_allSceneShapes.push_back(computeBlur2);
		break;
	}
	case SCENE::MENU:
	{
		InputHandler::getInstance().SetMouseMode(Mouse::Mode::MODE_ABSOLUTE);
		InputHandler::getInstance().FreeMouse();
		break;
	}
	case SCENE::DEBUG_LIGHT_CULL:
	{
		Shape* quad = new Shape(Renderer::getDevice(), DC_TYPE::FULLSCREEN_QUAD,RENDERPASS::BASIC_PASS_QUAD);
		quad->getDrawCall()->g_srvs.erase(SRV_ALBEDO_QUAD);
		quad->getDrawCall()->g_srvs.insert({ SRV_ALBEDO_QUAD ,"heat" });
		
		m_allSceneShapes.push_back(quad);
		Light* pointLight = nullptr;
		for (int i = 0; i < 35; i++)
		{
			pointLight = new Light(LIGHT_TYPE::POINT, XMFLOAT4((float)10*i, 0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 0.5f, 0.4f, 1.0f), 1.0f);
			m_allSceneLights.push_back(pointLight);

		}

		TextureHandler::getInstance().addTextureSRV("../Resources/Textures/LightCountHeatMap.png", Renderer::getDevice());

		InputHandler::getInstance().SetMouseMode(Mouse::Mode::MODE_RELATIVE);
		InputHandler::getInstance().ConfineMouse();
		char path[100];
		std::string box = "../Resources/Models/BoxMESH.x";
		strcpy_s(path, box.c_str());
		Parser& p = Parser::getInstance();
		bool succededLoading = (p.importMeshes(path));
		assert(succededLoading);
		RENDERPASS rp = RENDERPASS::DEPTH_LIGHT_PASS;
		Shape* shape = new Shape(Renderer::getDevice(), DC_TYPE::BASIC, rp);
		shape->setName(p.getMeshInfo(0).meshName);
		shape->setVertices(Renderer::getDevice(), p.getVertices(0));
		shape->setIndices(Renderer::getDevice(), p.getIndices(0));
		shape->setTransformStatus(TRANSFORM_TYPE::DYNAMIC);

		std::vector<INSTANCE_DATA> instance(10);
		

		shape->setInstances(Renderer::getDevice(), instance);
		for (int i = 0; i < 10; i++)
		{
			shape->setScale({ (float)i * 2 + 1, (float)i * 2 + 1, (float)i * 2 + 1 }, i);
			shape->setTranslation({ (float)i * 20, (float)0, (float)0 }, i);
		}
		m_allSceneShapes.push_back(shape);

		
		break;
	}
	case SCENE::PARTICLE:
	{
		InputHandler::getInstance().SetMouseMode(Mouse::Mode::MODE_RELATIVE);
		InputHandler::getInstance().ConfineMouse();
		char path[100];
		std::string box = "../Resources/Models/cubeMESH.x";
		strcpy_s(path, box.c_str());
		Parser& p = Parser::getInstance();
		bool succededLoading = (p.importMeshes(path));
		assert(succededLoading);
		RENDERPASS rp = RENDERPASS::DEPTH_LIGHT_PASS;
		rp |= RENDERPASS::EDGE_DETECTION_PASS;
		rp |= RENDERPASS::DYNAMIC_CUBE_MAP_PASS;
		rp |= RENDERPASS::BASIC_PASS_SINGLE;
		Shape* shape = new Shape(Renderer::getDevice(), DC_TYPE::BASIC, rp);
		shape->setName(p.getMeshInfo(0).meshName);
		shape->setVertices(Renderer::getDevice(), p.getVertices(0));
		shape->setIndices(Renderer::getDevice(), p.getIndices(0));
		shape->setTransformStatus(TRANSFORM_TYPE::DYNAMIC);
		std::vector<INSTANCE_DATA> instance(10);
		shape->setInstances(Renderer::getDevice(), instance);
		for (int i = 0; i < 10; i++)
		{
			
			shape->setScale({ (float)i * 2 + 1, (float)i * 2 + 1, (float)i * 2 + 1 }, i);
			shape->setTranslation({ (float)i * 20, (float)40, (float)i * 20 }, i);
			
			Light* light = new Light(LIGHT_TYPE::POINT,XMFLOAT4(shape->getLocalTranformations(i).trans.x, shape->getLocalTranformations(i).trans.y, shape->getLocalTranformations(i).trans.z, 1), XMFLOAT4(randF(0,1.0f/(float)(i+0.1)), randF(0, 1), randF(0, 1), 1), 
				100, XMFLOAT4(0, 0, 0, 0), 0, LIGHT_SHADOW_TYPE::NO_SHADOW, 50);

			m_allSceneLights.push_back(light);
		}
		shape->updateInstances(Renderer::getContext());
		m_moveAndRotateShapes.push_back(shape);
		m_allSceneShapes.push_back(shape);
		char pathGround[100] = "../Resources/Models/groundMESH.x";
		succededLoading = (p.importMeshes(pathGround));
		assert(succededLoading);
		RENDERPASS rp0 = RENDERPASS::DEPTH_LIGHT_PASS;
		rp0 |= RENDERPASS::DYNAMIC_CUBE_MAP_PASS;
		rp0 |= RENDERPASS::BASIC_PASS_SINGLE;
		shape = new Shape(Renderer::getDevice(), DC_TYPE::BASIC, rp0, RENDER_TYPE::QUAD_TREE);
		shape->setVertices(Renderer::getDevice(), p.getVertices(0));
		shape->setIndices(Renderer::getDevice(), p.getIndices(0));
		m_quadShapes.push_back(shape);
		TextureHandler::getInstance().addTextureSRV("../Resources/Textures/grass.png", Renderer::getDevice());
		shape->getDrawCall()->g_srvs.erase(SRV_ALBEDO_SLOT);
		shape->getDrawCall()->g_srvs.insert({ SRV_ALBEDO_SLOT,"../Resources/Textures/grass.png" });

		////m_allSceneShapes.push_back(createTerrain());
		m_allSceneShapes.push_back(createSkySphere());
		shape = new Shape(Renderer::getDevice(), DC_TYPE::FULLSCREEN_QUAD, RENDERPASS::FXAA);
		m_allSceneShapes.push_back(shape);
		m_dirLight = new Light(LIGHT_TYPE::DIRECTIONAL, XMFLOAT4(0, 0, 0, 1), XMFLOAT4(1, 1, 1, 1), 2, XMFLOAT4(0, 0, 1, 0), XMConvertToRadians(90));
		m_dirLight->setTransformStatus(TRANSFORM_TYPE::DYNAMIC);
		char meshPathChar[100] = "../Resources/Models/skySphereMESH.x";
		p.importMeshes(meshPathChar);
		shape = new Shape(Renderer::getDevice(), DC_TYPE::BASIC, RENDERPASS::BASIC_PASS_SINGLE, RENDER_TYPE::DYNAMIC_SKY);
		shape->setVertices(Renderer::getDevice(),p.getVertices(0));
		shape->setIndices(Renderer::getDevice(), p.getIndices(0));
		shape->setScale({ 0.005f, 0.005f, 0.005f });
		shape->setTranslation({ 5,50,50 });
		shape->updateCBfr();
		m_allSceneShapes.push_back(shape);
		char meshPathChar1[100] = "../Resources/Models/CookingPlaceMESH.x";
		succededLoading = p.importMeshes(meshPathChar1);
		assert(succededLoading);
		RENDERPASS rp1 = RENDERPASS::DEPTH_LIGHT_PASS;
		rp1 |= RENDERPASS::EDGE_DETECTION_PASS;
		rp1 |= RENDERPASS::DYNAMIC_CUBE_MAP_PASS;
		rp1 |= RENDERPASS::BASIC_PASS_SINGLE;
		for (int i = 0; i < p.getSceneInfo().meshCount; i++)
		{
			shape = new Shape(Renderer::getDevice(), DC_TYPE::BASIC, rp1);
			shape->setVertices(Renderer::getDevice(), p.getVertices(i));
			shape->setIndices(Renderer::getDevice(), p.getIndices(i));
			shape->setTranslation({400,10,200 });
			shape->setScale({ 5,5, 5 });
			shape->updateCBfr();
			shape->setName(p.getMeshInfo(i).meshName);
			m_allSceneShapes.push_back(shape);
			p.importTextures(shape->getName(), shape);
		}


		TextureHandler::getInstance().addTextureSRV("../Resources/Textures/LightCountHeatMap.png", Renderer::getDevice());
		TextureHandler::getInstance().addTextureSRV("../Resources/Textures/fire.png", Renderer::getDevice());
		TextureHandler::getInstance().addTextureSRV("../Resources/Textures/smoke.png", Renderer::getDevice());
		TextureHandler::getInstance().addTextureSRV("../Resources/Textures/oakLeaf.png", Renderer::getDevice());
		TextureHandler::getInstance().addTextureSRV("../Resources/Textures/Claw.png", Renderer::getDevice());
		shape = createParticleSystem(RENDER_TYPE::PARTICLE_FIRE_TYPE);
		
		shape->setTranslation({ 400, 10, 250 });
		shape->updateCBfr();
		m_allSceneShapes.push_back(shape);
		shape = createParticleSystem(RENDER_TYPE::PARTICLE_FIRE_TYPE);

		shape->setTranslation({ 400, 10, 150 });
		shape->updateCBfr();
		m_allSceneShapes.push_back(shape);

		shape = createParticleSystem(RENDER_TYPE::PARTICLE_FIRE_TYPE);

		shape->setTranslation({ 400, 50, 200 });
		shape->updateCBfr();
		shape->getDrawCall()->g_srvs.erase(SRV_ALBEDO_SLOT);
		shape->getDrawCall()->g_srvs.insert({ SRV_ALBEDO_SLOT,"../Resources/Textures/smoke.png" });
		shape->setName("smoke");
		m_allSceneShapes.push_back(shape);

		shape = createParticleSystem(RENDER_TYPE::PARTICLE_RAIN_TYPE);
		m_allSceneShapes.push_back(shape);

		shape = createParticleSystem(RENDER_TYPE::PARTICLE_TEXT_TYPE);
		shape->setTranslation({ 0, 100, 0 });
		shape->updateCBfr();
		m_allSceneShapes.push_back(shape);


		shape = createParticleSystem(RENDER_TYPE::PARTICLE_LEAVES_TYPE);
		shape->setTranslation({ 40, 100, 400 });
		shape->updateCBfr();
		m_allSceneShapes.push_back(shape);


		char meshPathChar2[100] = "../Resources/Models/firTreeMESH.x";
		succededLoading = p.importMeshes(meshPathChar2);
		assert(succededLoading);
		
		for (int i = 0; i < p.getSceneInfo().meshCount; i++)
		{

			if (i == 1)
			{
				rp1 &= ~RENDERPASS::BASIC_PASS_SINGLE;
				rp1 &= ~RENDERPASS::EDGE_DETECTION_PASS;
				rp1 |= RENDERPASS::BASIC_PASS_TRANSPARENT;
			}
			shape = new Shape(Renderer::getDevice(), DC_TYPE::BASIC, rp1);
			shape->setVertices(Renderer::getDevice(), p.getVertices(i));
			shape->setIndices(Renderer::getDevice(), p.getIndices(i));
			shape->setTranslation({ 40, 10, 400 });
			shape->setScale({ 5,5, 5 });
			shape->updateCBfr();
			shape->setName(p.getMeshInfo(i).meshName);
			m_allSceneShapes.push_back(shape);

			if (i == 1)
			{
				shape->g_renderBack = true;
			}
			p.importTextures(shape->getName(), shape);
		}


		break;
	}
	case SCENE::LOD:
	{
		InputHandler::getInstance().SetMouseMode(Mouse::Mode::MODE_RELATIVE);
		InputHandler::getInstance().ConfineMouse();
		Parser& p = Parser::getInstance();
		m_dirLight = new Light(LIGHT_TYPE::DIRECTIONAL, XMFLOAT4(0, 0, 0, 1), XMFLOAT4(1, 1, 1, 1), 10, XMFLOAT4(0, 0, 1, 0), XMConvertToRadians(90), LIGHT_SHADOW_TYPE::HAS_SHADOW);
		BOUNDING_SPHERE bs;
		bs.center = XMFLOAT3(0, 0, 0);
		bs.radius = 1000;
		m_dirLight->setShadow(bs);
		m_dirLight->setTransformStatus(TRANSFORM_TYPE::DYNAMIC);
		RENDERPASS rp = RENDERPASS::DEPTH_LIGHT_PASS;
		rp |= RENDERPASS::DEPTH_SHADOW_DIR;
		rp |= RENDERPASS::BASIC_PASS_SINGLE;
		rp |= RENDERPASS::DEPTH_SHADOW_POINT;
		char path[100] = "../Resources/Models/churchMESH.x";
		bool succededLoading = (p.importMeshes(path));
		assert(succededLoading);
		Shape* ocshape = nullptr;
		for (int i = 0; i < p.getSceneInfo().meshCount; i++)
		{
			ocshape = new Shape(Renderer::getDevice(), DC_TYPE::BASIC, rp, RENDER_TYPE::OC_TREE);
			ocshape->setVertices(Renderer::getDevice(), p.getVertices(i));
			ocshape->setIndices(Renderer::getDevice(), p.getIndices(i));
			ocshape->setName(p.getMeshInfo(i).meshName);
			m_ocShapes.push_back(ocshape);
			p.importTextures(ocshape->getName(), ocshape);

		}

		char path1[100] = "../Resources/Models/BuildingsMESH.x";
		succededLoading = (p.importMeshes(path1));
		assert(succededLoading);
		for (int i = 0; i < p.getSceneInfo().meshCount; i++)
		{
			ocshape = new Shape(Renderer::getDevice(), DC_TYPE::BASIC, rp, RENDER_TYPE::OC_TREE);
			ocshape->setVertices(Renderer::getDevice(), p.getVertices(i));
			ocshape->setIndices(Renderer::getDevice(), p.getIndices(i));
			ocshape->setName(p.getMeshInfo(i).meshName);
			m_ocShapes.push_back(ocshape);
			p.importTextures(ocshape->getName(), ocshape);

		}
		m_allSceneShapes.push_back(createTerrain());
		m_allSceneShapes.push_back(createSkySphere());

		rp = RENDERPASS::BASIC_PASS_QUAD;


		Shape* quadShape = new Shape(Renderer::getDevice(), DC_TYPE::FULLSCREEN_QUAD, rp);
		m_allSceneShapes.push_back(quadShape);

		rp = RENDERPASS::DOWN_SAMPLE_EMISSIVE_PASS;
		quadShape = new Shape(Renderer::getDevice(), DC_TYPE::FULLSCREEN_QUAD, rp);
		m_allSceneShapes.push_back(quadShape);


		rp = RENDERPASS::UP_SAMPLE_EMISSIVE_PASS;

		quadShape = new Shape(Renderer::getDevice(), DC_TYPE::FULLSCREEN_QUAD, rp);
		m_allSceneShapes.push_back(quadShape);

		rp = RENDERPASS::GAUSSIAN_HOR;

		Shape* computeblur = new Shape(Renderer::getDevice(), DC_TYPE::COMPUTE_SHADER, rp);

		DC_COMPUTE* dcCompute = dynamic_cast<DC_COMPUTE*>(computeblur->getDrawCall());
		UINT numGroupsX = (UINT)ceilf(((float)m_currResX / 4.0f) / 256.0f);

		dcCompute->g_dispatchThreads.x = numGroupsX;
		dcCompute->g_dispatchThreads.y = (UINT)((float)m_currResY / 4.0f);
		dcCompute->g_dispatchThreads.z = 1;
		m_allSceneShapes.push_back(computeblur);
		rp = RENDERPASS::GAUSSIAN_VER;
		Shape* computeBlur2 = new Shape(Renderer::getDevice(), DC_TYPE::COMPUTE_SHADER, rp);
		dcCompute = dynamic_cast<DC_COMPUTE*>(computeBlur2->getDrawCall());

		UINT numGroupsY = (UINT)ceilf(((float)m_currResY / 4.0f) / 256.0f);
		dcCompute->g_dispatchThreads.x = (UINT)((float)m_currResX / 4.0f);
		dcCompute->g_dispatchThreads.y = numGroupsY;
		dcCompute->g_dispatchThreads.z = 1;
		m_allSceneShapes.push_back(computeBlur2);

		rp = RENDERPASS::DEPTH_LIGHT_PASS;
		rp |= RENDERPASS::DEPTH_SHADOW_DIR;
		rp |= RENDERPASS::BASIC_PASS_SINGLE;
		rp |= RENDERPASS::DEPTH_SHADOW_POINT;
		char path2[100] = "../Resources/Models/streetLampMESH.x";
		succededLoading = (p.importMeshes(path2));
		assert(succededLoading);
		Shape* shape = new Shape(Renderer::getDevice(), DC_TYPE::BASIC, rp);
		shape->setName(p.getMeshInfo(0).meshName);
		shape->setVertices(Renderer::getDevice(), p.getVertices(0));
		shape->setIndices(Renderer::getDevice(), p.getIndices(0));
		p.importTextures(shape->getName(), shape);
		std::vector<INSTANCE_DATA> instance(50);
		shape->setInstances(Renderer::getDevice(), instance);
		for (int i = 0; i < 50; i++)
		{
			float x = randF(-500, 500);
			float z = randF(-500, 500);
			shape->setTranslation({ x,60,z }, i);
			shape->setScale({ 3,3,3 }, i);
			Light* sLight = new Light(LIGHT_TYPE::POINT, XMFLOAT4(x, 70, z, 1), XMFLOAT4(1, 0.2f, 0.2f, 1), 40, XMFLOAT4(0, -1, 0, 0), XMConvertToRadians(90),LIGHT_SHADOW_TYPE::NO_SHADOW, 50);
			m_allSceneLights.push_back(sLight);
		}
		shape->generateBounds();
		shape->g_cull = true;
		shape->updateInstances(Renderer::getContext());
		m_allSceneShapes.push_back(shape);

		std::vector<Shape*> bb;
		bb.push_back(shape);
		visualizeBoundingBoxes(bb,false);
		Light* pLight = new Light(LIGHT_TYPE::POINT, XMFLOAT4(0,0,0,1), XMFLOAT4(1, 1, 1, 1), 40, XMFLOAT4(0, -1, 0, 0), XMConvertToRadians(90), LIGHT_SHADOW_TYPE::HAS_SHADOW, 50);
		pLight->setTransformStatus(TRANSFORM_TYPE::DYNAMIC);
		m_allSceneLights.push_back(pLight);
		m_shadowPoint = pLight;
		break;
	}
	case SCENE::HEAT:
	{
		InputHandler::getInstance().SetMouseMode(Mouse::Mode::MODE_RELATIVE);
		InputHandler::getInstance().ConfineMouse();
		Parser& p = Parser::getInstance();

		RENDERPASS rp = RENDERPASS::DEPTH_LIGHT_PASS;
		char path[100] = "../Resources/Models/churchMESH.x";
		bool succededLoading = (p.importMeshes(path));
		assert(succededLoading);
		Shape* ocshape = nullptr;
		for (int i = 0; i < p.getSceneInfo().meshCount; i++)
		{
			ocshape = new Shape(Renderer::getDevice(), DC_TYPE::BASIC, rp, RENDER_TYPE::OC_TREE);
			ocshape->setVertices(Renderer::getDevice(), p.getVertices(i));
			ocshape->setIndices(Renderer::getDevice(), p.getIndices(i));
			ocshape->setName(p.getMeshInfo(i).meshName);
			m_ocShapes.push_back(ocshape);
			p.importTextures(ocshape->getName(), ocshape);

		}

		char path1[100] = "../Resources/Models/BuildingsMESH.x";
		succededLoading = (p.importMeshes(path1));
		assert(succededLoading);
		for (int i = 0; i < p.getSceneInfo().meshCount; i++)
		{
			ocshape = new Shape(Renderer::getDevice(), DC_TYPE::BASIC, rp, RENDER_TYPE::OC_TREE);
			ocshape->setVertices(Renderer::getDevice(), p.getVertices(i));
			ocshape->setIndices(Renderer::getDevice(), p.getIndices(i));
			ocshape->setName(p.getMeshInfo(i).meshName);
			m_ocShapes.push_back(ocshape);
			p.importTextures(ocshape->getName(), ocshape);

		}
		m_allSceneShapes.push_back(createTerrain());
		char path2[100] = "../Resources/Models/streetLampMESH.x";
		succededLoading = (p.importMeshes(path2));
		assert(succededLoading);
		Shape* shape = new Shape(Renderer::getDevice(), DC_TYPE::BASIC, rp);
		shape->setName(p.getMeshInfo(0).meshName);
		shape->setVertices(Renderer::getDevice(), p.getVertices(0));
		shape->setIndices(Renderer::getDevice(), p.getIndices(0));
		p.importTextures(shape->getName(), shape);
		std::vector<INSTANCE_DATA> instance(50);
		shape->setInstances(Renderer::getDevice(), instance);
		for (int i = 0; i < 50; i++)
		{
			float x = randF(-500, 500);
			float z = randF(-500, 500);
			shape->setTranslation({ x,60,z }, i);
			shape->setScale({ 3,3,3 }, i);
			Light* sLight = new Light(LIGHT_TYPE::POINT, XMFLOAT4(x, 70, z, 1), XMFLOAT4(1, 0.2f, 0.2f, 1), 40, XMFLOAT4(0, -1, 0, 0), XMConvertToRadians(90), LIGHT_SHADOW_TYPE::NO_SHADOW, 50);
			m_allSceneLights.push_back(sLight);
		}
		shape->generateBounds();
		shape->g_cull = true;
		shape->updateInstances(Renderer::getContext());
		m_allSceneShapes.push_back(shape);

		rp = RENDERPASS::BASIC_PASS_QUAD;

		Shape* quadShape = new Shape(Renderer::getDevice(), DC_TYPE::FULLSCREEN_QUAD, rp, RENDER_TYPE::HEAT_MAP);
		m_allSceneShapes.push_back(quadShape);
		break;
	}
	case SCENE::ANIMATION:
	{

		InputHandler::getInstance().SetMouseMode(Mouse::Mode::MODE_RELATIVE);
		InputHandler::getInstance().ConfineMouse();
		Parser& p = Parser::getInstance();
		m_dirLight = new Light(LIGHT_TYPE::DIRECTIONAL, XMFLOAT4(0, 0, 0, 1), XMFLOAT4(1, 1, 1, 1), 10, XMFLOAT4(0, 0, 1, 0), XMConvertToRadians(90), LIGHT_SHADOW_TYPE::NO_SHADOW);
	
		m_dirLight->setTransformStatus(TRANSFORM_TYPE::DYNAMIC);
		RENDERPASS rp = RENDERPASS::DEPTH_LIGHT_PASS;
		rp |= RENDERPASS::BASIC_PASS_SINGLE;
	
		char path[100] = "../Resources/Models/churchMESH.x";
		bool succededLoading = (p.importMeshes(path));
		assert(succededLoading);


		float charScale = 0.4444444444f;//used to be 0.0444...
		std::string character = "../Resources/Models/mainCharIdleMESH.x";
		std::string characterIdle = "../Resources/Animations/Skeletons/mainCharIdleSKELETON.x";
		std::string characterRun = "../Resources/Animations/Skeletons/mainCharRunSKELETON.x";
		std::string cloth = "../Resources/Models/coatIdleMESH.x";
		std::string clothIdle = "../Resources/Animations/Skeletons/coatIdleSKELETON.x";
		std::string clothRun = "../Resources/Animations/Skeletons/coatRunSKELETON.x";
		std::string clothWalk = "../Resources/Animations/Skeletons/kappaWalkSKELETON.x";
		std::string clothAttack = "../Resources/Animations/Skeletons/kappaFireBallSKELETON.x";
		std::string characterWalk = "../Resources/Animations/Skeletons/magikerWalkSKELETON.x";
		std::string characterAttack = "../Resources/Animations/Skeletons/magikerFireBallSKELETON.x";
		strcpy_s(path, character.c_str());
		succededLoading = (p.importMeshes(path));
		assert(succededLoading);
		Shape* shapeChar = new Shape(Renderer::getDevice(), DC_TYPE::SKELETON, rp, RENDER_TYPE::PLAYER_CHARACTER);
		shapeChar->setScale({ charScale ,charScale ,charScale });
		DC_SKELETON* dcSkel = dynamic_cast<DC_SKELETON*>(shapeChar->getDrawCall());
		shapeChar->updateCBfr();
		shapeChar->setName(p.getMeshInfo(0).meshName);
		shapeChar->setVertices(Renderer::getDevice(), p.getVertices(0));
		shapeChar->setIndices(Renderer::getDevice(), p.getIndices(0));
		shapeChar->setTransformStatus(TRANSFORM_TYPE::DYNAMIC);
		m_playerShape = shapeChar;
		p.importTextures("mainChar", m_playerShape);
		DC_SKELETON* skeletonTemp = nullptr;
		skeletonTemp = dynamic_cast<DC_SKELETON*>(shapeChar->getDrawCall());
		Skeleton* idleTrack = nullptr;
		Skeleton* runTrack = nullptr;
		Skeleton* attackTrack = nullptr;
		Skeleton* walkTrack = nullptr;
		strcpy_s(path, characterIdle.c_str());
		succededLoading = (p.importSkeletonTrack(path));
		assert(succededLoading);
		idleTrack = new Skeleton(G_S_IDLE);
		for (int i = 0; i < p.getBones().size(); i++)
		{
			idleTrack->setBone(p.getBones()[i], p.getBoneKeys(i));
		}
		idleTrack->setTimeKeyCountAndOffsets(p.getFPS(), p.getAniLength(), p.getOffsetMatrices());
		strcpy_s(path, characterRun.c_str());
		succededLoading = (p.importSkeletonTrack(path));
		assert(succededLoading);
		runTrack = new Skeleton(G_S_RUN);
		for (int i = 0; i < p.getBones().size(); i++)
		{
			runTrack->setBone(p.getBones()[i], p.getBoneKeys(i));
		}
		runTrack->setTimeKeyCountAndOffsets(p.getFPS(), p.getAniLength(), p.getOffsetMatrices());
		//
		strcpy_s(path, characterAttack.c_str());
		succededLoading = (p.importSkeletonTrack(path));
		assert(succededLoading);
		attackTrack = new Skeleton(G_S_ATTACK);
		for (int i = 0; i < p.getBones().size(); i++)
		{
			attackTrack->setBone(p.getBones()[i], p.getBoneKeys(i));
		}
		attackTrack->setTimeKeyCountAndOffsets(p.getFPS(), p.getAniLength(), p.getOffsetMatrices());

		strcpy_s(path, characterWalk.c_str());
		succededLoading = (p.importSkeletonTrack(path));
		assert(succededLoading);
		walkTrack = new Skeleton(G_S_WALK);
		for (int i = 0; i < p.getBones().size(); i++)
		{
			walkTrack->setBone(p.getBones()[i], p.getBoneKeys(i));
		}
		walkTrack->setTimeKeyCountAndOffsets(p.getFPS(), p.getAniLength(), p.getOffsetMatrices());
		//
		skeletonTemp->g_animations.insert({ G_S_IDLE, idleTrack });
		skeletonTemp->g_animations.insert({ G_S_RUN, runTrack });
		skeletonTemp->g_animations.insert({ G_S_ATTACK, attackTrack });
		skeletonTemp->g_animations.insert({ G_S_WALK, walkTrack });
		m_allSceneShapes.push_back(shapeChar);



		strcpy_s(path, cloth.c_str());
		succededLoading = (p.importMeshes(path));
		assert(succededLoading);
		Shape* shapeCloth = new Shape(Renderer::getDevice(), DC_TYPE::SKELETON, rp, RENDER_TYPE::CLOTH);
		shapeCloth->setName(p.getMeshInfo(0).meshName);
		shapeCloth->setVertices(Renderer::getDevice(), p.getVertices(0));
		shapeCloth->setIndices(Renderer::getDevice(), p.getIndices(0));
		shapeCloth->setTransformStatus(TRANSFORM_TYPE::DYNAMIC);
		shapeCloth->setParent(shapeChar);
		strcpy_s(path, clothIdle.c_str());
		succededLoading = (p.importSkeletonTrack(path));
		assert(succededLoading);
		Skeleton* idleTrackCloth = new Skeleton(G_S_IDLE);
		for (int i = 0; i < p.getBones().size(); i++)
		{
			idleTrackCloth->setBone(p.getBones()[i], p.getBoneKeys(i));
		}
		idleTrackCloth->setTimeKeyCountAndOffsets(p.getFPS(), p.getAniLength(), p.getOffsetMatrices());

		skeletonTemp = dynamic_cast<DC_SKELETON*>(shapeCloth->getDrawCall());
		skeletonTemp->g_animations.insert({ G_S_IDLE,idleTrackCloth });

		strcpy_s(path, clothRun.c_str());
		succededLoading = (p.importSkeletonTrack(path));
		assert(succededLoading);
		Skeleton* runTrackCloth = new Skeleton(G_S_RUN);

		for (int i = 0; i < p.getBones().size(); i++)
		{
			runTrackCloth->setBone(p.getBones()[i], p.getBoneKeys(i));
		}
		runTrackCloth->setTimeKeyCountAndOffsets(p.getFPS(), p.getAniLength(), p.getOffsetMatrices());

		skeletonTemp->g_animations.insert({ G_S_RUN,runTrackCloth });

		m_allSceneShapes.push_back(shapeCloth);
		m_playerClothShape = shapeCloth;
		p.importTextures("coat", m_playerClothShape);
		shapeCloth->g_renderBack = true;


		strcpy_s(path, clothAttack.c_str());
		succededLoading = (p.importSkeletonTrack(path));
		assert(succededLoading);
		Skeleton* clothAttackTrack = new Skeleton(G_S_ATTACK);
		for (int i = 0; i < p.getBones().size(); i++)
		{
			clothAttackTrack->setBone(p.getBones()[i], p.getBoneKeys(i));
		}
		clothAttackTrack->setTimeKeyCountAndOffsets(p.getFPS(), p.getAniLength(), p.getOffsetMatrices());

		skeletonTemp->g_animations.insert({ G_S_ATTACK,clothAttackTrack });


		strcpy_s(path, clothWalk.c_str());
		succededLoading = (p.importSkeletonTrack(path));
		assert(succededLoading);
		Skeleton* clothWalkSkeleton = new Skeleton(G_S_WALK);
		for (int i = 0; i < p.getBones().size(); i++)
		{
			clothWalkSkeleton->setBone(p.getBones()[i], p.getBoneKeys(i));
		}
		clothWalkSkeleton->setTimeKeyCountAndOffsets(p.getFPS(), p.getAniLength(), p.getOffsetMatrices());

		skeletonTemp->g_animations.insert({ G_S_WALK,clothWalkSkeleton });

		m_allSceneShapes.push_back(createTerrain());

		rp = RENDERPASS::BASIC_PASS_QUAD;

		Shape* quadShape = new Shape(Renderer::getDevice(), DC_TYPE::FULLSCREEN_QUAD, rp);
		m_allSceneShapes.push_back(quadShape);

		break;
	}
	}
	}
}

float SceneHandler::randF(float a, float b)
{
	return a + satRandF() * (b - a);
}

void SceneHandler::Barycentric(Vector3 p, Vector3 a, Vector3 b, Vector3 c, float& u, float& v, float& w)
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

SceneHandler::~SceneHandler()
{
	for (auto s : m_allSceneShapes)
	{
		RELEASE(s);
	}
	m_allSceneShapes.clear();
	for (auto l : m_allSceneLights)
	{
		RELEASE(l);
	}
	m_allSceneLights.clear();
	RELEASE(m_dirLight);

	
}

void SceneHandler::calcAllPatchBoundsY()
{
	m_patchBoundsY.resize(m_numPatchQuadFace);
	for (UINT i = 0; i < m_numPatchVerPerRow - 1; i++)
	{

		for (UINT j = 0; j < m_numPatchVerPerCol - 1; j++)
		{
			calcPatchBoundsY(i, j);
		}
	}

}

void SceneHandler::update()
{
	m_timer.run();
	switch (m_scene)
	{
	default:
	{
		break;
	}
	case SCENE::MAIN_TEST:
	{
		for (size_t i = 0; i < m_moveAndRotateShapes.size(); i++)
		{
			moveAndRotate(m_moveAndRotateShapes[i]);
			
		}


		for (size_t i = 0; i < m_allSceneLights.size(); i++)
		{
			m_allSceneLights[i]->updateParentLocal();
		}
		break;
	}
	case SCENE::DEBUG_LIGHT_CULL:
	{
		for (size_t i = 0; i < m_allSceneShapes.size(); i++)
		{
			moveAndRotate(m_allSceneShapes[i]);

		}
		break;
	}
	case SCENE::MENU:
	{
		
		break;
	}
	case SCENE::PARTICLE:
	{
		for (size_t i = 0; i < m_moveAndRotateShapes.size(); i++)
		{
			moveAndRotate(m_moveAndRotateShapes[i]);

		}
		break;
	}
	case SCENE::LOD:
	{
		if (m_shadowPoint)
		{
			m_shadowPoint->updateParentLocal();
		}
		break;
	}
	}
	
}


void SceneHandler::calcPatchBoundsY(UINT i, UINT j)
{
	UINT x0 = j *CELLS_PER_PATCH_HM;
	UINT x1 = (j + 1) * CELLS_PER_PATCH_HM;

	UINT y0 = i * CELLS_PER_PATCH_HM;
	UINT y1 = (i + 1) * CELLS_PER_PATCH_HM;
	
	float minY = INFINITY;
	float maxY = -INFINITY;
	for (UINT y = y0; y <= y1; ++y)
	{
		for (UINT x = x0; x <= x1; ++x)
		{
			UINT k = y * m_hmW + x;
			minY = fmin(minY, m_hm[k]);
			maxY = fmax(maxY, m_hm[k]);
		}
	}

	UINT patchID = i * (m_numPatchVerPerCol - 1) + j;
	m_patchBoundsY[patchID] = XMFLOAT2(minY, maxY);
}

void SceneHandler::loadPortalCullingTest()
{
	char meshPathChar[100] = "../Resources/Models/portalTestMESH.x";
	
	Parser& p = Parser::getInstance();

	bool loaded = p.importMeshes(meshPathChar);
	assert(loaded);

	for (unsigned short i = 0; i < p.getSceneInfo().meshCount; i++)
	{
		Shape* shape = nullptr;
		shape = new Shape(Renderer::getDevice(),DC_TYPE::BASIC, RENDERPASS::BASIC_PASS_SINGLE, RENDER_TYPE::PORTAL);
		m_allSceneShapes.push_back(shape);
		shape->setName(p.getMeshInfo(i).meshName);
		shape->setVertices(Renderer::getDevice(), p.getVertices(i));
		shape->setIndices(Renderer::getDevice(), p.getIndices(i));
		shape->setScale({ 0.1f,0.1f,0.1f });
		shape->setTranslation({ 0,-10,0 });
		auto p = dynamic_cast<DC_BASIC*>(shape->getDrawCall());
		shape->updateCBfr();
	}
	

}





