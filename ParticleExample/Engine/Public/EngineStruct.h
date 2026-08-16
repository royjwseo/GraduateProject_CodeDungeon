
#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_EMGINESTRUCT_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_EMGINESTRUCT_H


namespace Engine {

	typedef  DEF_CACHE_ALGIN struct tagGraphicDesc {

		enum WINMODE { MODE_FULL, MODE_WIN, MODE_END };

		HWND hWnd = { NULL };
		HINSTANCE hInst = { NULL };
		unsigned int iWinCX{ 0 }, iWinCY{ 0 };
		WINMODE eWinMode{ MODE_END };
	}GRAPHICDESC;

	typedef struct tagOutputData {
		WKPTR<class UDevice>					wpDevice{};
		WKPTR<class UCommand>			wpGpuCmd{};
	}OUTPUTDATA;

#pragma region INDICES

	typedef struct tagIndices16 {
		_ushort _1{ 0 }, _2{ 0 }, _3{ 0 };
	}INDICIES16;

	typedef struct tagIndices32 {
		_uint _1{ 0 }, _2{ 0 }, _3{ 0 };
	}INDICIES32;

#pragma endregion INDICES

#pragma region VTXINDEX

	typedef struct tagVertexPoint
	{
		_float3 vPosition{0.f, 0.f, 0.f};
		_float3 vNormal{ 0.f, 0.f, 0.f };
		_float3 vTagent{ 0.f, 0.f, 0.f };
		_float2 vTexUV{0.f, 0.f};
	}VTXPOINT;

	typedef struct tagVertexPoint_Declaration
	{
		static constexpr unsigned int					iNumElement{ 4 };
		static D3D12_INPUT_ELEMENT_DESC		Element[iNumElement];
	}VTXPOINT_DELCARTION;

	// VTXRect
	typedef struct tagVertexDefault
	{
		_float3			vPosition = _float3(0.f, 0.f, 0.f);
		_float2			vTexUV = _float2(0.f, 0.f);
	}VTXDEFAULT;

	typedef struct tagVertexRect_Declaration
	{
		static const unsigned int							iNumElement = 2;
		static D3D12_INPUT_ELEMENT_DESC		Element[iNumElement];
	}VTXDEFAULT_DECLARATION;

	typedef struct tagVertexNormal {

		_float3		vPosition{ 0.f, 0.f, 0.f };
		_float3		vNormal{ 0.f, 0.f, 0.f };
		_float2		vTexUV{ 0.f, 0.f };
	}VTXNORMAL;

	typedef struct tagVertexNormal_Decalaration {
		static const unsigned int							iNumElement = 3;
		static D3D12_INPUT_ELEMENT_DESC		Element[iNumElement];
	}VTXNORMAL_DECLARATION;

#pragma region CUBE
	typedef struct tagVertexCube
	{
		_float3		vPosition{ 0.f, 0.f, 0.f };
		_float3		vTexUV{ 0.f, 0.f, 0.f };
	}VTXCUBE;

	typedef struct tagVertexCube_Declaration {
		static const unsigned int							iNumElement = 2;
		static D3D12_INPUT_ELEMENT_DESC		Element[iNumElement];
	}VTXCUBE_DECLARATION;

	typedef struct tagVertexNormalCube
	{
		_float3		vPosition{ 0.f, 0.f, 0.f };
		_float3		vNormal{ 0.f, 0.f, 0.f };
		_float3		vTexUV{ 0.f, 0.f, 0.f };
	}VTXNORCUBE;

	typedef struct tagVertexNormalCube_Declaration {
		static const unsigned int							iNumElement = 3;
		static D3D12_INPUT_ELEMENT_DESC		Element[iNumElement];
	}VTXNORMALCUBE_DECLARATION;

	typedef struct tagVertexSkyBox
	{
		_float3		vPosition{ 0.f, 0.f, 0.f };
	}VTXSKYBOX;

