#include "pch.h"
#include "RootSignature.h"
#include "Engine.h"

void RootSignature::InitRootSignature(ComPtr<ID3D12Device>& _Device) {
	CreateGraphicsRootSignature(_Device);
}

void RootSignature::CreateGraphicsRootSignature(ComPtr<ID3D12Device>& _Device)
{
	//예시
	CD3DX12_DESCRIPTOR_RANGE pd3dDescriptorRanges[1]{};
	pd3dDescriptorRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 7, 6, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND);
	// b6부터 b13까지 7개 디스크립터 사용 TexturedModel SRV들


	CD3DX12_ROOT_PARAMETER pd3dRootParameters[3]{};
	pd3dRootParameters[0].InitAsConstantBufferView(1); // b1 Camera관련
	//pd3dRootParameters[0].InitAsConstants(32, 1, 0, D3D12_SHADER_VISIBILITY_ALL); //Camera관련.
	pd3dRootParameters[1].InitAsConstants(17, 2, 0, D3D12_SHADER_VISIBILITY_ALL); //GameObject관련.
	pd3dRootParameters[2].InitAsDescriptorTable(1, pd3dDescriptorRanges);	//텍스처 
	
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc[1]{};
	samplerDesc[0].Init(
		0,								 				// ShaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, 				// filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP, 				// addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP, 				// addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP, 				// addressW
		0.0f,											// mipLODBias
		1,												// maxAnisotropy
		D3D12_COMPARISON_FUNC_ALWAYS,					// comparisonFunc
		D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK,	// borderColor
		0.0f,											// minLOD
		D3D12_FLOAT32_MAX,								// maxLOD
		D3D12_SHADER_VISIBILITY_PIXEL,					// shaderVisibility
		0												// registerSpace
	);
	
	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	CD3DX12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	d3dRootSignatureDesc.Init(_countof(pd3dRootParameters), pd3dRootParameters,_countof(samplerDesc), samplerDesc, d3dRootSignatureFlags);


	ComPtr<ID3DBlob> pd3dSignatureBlob, pd3dErrorBlob;
	::D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	DX::ThrowIfFailed(DEVICE->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_pd3dGraphicsRootSignature)));


}
