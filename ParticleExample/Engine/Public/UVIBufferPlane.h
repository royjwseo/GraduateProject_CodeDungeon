#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERPLANE_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERPLANE_H

#include "UVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL DEF_CACHE_ALGIN  UVIBufferPlane  : public UVIBuffer{
public:
	UVIBufferPlane(CSHPTRREF <UDevice> _spDevice, const VIBUFFERTYPE& _eBufferType);
	UVIBufferPlane(const UVIBufferPlane& _rhs);
	DESTRUCTOR(UVIBufferPlane)
public:
	CLONE_THIS(UVIBufferPlane)
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERPLANE_H