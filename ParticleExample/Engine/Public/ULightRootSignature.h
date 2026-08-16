#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_ULIGHTROOTSIGNATURE_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_ULIGHTROOTSIGNATURE_H

#include "URootSignature.h"

BEGIN(Engine)
class UDevice;
class UGpuCommand;

class DEF_CACHE_ALGIN ULightRootSignature final : public URootSignature {
public:
	ULightRootSignature();
	NO_COPY(ULightRootSignature)
	DESTRUCTOR(ULightRootSignature)
public:
	virtual void Free() override;
protected:
	virtual HRESULT CreateRootSignature(CSHPTRREF<UDevice> _spDevice) override;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_ULIGHTROOTSIGNATURE_H