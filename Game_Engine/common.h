#pragma once
#ifndef COMMON_H
#define COMMON_H
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#ifndef UNICODE
#define UNICODE
#endif
#include <Windows.h>
#include <wrl\client.h>
#include <stdio.h>  
#include <d3d11.h>
#include <assert.h> 
#include <dxgi.h>
#include <d3dcompiler.h>
#include <windowsx.h>
#include <DirectXMath.h>
#include <map>
#include <unordered_map>
#include <WICTextureLoader.h>
#include <chrono>
#include <wincodec.h>
#include <DDSTextureLoader.h>
#include <SimpleMath.h>
#include <queue>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <crtdbg.h>
#include <memory>
#include <XFileFormatLibr.h>
#include <XFileFormat.h>
#include <fstream>
#include <dxgi1_6.h>
#include <psapi.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <Audio.h>
#include <Keyboard.h>
#include <Mouse.h>

#pragma comment( lib, "user32" )     
#pragma comment( lib, "gdi32" )         
#pragma comment( lib, "d3d11" )        
#pragma comment( lib, "d3dcompiler.lib" )
#pragma comment(lib, "dxgi")
#define USE_IMGUI TRUE
#if USE_IMGUI
#include "Imgui\imgui.h"
#include "Imgui\imgui_impl_win32.h"
#include "Imgui\imgui_impl_dx11.h"
#endif

using namespace DirectX;
using namespace SimpleMath;
using namespace Microsoft;

#define SATURATE(x)  (((x) > (1.0f)) ? (1.0f) : (((x) < (0.0f)) ? (0.0f) : (x)))
#define RELEASE(ptr) if(ptr) { delete ptr; ptr = nullptr; }
#define RELEASE_ARR(ptrA) if(ptrA) { delete[] ptrA; ptrA = nullptr; }
#define RELEASE_COM(cptr) if(cptr) { cptr->Release(); cptr = nullptr; }
#define RELEASE_U_PTR(ptr) if(ptr.get()){ptr.release();}
#define _CRTDBG_MAP_ALLOC

#define ASSERT_SHADER(RESULT, eBLOB, wTEXT) \
		if (FAILED(RESULT)) { \
			MessageBox(NULL, wTEXT.c_str(), L"Error compiling shaders. Check output for more information.", MB_OK); \
			if(eBLOB){		\
				OutputDebugStringA((char*)eBLOB->GetBufferPointer()); \
				eBLOB->Release(); \
			}} \

//Window Constants
const size_t G_HIGH_RES_X = 1920;
const size_t G_HIGH_RES_Y = 1080;

const size_t G_MEDIUM_RES_X = 1280;
const size_t G_MEDIUM_RES_Y = 720;

const size_t G_LOW_RES_X = 640;
const size_t G_LOW_RES_Y = 480;

//Shader constants

//Boolean Macros
#define POINT_SHADOWS FALSE
#define START_RESOLUTION_HIGH TRUE
#define START_RESOLUTION_MEDIUM FALSE
#define START_RESOLUTION_LOW FALSE
#define START_FULLSCREEN FALSE
#define LIGHT_CULLING_BLOCK_SIZE 16
#define DEBUG_LIGHT_GRID FALSE
#define DEBUG_HEAT_MAP TRUE
#define FIRST_PERSON_CAM FALSE
#define CEL_SHADING TRUE
//enums
enum class ANIMATION_TYPE
{
	SKELETON,
	MORPH,
	BOTH
};
enum class LIGHT_TYPE
{
	POINT,
	SPOT,
	DIRECTIONAL
};
enum class PLAYER_TYPE
{
	FIRST_PERSON,
	THIRD_PERSON
};
enum class SCENE
{
	MENU,
	ANIMATION,
	PARTICLE,
	LOD,
	HEAT,
	MAIN_TEST,
	DEBUG_LIGHT_CULL
};
enum class MENU_OPTIONS
{
	SCENE_CEL,
	SCENE_ANIMATION,
	SCENE_LOD,
	SCENE_HEAT,
	QUIT,
	HIGH,
	MEDIUM,
	LOW,
	NONE
};

