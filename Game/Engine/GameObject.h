#pragma once
#include "Mesh.h"

class CShader;
class CCamera;
class CMaterial;
class CTexture;

#define MATERIAL_ALBEDO_MAP			0x01
#define MATERIAL_SPECULAR_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_DETAIL_ALBEDO_MAP	0x20
#define MATERIAL_DETAIL_NORMAL_MAP	0x40

enum OBJECT_TYPE : unsigned int {
	PLAYER, MOB, TERRAIN, GRID, BOUNDING_BOX, NOTYPE
};

class CGameObject
{
private:
	int								m_nReferences = 0;
	OBJECT_TYPE						m_eType;
public:
	void AddRef();
	void Release();

public:
	CGameObject();
	CGameObject(int nMaterials);
	virtual ~CGameObject();
	
public:
	//2024-01-20 이성현 오브젝트 구분용 태그 추가
	void SetType(OBJECT_TYPE pType) { m_eType = pType; }
	OBJECT_TYPE GetType() { return m_eType; }

	char							m_pstrFrameName[64];
	shared_ptr<CMesh>				m_pMesh;

	int	m_nMaterials = 0;
	vector<shared_ptr<CMaterial>> m_ppMaterials;

	XMFLOAT4X4						m_xmf4x4Transform;
	XMFLOAT4X4						m_xmf4x4World;

	CGameObject* m_pParent = NULL;
	shared_ptr<CGameObject> m_pChild = nullptr;
	shared_ptr<CGameObject> m_pSibling = nullptr;

	vector<shared_ptr<CGameObject>> m_ppBOBObject;

	shared_ptr<CMesh> GetMesh() { return m_pMesh; }
	XMFLOAT3 GetAABBCenter() const { return m_pMesh->GetAABBCenter(); }
	XMFLOAT3 GetAABBExtents() const { return m_pMesh->GetAABBExtents(); }

	void SetMesh(shared_ptr<CMesh> pMesh) { m_pMesh = pMesh; }

	void SetShader(shared_ptr<CShader> pShader, shared_ptr<CTexture> pTexture = NULL);
	void SetMaterial(int nMaterial, shared_ptr<CMaterial> pMaterial);
	

	void SetChild(shared_ptr<CGameObject> pChild);

	virtual void Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL);

	virtual void Render(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, const shared_ptr<CCamera>& pCamera = nullptr);
	//virtual void Render(bool Terrain, ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL);

	virtual void CreateShaderVariables(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList);
	virtual void UpdateShaderVariables(const ComPtr<ID3D12GraphicsCommandList>& _CommandList);

	virtual void ReleaseShaderVariables();

	//void SetCbvGPUDescriptorHandle(D3D12_GPU_DESCRIPTOR_HANDLE d3dCbvGPUDescriptorHandle) { m_d3dCbvGPUDescriptorHandle = d3dCbvGPUDescriptorHandle; }

	//virtual void UpdateShaderVariable( const ComPtr<ID3D12GraphicsCommandList>& _CommandList, XMFLOAT4X4* pxmf4x4World);
	//virtual void UpdateShaderVariable( const ComPtr<ID3D12GraphicsCommandList>& _CommandList, CMaterial* pMaterial);

	virtual void ReleaseUploadBuffers();

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();
	XMFLOAT4X4 GetTransform4X4() { return m_xmf4x4Transform; };
	XMFLOAT4X4 GetWorld4X4() { return m_xmf4x4World; };
	CGameObject* GetParent() { return (m_pParent); }
	UINT GetMeshType() { return((m_pMesh) ? m_pMesh->GetType() : 0x00); }

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);
	void SetScale(float x, float y, float z);

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3* pxmf3Axis, float fAngle);


	void UpdateTransform(XMFLOAT4X4* pxmf4x4Parent = NULL);
	CGameObject* FindFrame(char* pstrFrameName);

	int FindReplicatedTexture(_TCHAR* pstrTextureName, D3D12_GPU_DESCRIPTOR_HANDLE* pd3dSrvGpuDescriptorHandle);

public:
	//BULLET
	float m_fMovingSpeed = 0.0f;
	float m_fMovingRange = 0.0f;
	float m_fRotationSpeed = 0.0f;
	bool m_bActive = true;

	int lifeCount = 5;
	XMFLOAT3 m_xmf3RotationAxis = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3 m_xmf3MovingDirection = XMFLOAT3(0.0f, 1.0f, 0.0f);

	void SetMovingDirection(const XMFLOAT3& xmf3MovingDirection);
	void SetRotationSpeed(float fSpeed) { m_fRotationSpeed = fSpeed; }
	void SetMovingSpeed(float fSpeed) { m_fMovingSpeed = fSpeed; }
	void SetActive(bool bActive) { m_bActive = bActive; }
	void SetLookAt(XMFLOAT3 xmf3Target, XMFLOAT3 xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f));
	void SetNewUp(XMFLOAT3 newUp);

public:
	void LoadMaterialsFromFile(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, CGameObject*, FILE* pInFile, shared_ptr<CShader> pShader);

	void LoadFrameHierarchyFromFile(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CGameObject* pParent, FILE* pInFile, shared_ptr<CShader> pShader);
	void LoadGeometryFromFile(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, const char* pstrFileName, shared_ptr<CShader> pShader);

	static void PrintFrameInfo(shared_ptr<CGameObject> pGameObject, shared_ptr<CGameObject> pParent );
};

class BoundingBoxObject : public CGameObject
{
public:
	BoundingBoxObject(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, const ComPtr<ID3D12RootSignature>& _RootSignature, XMFLOAT3 xmf3Center, XMFLOAT3 xmf3Extent);
	~BoundingBoxObject();
};
