#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERSKYBOX_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERSKYBOX_H
#include "UVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL DEF_CACHE_ALGIN UVIBufferSkyBox : public UVIBuffer{
public:
	UVIBufferSkyBox(CSHPTRREF <UDevice> _spDevice);
	UVIBufferSkyBox(const UVIBufferSkyBox& _rhs);
	DESTRUCTOR(UVIBufferSkyBox)
public:
	CLONE_THIS(UVIBufferSkyBox)
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _stDatas) override;
};

END

#endif
