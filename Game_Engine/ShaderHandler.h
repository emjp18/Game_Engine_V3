#pragma once
#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H
#include "common.h"
class ShaderHandler
{
private:
	WRL::ComPtr<ID3D11InputLayout> m_IL;
	WRL::ComPtr<ID3D11InputLayout> m_instanceIL;
	WRL::ComPtr<ID3D11InputLayout> m_particleIL;
	WRL::ComPtr<ID3D11InputLayout> m_terrainIL;
	//VS
	WRL::ComPtr<ID3D11VertexShader> m_basicVS;
	WRL::ComPtr<ID3D11VertexShader> m_basicInstanceVS;
	WRL::ComPtr<ID3D11VertexShader> m_quadVS0;
	WRL::ComPtr<ID3D11VertexShader> m_quadVS1;
	WRL::ComPtr<ID3D11VertexShader> m_skeletonVS;
	WRL::ComPtr<ID3D11VertexShader> m_particleMainVS;
	WRL::ComPtr<ID3D11VertexShader> m_particleFireVS;
	WRL::ComPtr<ID3D11VertexShader> m_particleRainVS;
	WRL::ComPtr<ID3D11VertexShader> m_skyVS;
	WRL::ComPtr<ID3D11VertexShader> m_terrainVS;
	WRL::ComPtr<ID3D11VertexShader> m_grassVS;
	WRL::ComPtr<ID3D11VertexShader> m_celBasicVS;
	WRL::ComPtr<ID3D11VertexShader> m_celInstanceVS;
	WRL::ComPtr<ID3D11VertexShader> m_celSkeletonVS;
	//HS
	WRL::ComPtr<ID3D11HullShader> m_terrainHS;
	WRL::ComPtr<ID3D11HullShader> m_grassHS;
	//DS 
	WRL::ComPtr<ID3D11DomainShader> m_terrainDS;
	WRL::ComPtr<ID3D11DomainShader> m_grassDS;
	//GS
	WRL::ComPtr<ID3D11GeometryShader> m_depthSkyGS;
	WRL::ComPtr<ID3D11GeometryShader> m_depthGS;
	WRL::ComPtr<ID3D11GeometryShader> m_depthTerrainGS;
	WRL::ComPtr<ID3D11GeometryShader> m_particleFireOneGS;
	WRL::ComPtr<ID3D11GeometryShader> m_particleFireTwoGS;
	WRL::ComPtr<ID3D11GeometryShader> m_particleRainOneGS;
	WRL::ComPtr<ID3D11GeometryShader> m_particleRainTwoGS;
	WRL::ComPtr<ID3D11GeometryShader> m_particleTextGS;
	WRL::ComPtr<ID3D11GeometryShader> m_particleLeafOneGS;
	WRL::ComPtr<ID3D11GeometryShader> m_particleLeafTwoGS;
	WRL::ComPtr<ID3D11GeometryShader> m_grassGS;
	//PS
	WRL::ComPtr<ID3D11PixelShader> m_basicPS;
	WRL::ComPtr<ID3D11PixelShader> m_fullScreenQuadPS;
	WRL::ComPtr<ID3D11PixelShader> m_fullScreenQuadHeatMapPS;
	WRL::ComPtr<ID3D11PixelShader> m_particlePS;
	WRL::ComPtr<ID3D11PixelShader> m_skyPS;
	WRL::ComPtr<ID3D11PixelShader> m_dynamicSkyPS;
	WRL::ComPtr<ID3D11PixelShader> m_terrainPS;
	WRL::ComPtr<ID3D11PixelShader> m_grassPS;
	WRL::ComPtr<ID3D11PixelShader> m_addFramesPS;
	WRL::ComPtr<ID3D11PixelShader> m_rayMarchPS;
	WRL::ComPtr<ID3D11PixelShader> m_bilateralHorPS;
	WRL::ComPtr<ID3D11PixelShader> m_bilateralVerPS;
	WRL::ComPtr<ID3D11PixelShader> m_edgePS;
	WRL::ComPtr<ID3D11PixelShader> m_fxaaPS;
	//CS
	WRL::ComPtr<ID3D11ComputeShader> m_lightCS;
	WRL::ComPtr<ID3D11ComputeShader> m_gaussianBlurHorCS;
	WRL::ComPtr<ID3D11ComputeShader> m_gaussianBlurVerCS;

	std::unordered_map < std::string, WRL::ComPtr< ID3D11VertexShader>> m_vertexShaders;
	std::unordered_map < std::string, WRL::ComPtr< ID3D11HullShader>> m_hullShaders;
	std::unordered_map < std::string, WRL::ComPtr< ID3D11DomainShader>> m_domainShaders;
	std::unordered_map < std::string, WRL::ComPtr< ID3D11GeometryShader>> m_geometryShaders;
	std::unordered_map < std::string, WRL::ComPtr< ID3D11ComputeShader>> m_computeShaders;
	std::unordered_map < std::string, WRL::ComPtr< ID3D11PixelShader>> m_pixelShaders;


	ID3DBlob* m_errorBlob = nullptr;
	ID3D11VertexShader* m_vs = nullptr;
	ID3D11HullShader* m_hs = nullptr;
	ID3D11DomainShader* m_ds = nullptr;
	ID3D11GeometryShader* m_gs = nullptr;
	ID3D11ComputeShader* m_cs = nullptr;
	ID3D11PixelShader* m_ps = nullptr;
	DWORD m_shaderCompilationFlag;