	typedef struct tagVertexSkyBox_Declaration {
		static const unsigned int							iNumElement = 1;
		static D3D12_INPUT_ELEMENT_DESC		Element[iNumElement];
	}VTXSKYBOX_DECLARATION;

#pragma endregion CUBE

	typedef struct  tagVertexModel
	{
		_float3			vPosition = _float3(0.f, 0.f, 0.f);
		_float3			vNormal = _float3(0.f, 0.f, 0.f);
		_float2			vTexUV[5]{};
		_float3			vTangent = _float3(0.f, 0.f, 0.f);
	}VTXMODEL;

	typedef struct tagVertexModel_Declaration
	{
		static const unsigned int iNumElement = 8;
		static D3D12_INPUT_ELEMENT_DESC  Element[iNumElement];
	}VTXMODEL_DECLARATION;

	typedef struct tagVertexAnimModel
	{
		_float3			vPosition = _float3(0.f, 0.f, 0.f);
		_float3			vNormal = _float3(0.f, 0.f, 0.f);
		_float2			vTexUV[5]{};
		_float3			vTangent = _float3(0.f, 0.f, 0.f);

		_int4				vBlendIndex = _int4(0, 0, 0, 0);
		_float4			vBlendWeight = _float4(0.f, 0.f, 0.f, 0.f);
	}VTXANIMMODEL;

	typedef struct tagVertexAnimModel_Declaration
	{
		static const unsigned int iNumElement = 10;
		static D3D12_INPUT_ELEMENT_DESC  Element[iNumElement];
	}VTXANIMMODEL_DECLARATION;

	typedef struct tagVertexModelColor {

		_float3			vPosition = _float3(0.f, 0.f, 0.f);
		_float3			vNormal = _float3(0.f, 0.f, 0.f);
		_float2			vTexUV[5]{};
		_float3			vTangent = _float3(0.f, 0.f, 0.f);
		_float4			vColor{ 0.f, 0.f, 0.f, 0.f };
	}VTXMODELCOLOR;

	typedef struct tagVertexModelColor_Declaration
	{
		static const unsigned int iNumElement = 9;
		static D3D12_INPUT_ELEMENT_DESC  Element[iNumElement];
	}VTXMODELCOLOR_DECLARATION;

#pragma endregion VTXINDEX

#pragma region VTXINSTANCE

	typedef struct tagVertexIntanceRect {
		_float3			vPosition = _float3(0.f, 0.f, 0.f);
		_float2			vTexUV = _float2(0.f, 0.f);

		_float4			vRight{ 0, 0, 0, 0 };
		_float4			vUp{ 0, 0, 0, 0 };
		_float4			vLook{ 0, 0, 0, 0 };
		_float4			vPos{ 0, 0, 0, 1 };
	}VTXINSTANCERECT;

	typedef struct tagVertexInstanceRect_Declaration
	{
		static const unsigned int							iNumElement = 6;
		static D3D12_INPUT_ELEMENT_DESC		Element[iNumElement];
	}VTXINSTANCERECT_DECLARATION;

#pragma endregion VTXINSTNACE

#pragma region CLONEINFO

	typedef struct tagCloneData {
		_wstring			wstrCloneLayer = L"";
		_ubyte			bCloneType = 0;
	}CLONEDATA;

#pragma endregion CLONEINFO

#pragma region CUSTIMER
	// TODO Shader

	typedef struct ENGINE_DLL tagCusTimer
	{
		tagCusTimer() = default;
		tagCusTimer(const _float& _fStandardTime)
			: fStandardTime(_fStandardTime)
		{
			fTimer = 0.f;
			isPass = false;
		}
		_bool IsOver(const _double& _dTimeDelta);
		void ResetTimer();
		_bool IsPass() { return isPass; }
		// Timer
		_float		fTimer = 0.f;
		_float		fStandardTime = 0.f;
	private:
		_bool		isPass{ false };
	}CUSTIMER;

#pragma endregion CUSTIMER

#pragma region FONT


#pragma endregion FONT 

#pragma region INSTANCEBUFFER

