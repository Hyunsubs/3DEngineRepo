#pragma once


#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <wrl.h>
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// DirectxTex
#include <DirectxTex/DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectxTex//DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectxTex//DirectXTex.lib")
#endif


#include "SimpleMath.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;


#include <string>
using std::string;
using std::wstring;

#include <vector>
using std::vector;

#include <list>
using std::list;

#include <map>
using std::map;
using std::make_pair;

#include <typeinfo>

#include <filesystem>
using namespace std::filesystem;


// FMOD
#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>

#ifdef _DEBUG
#pragma comment(lib, "FMOD/fmodL64_vc.lib")
#else
#pragma comment(lib, "FMOD/fmod64_vc.lib")
#endif



#include "singleton.h"
#include "define.h"
#include "enum.h"
#include "struct.h"
#include "func.h"
#include "Ptr.h"