#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCOMPUTEROOTSIGNATURE_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCOMPUTEROOTSIGNATURE_H

#include "URootSignature.h"

BEGIN(Engine)

class DEF_CACHE_ALGIN UComputeRootSignature final : public URootSignature {
public:
	UComputeRootSignature();
	NO_COPY(UComputeRootSignature)
	DESTRUCTOR(UComputeRootSignature)
public:
	virtual void Free() override;
protected:
	virtual HRESULT CreateRootSignature(CSHPTRREF<UDevice> _spDevice);
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCOMPUTEROOTSIGNATURE_H
