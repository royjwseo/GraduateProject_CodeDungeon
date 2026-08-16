
#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_EMGINE_ENGINE_TYPEDEF_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_EMGINE_ENGINE_TYPEDEF_H


namespace Engine {

	using _ubyte =  unsigned char;
	using _byte = signed char;
	using  _char = char;

	using _ushort = unsigned short;
	using _short = signed short;

	using _uint = unsigned int;
	using _int = signed int;

	using _ulong = unsigned long;
	using _long = signed long;
	using _ulonglong = unsigned long long;
	using _longlong = signed long long;

	using	CLASSID = _uint;

	using _float = float;
	using _double = double;

	using _bool = bool;
	using _tchar = wchar_t;

	using CAMID = _ushort;

	using _float2 = DirectX::PTH::Vector2;
	using _float3 = DirectX::PTH::Vector3;
	using _float4 = DirectX::PTH::Vector4;

	using _int2 = DirectX::XMINT2;
	using _int3 = DirectX::XMINT3;
	using _int4 = DirectX::XMINT4;

	using _retangle = DirectX::PTH::RECTANGLE;
	using _float4x4 = DirectX::PTH::Matrix;
	using _quaternion = DirectX::PTH::Quaternion;

	using _xmvector = DirectX::XMVECTOR;
	using _xmmatrix = DirectX::XMMATRIX;

	using _wstring = std::wstring;
	using _string = std::string;

	namespace PATH {
		using PATHS = std::vector<std::wstring>;
	}

	using PATHS = PATH::PATHS;

	namespace RTS {
		using RENDERFORMATS = std::vector<DXGI_FORMAT>;
	}

	using RENDERFORMATS = RTS::RENDERFORMATS;

	template<class T>
	using ComPtr = DEF_CACHE_ALGIN Microsoft::WRL::ComPtr<T>;

	using Dx12Device = DEF_CACHE_ALGIN ID3D12Device5;
	using Dx12Fence = DEF_CACHE_ALGIN ID3D12Fence;
	using Dx12CommandQueue = DEF_CACHE_ALGIN ID3D12CommandQueue;
	using Dx12CommandAllocator = DEF_CACHE_ALGIN ID3D12CommandAllocator;
	using Dx12GraphicsCommandList = DEF_CACHE_ALGIN ID3D12GraphicsCommandList4;
	using Dx12Resource = DEF_CACHE_ALGIN ID3D12Resource;
	using Dx12DescriptorHeap = DEF_CACHE_ALGIN ID3D12DescriptorHeap;
	using Dx12PipelineState = DEF_CACHE_ALGIN ID3D12PipelineState;
	using Dx12RootSignature = DEF_CACHE_ALGIN ID3D12RootSignature;
	using DxBlob = ID3DBlob;
	using Dx12StateObject = ID3D12StateObject;
	using DxGIFactory = IDXGIFactory4;
	using DxGISwapChain = IDXGISwapChain3;
	using Dx12QueryHeap = ID3D12QueryHeap;

	// Using
	template<class Type, _uint Num>
	using ARRAY = std::array<Type, Num>;

	template<class T>
	using LIST = std::list<T>;

	template<class T>
	using VECTOR = std::vector<T>;

	template<class Type, class Other>
	using UNORMAP =  std::unordered_map<Type, Other>;

	template<class Type>
	using UNORSET = std::unordered_set<Type>;

	template<class Type>
	using SET = std::set<Type>;

	template<class Type>
	using VECTOR = std::vector<Type>;

	template<class Type>
	using QUEUE = std::queue<Type>;

	template<class Type>
	using PRIORITYQUEUE = std::priority_queue<Type>;

	namespace VOIDS {
		using VOIDDATAS = std::vector<void*>;
	}

	using VOIDDATAS = VOIDS::VOIDDATAS;

	template<typename T>
	using SHPTR = DEF_CACHE_ALGIN std::shared_ptr<T>;

	template <typename T>
	using SHPTRREF = SHPTR<T>&;

	template<typename T>
	using CSHPTR = const SHPTR<T>;

	template<typename T>
	using CSHPTRREF = const SHPTR<T>&;

	template<class T>
	using WKPTR = DEF_CACHE_ALGIN std::weak_ptr<T>;

	template<class T>
	using CWKPTR = const WKPTR<T>;

	template<class T>
	using CWKPTRREF = const WKPTR<T>&;

	template<class T>
	using UNPTR = std::unique_ptr<T>;

	template<class T>
	using CUNPTR = const UNPTR<T>;

	template<class T>
	using CUNPTRREF = const UNPTR<T>&;
}

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_EMGINE_ENGINE_TYPEDEF_H