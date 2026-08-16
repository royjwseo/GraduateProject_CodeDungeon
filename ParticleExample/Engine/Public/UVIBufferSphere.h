#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERSPHERE_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERSPHERE_H

#include "UVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL DEF_CACHE_ALGIN   UVIBufferSphere : public UVIBuffer {
public:
	UVIBufferSphere(CSHPTRREF <UDevice> _spDevice,	const VIBUFFERTYPE _eBufferType);
	UVIBufferSphere(const UVIBufferSphere& _rhs);
	DESTRUCTOR(UVIBufferSphere)
public:
	CLONE_THIS(UVIBufferSphere)
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERSPHERE_H