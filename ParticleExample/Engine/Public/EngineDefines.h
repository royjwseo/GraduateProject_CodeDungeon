#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UENGINEDEFINES_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UENGINEDEFINES_H

#pragma warning (disable : 4251)

#pragma region DX12

#pragma comment (lib, "d3d12")
#pragma comment (lib, "dxgi")
#pragma comment (lib, "dxguid")
#pragma comment (lib, "d3dcompiler")
#pragma comment (lib, "dinput8")


#ifdef _DEBUG
#pragma comment (lib, "DirectXTKd12")
#pragma comment (lib, "fmodL_vc")
#pragma comment (lib, "PTHLib_Debug")
#else
#pragma comment (lib, "DirectXTK12")
#pragma comment (lib, "fmod_vc")
#pragma comment (lib, "PTHLib")
#endif

#pragma comment (lib, "libxl")


#if !defined(_CLIENT)
#define _USE_IMGUI
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx12.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imstb_rectpack.h"
#include "ImGui/imstb_textedit.h"
#include "ImGui/imstb_truetype.h"

static const char* CLONETYPE_STRING{ "CloneType" };
static const char* CLONETYPE_STATIC_STRING{ "CLONETYPE_STATIC" };
static const char* CLONETYPE_ONCE_STRING{ "CLONETYPE_ONCE" };

#endif // _USE_IMGUI

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK, __FILE__, __LINE__ ) 
#define new DBG_NEW 

#endif

// Use Atomic Shared PTr
#define _USE_ATOMIC
// Use Debugging Values
//#define _USE_DEBUGGING

#include <d3d12.h>
#include <dxgi1_4.h>
#include <dxgi.h>

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
#include <DirectXTK/GraphicsMemory.h>
#include <DirectXTK/SpriteFont.h>
#include <DirectXTK/DescriptorHeap.h>
#include "DirectXTK/d3dx12.h"
#include "PTHLib/CusMath.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "DirectXTK/GamePad.h"

#include <array>
#include<concepts>
#include <string_view>
#include <mutex>
#include <thread>
#include <set>
#include <unordered_set>
#include <queue>
#include <list>
#include <vector>
#include <set>
#include <random>

#include "EngineConcept.h"
#include "UDelegate.h"

namespace Engine {
	namespace DXVALUE {
		static constexpr unsigned short ALIGNVALUE{ 128 };
	}
}

#define DEF_CACHE_ALGIN __declspec(align(Engine::DXVALUE::ALIGNVALUE))
#define ALIGNAS alignas(Engine::DXVALUE::ALIGNVALUE)


#include "EngineTypedef.h"
#include "EngineEnum.h"
#include "EngineFunction.h"
#include "EngineMacro.h"
#include "EngineMake.h"
#include "EngineStruct.h"
#include "EngineShaderParam.h"


#pragma endregion DX12

namespace Engine {
	namespace DXVALUE {
		static const _float3					 g_BasicScale = _float3(1.f, 1.f, 1.f);
		static constexpr _uint				 g_maxRegisterCnt{REGISTER_CNT};
		static const _wstring				 g_EmptyWstr{L""};
		static const _wstring				 g_OutFolderExtension{L".bin"};
	}
}
#define BASIC_SCALE										Engine::DXVALUE::g_BasicScale
#define MAX_REGISTER_CNT						Engine::DXVALUE::g_maxRegisterCnt
#define EMPTY_WSTR									Engine::DXVALUE::g_EmptyWstr
#define DEFAULT_OUTFOLDEREXTENSION		Engine::DXVALUE::g_OutFolderExtension
using namespace Engine;

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UENGINEDEFINES_H