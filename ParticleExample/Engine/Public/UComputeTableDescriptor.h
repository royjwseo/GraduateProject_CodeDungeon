#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCOMPUTETABLEDESCRIPTOR_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCOMPUTETABLEDESCRIPTOR_H

#include "UTableDescriptor.h"

BEGIN(Engine)
class UDevice;

class DEF_CACHE_ALGIN UComputeTableDescriptor  final  : public UTableDescriptor {
public:
	UComputeTableDescriptor();
	NO_COPY(UComputeTableDescriptor)
	DESTRUCTOR(UComputeTableDescriptor)

public:
	virtual void Free() override;
	// Set 
	void BindUAV(const D3D12_CPU_DESCRIPTOR_HANDLE& _uavHandle, const UAV_REGISTER& _eReg);
	virtual void CommitTable(CSHPTRREF<UCommand> _spCommand) override;
	const D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(const UAV_REGISTER& _eReg);
	using UTableDescriptor::GetCpuHandle;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCOMPUTETABLEDESCRIPTOR_H