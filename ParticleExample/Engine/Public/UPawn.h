#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UPAWN_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UPAWN_H

#include "UActor.h"

BEGIN(Engine)
class URenderer;
class UShader;

class ENGINE_DLL DEF_CACHE_ALGIN  UPawn  abstract : public UActor{
public:
	UPawn(CSHPTRREF<UDevice> _spDevice,  const _wstring& _wstrLayer, const CLONETYPE& _eCloneType, const BACKINGTYPE _eBackingType = BACKINGTYPE::STATIC);
	UPawn(const UPawn& _rhs);
	DESTRUCTOR(UPawn)
public:
//	CSHPTRREF<URenderer> GetRenderer() const;
	CSHPTRREF<UShader> GetShader() const;

	const 	VECTOR<_uint>& GetRenderingDatas() const { return m_vecRenderingDatas; }
#ifdef _USE_DEBUGGING
	const std::atomic<_bool>& IsDebugRenderingType() const { return m_isDebugRenderingType; }
#endif 
public:
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _vecDatas) override PURE;
	virtual void Free() override PURE;
	virtual HRESULT NativeConstruct() override PURE;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override PURE;
	// Hit 
	virtual bool IsHit(CSHPTRREF<UPawn> _pEnemy);
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override PURE;
	virtual void LateTickActive(const _double& _dTimeDelta) override PURE;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor>& _spTableDescriptor) override PURE;
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) PURE;
	// Add Shader
	void AddShader(const _wstring& _wstrProtoTag, const _wstring& _wstrTag = RES_SHADER, const VOIDDATAS& _vecDatas = VOIDDATAS{});
	void AddRenderGroup(const RENDERID _iRenderID);
#ifdef _USE_DEBUGGING
	void AddDebugRenderGroup(const DEBUGRENDERID _iRenderID);
#endif
#ifdef _USE_IMGUI
public:
	virtual void ShowObjectInfo() override;
#endif
private:
	SHPTR<URenderer>	m_spRenderer;
	SHPTR<UShader>		m_spShader;

	VECTOR<_uint>	m_vecRenderingDatas;
#ifdef _USE_DEBUGGING
	std::atomic<_bool>		m_isDebugRenderingType;
#endif 
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UPAWN_H