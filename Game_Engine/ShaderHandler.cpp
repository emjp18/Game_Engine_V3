#include "stdafx.h"
#include "ShaderHandler.h"

void ShaderHandler::compileVSAndIL(std::string path, ID3D11Device* device, ID3D11VertexShader** shader, D3D11_INPUT_ELEMENT_DESC* desc, UINT size, ID3D11InputLayout** layout, std::string main)
{
	ID3DBlob* VSBlob = nullptr;


	std::wstring wPath = std::wstring(path.begin(), path.end());

	const D3D_SHADER_MACRO defines[] =
	{
#if _DEBUG
			"_DEBUG", "1",
#else
			"_DEBUG", "0",
#endif
			NULL, NULL
	};

	// VERTEX SHADER
	HRESULT	VSCompileResult = D3DCompileFromFile
	(
		wPath.c_str(),
		defines,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		main.c_str(),
		"vs_5_0",
		m_shaderCompilationFlag,
		0,
		&VSBlob,
		&m_errorBlob
	);

	ASSERT_SHADER(VSCompileResult, m_errorBlob, wPath);

	HRESULT VSCreateResult = device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, shader);
	assert(SUCCEEDED(VSCreateResult));

	HRESULT createInputLayoutResult = device->CreateInputLayout(desc, size, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), layout);
	assert(SUCCEEDED(createInputLayoutResult));
	RELEASE_COM(VSBlob);
}

void ShaderHandler::compileVS(std::string path, ID3D11Device* device, ID3D11VertexShader** shader, std::string main)
{
	ID3DBlob* VSBlob = nullptr;


	std::wstring wPath = std::wstring(path.begin(), path.end());

	const D3D_SHADER_MACRO defines[] =
	{
#if _DEBUG
			"_DEBUG", "1",
#else
			"_DEBUG", "0",
#endif
			NULL, NULL
	};

	// VERTEX SHADER
	HRESULT	VSCompileResult = D3DCompileFromFile
	(
		wPath.c_str(),
		defines,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		main.c_str(),
		"vs_5_0",
		m_shaderCompilationFlag,
		0,
		&VSBlob,
		&m_errorBlob
	);

	ASSERT_SHADER(VSCompileResult, m_errorBlob, wPath);

	HRESULT VSCreateResult = device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, shader);
	assert(SUCCEEDED(VSCreateResult));

	RELEASE_COM(VSBlob);
}

void ShaderHandler::compileHS(std::string path, ID3D11Device* device, ID3D11HullShader** shader, std::string main)
{
	ID3DBlob* HSBlob = nullptr;


	std::wstring wPath = std::wstring(path.begin(), path.end());

	const D3D_SHADER_MACRO defines[] =
	{
#if _DEBUG
			"_DEBUG", "1",
#else
			"_DEBUG", "0",
#endif
			NULL, NULL
	};

	// VERTEX SHADER
	HRESULT	CompileResult = D3DCompileFromFile
	(
		wPath.c_str(),
		defines,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		main.c_str(),
		"hs_5_0",
		m_shaderCompilationFlag,
		0,
		&HSBlob,
		&m_errorBlob
	);

	ASSERT_SHADER(CompileResult, m_errorBlob, wPath);

	HRESULT CreateResult = device->CreateHullShader(HSBlob->GetBufferPointer(), HSBlob->GetBufferSize(), nullptr, shader);
	assert(SUCCEEDED(CreateResult));
	RELEASE_COM(HSBlob);
}

void ShaderHandler::compileDS(std::string path, ID3D11Device* device, ID3D11DomainShader** shader, std::string main)
{
	ID3DBlob* DSBlob = nullptr;


	std::wstring wPath = std::wstring(path.begin(), path.end());

	const D3D_SHADER_MACRO defines[] =
	{
#if _DEBUG
			"_DEBUG", "1",
#else
			"_DEBUG", "0",
#endif
			NULL, NULL
	};

	// VERTEX SHADER
	HRESULT	CompileResult = D3DCompileFromFile
	(
		wPath.c_str(),
		defines,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		main.c_str(),
		"ds_5_0",
		m_shaderCompilationFlag,
		0,
		&DSBlob,
		&m_errorBlob
	);

	ASSERT_SHADER(CompileResult, m_errorBlob, wPath);

	HRESULT CreateResult = device->CreateDomainShader(DSBlob->GetBufferPointer(), DSBlob->GetBufferSize(), nullptr, shader);
	assert(SUCCEEDED(CreateResult));
	RELEASE_COM(DSBlob);
}

