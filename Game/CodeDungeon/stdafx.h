#pragma once

#pragma comment(lib, "Engine.lib")
#include "EnginePch.h"

#include <format>
#define _CRTDEBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
#endif
