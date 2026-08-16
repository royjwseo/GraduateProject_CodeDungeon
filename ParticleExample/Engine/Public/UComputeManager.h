#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCOMPUTEMANAGER_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCOMPUTEMANAGER_H

#include "UBase.h"

BEGIN(Engine)
class UComputeShader;
class UComputeCommand;
class UTexture;
class UGraphicDevice;
class UDevice;
class UGpuCommand;
class UVIBufferRect;
class UShader;
class UShaderConstantBuffer;
class URenderObject;

class UComputeManager : public UBase {
public:
	enum TEXTUREORDER
	{
		COMPUTE_ANIMTEXTURE, TEXTURE_END
	};

#ifdef _USE_DEBUGGING
	typedef struct tagComputeDebugDesc
	{
		tagComputeDebugDesc() = default;
		tagComputeDebugDesc(const TEXTUREORDER& _eTextureOrder, const _float4x4& _mWorldMatrix,
			const _float2& _vSize, const _float2& _vPos)
			: eTextureOrder(_eTextureOrder), stTransformParam({_mWorldMatrix, _mWorldMatrix ,1}), vSize(_vSize), vPos(_vPos)
		{}
		TEXTUREORDER			eTextureOrder;
		TRANSFORMPARAM	stTransformParam;
		_float2							vSize, vPos;
	}COMPUTEDEBUGDESC;
#endif
	using COMPUTESHADERS = UNORMAP<_wstring, SHPTR<UComputeShader>>;
	using TEXTURES = VECTOR<SHPTR<UTexture>>;
public:
	UComputeManager();
	NO_COPY(UComputeManager)
	DESTRUCTOR(UComputeManager)

public:
	virtual void Free() override;
	HRESULT NativeComputeManager(CSHPTRREF<URenderObject> _spGpuRenderObject);
	HRESULT ReadyComputeManager(CSHPTRREF<UGraphicDevice> _spGraphicDevice);
	// Compute Texture
	void ComputeTexture(const TEXTUREORDER& _eOrder);

#ifdef _USE_DEBUGGING
	void OnResizeDebugRenderObject(CSHPTRREF<UGraphicDevice> _spGraphicDevice);
#endif

private:
	SHPTR<UComputeShader> FindShader(const _wstring& _wstrShaderProtoName);
#ifdef _USE_DEBUGGING
	void AddDebugingObject(const TEXTUREORDER& _eTextureOrder, const _float2& _vPos, const _float2& _vScale,
		CSHPTRREF<GRAPHICDESC> _spGraphicDesc);
	void RenderDebugObjects();
#endif 
private:
	// Compute
	SHPTR<UDevice>										m_spDevice;
	SHPTR<UGpuCommand>						m_spGpuCommand;
	SHPTR<URenderObject>						m_spComputeRenderObject;
	// Map
	COMPUTESHADERS								m_ComputeShader;
	// Textures
	TEXTURES													m_vecTextures;
	// Using Debug
#ifdef _USE_DEBUGGING
	using COMPUTEDEBUGS = VECTOR<COMPUTEDEBUGDESC>;
	COMPUTEDEBUGS									m_vecComputeDebugs;
	SHPTR< UShaderConstantBuffer>		m_spTransformConstantBuffer;
	SHPTR<UVIBufferRect>							m_spVIBufferRect;
	SHPTR<UShader>									m_spFinalShader;
#endif 
};

END


#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCOMPUTEMANAGER_H