#ifndef _PTH_FRAMEWORK_CLIENT_PUBLIC_CLIENTDEFINE_H
#define _PTH_FRAMEWORK_CLIENT_PUBLIC_CLIENTDEFINE_H

#define _CLINET

#include "EngineDefines.h"
#ifdef _DEBUG
#pragma comment(lib, "EngineD.lib")
#else
#pragma comment(lib, "Engine.lib")
#endif

namespace Client
{
	namespace DXVALUE
	{
		static const unsigned short g_iWindowWidth = 1280;
		static const unsigned short g_iWindowHeight = 1080;

		static const float g_iWindowHalfWidth = (float)g_iWindowWidth * 0.5f;
		static const float g_iWindowHalfHeight = (float)g_iWindowHeight * 0.5f;
	}

	enum SCENE {
		SCENE_LOGO, SCENE_STAGE1, SCENE_END
	};

#define WINDOW_WIDTH					Client::DXVALUE::g_iWindowWidth
#define WINDOW_HEIGHT					Client::DXVALUE::g_iWindowHeight

#define WINDOW_HALF_WIDTH		Client::DXVALUE::g_iWindowHalfWidth
#define WINDOW_HALF_HEIGHT		Client::DXVALUE::g_iWindowHalfHeight

#define FIRST_RESOURCE_FOLDER L"..\\..\\Resource"
}

#include "ClientProto.h"

using namespace Client;

#endif // _PTH_FRAMEWORK_CLIENT__PUBLIC_CLIENTDEFINE_H