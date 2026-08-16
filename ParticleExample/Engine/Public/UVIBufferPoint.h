#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERPOINT_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERPOINT_H

#include "UVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL DEF_CACHE_ALGIN  UVIBufferPoint : public UVIBuffer {
public:
	UVIBufferPoint(CSHPTRREF <UDevice> _spDevice);
	UVIBufferPoint(const UVIBufferPoint& _rhs);
	DESTRUCTOR(UVIBufferPoint)
public:
	CLONE_THIS(UVIBufferPoint)
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _convecDatas) override;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERPOINT_H