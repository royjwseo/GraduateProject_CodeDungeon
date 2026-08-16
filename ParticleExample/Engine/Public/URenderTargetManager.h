#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_URENDERTARGETMANAGER_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_URENDERTARGETMANAGER_H

#include "UBase.h"

BEGIN(Engine)
class UGraphicDevice;
class URenderTargetGroup;
class UDevice;
class UGpuCommand;
class UTexture;
class UShaderConstantBuffer;
class UShader;
class UVIBufferRect;
class URenderObject;

class URenderTargetManager : public UBase {
public:
#ifdef _USE_DEBUGGING
	typedef struct tagRenderDebugDesc
	{
		tagRenderDebugDesc() = default;
		tagRenderDebugDesc(const RTGROUPID _eGroupID, const RTOBJID _eRtObjID, const _float4x4& _mWorldMatrix, 
			const _float2& _vSize, const _float2& _vPos)
			: eRtGroupID(_eGroupID), eRtObjID(_eRtObjID), stTransformParam({_mWorldMatrix, _mWorldMatrix ,1}), vSize(_vSize), vPos(_vPos)
		{}
		RTGROUPID					eRtGroupID{ RTGROUPID::ENDGROUP };
		RTOBJID							eRtObjID{ RTOBJID::RTOBJ_END };
		TRANSFORMPARAM	stTransformParam;
		_float2							vSize, vPos;
	}RTDEBUGDESC;
#endif
public:
	URenderTargetManager();
	NO_COPY(URenderTargetManager)
	DESTRUCTOR(URenderTargetManager)
public:
	const DXGI_FORMAT& GetDepthFormat() const { return m_eDepthFormat; }
public:
	virtual void		 Free() override;
	HRESULT		 ReadyRenderTarget(CSHPTRREF<UGraphicDevice> _spGraphicDevice,
		CSHPTRREF<URenderObject> _spGraphicRenderObject);
public:
	HRESULT		AddRenderTargetGroup(const RTGROUPID& _eGroupID, const std::vector<RTDESC>& _rtVec);
	void					RemoveRenderTargetGroup(const RTGROUPID _eGroupID);
	// OMSetRenderTarget 
	void					OmSetRenderTargets(CSHPTRREF<URenderObject> _spRenderObject, const RTGROUPID& _eGroupID, const _uint _index, const _uint _iOffset);
	void					OmSetRenderTargets(CSHPTRREF<URenderObject> _spRenderObject, const RTGROUPID& _eGroupID);
	// ClearRenderTargetView
	void					ClearRenderTargetView(CSHPTRREF<URenderObject> _spRenderObject, const RTGROUPID& _eGroupID, const _uint _index);
	void					ClearRenderTargetView(CSHPTRREF<URenderObject> _spRenderObject, const RTGROUPID& _eGroupID);
	// Waiting
	void					WaitTargetToResource(CSHPTRREF<URenderObject> _spRenderObject, const RTGROUPID& _eGroupID);
	void					WaitResourceToTarget(CSHPTRREF<URenderObject> _spRenderObject, const RTGROUPID& _eGroupID);

	// OMSetRenderTarget 
	void					OmSetRenderTargets(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID, const _uint _index, const _uint _iOffset);
	void					OmSetRenderTargets(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID);
	// ClearRenderTargetView
	void					ClearRenderTargetView(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID, const _uint _index);
	void					ClearRenderTargetView(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID);
	// Waiting
	void					WaitTargetToResource(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID);
	void					WaitResourceToTarget(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID);

	// Get RT Group 
	SHPTR<URenderTargetGroup> FindRenderTargetGroup(const RTGROUPID& _eGroupID);
	SHPTR<UTexture> FindRenderTargetTexture(const RTGROUPID _eGroupID, const RTOBJID _eObjID);

	void					OnResizeWindow(CSHPTRREF<UGraphicDevice> _spGraphicDevice);
#ifdef _USE_DEBUGGING
	void AddDebugRenderObjects(const RTGROUPID _eGroupID, const RTOBJID _eRtObj, const _float2& _vPos, const _float2& _vScale,
		CSHPTRREF<GRAPHICDESC> _spGraphicDesc);
	void RenderDebugObjects(CSHPTRREF<UShader> _spShader, CSHPTRREF<UVIBufferRect> _spBuffer,
		CSHPTRREF<UCommand> _spCommand, const SRV_REGISTER _eSrvRegister);
#endif
private:
	// Find Group
	SHPTR<URenderTargetGroup> FindRtGroup(const RTGROUPID& _eGroupID);
	void CreateDepthStencilTexture();
	void CreateDefaultRenderTargets(CSHPTRREF<UGraphicDevice> _spGraphicDevice);
#ifdef _USE_DEBUGGING
	void OnResizeDebugRenderObject(CSHPTRREF<UGraphicDevice> _spGraphicDevice);
#endif
private:
	using RTGROUPS = UNORMAP<RTGROUPID, SHPTR<URenderTargetGroup>>;
	RTGROUPS											m_RenderTargetGroups;
	// DepthStencilTexture
	SHPTR<UTexture>								m_spDepthStencilTexture;
	// DxObject
	SHPTR<URenderObject>					m_spGraphicrenderObject;
	//Graphics
	SHPTR<GRAPHICDESC>						m_spGraphicDesc;
	DXGI_FORMAT										m_eDepthFormat;
#ifdef _USE_DEBUGGING
	using RTOBJS = VECTOR<RTDEBUGDESC>;
	RTOBJS													m_vecRtObjects;
	SHPTR< UShaderConstantBuffer>	m_spTransformConstantBuffer;
#endif
};

END


#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_URENDERTARGETMANAGER_H
