#include "GameObject.h"
#include "Camera.h"

struct CB_GAMEOBJECT_INFO
{
	XMFLOAT4X4 m_xmf4x4World;
};

class CShader
{
public:
	CShader();
	CShader(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12RootSignature>& _RootSignature, const wstring& vs, const wstring& ps);
	virtual ~CShader();
public:
	
	/*virtual void CreateShaderVariables(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList);
	virtual void UpdateShaderVariables(const ComPtr<ID3D12GraphicsCommandList>& _CommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariable(const ComPtr<ID3D12GraphicsCommandList>& _CommandList,
		XMFLOAT4X4* pxmf4x4World);*/

	//2024-01-15 이성현 추가
	D3D12_SHADER_BYTECODE ReadCompiledShaderFromFile(const wchar_t* pszFileName, ComPtr<ID3DBlob>& pd3dShaderBlob);
	
virtual void SetPipelineState(const ComPtr<ID3D12GraphicsCommandList>& _CommandList);
	virtual void Render(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, CCamera* pCamera);
protected:
	ComPtr<ID3D12PipelineState> m_pd3dPipelineStates;
	vector<D3D12_INPUT_ELEMENT_DESC> m_pd3dInputLayouts;
};


class CDiffusedFilledShader : public CShader
{
public:
	CDiffusedFilledShader(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12RootSignature>& _RootSignature, const wstring& vs, const wstring& ps);
	virtual ~CDiffusedFilledShader();
	
};

class CDiffusedWireFrameShader : public CShader
{
public:
	CDiffusedWireFrameShader(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12RootSignature>& _RootSignature, const wstring& vs, const wstring& ps);
	virtual ~CDiffusedWireFrameShader();

};

class CDiffusedWireFrameShaderForBOB : public CShader
{
public:
	CDiffusedWireFrameShaderForBOB(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12RootSignature>& _RootSignature, const wstring& vs, const wstring& ps);
	virtual ~CDiffusedWireFrameShaderForBOB();

};

class CTexturedModelShader : public CShader
{
public:
	CTexturedModelShader(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12RootSignature>& _RootSignature, const wstring& vs, const wstring& ps);
	virtual ~CTexturedModelShader();
	
};
