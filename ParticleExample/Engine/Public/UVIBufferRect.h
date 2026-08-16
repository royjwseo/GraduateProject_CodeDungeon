#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERRECT_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERRECT_H

#include "UVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL DEF_CACHE_ALGIN  UVIBufferRect : public UVIBuffer {
public:
	UVIBufferRect(CSHPTRREF <UDevice> _spDevice, 	const VIBUFFERTYPE _eBufferType);
	UVIBufferRect(const UVIBufferRect& _rhs);
	DESTRUCTOR(UVIBufferRect)
public:
	CLONE_THIS(UVIBufferRect)
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
private:
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERRECT_H

