#include "pch.h"
#include "Vertex.h"
#include "Mesh.h"


void CMesh::ReleaseUploadBuffers()
{
	if (m_pd3dPositionUploadBuffer) m_pd3dPositionUploadBuffer.Reset();
	if (m_pd3dNormalUploadBuffer) m_pd3dNormalUploadBuffer.Reset();
	if (m_ppd3dIndexUploadBuffers.data())
	{
		m_ppd3dIndexUploadBuffers.clear();
		vector<ComPtr<ID3D12Resource>>().swap(m_ppd3dIndexUploadBuffers);
	}
};

void CMesh::CreateBoundingBox()
{
	if (m_pxmf3Positions.empty()) {
		return;
	}
	
	XMFLOAT3 minCoordinate = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	XMFLOAT3 maxCoordinate = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (const XMFLOAT3& position : m_pxmf3Positions)
	{
		minCoordinate.x = min(minCoordinate.x, position.x);
		minCoordinate.y = min(minCoordinate.y, position.y);
		minCoordinate.z = min(minCoordinate.z, position.z);

		maxCoordinate.x = max(maxCoordinate.x, position.x);
		maxCoordinate.y = max(maxCoordinate.y, position.y);
		maxCoordinate.z = max(maxCoordinate.z, position.z);
	}

	m_xmf3AABBCenter = XMFLOAT3(
		(minCoordinate.x + maxCoordinate.x) * 0.5f,
		(minCoordinate.y + maxCoordinate.y) * 0.5f,
		(minCoordinate.z + maxCoordinate.z) * 0.5f
	);

	m_xmf3AABBExtents = XMFLOAT3(
		(maxCoordinate.x - minCoordinate.x) * 0.5f,
		(maxCoordinate.y - minCoordinate.y) * 0.5f,
		(maxCoordinate.z - minCoordinate.z) * 0.5f
	);

	CreateOrientedBoxFromAABB();
}

void CMesh::CreateOrientedBoxFromAABB()
{
	BoundingOrientedBox::CreateFromBoundingBox(m_BOB, BoundingBox(m_xmf3AABBCenter, m_xmf3AABBExtents));
}

void CMesh::UpdateBoundingBox(const XMFLOAT4X4& worldMatrix)
{
	XMMATRIX xmWorldMatrix = XMLoadFloat4x4(&worldMatrix);
	m_BOB.Transform(m_BOB, xmWorldMatrix);
}

XMFLOAT3 CMesh::TransformPoint(const XMFLOAT3& point, const XMFLOAT4X4& worldMatrix) const
{
	XMVECTOR pointVec = XMLoadFloat3(&point);
	XMMATRIX worldMatrixXM = XMLoadFloat4x4(&worldMatrix);
	XMVECTOR worldtransformedpointVec = XMVector3TransformCoord(pointVec, worldMatrixXM);
	XMFLOAT3 worldtransformedpointf3;
	XMStoreFloat3(&worldtransformedpointf3, worldtransformedpointVec);

	return worldtransformedpointf3;
}

void CMesh::IASetBeforeRender(const ComPtr<ID3D12GraphicsCommandList>& _CommandList)
{
	_CommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	_CommandList->IASetVertexBuffers(m_nSlot, m_pd3dVertexBufferViews.size(), m_pd3dVertexBufferViews.data());
}

void CMesh::RenderMesh(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, UINT _nSubset)
{
	IASetBeforeRender(_CommandList);

	if ((m_nSubMeshes > 0) && (_nSubset < m_nSubMeshes))
	{
		_CommandList->IASetIndexBuffer(&m_pd3dIndexBufferViews[_nSubset]);
		_CommandList->DrawIndexedInstanced(m_pnSubSetIndices[_nSubset], 1, 0, 0, 0);
	}
	else
	{

		_CommandList->DrawInstanced(m_nVertices, 1, m_nOffset, 0);
	}
}

void CMesh::RenderMesh_OneIndexBuffer(const ComPtr<ID3D12GraphicsCommandList>& _CommandList)
{

	IASetBeforeRender(_CommandList);

	if (!m_ppd3dIndexBuffers.empty())
	{
		_CommandList->IASetIndexBuffer(&m_pd3dIndexBufferViews[0]);
		_CommandList->DrawIndexedInstanced(m_nIndices, 1, 0, 0, 0);
	}
	else
	{
		_CommandList->DrawInstanced(m_nVertices, 1, m_nOffset, 0);
	}
}