	struct INSTANCEBUFFERDESC {
		_uint	iInstanceBufferCnt{ 0 };
	};


#pragma endregion INSTANCEBUFFER

#pragma region FILEDATA

	// File Data
	typedef struct  tagFileData
	{
		_wstring											wstrfileName{ L"" };
		_wstring											wstrfilePath{ L"" };
		_ushort											sindex{ 0 };
	}FILEDATA;

	typedef struct ENGINE_DLL tagFileGroup
	{
		_wstring																				wstrPath{ L"" };
		_wstring																				wstrFolderName{ L"" };
		UNORMAP<_wstring, SHPTR<tagFileGroup>>	conuomapUnderFileGroupList;
		UNORMAP<_wstring, SHPTR<FILEDATA>>			FileDataList;
		SHPTR<tagFileGroup>													spParentsUpper = nullptr;

		void AddFileGroup(const _wstring& _wstrFileName, CSHPTRREF<tagFileGroup> _spFileGroup);
		void AddFileData(const _wstring& _wstrFileName, CSHPTRREF<FILEDATA> _spData);
		void FileAllData(const _wstring& _wstrEct, VECTOR<SHPTR<FILEDATA>>& _lsFileData);

		SHPTR<FILEDATA> FindData(const _wstring& _wstrFileName);
		SHPTR<tagFileGroup> FindGroup(const _wstring& _wstrFileName);
		SHPTR<tagFileGroup> FindUnder(const _wstring& _wstrFileName);
		SHPTR<tagFileGroup> FindUpper(const _wstring& _wstrFileName);

	}FILEGROUP;

#pragma endregion FILEDATA

#pragma region SHADERDESC