	void compileVSAndIL(std::string path, ID3D11Device* device, ID3D11VertexShader** shader, D3D11_INPUT_ELEMENT_DESC* desc, UINT size, ID3D11InputLayout** layout,
		std::string main = "main");
	void compileVS(std::string path, ID3D11Device* device, ID3D11VertexShader** shader, std::string main = "main");
	void compileHS(std::string path, ID3D11Device* device, ID3D11HullShader** shader, std::string main = "main");
	void compileDS(std::string path, ID3D11Device* device, ID3D11DomainShader** shader, std::string main = "main");
	void compileGS(std::string path, ID3D11Device* device, ID3D11GeometryShader** shader, std::string main = "main");
	void compileGSSO(std::string path, ID3D11Device* device, ID3D11GeometryShader** shader, std::string main = "main");
	void compileCS(std::string path, ID3D11Device* device, ID3D11ComputeShader** shader, std::string main = "main");
	void compilePS(std::string path, ID3D11Device* device, ID3D11PixelShader** shader, std::string main = "main");

	

public:
	ShaderHandler();
	~ShaderHandler();
	static ShaderHandler& getInstance() { static ShaderHandler instance; return instance; }
	ShaderHandler(const ShaderHandler&) = delete;
	ShaderHandler& operator=(const ShaderHandler&) = delete;
	ShaderHandler(ShaderHandler&&) = delete;
	ShaderHandler& operator=(ShaderHandler&&) = delete;
	void init(ID3D11Device* device);
	WRL::ComPtr< ID3D11VertexShader> getVS(std::string key) { return m_vertexShaders[key]; }
	WRL::ComPtr < ID3D11HullShader> getHS(std::string key) { return m_hullShaders[key]; }
	WRL::ComPtr< ID3D11DomainShader> getDS(std::string key) { return m_domainShaders[key]; }
	WRL::ComPtr< ID3D11GeometryShader> getGS(std::string key) { return m_geometryShaders[key]; }
	WRL::ComPtr< ID3D11ComputeShader> getCS(std::string key) { return m_computeShaders[key]; }
	WRL::ComPtr< ID3D11PixelShader> getPS(std::string key) { return m_pixelShaders[key]; }
	void bindNull(ID3D11DeviceContext*);
	ID3D11InputLayout* getDefaultInputLayout() { return m_IL.Get(); }
	ID3D11InputLayout* getDefaultInstanceInputLayout() { return m_instanceIL.Get(); }
	ID3D11InputLayout* getParticleInputLayout() { return m_particleIL.Get(); }
	ID3D11InputLayout* getTerrainInputLayout() { return m_terrainIL.Get(); }

	std::string g_basicVS = "VS";
	std::string g_basicInstanceVS = "instanceVS";
	std::string g_autoQuadVS = "autoQuad";
	std::string g_basicQuadVS = "basicQuad";
	std::string g_fullQuadPS = "quadPS";
	std::string g_PS = "PS";
	std::string g_lightCS = "lightCS";
	std::string g_heatMap = "heatMap";
	std::string g_skeleton = "skeleton";
	std::string g_depthGS = "depthGS";
	std::string g_particleFireVS = "fireVS";
	std::string g_particleMainVS = "pMainVS";
	std::string g_particleRainVS = "rainVS";
	std::string g_particleFireOneGS = "fireOneGS";
	std::string g_particleTextGS = "textGS";
	std::string g_particleFireTwoGS = "fireTwoGS";
	std::string g_particleRainOneGS = "rainOneGS";
	std::string g_particleRainTwoGS = "rainTwoGS";
	std::string g_particlePS = "particlePS";
	std::string g_particleLeafOneGS = "particleLeafOneGS";
	std::string g_particleLeafTwoGS = "particleLeafTwoGS";
	std::string g_skyVS = "skyVS";
	std::string g_skyPS = "skyPS";
	std::string g_dynamicSkyPS = "dynamicSkyPS";
	std::string g_depthSkyGS = "dynamicSkyDepthGS";
	std::string g_terrainVS = "terrainVS";
	std::string g_terrainHS = "terrainHS";
	std::string g_terrainDS = "terrainDS";
	std::string g_terrainPS = "terrainPS";
	std::string g_terrainDepthGS = "terrainDepthGS";
	std::string g_grassVS = "grassvS";
	std::string g_grassPS = "grasssPS";
	std::string g_grassHS = "grassHS";
	std::string g_grassDS = "grassDS";
	std::string g_grassGS = "grassGS";
	std::string g_computeVerCS = "BlurVerCS";
	std::string g_computeHorCS = "BlurHorCS";
	std::string g_addFramesPS = "addPS";
	std::string g_rayMarchPS = "rayMarchPS";
	std::string g_bilateralHorPS = "bilateralHorPS";
	std::string g_bilateralVerPS = "bilateralVerPS";
	std::string g_celBasicVS = "celBasicVS";
	std::string g_celInstanceVS = "celInstanceVS";
	std::string g_celSkeletonVS = "celSkeletonVS";
	std::string g_celEdgePS = "celEdgePs";
	std::string g_fxaa = "fxaa";
};

#endif