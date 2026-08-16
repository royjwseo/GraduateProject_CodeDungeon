#pragma once
//#define _WITH_SWAPCHAIN_FULLSCREEN_STATE

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2
};

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "windowscodecs.lib")

#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <cstdio>
#include <memory.h>
#include <tchar.h>
#include <math.h>
#include <array>
#include <vector>
#include <random>
#include <string>
#include <wrl.h>
#include <shellapi.h>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <chrono>
#include <Mmsystem.h>
#include <iostream>
#include <sstream>
using namespace std;

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <dxgi1_6.h>
#include <dwrite.h>
#include <d2d1_3.h>
#include <d3d11on12.h>
#include <d3d12sdklayers.h>
#include <d3dcompiler.h>
#include <DirectXCollision.h>
#include <DXGIDebug.h>
#include "d3dx12.h"
using namespace DirectX;
using namespace DirectX::PackedVector;
using Microsoft::WRL::ComPtr;


#define DEVICE gGameFrameWork->GetDeviceAndFactory()->GetDevice()
#define CMDLIST gGameFrameWork->GetCommandQueue()->GetCmdList()
#define CMDALLOCATOR gGameFrameWork->GetCommandQueue()->GetCmdAllocator()
#define CMDQUEUE gGameFrameWork->GetCommandQueue()->GetCmdQueue()
#define CMD gGameFrameWork->GetCommandQueue()

#define GRAPHICS_ROOT_SIGNATURE	gGameFrameWork->GetRootSignature()->GetGraphicsRootSignature()
//2024-01-07 이성현
#define RTVDSVDESCRIPTORHEAP gGameFrameWork->GetSwapChainAndRtvDsvHeap()
#define RTVDESCRIPTORHEAP gGameFrameWork->GetSwapChainAndRtvDsvHeap()->GetRTVHeap()
#define SWAPCHAIN gGameFrameWork->GetSwapChainAndRtvDsvHeap()->GetSwapChain()
#define FENCE gGameFrameWork->GetCommandQueue()->GetFence()


//-------------
extern unique_ptr<class Engine> gGameFrameWork;

extern UINT gnCbvSrvDescriptorIncrementSize;
extern UINT	gnRtvDescriptorIncrementSize;
extern UINT gnDsvDescriptorIncrementSize;
//-------------
namespace Factor {
	constexpr int FRAME_BUFFER_WIDTH = 800;
	constexpr int FRAME_BUFFER_HEIGHT = 600;
	constexpr int ASPECT_RATIO = FRAME_BUFFER_WIDTH / FRAME_BUFFER_HEIGHT;
}

//--------------
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;
//------------------------------

#define RANDOM_COLOR XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX))

//2024-01-17 이성현
#define DIR_FORWARD					0x01
#define DIR_BACKWARD				0x02
#define DIR_LEFT					0x04
#define DIR_RIGHT					0x08
#define DIR_UP						0x10
#define DIR_DOWN					0x20

enum CameraMode {
	NONE,
	DEBUG_CAMERA,
	GAME_CAMERA
};


extern ComPtr<ID3D12Resource> CreateTextureResourceFromDDSFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const wchar_t* pszFileName, ID3D12Resource** ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
extern ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType = D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, ID3D12Resource** ppd3dUploadBuffer = NULL);

namespace Util {
	extern ComPtr<ID3D12Resource> CreateBufferResource(const ComPtr<ID3D12Device>& _Device,
		const ComPtr<ID3D12GraphicsCommandList>& _CommandList, void* pData, UINT nBytes, D3D12_HEAP_TYPE
		d3dHeapType = D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATES d3dResourceStates =
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, ID3D12Resource** ppd3dUploadBuffer =
		NULL);

	extern ComPtr<ID3D12Resource> CreateTextureResourceFromDDSFile(const ComPtr<ID3D12Device>& _Device, wchar_t* pszFileName, ID3D12Resource** ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	extern ComPtr<ID3D12Resource> CreateTextureResourceFromWICFile(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, wchar_t* pszFileName, ID3D12Resource** ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	extern ComPtr<ID3D12Resource> CreateTexture2DResource(const ComPtr<ID3D12Device>& _Device,
		const ComPtr<ID3D12GraphicsCommandList>& _CommandList, UINT nWidth, UINT nHeight, UINT nElements, UINT nMipLevels, DXGI_FORMAT dxgiFormat, D3D12_RESOURCE_FLAGS d3dResourceFlags, D3D12_RESOURCE_STATES d3dResourceStates, D3D12_CLEAR_VALUE* pd3dClearValue);

	extern 	ComPtr<ID3D12Resource> CreateBufferResource(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList,
		const void* pData, UINT sizePerData, UINT dataCount, D3D12_HEAP_TYPE d3dHeapType, D3D12_RESOURCE_STATES d3dResourceStates, ID3D12Resource** ppd3dUploadBuffer = nullptr);

	extern void SynchronizeResourceTransition(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, ComPtr<ID3D12Resource> pd3dResource, D3D12_RESOURCE_STATES d3dStateBefore, D3D12_RESOURCE_STATES d3dStateAfter);
	extern void ExecuteCommandList(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, const ComPtr<ID3D12CommandQueue>& _CmdQueue, const ComPtr<ID3D12Fence>& _Fence, UINT64 nFenceValue, HANDLE hFenceEvent);
	extern void WaitForGpuComplete(const ComPtr<ID3D12CommandQueue>& _CmdQueue, const ComPtr<ID3D12Fence>& _Fence, UINT64 nFenceValue, HANDLE hFenceEvent);
}

namespace DX
{
	// Helper class for COM exceptions
	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr) : result(hr) {}

		const char* what() const override
		{
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X",
				static_cast<unsigned int>(result));
			return s_str;
		}

	private:
		HRESULT result;
	};

	// Helper utility converts D3D API failures into exceptions.
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw com_exception(hr);
		}
	}
}

namespace Vector3
{
	inline XMFLOAT3 MultiplyXMFLOAT3(const XMFLOAT3& v1, const XMFLOAT3& v2)
	{
		XMFLOAT3 result;
		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;
		result.z = v1.z * v2.z;
		return result;
	}

	inline float Distance(const XMFLOAT3& point1, const XMFLOAT3& point2)
	{
		float dx = point2.x - point1.x;
		float dy = point2.y - point1.y;
		float dz = point2.z - point1.z;

		return sqrt(dx * dx + dy * dy + dz * dz);
	}

	inline XMFLOAT3 XMVectorToFloat3(const XMVECTOR& xmvVector)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, xmvVector);
		return xmf3Result;
	}

	inline XMFLOAT3 ScalarProduct(const XMFLOAT3& xmf3Vector, float fScalar, bool bNormalize = true)
	{
		XMFLOAT3 xmf3Result;
		if (bNormalize)
			XMStoreFloat3(&xmf3Result, XMVector3Normalize(XMLoadFloat3(&xmf3Vector)) * fScalar);
		else
			XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector) * fScalar);
		return xmf3Result;
	}

	inline XMFLOAT3 Add(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) + XMLoadFloat3(&xmf3Vector2));
		return xmf3Result;
	}

	inline XMFLOAT3 Add(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2, float fScalar)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) + (XMLoadFloat3(&xmf3Vector2) * fScalar));
		return xmf3Result;
	}

	inline XMFLOAT3 Subtract(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) - XMLoadFloat3(&xmf3Vector2));
		return xmf3Result;
	}

	inline float DotProduct(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3Dot(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)));
		return xmf3Result.x;
	}

	inline XMFLOAT3 CrossProduct(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2, bool bNormalize = true)
	{
		XMFLOAT3 xmf3Result;
		if (bNormalize)
			XMStoreFloat3(&xmf3Result, XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2))));
		else
			XMStoreFloat3(&xmf3Result, XMVector3Cross(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)));
		return xmf3Result;
	}

	inline XMFLOAT3 Normalize(const XMFLOAT3& xmf3Vector)
	{
		XMFLOAT3 m_xmf3Normal;
		XMStoreFloat3(&m_xmf3Normal, XMVector3Normalize(XMLoadFloat3(&xmf3Vector)));
		return m_xmf3Normal;
	}

	inline float Length(const XMFLOAT3& xmf3Vector)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3Length(XMLoadFloat3(&xmf3Vector)));
		return xmf3Result.x;
	}

	inline float Angle(const XMVECTOR& xmvVector1, const XMVECTOR& xmvVector2)
	{
		XMVECTOR xmvAngle = XMVector3AngleBetweenNormals(xmvVector1, xmvVector2);
		return XMConvertToDegrees(XMVectorGetX(xmvAngle));
	}

	inline float Angle(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2)
	{
		XMVECTOR xmvAngle = XMVector3AngleBetweenNormals(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2));
		return XMConvertToDegrees(XMVectorGetX(xmvAngle));
	}

	inline XMFLOAT3 TransformNormal(const XMFLOAT3& xmf3Vector, const XMMATRIX& xmmtxTransform)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3TransformNormal(XMLoadFloat3(&xmf3Vector), xmmtxTransform));
		return xmf3Result;
	}

	inline XMFLOAT3 TransformCoord(const XMFLOAT3& xmf3Vector, const XMMATRIX& xmmtxTransform)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3TransformCoord(XMLoadFloat3(&xmf3Vector), xmmtxTransform));
		return xmf3Result;
	}

	inline XMFLOAT3 TransformCoord(const XMFLOAT3& xmf3Vector, const XMFLOAT4X4& xmmtx4x4Matrix)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3TransformCoord(XMLoadFloat3(&xmf3Vector), XMLoadFloat4x4(&xmmtx4x4Matrix)));
		return(xmf3Result);
	}
}

