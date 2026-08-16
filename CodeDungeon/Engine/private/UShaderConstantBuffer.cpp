#include "EngineDefine.h"
#include "UShaderConstantBuffer.h"
#include "UDevice.h"
#include "UGpuCommand.h"
#include "UTableDescriptor.h"
#include "UMethod.h"
#include "UCommand.h"
#include "UGameInstance.h"

UShaderConstantBuffer::UShaderConstantBuffer() :
	m_cpUploadBuffer{ NULL },
	m_cpDefaultBuffer{ NULL },
	m_cpCpuDescriptorHeap{ NULL },
	m_stD3DCpuDescriptorHandle{ NULL },
	m_iHandleIncrementSize{ 0 },
	m_pMapBuffer{ nullptr },
	m_iElementSize{ 0 },
	m_eCbvRegisterNumber{ CBV_REGISTER::CBV_END },
	m_iShaderConstantBufferIndex{ 0 },
	m_iShaderConstantBufferCpuLocation{ 0 },
	m_iElementNum{ 1 },
	m_bDataUpdated{ true },
	m_bUseDefaultBuffer{ false }
{
}

void UShaderConstantBuffer::Free()
{
}

HRESULT UShaderConstantBuffer::NativeConstruct(CSHPTRREF<UDevice> _spDevice, const CBV_REGISTER& _eReg, const _uint& _iSize, const _uint& _iNum, const _bool& UseDefault)
{
	//RETURN_CHECK(nullptr == _spDevice, E_FAIL);
	//m_iElementSize = UMethod::CalcConstantBufferByteSize(_iSize);
	//m_eCbvRegisterNumber = _eReg;
	//m_iElementNum = _iNum;

	//// Create Buffer
	//{
	//	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	//	D3D12_RESOURCE_DESC Desc = CD3DX12_RESOURCE_DESC::Buffer(m_iElementSize * m_iElementNum);
	//	RETURN_CHECK_DXOBJECT(_spDevice->GetDV()->CreateCommittedResource(&heapProperty, D3D12_HEAP_FLAG_NONE,
	//		&Desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_cpUploadBuffer)), E_FAIL);
	//	// UploadBuffer
	//	m_cpUploadBuffer->Map(0, nullptr, (void**)&m_pMapBuffer);
	//}

	//// CreateView 
	//{
	//	D3D12_DESCRIPTOR_HEAP_DESC cbvDesc{};
	//	ZeroMemory(&cbvDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	//	cbvDesc.NumDescriptors = _iNum;
	//	cbvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	//	cbvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//	RETURN_CHECK_DXOBJECT(_spDevice->GetDV()->CreateDescriptorHeap(&cbvDesc, IID_PPV_ARGS(&m_cpCpuDescriptorHeap)), E_FAIL);
	//	// CpuStart
	//	m_stD3DCpuDescriptorHandle = m_cpCpuDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	//	// IncreamentSize
	//	m_iHandleIncrementSize = _spDevice->GetDV()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//	for (_uint i = 0; i < m_iElementNum; ++i)
	//	{
	//		D3D12_CONSTANT_BUFFER_VIEW_DESC bufferViewDesc{};
	//		// 버퍼가 할당된 공간을 지정한다. 
	//		bufferViewDesc.BufferLocation = m_cpUploadBuffer->GetGPUVirtualAddress() +
	//			UINT64(i * m_iElementSize);
	//		bufferViewDesc.SizeInBytes = m_iElementSize;
	//		D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle = GetCpuDescriptorHandle(i);
	//		_spDevice->GetDV()->CreateConstantBufferView(&bufferViewDesc, cbvHandle);
	//	}
	//}
	
	RETURN_CHECK(nullptr == _spDevice, E_FAIL);
	m_iElementSize = UMethod::CalcConstantBufferByteSize(_iSize);
	m_eCbvRegisterNumber = _eReg;
	m_iElementNum = _iNum;
	m_bUseDefaultBuffer = UseDefault;

	// 업로드 버퍼 생성
	D3D12_HEAP_PROPERTIES uploadHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(m_iElementSize * m_iElementNum);

	RETURN_CHECK_DXOBJECT(_spDevice->GetDV()->CreateCommittedResource(
		&uploadHeapProp, D3D12_HEAP_FLAG_NONE,
		&bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&m_cpUploadBuffer)), E_FAIL);

	//업로드 버퍼 매핑
	m_cpUploadBuffer->Map(0, nullptr, (void**)&m_pMapBuffer);

	//if (m_bUseDefaultBuffer)
	//{
	//	// 디폴트 버퍼
	//	RETURN_CHECK_DXOBJECT(UMethod::CreateBufferResource(
	//		_spDevice->GetDV(), nullptr,
	//		m_iElementSize * m_iElementNum, nullptr,
	//		m_cpDefaultBuffer, m_cpUploadBuffer,
	//		D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER), E_FAIL);
	//}
	if (m_bUseDefaultBuffer)
	{
		D3D12_HEAP_PROPERTIES defaultHeapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		D3D12_RESOURCE_DESC defaultDesc = CD3DX12_RESOURCE_DESC::Buffer(m_iElementSize * m_iElementNum);

		RETURN_CHECK_DXOBJECT(_spDevice->GetDV()->CreateCommittedResource(
			&defaultHeapProperty, D3D12_HEAP_FLAG_NONE,
			&defaultDesc, D3D12_RESOURCE_STATE_COMMON, nullptr,
			IID_PPV_ARGS(&m_cpDefaultBuffer)), E_FAIL);
	}

	//CBV 생성
	D3D12_DESCRIPTOR_HEAP_DESC cbvDesc{};
	cbvDesc.NumDescriptors = _iNum;
	cbvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	cbvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	RETURN_CHECK_DXOBJECT(_spDevice->GetDV()->CreateDescriptorHeap(&cbvDesc, IID_PPV_ARGS(&m_cpCpuDescriptorHeap)), E_FAIL);

	//CBV Descriptor 설정
	m_stD3DCpuDescriptorHandle = m_cpCpuDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_iHandleIncrementSize = _spDevice->GetDV()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (_uint i = 0; i < m_iElementNum; ++i)
	{
		D3D12_CONSTANT_BUFFER_VIEW_DESC bufferViewDesc{};
		bufferViewDesc.BufferLocation = m_bUseDefaultBuffer ? m_cpDefaultBuffer->GetGPUVirtualAddress()+ UINT64(i * m_iElementSize): m_cpUploadBuffer->GetGPUVirtualAddress()+ UINT64(i * m_iElementSize);;
		bufferViewDesc.SizeInBytes = m_iElementSize;
		D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle = GetCpuDescriptorHandle(i);
		_spDevice->GetDV()->CreateConstantBufferView(&bufferViewDesc, cbvHandle);
	}
	return S_OK;
}