void ShaderHandler::compileGS(std::string path, ID3D11Device* device, ID3D11GeometryShader** shader, std::string main)
{
	ID3DBlob* GSBlob = nullptr;


	std::wstring wPath = std::wstring(path.begin(), path.end());

	const D3D_SHADER_MACRO defines[] =
	{
#if _DEBUG
			"_DEBUG", "1",
#else
			"_DEBUG", "0",
#endif
			NULL, NULL
	};

	// VERTEX SHADER
	HRESULT	CompileResult = D3DCompileFromFile
	(
		wPath.c_str(),
		defines,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		main.c_str(),
		"gs_5_0",
		m_shaderCompilationFlag,
		0,
		&GSBlob,
		&m_errorBlob
	);

	ASSERT_SHADER(CompileResult, m_errorBlob, wPath);

	HRESULT CreateResult = device->CreateGeometryShader(GSBlob->GetBufferPointer(), GSBlob->GetBufferSize(), nullptr, shader);
	assert(SUCCEEDED(CreateResult));
	RELEASE_COM(GSBlob);
}

void ShaderHandler::compileGSSO(std::string path, ID3D11Device* device, ID3D11GeometryShader** shader, std::string main) //Used only for particle atm
{
	ID3DBlob* GSBlob = nullptr;


	std::wstring wPath = std::wstring(path.begin(), path.end());

	const D3D_SHADER_MACRO defines[] =
	{
#if _DEBUG
			"_DEBUG", "1",
#else
			"_DEBUG", "0",
#endif
			NULL, NULL
	};

	// VERTEX SHADER
	HRESULT	CompileResult = D3DCompileFromFile
	(
		wPath.c_str(),
		defines,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		main.c_str(),
		"gs_5_0",
		m_shaderCompilationFlag,
		0,
		&GSBlob,
		&m_errorBlob
	);

	ASSERT_SHADER(CompileResult, m_errorBlob, wPath);

	D3D11_SO_DECLARATION_ENTRY soDeclaration[] = {

			{0,"POSITION", 0, 0,3,0},
			{0,"VELOCITY", 0, 0, 3, 0},
			{0,"SIZE", 0, 0, 2, 0},
			{0,"AGE", 0, 0, 1, 0},
			{0,"TYPE", 0, 0, 1, 0}
	};

	UINT stride = sizeof(PARTICLE);
	HRESULT hr = device->CreateGeometryShaderWithStreamOutput(GSBlob->GetBufferPointer(), GSBlob->GetBufferSize(),
		soDeclaration, _countof(soDeclaration), &stride, 1, D3D11_SO_NO_RASTERIZED_STREAM, NULL, shader);
	assert(SUCCEEDED(hr));

	RELEASE_COM(GSBlob);
}

void ShaderHandler::compileCS(std::string path, ID3D11Device* device, ID3D11ComputeShader** shader, std::string main)
{
	ID3DBlob* CSBlob = nullptr;


	std::wstring wPath = std::wstring(path.begin(), path.end());

	const D3D_SHADER_MACRO defines[] =
	{
#if _DEBUG
			"_DEBUG", "1",
#else
			"_DEBUG", "0",
#endif
			NULL, NULL
	};

	// VERTEX SHADER
	HRESULT	CompileResult = D3DCompileFromFile
	(
		wPath.c_str(),
		defines,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		main.c_str(),
		"cs_5_0",
		m_shaderCompilationFlag,
		0,
		&CSBlob,
		&m_errorBlob
	);

	ASSERT_SHADER(CompileResult, m_errorBlob, wPath);

	HRESULT CreateResult = device->CreateComputeShader(CSBlob->GetBufferPointer(), CSBlob->GetBufferSize(), nullptr, shader);
	assert(SUCCEEDED(CreateResult));

	RELEASE_COM(CSBlob);
}

