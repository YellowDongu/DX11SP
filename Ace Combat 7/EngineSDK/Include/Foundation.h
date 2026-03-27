#pragma once

#pragma warning (disable : 4251)

#define useAssimp

#ifdef useAssimp
#define NOMINMAX
#endif

#pragma region ThirdPartyLibrary
#pragma region d3dx11
#include "d3d11.h"
#pragma comment(lib, "d3d11.lib")
#include "DirectXMath.h"
#include "DirectXCollision.h"
#pragma endregion

#pragma region fmod
#include "fmod.h"
#include "fmod.hpp"
#pragma comment (lib, "fmodL_vc.lib")
#pragma endregion


#ifdef useAssimp
#pragma region assimp
#ifdef _DEBUG
//#pragma comment (lib, "assimp-vc140-mtd.lib")
#pragma comment (lib, "assimp-vc143-mtd.lib")
#elif
#pragma comment (lib, "assimp-vc143-mt.lib")
#endif // _DEBUG
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#pragma endregion
#endif

#pragma region DirectXTK
#ifdef _DEBUG
#pragma comment (lib, "DirectXTKd.lib")
#elif
#pragma comment (lib, "DirectXTK.lib")
#endif // _DEBUG
#include "DirectXTK/WICTextureLoader.h"
#include "DirectXTK/DDSTextureLoader.h"
#include "DirectXTK/GeometricPrimitive.h"
#include "DirectXTK/SpriteFont.h"
#include "DirectXTK/SpriteBatch.h"
#include "DirectXTK/PrimitiveBatch.h"
#include "DirectXTK/VertexTypes.h"
#include "DirectXTK/Effects.h"
#include "DirectXTK/CommonStates.h"
//#include "DebugDraw.h"
#pragma endregion

#pragma region FX11
#ifdef _DEBUG
#pragma comment(lib, "Effects11d.lib")
#elif
#pragma comment (lib, "Effects11.lib")
#endif // _DEBUG
#include <Effects11/d3dx11effect.h>
#pragma endregion

#pragma region ShaderCompiler
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
#pragma endregion

#pragma endregion

#include <random>
#include <codecvt>
#include <locale>
#include <fstream>
#include <functional>
#include <string>
#include <vector>
#include <list>
#include <thread>
#include <memory>
#include <map>
#include <unordered_map>
#include <cwctype> /*std::iswalpha()*/ 
#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <wrl/client.h>
namespace wrl = Microsoft::WRL;

#ifdef ENGINE_EXPORT
#define ENGINEDLL __declspec(dllexport)
#else
#define ENGINEDLL __declspec(dllimport)
#endif

// 메모리 누수 검사
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#include "DebugDraw.h"
#endif
#include "Enums.h"
#include "EngineTypename.h"
#include "Structs.h"
#include "GlobalArea.h"

#ifdef useAssimp
#include "ModelConverter.h"
#endif