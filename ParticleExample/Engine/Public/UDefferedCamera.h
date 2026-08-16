#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UDEFFEREDCAMERA_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UDEFFEREDCAMERA_H

#include "UCamera.h"

BEGIN(Engine)

class   DEF_CACHE_ALGIN  UDefferedCamera : public UCamera {
public:
	UDefferedCamera(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UDefferedCamera(const UDefferedCamera& _rhs);
	DESTRUCTOR(UDefferedCamera)

public:
	virtual void Free() override;
	CLONE_MACRO(UDefferedCamera, "UDefferedCamera::Clone To Failed")
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta)  override;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UDEFFEREDCAMERA_H