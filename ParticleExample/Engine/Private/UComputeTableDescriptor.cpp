#include "EngineDefines.h"
#include "UComputeTableDescriptor.h"
#include "UDevice.h"
#include "UComputeCommand.h"

UComputeTableDescriptor::UComputeTableDescriptor() : UTableDescriptor()
{
}

void UComputeTableDescriptor::Free()
{
}

void UComputeTableDescriptor::BindUAV(const D3D12_CPU_DESCRIPTOR_HANDLE& _uavHandle, const UAV_REGISTER& _eReg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCpuHandle(_eReg);
	CopyDesc(destHandle, _uavHandle);
	// 리소스 상태 변경
}

void UComputeTableDescriptor::CommitTable(CSHPTRREF<UCommand> _spCommand)
{
	_spCommand->GetGpuCmdList()->SetComputeRootDescriptorTable(GetStaticValue(), ReturnCommitTableHandle(_spCommand));
}

const D3D12_CPU_DESCRIPTOR_HANDLE UComputeTableDescriptor::GetCpuHandle(const UAV_REGISTER& _eReg)
{
	return ComputeCpuHandle(static_cast<_uint>(_eReg));
}