CTriangleMesh::CTriangleMesh(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList) : CMesh(_Device, _CommandList)
{
	//삼각형 메쉬를 정의한다. 
	m_nVertices = 3;
	m_nStride = sizeof(CDiffusedVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	/*정점(삼각형의 꼭지점)의 색상은 시계방향 순서대로 빨간색, 녹색, 파란색으로 지정한다. RGBA(Red, Green, Blue,
	Alpha) 4개의 파라메터를 사용하여 색상을 표현한다. 각 파라메터는 0.0~1.0 사이의 실수값을 가진다.*/
	CDiffusedVertex pVertices[3];
	pVertices[0] = CDiffusedVertex(XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f,
		1.0f));
	pVertices[1] = CDiffusedVertex(XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f,
		1.0f));
	pVertices[2] = CDiffusedVertex(XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(Colors::Blue));
	//삼각형 메쉬를 리소스(정점 버퍼)로 생성한다. 
	m_pd3dPositionBuffer = Util::CreateBufferResource(_Device, _CommandList, pVertices, m_nStride* m_nVertices, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);
		//정점 버퍼 뷰를 생성한다. 

	D3D12_VERTEX_BUFFER_VIEW m_pd3dPositionBufferView;
	m_pd3dPositionBufferView.BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_pd3dPositionBufferView.StrideInBytes = m_nStride;
		m_pd3dPositionBufferView.SizeInBytes = m_nStride * m_nVertices;

	m_pd3dVertexBufferViews.emplace_back(m_pd3dPositionBufferView);
}



/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CBoundingBoxMesh::CBoundingBoxMesh(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, XMFLOAT3 center, XMFLOAT3 extents)
	: CMesh(_Device, _CommandList)
{

	const XMFLOAT4 randomColor = XMFLOAT4(1.0f , 1.0f, 1.0f, 1.0f);

	// 정점 생성
	m_nVertices = 8;
	m_nStride = sizeof(CDiffusedVertex);
	m_nOffset = 0;
	m_nSlot = 0;

	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;

	vector<CDiffusedVertex> pVertices;
	pVertices.resize(m_nVertices);

	pVertices[0] = CDiffusedVertex(XMFLOAT3(center.x - extents.x, center.y - extents.y, center.z - extents.z), randomColor);
	pVertices[1] = CDiffusedVertex(XMFLOAT3(center.x - extents.x, center.y - extents.y, center.z + extents.z), randomColor);
	pVertices[2] = CDiffusedVertex(XMFLOAT3(center.x + extents.x, center.y - extents.y, center.z + extents.z), randomColor);
	pVertices[3] = CDiffusedVertex(XMFLOAT3(center.x + extents.x, center.y - extents.y, center.z - extents.z), randomColor);

	pVertices[4] = CDiffusedVertex(XMFLOAT3(center.x - extents.x, center.y + extents.y, center.z - extents.z), randomColor);
	pVertices[5] = CDiffusedVertex(XMFLOAT3(center.x - extents.x, center.y + extents.y, center.z + extents.z), randomColor);
	pVertices[6] = CDiffusedVertex(XMFLOAT3(center.x + extents.x, center.y + extents.y, center.z + extents.z), randomColor);
	pVertices[7] = CDiffusedVertex(XMFLOAT3(center.x + extents.x, center.y + extents.y, center.z - extents.z), randomColor);

	m_pd3dPositionBuffer = Util::CreateBufferResource(_Device, _CommandList, pVertices.data(),
		m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);

	// 정점 버퍼 뷰 설정
	D3D12_VERTEX_BUFFER_VIEW m_pd3dPositionBufferView;
	m_pd3dPositionBufferView.BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_pd3dPositionBufferView.StrideInBytes = m_nStride;
	m_pd3dPositionBufferView.SizeInBytes = m_nStride * m_nVertices;

	m_pd3dVertexBufferViews.push_back(m_pd3dPositionBufferView);

	// 인덱스 생성

	int indices[] = {
	0, 1, 1, 2, 2, 3, 3, 0, // Bottom face
	4, 5, 5, 6, 6, 7, 7, 4, // Top face
	0, 4, 1, 5, 2, 6, 3, 7  // Connecting lines
	};

	m_nSubMeshes = 1;
	m_pnSubSetIndices.resize(m_nSubMeshes);
	m_ppnSubSetIndices.resize(m_nSubMeshes);

	int numLines = sizeof(indices) / sizeof(indices[0]);
	m_pnSubSetIndices[0] = numLines;
	m_ppnSubSetIndices[0].resize(numLines);

	// Copy the indices to the vector
	for (int i = 0; i < numLines; ++i) {
		m_ppnSubSetIndices[0][i] = indices[i];
	}

	// 서브셋에 대한 인덱스 버퍼 생성
	m_ppd3dIndexBuffers.resize(m_nSubMeshes);
	m_ppd3dIndexUploadBuffers.resize(m_nSubMeshes);
	m_pd3dIndexBufferViews.resize(m_nSubMeshes);

	m_ppd3dIndexBuffers[0] = Util::CreateBufferResource(
		_Device, _CommandList,
		m_ppnSubSetIndices[0].data(),
		sizeof(UINT) * m_pnSubSetIndices[0],
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		&m_ppd3dIndexUploadBuffers[0]);

	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	indexBufferView.BufferLocation = m_ppd3dIndexBuffers[0]->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	indexBufferView.SizeInBytes = sizeof(UINT) * m_pnSubSetIndices[0];
	m_pd3dIndexBufferViews[0] = indexBufferView;
}

