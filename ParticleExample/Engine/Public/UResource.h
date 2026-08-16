#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_URESOURCE_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_URESOURCE_H

#include "UCloneObject.h"

BEGIN(Engine)

class ENGINE_DLL UResource : public UCloneObject {
public:
	UResource(CSHPTRREF<UDevice> _spDevice);
	UResource(const UResource& _rhs);
	virtual ~UResource() = default;
public:
	// UObject을(를) 통해 상속됨
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _stDatas) override PURE;

	virtual void Free() override PURE;
	virtual HRESULT NativeConstruct() override PURE;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _stDatas) override PURE;

private:
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_URESOURCE_H