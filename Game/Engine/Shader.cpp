#include "pch.h"
#include "Shader.h"

CShader::CShader()
{

}

//2024-01-15 이성현 
//컴파일된 cso에서 쉐이더 정보 읽는 함수 추가. 앞으론 이 함수를 통해 쉐이더를 생성하는 것을 지향한다.
D3D12_SHADER_BYTECODE CShader::ReadCompiledShaderFromFile(const wchar_t* pszFileName, ComPtr<ID3DBlob>& pd3dShaderBlob)
{
	ifstream file(pszFileName, ios::binary | ios::ate);
	if (!file.is_open())
	{
		// 파일 열기 실패 처리
		return {}; // 빈 D3D12_SHADER_BYTECODE 반환
	}

	streamsize size = file.tellg();
	file.seekg(0, ios::beg);

	vector<char> buffer(size);
	if (!file.read(buffer.data(), size))
	{
		// 파일 읽기 실패 처리
		return {}; // 빈 D3D12_SHADER_BYTECODE 반환
	}
	// Create a smart pointer to manage the blob
	pd3dShaderBlob.Reset();
	DX::ThrowIfFailed(D3DCreateBlob(size, pd3dShaderBlob.GetAddressOf()));
	std::memcpy(pd3dShaderBlob->GetBufferPointer(), buffer.data(), size);

	D3D12_SHADER_BYTECODE d3dShaderByteCode = {};
	d3dShaderByteCode.BytecodeLength = size;
	d3dShaderByteCode.pShaderBytecode = pd3dShaderBlob->GetBufferPointer();

	return d3dShaderByteCode;
}

//그래픽스 파이프라인 상태 객체를 생성한다.
CShader::CShader(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12RootSignature>& _RootSignature, const wstring& vs, const wstring& ps)
{
	ComPtr<ID3DBlob> pd3dVertexShaderBlob, pd3dPixelShaderBlob;

	ReadCompiledShaderFromFile(vs.c_str(), pd3dVertexShaderBlob);
	ReadCompiledShaderFromFile(ps.c_str(), pd3dPixelShaderBlob);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = _RootSignature.Get();
	d3dPipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(pd3dVertexShaderBlob.Get());
	d3dPipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(pd3dPixelShaderBlob.Get());
	d3dPipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.InputLayout = { m_pd3dInputLayouts.data(), (UINT)m_pd3dInputLayouts.size() };
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	_Device->CreateGraphicsPipelineState(&d3dPipelineStateDesc,
		IID_PPV_ARGS(&m_pd3dPipelineStates));
}

CShader::~CShader()
{
}


void CShader::SetPipelineState(const ComPtr<ID3D12GraphicsCommandList>& _CommandList)
{
	//파이프라인에 그래픽스 상태 객체를 설정한다. 
	_CommandList->SetPipelineState(m_pd3dPipelineStates.Get());
}

void CShader::Render(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, CCamera* pCamera)
{
	SetPipelineState(_CommandList);
}

CDiffusedFilledShader::CDiffusedFilledShader(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12RootSignature>& _RootSignature, const wstring& vs, const wstring& ps)
{
	m_pd3dInputLayouts.clear();
	m_pd3dInputLayouts =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	ComPtr<ID3DBlob> pd3dVertexShaderBlob, pd3dPixelShaderBlob;

	ReadCompiledShaderFromFile(vs.c_str(), pd3dVertexShaderBlob);
	ReadCompiledShaderFromFile(ps.c_str(), pd3dPixelShaderBlob);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = _RootSignature.Get();
	d3dPipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(pd3dVertexShaderBlob.Get());
	d3dPipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(pd3dPixelShaderBlob.Get());
	d3dPipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.InputLayout = { m_pd3dInputLayouts.data(), (UINT)m_pd3dInputLayouts.size() };
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	_Device->CreateGraphicsPipelineState(&d3dPipelineStateDesc,
		IID_PPV_ARGS(&m_pd3dPipelineStates));
}

CDiffusedFilledShader::~CDiffusedFilledShader()
{
}


CDiffusedWireFrameShader::CDiffusedWireFrameShader(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12RootSignature>& _RootSignature, const wstring& vs, const wstring& ps)
{
	m_pd3dInputLayouts.clear();
	m_pd3dInputLayouts =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	ComPtr<ID3DBlob> pd3dVertexShaderBlob, pd3dPixelShaderBlob;

	ReadCompiledShaderFromFile(vs.c_str(), pd3dVertexShaderBlob);
	ReadCompiledShaderFromFile(ps.c_str(), pd3dPixelShaderBlob);

	CD3DX12_RASTERIZER_DESC RasterizerDesc{ D3D12_DEFAULT };
	RasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = _RootSignature.Get();
	d3dPipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(pd3dVertexShaderBlob.Get());
	d3dPipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(pd3dPixelShaderBlob.Get());
	d3dPipelineStateDesc.RasterizerState = RasterizerDesc;
	d3dPipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.InputLayout = { m_pd3dInputLayouts.data(), (UINT)m_pd3dInputLayouts.size() };
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	_Device->CreateGraphicsPipelineState(&d3dPipelineStateDesc,
		IID_PPV_ARGS(&m_pd3dPipelineStates));
}

CDiffusedWireFrameShader::~CDiffusedWireFrameShader()
{
}

CDiffusedWireFrameShaderForBOB::CDiffusedWireFrameShaderForBOB(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12RootSignature>& _RootSignature, const wstring& vs, const wstring& ps)
{
	m_pd3dInputLayouts.clear();
	m_pd3dInputLayouts =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	ComPtr<ID3DBlob> pd3dVertexShaderBlob, pd3dPixelShaderBlob;

	ReadCompiledShaderFromFile(vs.c_str(), pd3dVertexShaderBlob);
	ReadCompiledShaderFromFile(ps.c_str(), pd3dPixelShaderBlob);

	CD3DX12_RASTERIZER_DESC RasterizerDesc{ D3D12_DEFAULT };
	RasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = _RootSignature.Get();
	d3dPipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(pd3dVertexShaderBlob.Get());
	d3dPipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(pd3dPixelShaderBlob.Get());
	d3dPipelineStateDesc.RasterizerState = RasterizerDesc;
	d3dPipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.InputLayout = { m_pd3dInputLayouts.data(), (UINT)m_pd3dInputLayouts.size() };
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	_Device->CreateGraphicsPipelineState(&d3dPipelineStateDesc,
		IID_PPV_ARGS(&m_pd3dPipelineStates));
}

CDiffusedWireFrameShaderForBOB::~CDiffusedWireFrameShaderForBOB()
{
}

CTexturedModelShader::CTexturedModelShader(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12RootSignature>& _RootSignature, const wstring& vs, const wstring& ps)
{
	m_pd3dInputLayouts.clear();
	m_pd3dInputLayouts =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 4, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	ComPtr<ID3DBlob> pd3dVertexShaderBlob, pd3dPixelShaderBlob;

	ReadCompiledShaderFromFile(vs.c_str(), pd3dVertexShaderBlob);
	ReadCompiledShaderFromFile(ps.c_str(), pd3dPixelShaderBlob);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = _RootSignature.Get();
	d3dPipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(pd3dVertexShaderBlob.Get());
	d3dPipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(pd3dPixelShaderBlob.Get());
	d3dPipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.InputLayout = { m_pd3dInputLayouts.data(), (UINT)m_pd3dInputLayouts.size() };
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	_Device->CreateGraphicsPipelineState(&d3dPipelineStateDesc,
		IID_PPV_ARGS(&m_pd3dPipelineStates));
}

CTexturedModelShader::~CTexturedModelShader()
{
}