	typedef VECTOR<SHADERMAININFO> SHADERLIST;
	typedef struct tagShaderDesc
	{
		// 이름만 있으면 Compute Shader를 빌드한다.
		tagShaderDesc(const _wstring& _wstrShaderName, const D3D_PRIMITIVE_TOPOLOGY
		_eTopology = D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST) :
			eType(GRAPHICISCONTROLTYPE::COMPUTE),
			eTopology(_eTopology),
			wstrShaderName(_wstrShaderName), pElement(nullptr), iNumElement(0), ShaderList({ SHADERMAININFO::CS_MAIN}),
			iRenderTargets(1),
			RenderFormats(DXGI_FORMAT_UNKNOWN),
			eRasterRizerType(RASTERIZER_TYPE::CULL_BACK),
			eDepthStencilType(DEPTH_STENCIL_TYPE::LESS),
			eBlendType(BLEND_TYPE::DEFAULT)
		{}

		// 렌더 타겟에 그릴 수 있게 해주는 쉐이더이다. 
		 tagShaderDesc(const _wstring& _wstrShaderName,
			D3D12_INPUT_ELEMENT_DESC* _pDesc,
			const _uint& _iNumElement,
			const SHADERLIST& _ShaderList,
			const RENDERFORMATS& _RenderForamts,
			const RASTERIZER_TYPE& _eRasterRizerType = RASTERIZER_TYPE::CULL_BACK,
			const DEPTH_STENCIL_TYPE& _eDepthStencilType = DEPTH_STENCIL_TYPE::LESS,
			const BLEND_TYPE& _eBlendType = BLEND_TYPE::DEFAULT, 
			 const D3D_PRIMITIVE_TOPOLOGY _eTopology =
			 D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
			:
			 eType(GRAPHICISCONTROLTYPE::GRAPHICS),
			 eTopology(_eTopology),
			wstrShaderName(_wstrShaderName), pElement(_pDesc), iNumElement(_iNumElement), ShaderList(_ShaderList),
			iRenderTargets((_uint)_RenderForamts.size()),
			RenderFormats(_RenderForamts),
			eRasterRizerType(_eRasterRizerType),
			eDepthStencilType(_eDepthStencilType),
			eBlendType(_eBlendType)
		{}
		// 일반 화면에 그리는 쉐이더 1개의 렌더 타겟에 Format만 맞으면 그릴 수 있따. 
		 tagShaderDesc(const _wstring& _wstrShaderName,
			D3D12_INPUT_ELEMENT_DESC* _pDesc,
			const _uint& _iNumElement,
			const SHADERLIST& _ShaderList,
			const RASTERIZER_TYPE& _eRasterRizerType = RASTERIZER_TYPE::CULL_BACK,
			const DEPTH_STENCIL_TYPE& _eDepthStencilType = DEPTH_STENCIL_TYPE::LESS,
			const BLEND_TYPE& _eBlendType = BLEND_TYPE::DEFAULT,
			 const D3D_PRIMITIVE_TOPOLOGY _eTopology =
			 D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
			:
			 eType(GRAPHICISCONTROLTYPE::GRAPHICS),
			 eTopology(_eTopology),
			wstrShaderName(_wstrShaderName), pElement(_pDesc), iNumElement(_iNumElement), ShaderList(_ShaderList),
			 RenderFormats(RENDERFORMATS{DXGI_FORMAT_R8G8B8A8_UNORM}),
			 iRenderTargets(1),
			eRasterRizerType(_eRasterRizerType),
			eDepthStencilType(_eDepthStencilType),
			eBlendType(_eBlendType)
		{}
		
		 GRAPHICISCONTROLTYPE						eType;
		_wstring														wstrShaderName;
		D3D12_INPUT_ELEMENT_DESC*			pElement;
		_uint															iNumElement;
		SHADERLIST												ShaderList;
		_uint															iRenderTargets;
		RENDERFORMATS									RenderFormats;
		RASTERIZER_TYPE									eRasterRizerType;
		DEPTH_STENCIL_TYPE							eDepthStencilType;
		BLEND_TYPE												eBlendType;
		D3D_PRIMITIVE_TOPOLOGY					eTopology;
	}SHADERDESC;

	typedef struct tagShaderConstantBufferDesc
	{
		tagShaderConstantBufferDesc() = default;
		tagShaderConstantBufferDesc(const CBV_REGISTER& _eReg, const _uint& _iSize, const _uint& _iCnt)
			: eCbvRegister(_eReg), iSize(_iSize), iCount(_iCnt) {}

		CBV_REGISTER		eCbvRegister = CBV_REGISTER::CBV_END;
		_uint						iSize = 0;
		_uint						iCount = 0;
	}SHADERCBDESC;

#pragma endregion SHADERDESC

#pragma region RENDERTARGET

	typedef struct tagTextureTypeDesc
	{
		tagTextureTypeDesc(const DXGI_FORMAT& _eFormat, const _uint& _iWidth, const _uint& _iHeight,
			const _float4& _vClear, const D3D12_RESOURCE_FLAGS& _eResourceFlag)
			: eDxFormat(_eFormat), iWidth(_iWidth), iHeight(_iHeight), vClear(_vClear),
			eHeapFlags(D3D12_HEAP_FLAG_NONE), eResourceFlags(_eResourceFlag) 
		{
			HeapProPerty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		}
		tagTextureTypeDesc(const DXGI_FORMAT& _eFormat, const _uint& _iWidth, const _uint& _iHeight, const _float4& _vClear,
			const D3D12_HEAP_TYPE& _eHeapType = D3D12_HEAP_TYPE_DEFAULT,
			const D3D12_HEAP_FLAGS& _eHeapFlags = D3D12_HEAP_FLAG_NONE,
			const D3D12_RESOURCE_FLAGS& _ResourceFlags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
			: eDxFormat(_eFormat), iWidth(_iWidth), iHeight(_iHeight),
			eHeapFlags(_eHeapFlags), eResourceFlags(_ResourceFlags), vClear(_vClear)
		{
			HeapProPerty = CD3DX12_HEAP_PROPERTIES(_eHeapType);
		}
		DXGI_FORMAT									eDxFormat;
		_uint													iWidth;
		_uint													iHeight;
		CD3DX12_HEAP_PROPERTIES		HeapProPerty;
		D3D12_HEAP_FLAGS						eHeapFlags;
		D3D12_RESOURCE_FLAGS			eResourceFlags;
		_float4												vClear;
	}TEXTURETYPEDESC;

	typedef struct tagRenderTargetDesc : public tagTextureTypeDesc
	{
	public:
		tagRenderTargetDesc() : 
			eRTObjID(RTOBJID::RTOBJ_END),
			tagTextureTypeDesc(DXGI_FORMAT_UNKNOWN, 0, 0, _float4{}, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE) {}
		tagRenderTargetDesc(const RTOBJID& _eRTObjID, const DXGI_FORMAT& _eFormat, const _uint& _iWidth, const _uint& _iHeight, const _float4& _vClear,
			const D3D12_HEAP_TYPE& _eHeapType = D3D12_HEAP_TYPE_DEFAULT,
			const D3D12_HEAP_FLAGS& _eHeapFlags = D3D12_HEAP_FLAG_NONE,
			const D3D12_RESOURCE_FLAGS& _ResourceFlags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
			: eRTObjID(_eRTObjID), tagTextureTypeDesc(_eFormat, _iWidth, _iHeight, _vClear, _eHeapType, _eHeapFlags, _ResourceFlags)
		{}
		RTOBJID	 eRTObjID;
	}RTDESC;

#pragma endregion RENDERTARGET


#pragma region MODEL

	// Key Frame
	typedef struct tagKeyFrame
	{
		_double		dTime{ 0.0 };

		_float3	vScale;
		_float4	vRotation;
		_float4	vPosition;
	}KEYFRAME;

	// BuffDesc
	typedef struct tagBufferDesc
	{
		_uint										iNumVertices = 0;
		_uint										iNumIndices = 0;
		_wstring									wstrName;
		VECTOR<_float2>		vecUVTexCoords;
		const _float3*						pPosition{ nullptr };
		const INDICIES32*				pIndices = nullptr;
		_uint										iMatrialIndex = 0;
		_uint										iNumBones = 0;
		VECTOR<_wstring>		vecBoneNodeNameList;
		_uint										iBoneNodeCnt = 0;

		_float3									vMinVertex{};
		_float3									vMaxVertex{};
	}BUFFDESC;
	// Channel Desc 
	typedef struct tagChannelDesc
	{
		_wstring				wstrBoneName = L"";
		_uint					iNumMaxKeyFrames = 0;
		KEYFRAME*		pKeyFrames{ nullptr };
	}CHANNELDESC;

	typedef struct tagAnimExtraData {

		_uint												iNumChannels = 0;
		_double											dTickPerSeconds = 0.0;
		_double											dDuration = 0.0;
	}ANIMEXTRADATA;

	// Animation
	typedef struct tagAnimDesc
	{
		_wstring												wstrName = L"";
		VECTOR<CHANNELDESC>		vecChannels;

		ANIMEXTRADATA								stExtraData;
	}ANIMDESC;
	// HiearchyNode
	typedef struct tagBoneNodeDesc
	{
		_float4x4	mTransformMatrix{ _float4x4::Identity };
		_float4x4	mOffsetMatrix{ _float4x4::Identity };
		_float			fDepth{ 0 };
		_wstring		wstrName = L"";
		_wstring		wstrParentsName = L"";
	}BONENODEDESC;
	// Mesh
	typedef struct tagMeshDesc : public BUFFDESC
	{
		VTXMODEL*							pVtxModel = nullptr;
	}MESHDESC;
	// Animation Mesh 
	typedef struct tagAnimMeshDesc : public BUFFDESC
	{
		VTXANIMMODEL*				pVtxModel = nullptr;
	}ANIMMESHDESC;
	// Common Model Data
	typedef struct tagCommonModelData
	{
		UNORMAP<_uint, VECTOR<_wstring>>		uomapMatrialData;
		_uint																					iMeshContainersCount = 0;
		_uint																					iMatrialCount = 0;
		_uint																					iNodeCnt = 0;

		VECTOR<BONENODEDESC>									vecBNodeDatas;
	}COMMODELDATA;
	// Model Desc
	typedef struct tagModelDesc : public COMMODELDATA
	{
		VECTOR<MESHDESC>			vecMeshes;
	}MODELDESC;
	// Model Desc
	typedef struct tagAnimModelDesc : public COMMODELDATA
	{
		VECTOR<ANIMMESHDESC>	vecMeshes;
		VECTOR<ANIMDESC>				convecAnimes;
		_uint													iAnimCount{ 0 };
	}ANIMMODELDESC;

	// ModelMaterial
	typedef struct tagModelMatrial
	{
		// Texture
		ARRAY<SHPTR<class UTexGroup>,TEXTYPE::TextureType_UNKNOWN> arrMaterialTexture{ nullptr };
	}MODELMATRIALDESC;

#pragma endregion MODEL


#pragma region ANIMMATIONFASTSECTION 

	typedef struct tagAnimFastSection
	{
		_float fStartSpot = -1.f;
		_float fEndSpot = -1.f;
		_float fFastValue = 1.f;

		void Convert(const _double& _dValue, const _double& _dTimeValue, _double& _dConvert);
		_bool IsPass(const _double& _dTimeValue);
	}ANIMFASTSECTION;

	typedef struct  ENGINE_DLL tagAnimClipSection
	{
		_float			fChange = 0.f;
		_float			fEndValue = 0.f;
		_wstring		wstrName{ L"" };
		_bool IsPass(const _wstring& _wstrName, const _double& _dTimeValue, const _double& _dSupTime);
		_bool IsBetween(const _wstring& _wstrName, const _double& _dTimeValue);
	}ANIMCLIPSECTION;

#pragma endregion ANIMATIONFASTSECTION

#pragma region BUDLECOMMANDLIST 

	typedef struct tagBundCmdGroup
	{
		Microsoft::WRL::ComPtr<Dx12GraphicsCommandList>	  cpBundleCmdList{nullptr};
		Microsoft::WRL::ComPtr<Dx12CommandAllocator>		  cpAllocator{nullptr};
	}BUNDLECMDGROUP;

#pragma endregion BUDLECOMMANDLIST

	typedef struct tagFontDesc {

		tagFontDesc() = default;
		tagFontDesc(const _wstring& _strText, const _float2& _vPos, const _float4& _vColor = { 255.f, 255.f, 255.f, 255.f },
			const _float _fRotation = 0.f, const _float2& _vScale = {}, const _float _fDepths = 0.f, const _float2& _vOrigin = {},
			const DirectX::DX12::SpriteEffects& _eSpriteEffects = DirectX::DX12::SpriteEffects_None) :
			strText{ _strText },
			vPos{ _vPos },
			vColor{ _vColor },
			fRotation{ _fRotation },
			vOrigin{ _vOrigin },
			vScale{ _vScale },
			eSpriteEffects{ _eSpriteEffects },
			fDepths{ _fDepths }
		{}

		_wstring											strText{};
		_float2											vPos{};
		_float4											vColor{ 1.f, 1.f, 1.f, 1.f };
		_float												fRotation{ 0.f };
		_float2											vOrigin{};
		_float2											vScale{ 1.f, 1.f };
		DirectX::DX12::SpriteEffects	eSpriteEffects;
		_float												fDepths{ 1.f };
	}FONTDESC;
}

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_EMGINE_STRUCT_H