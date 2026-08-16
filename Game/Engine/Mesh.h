#pragma once


#define VERTEXT_POSITION				0x01
#define VERTEXT_COLOR					0x02
#define VERTEXT_NORMAL					0x04
#define VERTEXT_TANGENT					0x08
#define VERTEXT_TEXTURE_COORD0			0x10
#define VERTEXT_TEXTURE_COORD1			0x20
#define VERTEXT_BONE_INDEX_WEIGHT		0x1000

#include "Vertex.h"

class CMesh
{
public:
	CMesh() = default;
	CMesh(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList) {};
	virtual ~CMesh() = default;
private:
	int m_nReferences = 0;
public:
	//스마트 포인터 사용시 필요없음.
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }
	void ReleaseUploadBuffers();
protected:
//--------바운딩 박스 -------------
	XMFLOAT3 m_xmf3AABBCenter = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_xmf3AABBExtents = XMFLOAT3(0.0f, 0.0f, 0.0f);
	BoundingOrientedBox m_BOB;
//----------------------정점 정보들 버퍼 만들 떄 필요-------------------
protected:
	D3D12_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT m_nType = 0x00;
	UINT m_nSlot = 0;
	UINT m_nVertices = 0;
	UINT m_nStride = 0;
	UINT m_nOffset = 0;

//------------------------------정점 버퍼-------------------------------
protected:
	vector<XMFLOAT3> m_pxmf3Positions;
	ComPtr<ID3D12Resource> m_pd3dPositionBuffer;
	ComPtr<ID3D12Resource> m_pd3dPositionUploadBuffer;
	
	vector<XMFLOAT3> m_pxmf3Normals;
	ComPtr<ID3D12Resource> m_pd3dNormalBuffer = NULL;
	ComPtr<ID3D12Resource> m_pd3dNormalUploadBuffer = NULL;

	//여기에 정점 버퍼들 vector로 넣어 한꺼번에 IA에 Set
	vector<D3D12_VERTEX_BUFFER_VIEW> m_pd3dVertexBufferViews; 

	UINT m_nVertexBuffers = 0;
//------------------------------인덱스 버퍼-------------------------------
protected:
	UINT m_nIndices = 0;
	vector<UINT> m_pnIndices;

	UINT m_nSubMeshes = 0;
	vector<UINT> m_pnSubSetIndices;
	vector<UINT> m_pnSubSetStartIndices;
	vector<vector<UINT>> m_ppnSubSetIndices;
	
	// 모델 활용을 위해 벡터로 인덱스 버퍼 만듦 - SubsetIndexBuffer활용 
	vector<ComPtr<ID3D12Resource>> m_ppd3dIndexBuffers;
	vector<ComPtr<ID3D12Resource>> m_ppd3dIndexUploadBuffers;
	vector<D3D12_INDEX_BUFFER_VIEW> m_pd3dIndexBufferViews;

	//인덱스 버퍼에 포함되는 인덱스의 개수이다. 
	UINT m_nStartIndex = 0;
	//인덱스 버퍼에서 메쉬를 그리기 위해 사용되는 시작 인덱스이다. 
	UINT m_nBaseVertex = 0;
	//인덱스 버퍼의 인덱스에 더해질 인덱스이다. 
public:
	vector<XMFLOAT3> GetPositions() { return m_pxmf3Positions; }
	vector<vector<UINT>> GetSubSetIndices() { return m_ppnSubSetIndices; }
	vector<UINT> GetIndices() { return m_pnIndices; }

	//2024-01-18 이성현
	//BoundingOrientedBox생성
	void CreateBoundingBox();
	void CreateOrientedBoxFromAABB();
	void UpdateBoundingBox(const XMFLOAT4X4& worldMatrix);
	BoundingOrientedBox GetBoundingBox() const { return m_BOB; };
	XMFLOAT3 GetAABBCenter() const { return m_xmf3AABBCenter; }
	XMFLOAT3 GetAABBExtents() const { return m_xmf3AABBExtents; }
public:
	//2024-01-15 이성현
	//픽킹 처리를 위한 정점 월드변환 함수
	XMFLOAT3 TransformPoint(const XMFLOAT3& point, const XMFLOAT4X4& worldMatrix) const;
	virtual void IASetBeforeRender(const ComPtr<ID3D12GraphicsCommandList>& _CommandList);
	virtual void RenderMesh(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, UINT _nSubset);
	virtual void RenderMesh_OneIndexBuffer(const ComPtr<ID3D12GraphicsCommandList>& _CommandList);
	UINT GetType() { return(m_nType); }
};

//
class CTriangleMesh : public CMesh
{
public:
	CTriangleMesh(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList);
	virtual ~CTriangleMesh() { }
};


//2024-01-18 이성현 
//바운딩 박스 렌더링을 위한 메쉬 클래스
class CBoundingBoxMesh : public CMesh
{
public:
	CBoundingBoxMesh(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, XMFLOAT3 center, XMFLOAT3 extents);
	virtual ~CBoundingBoxMesh() {}
};

