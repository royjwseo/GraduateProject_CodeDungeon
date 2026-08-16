#include "EngineDefines.h"
#include "URootSignature.h"
#include "UDevice.h"
#include "UGpuCommand.h"

URootSignature::URootSignature() :
	m_iRegisterSpace{0},
	m_cpRootSignature{ NULL },
	m_arrSamplerDescs{}
{
}

void URootSignature::Free()
{
}

HRESULT URootSignature::NativeConstruct(CSHPTRREF<UDevice> _spDevice, const _uint _iRegisterSpace)
{
	RETURN_CHECK(nullptr == _spDevice, E_FAIL);
	m_iRegisterSpace = _iRegisterSpace;
	NativeSampler();
	// GraphicRoot
	RETURN_CHECK_FAILED(CreateRootSignature(_spDevice), E_FAIL);
	return S_OK;
}

void URootSignature::NativeSampler()
{	
	// Sampler
	{
		m_arrSamplerDescs[S0] = CD3DX12_STATIC_SAMPLER_DESC(S0);
		m_arrSamplerDescs[S1] = CD3DX12_STATIC_SAMPLER_DESC(S1, // 샘플러 레지스터 슬롯
			D3D12_FILTER_MIN_MAG_MIP_LINEAR, // 필터 설정
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // U 축 주소 모드
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // V 축 주소 모드
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP  // W 축 주소 모드
		);
	}

	for (auto& iter : m_arrSamplerDescs)
	{
		iter.RegisterSpace = GetRegisterSpace();
	}
}

HRESULT URootSignature::CreateRootSignature(CSHPTRREF<UDevice> _spDevice)
{
	{
		// B0 는 전역 상태
		ARRAY< CD3DX12_DESCRIPTOR_RANGE, 2> Ranges =
		{
			// Constant Buffer 값을 뺀 것이다. 
			CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBV_REGISTER_END - GRAPHICS_CONSTANT_BUFFER_VALUE,
			GRAPHICS_CONSTANT_BUFFER_VALUE, GetRegisterSpace()), 

			CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, SRV_REGISTER_END, 0, GetRegisterSpace()), 
		};

		// ConstantBuffer View를 만들고
		const _uint PARAM_SIZE = 3;
		ARRAY<CD3DX12_ROOT_PARAMETER, PARAM_SIZE> Param = {};
		Param[0].InitAsConstantBufferView(static_cast<_uint>(CBV_REGISTER::B0));
		Param[1].InitAsConstantBufferView(static_cast<_uint>(CBV_REGISTER::B1));
		// DescriptorTable을 Range 만큼 만든다. 
		Param[2].InitAsDescriptorTable(static_cast<_uint>(Ranges.size()), Ranges.data()); // 0번 -> b0 -> CBV

		for (auto& iter : Param)
		{
			iter.Constants.RegisterSpace = GetRegisterSpace();
			iter.Descriptor.RegisterSpace = GetRegisterSpace();
		}

		D3D12_ROOT_SIGNATURE_DESC tDesc = CD3DX12_ROOT_SIGNATURE_DESC(static_cast<_uint>(Param.size()), Param.data(),
			static_cast<_uint>(m_arrSamplerDescs.size()), m_arrSamplerDescs.data());

		// 입력 조립기 단계 사용
		tDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // 입력 조립기 단계
		
		ComPtr<DxBlob> BlobSignature = nullptr;
		ComPtr<DxBlob> BlobError = nullptr;
		// RootSignature 상태를 알리고 
		RETURN_CHECK_DXOBJECT(::D3D12SerializeRootSignature(&tDesc, D3D_ROOT_SIGNATURE_VERSION_1,
			&BlobSignature, &BlobError), E_FAIL);
		RETURN_CHECK(nullptr != BlobError, E_FAIL);
		// CreateRootSignature 
		RETURN_CHECK_DXOBJECT(_spDevice->GetDV()->CreateRootSignature(0, BlobSignature->GetBufferPointer(),
			BlobSignature->GetBufferSize(), IID_PPV_ARGS(&m_cpRootSignature)), E_FAIL);
	}
	return S_OK;
}

HRESULT URootSignature::Create(CSHPTRREF<UDevice> _spDevice, const ComPtr<DxBlob>& _spBlob)
{
	RETURN_CHECK_DXOBJECT(_spDevice->GetDV()->CreateRootSignature(0, _spBlob->GetBufferPointer(),
		_spBlob->GetBufferSize(), IID_PPV_ARGS(&m_cpRootSignature)), E_FAIL);

	return S_OK;
}