enum class SOUND_EFFECT
{
	GAME_WIN,
	GAME_PROGRESS,
	GAME_LOSE,
	MENU_SELECTION
};
enum class TRANSFORM_TYPE
{
	STATIC,
	DYNAMIC
};
enum class DC_TYPE 
{
	BASIC,
	MORPH,
	SKELETON,
	SKELETON_AND_MORPH,
	FULLSCREEN_QUAD,
	COMPUTE_SHADER,
	PARTICLE,
	GRASS
};
enum class RENDERPASS //Must be in order.
{
	CALC_FRUSTUM_PASS = 0,
	DEPTH_LIGHT_PASS = 1 << 0,
	DEPTH_SHADOW_DIR = 1 << 1,
	DEPTH_SHADOW_POINT = 1 << 2,
	LIGHT_CULL_PASS = 1 << 3,
	DYNAMIC_CUBE_MAP_PASS = 1 << 4,
	EDGE_DETECTION_PASS = 1 << 5,
	BASIC_PASS_SINGLE = 1 << 6,
	BASIC_PASS_TRANSPARENT = 1 << 7,
	DOWN_SAMPLE_EMISSIVE_PASS = 1 << 8,
	DOWN_SAMPLE_RAY_MARCH_PASS = 1 << 9,
	GAUSSIAN_HOR = 1 << 10,
	GAUSSIAN_VER = 1 << 11,
	RAY_MARCH = 1 << 12,
	BILATERA_HOR = 1 << 13,
	BILATERA_VER = 1 << 14,
	UP_SAMPLE_EMISSIVE_PASS = 1 << 15,
	UP_SAMPLE_RAY_MARCH_PASS = 1 << 16,
	FXAA = 1 << 17,
	BASIC_PASS_QUAD = 1 << 18 //for stuff that is rendered onto a quad in the end.
	
	
};
DEFINE_ENUM_FLAG_OPERATORS(RENDERPASS);

enum class RENDER_TYPE 
{
	BASIC,
	PARTICLE_FIRE_TYPE, //can be used for smoke 2.
	PARTICLE_RAIN_TYPE,
	PARTICLE_TEXT_TYPE,
	PARTICLE_LEAVES_TYPE,
	PLAYER_CHARACTER,
	TERRAIN,
	CLOTH,
	SKY,
	DYNAMIC_SKY,
	PORTAL,
	OC_TREE,
	QUAD_TREE,
	BOUNDING_BOX,
	HEAT_MAP
};
enum class SHADER_BIND_FLAG
{
	NONE = 0,
	VERTEX_SHADER = 1 << 0,
	PIXEL_SHADER = 1 << 1,
	GEOMETRY_SHADER = 1 << 2,
	HULL_SHADER = 1 << 3,
	DOMAIN_SHADER = 1 << 4,
	COMPUTE_SHADER = 1 << 5
};
DEFINE_ENUM_FLAG_OPERATORS(SHADER_BIND_FLAG);
enum class STRUCTUED_BFR
{
	NONE,
	MORPH0,
	MORPH1,
	MORPH2,
	SKELETON,
	LIGHT_FRUSTUM,
	GRASS,
	GRASS_INDICES,
	LIGHT,
	LIGHT_LIST_O,
	LIGHT_C_O,
	
};
enum class SAMPLER_TYPE
{
	WRAP,
	CLAMP,
	COMPARISON,
	ANISOTROPIC,
	TERRAIN
};
enum class CAMERA_TYPE
{
	PERSPECTIVE,
	ORTOGRAPHIC,
};

enum class LIGHT_SHADOW_TYPE
{
	NO_SHADOW = 0,
	HAS_SHADOW = 1
};
enum class BB_CULL_TYPE
{
	BB_CULL_AABB,
	BB_CULL_OBB,
	BB_CULL_SPHERE

};

