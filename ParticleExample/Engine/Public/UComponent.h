#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCOMPONENT_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCOMPONENT_H

#include "UCloneObject.h"

BEGIN(Engine)

class ENGINE_DLL UComponent abstract  : public UCloneObject{
public:
	UComponent(CSHPTRREF<UDevice> _spDevice);
	UComponent(const UComponent& _rhs);
	virtual ~UComponent() = default;
public:
	virtual void Free() override PURE;
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _tDatas) PURE;
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override PURE;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override PURE;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCOMPONENT_H