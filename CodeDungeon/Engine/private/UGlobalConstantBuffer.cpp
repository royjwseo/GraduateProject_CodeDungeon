#include "EngineDefine.h"
#include "UGlobalConstantBuffer.h"
#include "UDevice.h"
#include "UGpuCommand.h"
#include "UGameInstance.h"
#include "UGpuCommand.h"
#include "UMethod.h"

UGlobalConstantBuffer::UGlobalConstantBuffer() :
	m_cpUploadBuffer{ NULL },
	m_cpDefaultBuffer{ NULL },
	m_cpCpuDescriptorHeap{ NULL },
	m_cpCpuDescriptorHandle{ NULL },
	m_iElementSize{ 0 },
	m_iCbvRegisterNumber{ 0 },
	m_bUseDefaultBuffer{ false },
	m_bCopiedUploadToDefaultOnce{ false }
{
}

void UGlobalConstantBuffer::Free()
{
}

HRESULT UGlobalConstantBuffer::NativeConstruct(CSHPTRREF<UDevice> _spDevice, const CBV_REGISTER& _eReg, const _uint& _iSize, const _bool& UseDefault)
{
	RETURN_CHECK(nullptr == _spDevice, E_FAIL);
	m_iElementSize = UMethod::CalcConstantBufferByteSize(_iSize);
	m_iCbvRegisterNumber = (_uint)(_eReg);
	m_bUseDefaultBuffer = UseDefault;
	// Create Buffer
	{
		D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC Desc = CD3DX12_RESOURCE_DESC::Buffer(m_iElementSize);
		RETURN_CHECK_DXOBJECT(_spDevice->GetDV()->CreateCommittedResource(&heapProperty, D3D12_HEAP_FLAG_NONE,
			&Desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_cpUploadBuffer)), E_FAIL);
		// UploadBuffer
		m_cpUploadBuffer->Map(0, nullptr, (void**)&m_pMapBuffer);
	}
	//if (m_bUseDefaultBuffer)
	//{
	//	// 디폴트 버퍼
	//	RETURN_CHECK_DXOBJECT(UMethod::CreateBufferResource(
	//		_spDevice->GetDV(), nullptr,
	//		m_iElementSize, nullptr,
	//		m_cpDefaultBuffer, m_cpUploadBuffer,
	//		D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER), E_FAIL);
	//}
	if (m_bUseDefaultBuffer)
	{
		D3D12_HEAP_PROPERTIES defaultHeapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		D3D12_RESOURCE_DESC defaultDesc = CD3DX12_RESOURCE_DESC::Buffer(m_iElementSize);

		RETURN_CHECK_DXOBJECT(_spDevice->GetDV()->CreateCommittedResource(
			&defaultHeapProperty, D3D12_HEAP_FLAG_NONE,
			&defaultDesc, D3D12_RESOURCE_STATE_COMMON, nullptr,
			IID_PPV_ARGS(&m_cpDefaultBuffer)), E_FAIL);
	}
	// CreateView 
	{
		D3D12_DESCRIPTOR_HEAP_DESC cbvDesc{};
		ZeroMemory(&cbvDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
		cbvDesc.NumDescriptors = 1;
		cbvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		cbvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		RETURN_CHECK_DXOBJECT(_spDevice->GetDV()->CreateDescriptorHeap(&cbvDesc, IID_PPV_ARGS(&m_cpCpuDescriptorHeap)), E_FAIL);
		// CpuStart
		m_cpCpuDescriptorHandle = m_cpCpuDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

		D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_cpCpuDescriptorHandle, 0);
		D3D12_CONSTANT_BUFFER_VIEW_DESC bufferViewDesc{};
		bufferViewDesc.BufferLocation = m_bUseDefaultBuffer ? m_cpDefaultBuffer->GetGPUVirtualAddress() : m_cpUploadBuffer->GetGPUVirtualAddress();
		bufferViewDesc.SizeInBytes = m_iElementSize;
		_spDevice->GetDV()->CreateConstantBufferView(&bufferViewDesc, cbvHandle);
	}
	return S_OK;
}

void UGlobalConstantBuffer::SettingGlobalData(CSHPTRREF<UCommand> _spCommand, const void* _pBuffer, const _uint _iSize)
{
	RETURN_CHECK(nullptr == _spCommand, ;);
	RETURN_CHECK(m_iElementSize != ((_iSize + 255) & ~255), ;);

	::memcpy(&m_pMapBuffer[0], _pBuffer, _iSize);

	if (m_bUseDefaultBuffer && !m_bCopiedUploadToDefaultOnce)
	{
		CD3DX12_RESOURCE_BARRIER transitionToCopy = CD3DX12_RESOURCE_BARRIER::Transition(
			m_cpDefaultBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_COPY_DEST
		);
		_spCommand->GetGpuCmdList()->ResourceBarrier(1, &transitionToCopy);

		_spCommand->GetGpuCmdList()->CopyResource(m_cpDefaultBuffer.Get(), m_cpUploadBuffer.Get());

		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			m_cpDefaultBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
		);
		_spCommand->GetGpuCmdList()->ResourceBarrier(1, &barrier);

		m_bCopiedUploadToDefaultOnce = true;
	}
	
	
	_spCommand->GetGpuCmdList()->SetGraphicsRootConstantBufferView(m_iCbvRegisterNumber,
		m_bUseDefaultBuffer ? m_cpDefaultBuffer->GetGPUVirtualAddress() : m_cpUploadBuffer->GetGPUVirtualAddress());
}