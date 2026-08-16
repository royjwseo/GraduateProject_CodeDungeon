#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UDEFAULTDEBUGGING_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UDEFAULTDEBUGGING_H

#include "UPawn.h"



BEGIN(Engine)
class UVIBuffer;
class UShaderConstantBuffer;

class ENGINE_DLL DEF_CACHE_ALGIN  UDefaultDebugging : public UPawn {
public:
	enum class DEBUGTYPE
	{
		DEBUG_CUBE, DEBUG_SPHERE, DEBUG_END
	};
public:
	UDefaultDebugging(CSHPTRREF<UDevice> _spDevice, 	const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UDefaultDebugging(const UDefaultDebugging& _rhs);
	DESTRUCTOR(UDefaultDebugging)

	void SetColor(const _float4 _vColor) { m_stDebuggParam.vDebugging = _vColor; }
public:
	CLONE_MACRO(UDefaultDebugging, "UDefaultDebugging::Clone To Failed")
		virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;

	void AddRenderer(RENDERID _eRenderID);
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;
private:
	static constexpr _uint							DEBUGPARAM_SIZE {sizeof(DEBUGGINPARAM)};

	SHPTR< UVIBuffer>								m_spVIBuffer;
	SHPTR< UShaderConstantBuffer>	m_spDebuggingConstantBuffer;
	DEBUGGINPARAM								m_stDebuggParam;
	DEBUGTYPE											m_eDebugType;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UDEFAULTDEBUGGING_H