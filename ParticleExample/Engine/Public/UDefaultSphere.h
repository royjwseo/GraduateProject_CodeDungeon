#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UDEFAULTSPHERE_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UDEFAULTSPHERE_H

#include "UPawn.h"

BEGIN(Engine)
class UVIBufferSphere;

class ENGINE_DLL DEF_CACHE_ALGIN  UDefaultSphere : public UPawn {
public:
	UDefaultSphere(CSHPTRREF<UDevice> _spDevice,  const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UDefaultSphere(const UDefaultSphere& _rhs);
	DESTRUCTOR(UDefaultSphere)
public:
	CLONE_MACRO(UDefaultSphere, "UDefaultSphere::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;
private:
	// Cube
	SHPTR< UVIBufferSphere>		m_spVIBufferSphere;

};

END


#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UDEFAULTSPHERE_H