namespace Vector4
{
	inline XMFLOAT4 Add(const XMFLOAT4& xmf4Vector1, const XMFLOAT4& xmf4Vector2)
	{
		XMFLOAT4 xmf4Result;
		XMStoreFloat4(&xmf4Result, XMLoadFloat4(&xmf4Vector1) +
			XMLoadFloat4(&xmf4Vector2));
		return xmf4Result;
	}

	inline XMFLOAT4 Multiply(const XMFLOAT4& xmf4Vector1, const XMFLOAT4& xmf4Vector2)
	{
		XMFLOAT4 xmf4Result;
		XMStoreFloat4(&xmf4Result, XMLoadFloat4(&xmf4Vector1) *
			XMLoadFloat4(&xmf4Vector2));
		return xmf4Result;
	}

	inline XMFLOAT4 Multiply(float fScalar, const XMFLOAT4& xmf4Vector)
	{
		XMFLOAT4 xmf4Result;
		XMStoreFloat4(&xmf4Result, fScalar * XMLoadFloat4(&xmf4Vector));
		return xmf4Result;
	}
}

namespace Matrix4x4
{
	inline XMFLOAT4X4 Identity()
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixIdentity());
		return xmmtx4x4Result;
	}

	inline XMFLOAT4X4 Multiply(const XMFLOAT4X4& xmmtx4x4Matrix1, const XMFLOAT4X4& xmmtx4x4Matrix2)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMLoadFloat4x4(&xmmtx4x4Matrix1) *
			XMLoadFloat4x4(&xmmtx4x4Matrix2));
		return xmmtx4x4Result;
	}

	inline XMFLOAT4X4 Multiply(const XMFLOAT4X4& xmmtx4x4Matrix1, const XMMATRIX& xmmtxMatrix2)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMLoadFloat4x4(&xmmtx4x4Matrix1) * xmmtxMatrix2);
		return xmmtx4x4Result;
	}

	inline XMFLOAT4X4 Multiply(const XMMATRIX& xmmtxMatrix1, const XMFLOAT4X4& xmmtx4x4Matrix2)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, xmmtxMatrix1 * XMLoadFloat4x4(&xmmtx4x4Matrix2));
		return xmmtx4x4Result;
	}

	inline XMFLOAT4X4 Inverse(const XMFLOAT4X4& xmmtx4x4Matrix)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixInverse(nullptr,
			XMLoadFloat4x4(&xmmtx4x4Matrix)));
		return xmmtx4x4Result;
	}

	inline XMFLOAT4X4 Transpose(const XMFLOAT4X4& xmmtx4x4Matrix)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixTranspose(XMLoadFloat4x4(&xmmtx4x4Matrix)));
		return xmmtx4x4Result;
	}

	inline XMFLOAT4X4 PerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ,
		float FarZ)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixPerspectiveFovLH(FovAngleY, AspectRatio,
			NearZ, FarZ));
		return xmmtx4x4Result;
	}

	inline XMFLOAT4X4 LookAtLH(const XMFLOAT3& xmf3EyePosition, const XMFLOAT3& xmf3LookAtPosition, const XMFLOAT3& xmf3UpDirection)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixLookAtLH(XMLoadFloat3(&xmf3EyePosition), XMLoadFloat3(&xmf3LookAtPosition), XMLoadFloat3(&xmf3UpDirection)));
		return xmmtx4x4Result;
	}
}
