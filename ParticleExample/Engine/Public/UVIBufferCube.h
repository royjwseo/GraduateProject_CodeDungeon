#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERCUBE_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERCUBE_H

#include "UVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL DEF_CACHE_ALGIN  UVIBufferCube : public UVIBuffer {
public:
	UVIBufferCube(CSHPTRREF <UDevice> _spDevice, 	const VIBUFFERTYPE& _eCubeBufferType);
	UVIBufferCube(const UVIBufferCube& _rhs);
	DESTRUCTOR(UVIBufferCube)
public:
	CLONE_THIS(UVIBufferCube)
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _stDatas) override;
};

END


#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERCUBE_H