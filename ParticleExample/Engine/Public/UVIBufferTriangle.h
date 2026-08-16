#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERTRIANGLE_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERTRIANGLE_H

#include "UVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL DEF_CACHE_ALGIN  UVIBufferTriangle : public UVIBuffer {
public:
	UVIBufferTriangle(CSHPTRREF <UDevice> _spDevice);
	UVIBufferTriangle(const UVIBufferTriangle& _rhs);
	DESTRUCTOR(UVIBufferTriangle)
public:
	CLONE_THIS(UVIBufferRect)
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERTRIANGLE_H