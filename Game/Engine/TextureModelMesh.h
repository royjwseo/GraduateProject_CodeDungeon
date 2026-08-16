#pragma once
class TextureModelMesh:public CMesh
{
public:
	TextureModelMesh() = default;
	TextureModelMesh(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList) {};
	~TextureModelMesh() = default;
protected:
	char m_pstrMeshName[256] = { 0 };

	std::vector<XMFLOAT2> TexCoord_Data0;

	std::vector<XMFLOAT2> m_pxmf2TextureCoords0;
	ComPtr<ID3D12Resource> m_pd3dTextureCoord0Buffer = NULL;
	ComPtr<ID3D12Resource> m_pd3dTextureCoord0UploadBuffer = NULL;

	std::vector<XMFLOAT2> m_pxmf2TextureCoords1;
	ComPtr<ID3D12Resource> m_pd3dTextureCoord1Buffer = NULL;
	ComPtr<ID3D12Resource> m_pd3dTextureCoord1UploadBuffer = NULL;

	std::vector<XMFLOAT3> m_pxmf3Tangents;
	ComPtr<ID3D12Resource> m_pd3dTangentBuffer = NULL;
	ComPtr<ID3D12Resource> m_pd3dTangentUploadBuffer = NULL;

	std::vector<XMFLOAT3> m_pxmf3BiTangents;
	ComPtr<ID3D12Resource> m_pd3dBiTangentBuffer = NULL;
	ComPtr<ID3D12Resource> m_pd3dBiTangentUploadBuffer = NULL;

public:
	virtual void LoadMeshFromFile(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList,const string& pInFile);
	void LoadMeshFromFile(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, FILE* pInFile);
public:
	virtual void CreateShaderVariables(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList) {}
	virtual void UpdateShaderVariables(const ComPtr<ID3D12GraphicsCommandList>& _CommandList) {}
	virtual void ReleaseUploadBuffers();
};

