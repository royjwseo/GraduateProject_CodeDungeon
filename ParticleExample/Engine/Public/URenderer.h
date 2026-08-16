#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_URENDERER_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_URENDERER_H

#include "UComponent.h"

BEGIN(Engine)
class UShader;
class UComponent;
class UPipeLine;
class USceneManager;
class UVIBufferRect;
class URenderTargetManager;
class URenderHandler;
class UDefferedCamera;
class UShaderConstantBuffer;
class UGraphicDevice;
class UGrobalConstantBuffer;
class UComputeManager;
class UPawn;
class URenderObject;
class URenderTargetGroup;
class UGrobalConstantBuffer;

class ENGINE_DLL DEF_CACHE_ALGIN  URenderer : public UComponent {
	using PAWNLIST = LIST<SHPTR<UPawn>>;
	using DRAWINGRENDERCONTAINER = UNORMAP<_wstring, PAWNLIST>;
	using SHADERODERS = UNORMAP<_wstring, SHPTR<UShader>>;
public:
#ifdef _USE_DEBUGGING
	typedef struct tagRenderDebugging
	{
		_bool		isShowRenderTarget{ false };
		_bool		isShowComponents{ false };
	}RENDERDEBUGGING;
#endif

	URenderer(CSHPTRREF <UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand,
		CSHPTRREF<UGraphicDevice> _spGraphicDevice, 	CSHPTRREF<UPipeLine> _spPipeLine, 
		CSHPTRREF<USceneManager> _spSceneManager, CSHPTRREF<URenderTargetManager> _spRenderTargetManager, 
		CSHPTRREF<UComputeManager> _spComputeManager, CSHPTRREF<URenderObject> _spRenderObject);
	NO_COPY(URenderer)
	DESTRUCTOR(URenderer)
public:
#ifdef _USE_DEBUGGING
	RENDERDEBUGGING& GetRenderDebugging() { return m_stRenderDebugging; }
#endif
public:
	CLONE_THIS(URenderer)
	virtual void Free() override;
public:
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vectDatas) override;

	HRESULT AddRenderGroup(const RENDERID _eID, CSHPTRREF<UShader> _spShader, CSHPTRREF<UPawn> _spPawn);
#ifdef _USE_DEBUGGING
	HRESULT AddDebugRenderGroup(const DEBUGRENDERID _eID, 
		CSHPTRREF<UShader> _spShader, CSHPTRREF<UPawn> _spPawn);
#endif
	void Tick(const _double& _dTimeDelta);
	HRESULT Render();
	void ClearRenderingData();
	void ClearAllData();
private:
	// Priority
	void RenderPriority();
	// Shadow
	void	RenderShadowLight();
	// NonAlpha
	void RenderNonAlphaBlend();
	// Render Lights
	void RenderLights();
	// Render Blend
	void RenderBlend();
	// Non Light
	void RenderNonLight();
	// Alpha Blend
	void RenderAlphaBlend();
	// Distortion
	void RenderDistortion();
	// Render 3D UI
	void Render3DUI();
	// UI 
	void Render2DUI();
	// HDR
	void RenderHDR();
	// Render End 
	void RenderEnd();
#ifdef _USE_DEBUGGING
	void RenderDebug();
#endif
	SHPTR<UShader> FindShader(const _wstring& _wstrProto);
	// Draw Just Object
	void RenderObject(const _wstring& _wstrShaderName, PAWNLIST& _PawnList);
	void RenderObject(const _wstring& _wstrShaderName, PAWNLIST& _PawnList, CSHPTRREF<URenderObject> _spRenderObject);

	SHPTR<UShader> FrameReadyDrawFinalRenderTarget(const _wstring& _wstrShaderName, const RTGROUPID _eGroupID);
	SHPTR<UShader> FrameReadyDrawLast(const _wstring& _wstrShaderName);

	void BindGrobalBuffer(CSHPTRREF<URenderObject> _spRenderObject);
private:
	GROBALPARAM																									m_stGrobalParam;
	SHPTR< UGrobalConstantBuffer>																	m_spGrobalBuffer;
	constexpr static _uint																							GROBALBUFFER_SIZE{ sizeof(GROBALPARAM) };
	// Deffered Transform Param		
	TRANSFORMPARAM																							 m_stFinalRenderTransformParam;
	SHPTR<UShaderConstantBuffer>																	 m_spTransformConstantBuffer;
	SHPTR<UDefferedCamera>																				 m_spDefferedCamera;
	// Actives 																												 
	ARRAY<DRAWINGRENDERCONTAINER, RI_END>											m_arrActiveDrawRenderList;
	//Shader Container 																							   
	SHADERODERS																									m_ShaderObjects;
	// Manager																											  
	SHPTR<UGraphicDevice>																					m_spGraphicDevice;
	SHPTR<UPipeLine>																								m_spPipeLine;
	SHPTR<USceneManager>																					m_spSceneManager;
	SHPTR<URenderTargetManager>																	m_spRenderTargetManager;
	SHPTR<UComputeManager>																			m_spComputeManager;
	SHPTR<URenderObject>																					m_spGraphicsRenderObject;
	SHPTR<UVIBufferRect>																						m_spVIBufferRect;
	_short																														m_sNonAlphaBlendIndex;
#ifdef _USE_DEBUGGING																					    
	alignas(64)  ARRAY<DRAWINGRENDERCONTAINER, DEBUG_RI_END>		m_arrDrawDebugRenderList;
	RENDERDEBUGGING																							 m_stRenderDebugging;
#endif
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_URENDERER_H