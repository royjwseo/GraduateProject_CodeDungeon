#pragma once 

#pragma warning (disable : 4244)
#pragma warning (disable : 4018)

#include "CoreDefines.h"
#include "ProtocolBuffer.h"

#ifdef _DEBUG
#pragma comment(lib, "CoreD")
#pragma comment(lib, "ProtocolBufferD")
#else
#pragma comment(lib, "Core")
#pragma comment(lib, "ProtocolBuffer")
#endif
namespace Core {}

using namespace Core;

static const std::string IP_ADDRESS = "3.39.11.88";