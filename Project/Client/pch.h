#pragma once

// Engine
#ifdef _DEBUG
#pragma comment(lib, "Engine\\Engine_D")
#else
#pragma comment(lib, "Engine\\Engine")
#endif

#include <Engine/global.h>
#include <Engine/CEngine.h>


// Scripts
#ifdef _DEBUG
#pragma comment(lib, "Scripts\\Scripts_D")
#else
#pragma comment(lib, "Scripts\\Scripts")
#endif


#include <set>
using std::set;