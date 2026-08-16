#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UTABLEDESCRIPTOR_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UTABLEDESCRIPTOR_H

#include "UBase.h"

BEGIN(Engine)
class UCommand;

class ENGINE_DLL  DEF_CACHE_ALGIN UTableDescriptor abstract : public UBase {
public:
	UTableDescriptor();
	NO_COPY(UTableDescriptor)
	DESTRUCTOR(UTableDescriptor)
public:
	const ComPtr<Dx12DescriptorHeap>& GetDescriptorHeap() const { return m_cpDescriptorHeap; }
	virtual void Free() override;
	 HRESULT NativeConstruct(CSHPTRREF<UDevice> _spDevice, 
		const _uint _iCnt, const _uint _iReigsterCnt = REGISTER_CNT, const _uint _iStatic = 0);
	 void BindAndClear(CSHPTRREF<UCommand> _spCommand) ;
	 void Clear();

	 void BindCBV(const D3D12_CPU_DESCRIPTOR_HANDLE& _hCbvHandle, const CBV_REGISTER& _eCbvRegister);
	void BindSRV(const D3D12_CPU_DESCRIPTOR_HANDLE& _hSrvHandle, const SRV_REGISTER& _eSrvRegister);

	virtual void CommitTable(CSHPTRREF<UCommand> _spCommand) PURE;

	const D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(const CBV_REGISTER& _eCbvRegister);
	const D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(const  SRV_REGISTER& _eSrvRegister);

	void ReComputeVariable(const _uint _iReigsterCnt = REGISTER_CNT, const _uint _iStatic = 0);
protected:
	CSHPTRREF<UDevice> GetDevice() const { return m_spDevice; }

	HRESULT CreateDescriptor(const D3D12_DESCRIPTOR_HEAP_DESC& _DescriptorDesc);
	void CopyDesc(const D3D12_CPU_DESCRIPTOR_HANDLE& _hDescHandle, const D3D12_CPU_DESCRIPTOR_HANDLE& _hSrvHandle);
	D3D12_GPU_DESCRIPTOR_HANDLE ReturnCommitTableHandle(CSHPTRREF<UCommand> _spCommand);

	const _uint& GetStaticValue() const { return m_iStaticValue; }
	const D3D12_CPU_DESCRIPTOR_HANDLE ComputeCpuHandle(_uint _iReg);
private:
	/* DxValue */
	SHPTR<UDevice>									m_spDevice;
	/* Descriptor Value  */
	ComPtr<Dx12DescriptorHeap>		m_cpDescriptorHeap;
	_uint														m_iHandleSize;
	_uint														m_iGroupSize;
	_uint														m_iGroupCnt;
	_uint														m_iCurrentGroupIndex;
	_uint														m_iStaticValue;

	constexpr static _uint						DEST_RANGE{1};
	constexpr static _uint						SRC_RANGE{1};
	constexpr static _uint						COMMAND_BIND_RANGE{1};
};

END

#endif  // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UTABLEDESCRIPTOR_H