void ShaderHandler::compilePS(std::string path, ID3D11Device* device, ID3D11PixelShader** shader, std::string main)
{
	ID3DBlob* PSBlob = nullptr;


	std::wstring wPath = std::wstring(path.begin(), path.end());

	const D3D_SHADER_MACRO defines[] =
	{
#if _DEBUG
			"_DEBUG", "1",
#else
			"_DEBUG", "0",
#endif
			NULL, NULL
	};

	// VERTEX SHADER
	HRESULT	CompileResult = D3DCompileFromFile
	(
		wPath.c_str(),
		defines,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		main.c_str(),
		"ps_5_0",
		m_shaderCompilationFlag,
		0,
		&PSBlob,
		&m_errorBlob
	);

	ASSERT_SHADER(CompileResult, m_errorBlob, wPath);

	HRESULT CreateResult = device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, shader);
	assert(SUCCEEDED(CreateResult));

	RELEASE_COM(PSBlob);
}

ShaderHandler::ShaderHandler()
{
	m_shaderCompilationFlag = D3DCOMPILE_ENABLE_STRICTNESS;
	//m_shaderCompilationFlag |= D3DCOMPILE_SKIP_OPTIMIZATION;
#ifdef _DEBUG
	m_shaderCompilationFlag = m_shaderCompilationFlag | D3DCOMPILE_DEBUG;
#endif
}

ShaderHandler::~ShaderHandler()
{
	RELEASE_COM(m_errorBlob);
}

void ShaderHandler::init(ID3D11Device* device)
{
	D3D11_INPUT_ELEMENT_DESC DEFAULT_INPUT_LAYOUT[7]
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	 D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"JOINTNR", 0, DXGI_FORMAT_R32G32B32A32_UINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"SKINWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}

	};
	D3D11_INPUT_ELEMENT_DESC DEFAULT_INPUT_LAYOUT_INSTANCE[15]
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	 D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"JOINTNR", 0, DXGI_FORMAT_R32G32B32A32_UINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"SKINWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "INSTANCEWORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "INSTANCEWORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "INSTANCEWORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "INSTANCEWORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "INSTANCEINVERSEWORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "INSTANCEINVERSEWORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "INSTANCEINVERSEWORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 96, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "INSTANCEINVERSEWORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 112, D3D11_INPUT_PER_INSTANCE_DATA, 1}
	};

	D3D11_INPUT_ELEMENT_DESC DEFAULT_INPUT_LAYOUT_PARTICLE[5]
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"AGE",      0, DXGI_FORMAT_R32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TYPE",     0, DXGI_FORMAT_R32_UINT,        0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	D3D11_INPUT_ELEMENT_DESC DEFAULT_INPUT_LAYOUT_TERRAIN[3]
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BOUNDS", 0, DXGI_FORMAT_R32G32_FLOAT,    0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		
	};
	//VS
	compileVSAndIL("Shaders/BasicVS.hlsl", device, m_basicVS.GetAddressOf(), DEFAULT_INPUT_LAYOUT, 7, m_IL.GetAddressOf());
	m_vertexShaders.insert({ g_basicVS,m_basicVS });
	compileVSAndIL("Shaders/BasicVS.hlsl", device, m_basicInstanceVS.GetAddressOf(), DEFAULT_INPUT_LAYOUT_INSTANCE, 15, m_instanceIL.GetAddressOf(), "mainInstance");
	m_vertexShaders.insert({ g_basicInstanceVS,m_basicInstanceVS });
	compileVSAndIL("Shaders/BasicVS.hlsl", device, m_terrainVS.GetAddressOf(), DEFAULT_INPUT_LAYOUT_TERRAIN, 3, m_terrainIL.GetAddressOf(), "mainTerrain");
	m_vertexShaders.insert({ g_terrainVS,m_terrainVS });
	compileVS("Shaders/FullScreenQuadVS.hlsl", device, m_quadVS0.GetAddressOf());
	m_vertexShaders.insert({ g_basicQuadVS,m_quadVS0 });
	compileVS("Shaders/FullScreenQuadVS.hlsl", device, m_quadVS1.GetAddressOf(), "main1");
	m_vertexShaders.insert({ g_autoQuadVS,m_quadVS1 });
	compileVS("Shaders/SkeletonVS.hlsl", device, m_skeletonVS.GetAddressOf());
	m_vertexShaders.insert({ g_skeleton,m_skeletonVS });
	compileVSAndIL("Shaders/ParticleVS.hlsl", device, m_particleMainVS.GetAddressOf(), DEFAULT_INPUT_LAYOUT_PARTICLE, 5, m_particleIL.GetAddressOf());
	m_vertexShaders.insert({ g_particleMainVS,m_particleMainVS });
	compileVS("Shaders/ParticleVS.hlsl", device, m_particleFireVS.GetAddressOf(), "fire");
	m_vertexShaders.insert({ g_particleFireVS,m_particleFireVS });
	compileVS("Shaders/ParticleVS.hlsl", device, m_particleRainVS.GetAddressOf(), "rain");
	m_vertexShaders.insert({ g_particleRainVS,m_particleRainVS });
	compileVS("Shaders/SkyVS.hlsl", device, m_skyVS.GetAddressOf());
	m_vertexShaders.insert({ g_skyVS,m_skyVS });
	compileVS("Shaders/GrassVS.hlsl", device, m_grassVS.GetAddressOf());
	m_vertexShaders.insert({ g_grassVS,m_grassVS });
	compileVS("Shaders/BasicVS.hlsl", device, m_celBasicVS.GetAddressOf(),"mainEdge");
	m_vertexShaders.insert({ g_celBasicVS,m_celBasicVS });
	compileVS("Shaders/BasicVS.hlsl", device, m_celInstanceVS.GetAddressOf(),"mainEdgeInstance");
	m_vertexShaders.insert({ g_celInstanceVS,m_celInstanceVS });
	compileVS("Shaders/SkeletonVS.hlsl", device, m_celSkeletonVS.GetAddressOf(),"mainEdgeSkeleton");
	m_vertexShaders.insert({ g_celSkeletonVS,m_celSkeletonVS });
	////HS
	compileHS("Shaders/TerrainHS.hlsl", device, m_terrainHS.GetAddressOf());
	m_hullShaders.insert({ g_terrainHS,m_terrainHS });
	compileHS("Shaders/GrassHS.hlsl", device, m_grassHS.GetAddressOf());
	m_hullShaders.insert({ g_grassHS,m_grassHS });
	//DS
	compileDS("Shaders/TerrainDS.hlsl", device, m_terrainDS.GetAddressOf());
	m_domainShaders.insert({ g_terrainDS,m_terrainDS });
	compileDS("Shaders/GrassDS.hlsl", device, m_grassDS.GetAddressOf());
	m_domainShaders.insert({ g_grassDS,m_grassDS });
	//GS
	compileGS("Shaders/DepthGS.hlsl", device, m_depthGS.GetAddressOf());
	m_geometryShaders.insert({ g_depthGS,m_depthGS });
	compileGS("Shaders/DepthGS.hlsl", device, m_depthSkyGS.GetAddressOf(), "main2");
	m_geometryShaders.insert({ g_depthSkyGS,m_depthSkyGS });
	compileGS("Shaders/ParticleGS.hlsl", device, m_particleTextGS.GetAddressOf(), "textBubble");
	m_geometryShaders.insert({ g_particleTextGS,m_particleTextGS });
	compileGSSO("Shaders/ParticleGS.hlsl", device, m_particleFireOneGS.GetAddressOf(), "fire");
	m_geometryShaders.insert({ g_particleFireOneGS,m_particleFireOneGS });
	compileGS("Shaders/ParticleGS.hlsl", device, m_particleFireTwoGS.GetAddressOf(), "fire2");
	m_geometryShaders.insert({ g_particleFireTwoGS,m_particleFireTwoGS });
	compileGSSO("Shaders/ParticleGS.hlsl", device, m_particleRainOneGS.GetAddressOf(), "rain");
	m_geometryShaders.insert({ g_particleRainOneGS,m_particleRainOneGS });
	compileGS("Shaders/ParticleGS.hlsl", device, m_particleRainTwoGS.GetAddressOf(), "rain2");
	m_geometryShaders.insert({ g_particleRainTwoGS,m_particleRainTwoGS });
	compileGSSO("Shaders/ParticleGS.hlsl", device, m_particleLeafOneGS.GetAddressOf(), "leaf");
	m_geometryShaders.insert({ g_particleLeafOneGS,m_particleLeafOneGS });
	compileGS("Shaders/ParticleGS.hlsl", device, m_particleLeafTwoGS.GetAddressOf(), "leaf2");
	m_geometryShaders.insert({ g_particleLeafTwoGS,m_particleLeafTwoGS });
	compileGS("Shaders/DepthGS.hlsl", device, m_depthTerrainGS.GetAddressOf(), "main3");
	m_geometryShaders.insert({ g_terrainDepthGS,m_depthTerrainGS });
	compileGS("Shaders/GrassGS.hlsl", device, m_grassGS.GetAddressOf());
	m_geometryShaders.insert({ g_grassGS,m_grassGS });
	//PS
	compilePS("Shaders/FullScreenQuadPS.hlsl", device, m_fullScreenQuadPS.GetAddressOf());
	m_pixelShaders.insert({ g_fullQuadPS, m_fullScreenQuadPS });
	compilePS("Shaders/FullScreenQuadPS.hlsl", device, m_addFramesPS.GetAddressOf(), "add");
	m_pixelShaders.insert({ g_addFramesPS, m_addFramesPS });
	compilePS("Shaders/FullScreenQuadPS.hlsl", device, m_fullScreenQuadHeatMapPS.GetAddressOf(), "debugLightCulling");
	m_pixelShaders.insert({ g_heatMap, m_fullScreenQuadHeatMapPS });
	compilePS("Shaders/PS.hlsl", device, m_basicPS.GetAddressOf());
	m_pixelShaders.insert({ g_PS, m_basicPS });
	compilePS("Shaders/PS.hlsl", device, m_edgePS.GetAddressOf(), "mainEdge");
	m_pixelShaders.insert({ g_celEdgePS, m_edgePS });
	compilePS("Shaders/ParticlePS.hlsl", device, m_particlePS.GetAddressOf());
	m_pixelShaders.insert({ g_particlePS, m_particlePS });
	compilePS("Shaders/SkyPS.hlsl", device, m_skyPS.GetAddressOf());
	m_pixelShaders.insert({ g_skyPS, m_skyPS });
	compilePS("Shaders/DynamicSkyPS.hlsl", device, m_dynamicSkyPS.GetAddressOf());
	m_pixelShaders.insert({ g_dynamicSkyPS, m_dynamicSkyPS });
	compilePS("Shaders/TerrainPS.hlsl", device, m_terrainPS.GetAddressOf());
	m_pixelShaders.insert({ g_terrainPS, m_terrainPS});
	compilePS("Shaders/GrassPS.hlsl", device, m_grassPS.GetAddressOf());
	m_pixelShaders.insert({ g_grassPS, m_grassPS });
	compilePS("Shaders/FullScreenQuadPS.hlsl", device, m_rayMarchPS.GetAddressOf(), "rayMarch");
	m_pixelShaders.insert({ g_rayMarchPS, m_rayMarchPS });
	compilePS("Shaders/FullScreenQuadPS.hlsl", device, m_bilateralHorPS.GetAddressOf(), "bilateralBlurHor");
	m_pixelShaders.insert({ g_bilateralHorPS, m_bilateralHorPS });
	compilePS("Shaders/FullScreenQuadPS.hlsl", device, m_bilateralVerPS.GetAddressOf(), "bilateralBlurVer");
	m_pixelShaders.insert({ g_bilateralVerPS, m_bilateralVerPS });
	compilePS("Shaders/FXAAPS.hlsl", device, m_fxaaPS.GetAddressOf());
	m_pixelShaders.insert({ g_fxaa, m_fxaaPS });
	//CS
	
	compileCS("Shaders/GaussianCS.hlsl", device, m_gaussianBlurHorCS.GetAddressOf(),"HorzBlurCS");
	m_computeShaders.insert({ g_computeHorCS, m_gaussianBlurHorCS });
	compileCS("Shaders/GaussianCS.hlsl", device, m_gaussianBlurVerCS.GetAddressOf(),"VertBlurCS");
	m_computeShaders.insert({ g_computeVerCS, m_gaussianBlurVerCS });
	compileCS("Shaders/ForwardPlusCS.hlsl", device, m_lightCS.GetAddressOf());
	m_computeShaders.insert({ g_lightCS, m_lightCS });
}

void ShaderHandler::bindNull(ID3D11DeviceContext* context)
{
	context->VSSetShader(m_vs, nullptr, 0);
	context->HSSetShader(m_hs, nullptr, 0);
	context->DSSetShader(m_ds, nullptr, 0);
	context->GSSetShader(m_gs, nullptr, 0);
	context->CSSetShader(m_cs, nullptr, 0);
	context->PSSetShader(m_ps, nullptr, 0);
}
