#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCOMPUTECOMMAND_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCOMPUTECOMMAND_H

#include "UCommand.h"

BEGIN(Engine)
class UDevice;

class DEF_CACHE_ALGIN UComputeCommand : public UCommand {
public:
	using ALLOCATORARR = ARRAY<ComPtr<Dx12CommandAllocator>, 5>;
public:
	UComputeCommand();
	NO_COPY(UComputeCommand)
	DESTRUCTOR(UComputeCommand)
public:
	virtual void Free() override;
	virtual HRESULT NativeConstruct(CSHPTRREF<UDevice> _spDevice, const ComPtr<Dx12CommandQueue>& _cpCommandQueue) override;
	virtual void WaitForSynchronization() override;
	// Begin Bind
	virtual void BindRootSignature(CSHPTRREF<URootSignature> _spRootSignature) override;
	virtual void Clear() override;
/* Variable */
private:
	ALLOCATORARR		m_arrRenderAllocators;
	_uint							m_iAllocatorIndex;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCOMPUTECOMMAND_H