// CPU 공간에 할당되어 있는 DescriptorHeap에 Buffer를 Upload 한다. 
HRESULT UShaderConstantBuffer::PushData(CSHPTRREF<UCommand> _spCommand,CSHPTRREF< UTableDescriptor> _spTableDescriptor, const void* _pBuffer, const _uint& _iSize)
{
//	UMethod::CheckOverNumbers(m_iShaderConstantBufferIndex, m_iElementNum);
//	// 현재 인덱싱 된 사이즈 만큼 
//	_uint iIndex = m_iShaderConstantBufferIndex * m_iElementSize;
//	::memcpy(&m_pMapBuffer[iIndex], _pBuffer, _iSize);
//	_spTableDescriptor->BindCBV(GetCpuDescriptorHandle(m_iShaderConstantBufferIndex), m_eCbvRegisterNumber);
//	++m_iShaderConstantBufferIndex;

	UMethod::CheckOverNumbers(m_iShaderConstantBufferIndex, m_iElementNum);

	if (!m_pPreviousBuffer)
	{
		m_pPreviousBuffer = std::make_unique<_ubyte[]>(m_iElementSize * m_iElementNum);
		::memset(m_pPreviousBuffer.get(), 0, m_iElementSize * m_iElementNum);
	}
	// 업로드 버퍼에 데이터 복사
	_uint iIndex = m_iShaderConstantBufferIndex * m_iElementSize;
	::memcpy(&m_pMapBuffer[iIndex], _pBuffer, _iSize);

	if (m_bUseDefaultBuffer && ::memcmp(&m_pPreviousBuffer[iIndex], _pBuffer, _iSize) != 0)
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

		::memcpy(&m_pPreviousBuffer[iIndex], _pBuffer, _iSize);
	}

	// Table Descriptor에 CBV 등록
	_spTableDescriptor->BindCBV(GetCpuDescriptorHandle(m_iShaderConstantBufferIndex), m_eCbvRegisterNumber);
	++m_iShaderConstantBufferIndex;

	return S_OK;
}

D3D12_CPU_DESCRIPTOR_HANDLE UShaderConstantBuffer::GetCpuDescriptorHandle(const _uint& _iIndex)
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_stD3DCpuDescriptorHandle, m_iHandleIncrementSize * _iIndex);
}