enum class PARTICLE_TYPE
{
	PT_EMITTER = 0,
	PT_FLARE = 1

};
enum class CLASSIFICATION
{
	COINCIDENT,
	IN_BACK_OF,
	IN_FRONT_OF,
	SPANNING
};
enum class PLAYER_SKELETON
{
	foot_r,
	calf_r
	,neck_01
	,clavicle_l
	,thigh_l
	,thigh_r
	,pelvis
	,spine_02
	,spine_03
	,spine_01
	,lowerarm_l
	,hand_l
	,upperarm_l
	,upperarm_r
	,lowerarm_r
	,clavicle_r
	,hand_r
	,foot_l
	,calf_l
	,neck_02,
	pinky_01_l
	,pinky_02_l
	,pinky_03_l
	,ring_01_l
	,ring_02_l
	,ring_03_l
	,middle_01_l
	,middle_02_l
	,middle_03_l
	,index_01_l
	,index_02_l
	,index_03_l
	,thumb_01_l
	,thumb_02_l
	, thumb_03_l
	,pinky_01_r
	,pinky_02_r
	,pinky_03_r
	,ring_01_r
	,ring_02_r
	,ring_03_r
	, middle_02_r
	,middle_03_r
	,middle_01_r
	, index_01_r
	,index_02_r
	,index_03_r
	,thumb_01_r
	,thumb_02_r
	,thumb_03_r
	,ball_r
	,ball_l




	
};
//structs
struct TRANSFORM_VALUES_LOCAL
{
	Vector3 scale;
	Quaternion rotQ;
	Vector3 rotE;
	Vector3 trans;
};
struct TRANSFORM_VALUES_GLOBAL
{
	Vector3 scale;
	Quaternion rotQ;
	Vector3 trans;
};
struct VERTEX
{
	XMFLOAT3 pos = { 0,0,0 };
	XMFLOAT2 uv = { 0,0 };
	XMFLOAT3 normal = { 0,0,0 };
	XMFLOAT3 tangent = { 0,0,0 };
	XMFLOAT3 biNormal = { 0,0,0 };
	XMUINT4 jointNr = { 0,0,0,0 };
	XMFLOAT4 skinWeight = { 0.0f, 0.0f,0.0f, 0.0f };
	bool operator==(const VERTEX& other)
	{
		if (pos.x == other.pos.x && pos.y == other.pos.y && pos.z == other.pos.z
			&& uv.x == other.uv.x && uv.y == other.uv.y
			&& normal.x == other.normal.x && normal.y == other.normal.y && normal.z == other.normal.z
			&& tangent.x == other.tangent.x && tangent.y == other.tangent.y && tangent.z == other.tangent.z
			&& biNormal.x == other.biNormal.x && biNormal.y == other.biNormal.y && biNormal.z == other.biNormal.z
			&& jointNr.x == other.jointNr.x && jointNr.y == other.jointNr.y && jointNr.z == other.jointNr.z && jointNr.w == other.jointNr.w
			&& skinWeight.x == other.skinWeight.x && skinWeight.y == other.skinWeight.y && skinWeight.z == other.skinWeight.z && skinWeight.w == other.skinWeight.w)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

struct TERRAIN_PATCH
{
	XMFLOAT3 pos = { 0,0,0 };
	XMFLOAT2 uv = { 0,0 };
	XMFLOAT2 boundsY = { 0,0 };
	
};
struct MORPH_VERTEX
{
	XMFLOAT3 pos = { 0,0,0 };
	XMFLOAT3 normal = { 0,0,0 };
	XMFLOAT3 tangent = { 0,0,0 };
	XMFLOAT3 biNormal = { 0,0,0 };
};

struct INSTANCE_DATA
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 inverseWorld;
};

struct PARTICLE
{
	XMFLOAT3 InitialPos;
	XMFLOAT3 InitialVel;
	XMFLOAT2 Size;
	float Age;
	UINT Type;
};
struct LIGHT_DATA
{
	XMFLOAT4 position;
	XMFLOAT4 color;
	XMUINT4 type;//x is the type,
	XMFLOAT4 direction;
	XMFLOAT4 otherConstants;//x is intensity, y is range z is fov w is if it has shadow
};

struct GRASS_STRAND
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT3 biNormal;
	bool operator==(const GRASS_STRAND& other)
	{
		if (pos.x == other.pos.x && pos.y == other.pos.y && pos.z == other.pos.z
			&& uv.x == other.uv.x && uv.y == other.uv.y
			&& normal.x == other.normal.x && normal.y == other.normal.y && normal.z == other.normal.z
			&& tangent.x == other.tangent.x && tangent.y == other.tangent.y && tangent.z == other.tangent.z
			&& biNormal.x == other.biNormal.x && biNormal.y == other.biNormal.y && biNormal.z == other.biNormal.z
			)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

struct OBB
{
	Vector3 min, max;
	float extendsX = 0.0f;
	float extendsY = 0.0f;
	float extendsZ = 0.0f;
	Vector3 center;
	Vector3 uX, uY, uZ;
};
struct AABB
{
	Vector3 min, max, center;
	float diagonal = 0.0f;
};
struct BOUNDING_SPHERE
{
	float radius = 0.0f;
	Vector3 center;
};
struct Bounds
{
	OBB obb;
	AABB aabb;
	BOUNDING_SPHERE boundingSphere;
};

struct PLANE
{
	XMFLOAT3 N;
	float d;
	Vector3 p;
};
struct FRUSTUM
{
	PLANE plane[4];
};
struct BONE
{
	std::string name;
	std::string parentName;
	std::vector<std::string> childrenNames;
	int nr;
	short parentNr;
	std::vector<int> childrenNrs;
	bool leaf = false;
	bool root = false;
};
struct BONE_KEY
{
	Vector3 s;
	Quaternion r;
	Vector3 t;
};
//Constant Buffer Structs
struct CB_OBJECT
{
	XMFLOAT4X4 g_world;
	XMFLOAT4X4 g_inverseWorld;
	XMFLOAT4 g_particleWP;
};
struct CB_PARTICLE
{
	XMFLOAT4 g_particleConstants; //x,y, width height,
};
struct CB_GRASS
{
	XMFLOAT4 g_grassConstants; //x height, y width,z heightmap
};
struct CB_SCENE
{
	XMFLOAT4X4 g_view;
	XMFLOAT4X4 g_projection;
	XMFLOAT4X4 g_inverseView;
	XMFLOAT4X4 g_inverseProjection;
	XMFLOAT4X4 g_vp;
	XMFLOAT4X4 g_lvpt;
	XMFLOAT4 g_time;//y is timestep
	XMUINT4 g_lightCount;//x is only used
	XMFLOAT4 g_dirLightColor;
	XMFLOAT4 g_dirLightOtherConstants;//x is intensity, y is range z is fov w is if it has shadow
	XMFLOAT4 g_dirLightDirection;
	XMFLOAT4 g_cameraPosition;
	XMUINT4 g_screenDimenstions;//x and y are width and height, z and w is texel size
	XMFLOAT4 g_frustumPlanes[6];
	XMFLOAT4X4 g_shadowVP;
	XMFLOAT4 g_booleans; //x is cel shading 1 for true and 0 for false
};
struct CB_SHADOW_POINT
{
	XMFLOAT4X4 g_vpPL[6];

};
//Animation Types
static const std::string G_S_RUN = "G_S_RUN";
static const std::string G_S_WALK = "G_S_WALK";
static const std::string G_S_ATTACK = "G_S_ATTACK";
static const std::string G_S_IDLE = "G_S_IDLE";
static const std::string G_S_RUN_G_S_IDLE_COMBINED = "G_S_RUN_G_S_IDLE_COMBINED";
static const std::string G_USER_SELECTED_COMBINED = "G_USER_SELECTED_COMBINED";


constexpr UINT CELLS_PER_PATCH_HM = 64;
constexpr float CELL_SPACING = 0.5f;
constexpr UINT HMW = 2048;
constexpr UINT HMH = 2048;

constexpr UINT BSP_MAX_DEPTH = 4;
constexpr UINT MAX_LIGHTS = 100;
constexpr UINT MAX_BONES = 60;
constexpr UINT MAX_TRIANGLES_QUAD = 500;
constexpr UINT MAX_TRIANGLES_OC = 2000;
constexpr float CAMERA_NEAR_Z = 0.01f;
constexpr float CAMERA_FAR_Z = 800.0f;

constexpr float SHADOW_MAP_SIZE_X = 1024.0f;
constexpr float SHADOW_MAP_SIZE_Y = 1024.0f;

constexpr float DYNAMIC_CUBE_MAP_RES = 256.0f;

constexpr UINT CB_OBJECT_SLOT = 0;
constexpr UINT CB_SCENE_SLOT = 1;
constexpr UINT CB_GRASS_SLOT = 3;
constexpr UINT CB_MORPH_SLOT = 4;
constexpr UINT CB_SHADOW_POINT_SLOT = 5;
constexpr UINT CB_WATER_SLOT = 6;
constexpr UINT CB_PARTICLE_SLOT = 7;
//SRV SLOTS
constexpr UINT SRV_ALBEDO_SLOT = 0;
constexpr UINT SRV_NORMAL_SLOT = 1;
constexpr UINT SRV_ROUGH_SLOT = 2;
constexpr UINT SRV_METAL_SLOT = 3;
constexpr UINT SRV_AO_SLOT = 4;
constexpr UINT SRV_EMISSIVE_SLOT = 5;
constexpr UINT SRV_LIGHT_SLOT = 7;
constexpr UINT SRV_SKELETON_SLOT = 28;
constexpr UINT SRV_LIGHT_INDEX = 29;
constexpr UINT SRV_LIGHT_GRID = 30;
constexpr UINT SRV_TEXTURE_CUBE = 10;
constexpr UINT SRV_SHADOW_DIR_SLOT = 11;
constexpr UINT SRV_MORPH_ONE_SLOT = 12;
constexpr UINT SRV_MORPH_TWO_SLOT = 13;
constexpr UINT SRV_MORPH_THREE_SLOT = 14;
constexpr UINT SRV_NOISE_SLOT = 15;
constexpr UINT SRV_SHADOW_POINT_SLOT = 16;
constexpr UINT SRV_RANDOM_1D = 17;
constexpr UINT SRV_HEAT = 18;
constexpr UINT SRV_LIGHT_DEPTH = 0;
constexpr UINT SRV_N_VIEW = 20;
constexpr UINT SRV_WORLD_POS = 21;
constexpr UINT SRV_MORPH_INDICES = 21;
constexpr UINT SRV_LOADED_HEAT = 6;
constexpr UINT SRV_DYNAMIC_CUBE = 23;
constexpr UINT SRV_TERRAIN_MAP = 24;
constexpr UINT SRV_BLEND_MAP = 25;
constexpr UINT SRV_TERRAIN_LAYERS_MAP = 26;
constexpr UINT SRV_GAUSSIAN_HOR = 1;
constexpr UINT SRV_EMISSIVE_QUAD = 33;
constexpr UINT SRV_FRAME_QUAD = 34;
constexpr UINT SRV_GAUSSIAN_VER = 2;
constexpr UINT SRV_DOWN_SAMPLE = 5;
constexpr UINT SRV_UP_SAMPLE = 6;
constexpr UINT SRV_ALBEDO_QUAD = 10;
constexpr UINT SRV_VOL_LIGHT_QUAD = 12;
constexpr UINT SRV_BILATERAL_BLUR_QUAD = 13;
constexpr UINT SRV_GRASS_STRAND = 27;
constexpr UINT SRV_GRASS_INDEX = 32;
constexpr UINT SRV_GRASS_BC = 31;
//UAV SLOTS
constexpr UINT UAV_LIGHT_COUNTER = 0;
constexpr UINT UAV_LIGHT_INDEX = 1;
constexpr UINT UAV_LIGHT_GRID = 2;
constexpr UINT UAV_LIGHT_HEAT = 3;
constexpr UINT UAV_GAUSSIAN_HOR = 4;
constexpr UINT UAV_GAUSSIAN_VER = 5;
//SAMPLER SLOTS
constexpr UINT WRAP_SAMPLER = 0;
constexpr UINT COMPARISON_SAMPLER = 1;
constexpr UINT CLAMP_SAMPLER = 2;
constexpr UINT A_SAMPLER = 3;
constexpr UINT TERRAIN_SAMPLER = 4;
//Textures
/*
* green
* grey
* black
* white
* blue
* random1D
* frustum
* indexCO
* indexCT
* lightGridO
* lightGridT
* light
* indexListO
* indexListT
*/

#endif