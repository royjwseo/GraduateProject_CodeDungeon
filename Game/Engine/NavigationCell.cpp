#include "pch.h"
#include "Shader.h"
#include "NavigationCell.h"
#include "Material.h"

NavigationCell::NavigationCell(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, const ComPtr<ID3D12RootSignature>& _RootSignature, array<XMFLOAT3, POINT_END>& Points, const int iIndex) : CGameObject(0)
{
	m_arrPoints[POINT_A] = Points[POINT_A];
	m_arrPoints[POINT_B] = Points[POINT_B];
	m_arrPoints[POINT_C] = Points[POINT_C];

	m_iIndex = iIndex;
	//ResortPointsClockwise();

	XMFLOAT3 vCenterPos{};
	for (auto& iter : m_arrPoints) {
		vCenterPos.x += iter.x;
		vCenterPos.y += iter.y;
		vCenterPos.z += iter.z;
	}
	vCenterPos.x /= 3.f;
	vCenterPos.y /= 3.f;
	vCenterPos.z /= 3.f;
	m_xmf3Center = vCenterPos;
}

NavigationCell::~NavigationCell()
{
}

bool NavigationCell::IsInNeighbor(const XMFLOAT3& vPos, int& iNeighborIndex, XMFLOAT3& vLine)
{
	for (int i = 0; i < LINE_END; ++i) {
		XMFLOAT3 vDir;
		vDir.x = vPos.x - m_arrPoints[i].x;
		vDir.y = vPos.y - m_arrPoints[i].y;
		vDir.z = vPos.z - m_arrPoints[i].z;

		XMVECTOR vDirVector = XMLoadFloat3(&vDir);
		XMVector3Normalize(vDirVector);
		XMStoreFloat3(&vDir, vDirVector);

		float dotProduct;
		XMStoreFloat(&dotProduct, XMVector3Dot(vDirVector, XMLoadFloat3(&m_arrNormals[i])));

		if (dotProduct > 0) {
			iNeighborIndex = m_arrNeighbors[i];
			return false;
		}
	}
	return true;
}

bool NavigationCell::ComparePoints(const XMFLOAT3& vPointA, const XMFLOAT3& vPointB)
{
	if (DirectX::XMVector3Equal(XMLoadFloat3(&m_arrPoints[POINT_A]), XMLoadFloat3(&vPointA)))
	{
		if (DirectX::XMVector3Equal(XMLoadFloat3(&m_arrPoints[POINT_B]), XMLoadFloat3(&vPointB)) || DirectX::XMVector3Equal(XMLoadFloat3(&m_arrPoints[POINT_C]), XMLoadFloat3(&vPointB)))
			return true;
	}

	if (DirectX::XMVector3Equal(XMLoadFloat3(&m_arrPoints[POINT_B]), XMLoadFloat3(&vPointA)))
	{
		if (DirectX::XMVector3Equal(XMLoadFloat3(&m_arrPoints[POINT_C]), XMLoadFloat3(&vPointB)) || DirectX::XMVector3Equal(XMLoadFloat3(&m_arrPoints[POINT_A]), XMLoadFloat3(&vPointB)))
			return true;
	}

	if (DirectX::XMVector3Equal(XMLoadFloat3(&m_arrPoints[POINT_C]), XMLoadFloat3(&vPointA)))
	{
		if (DirectX::XMVector3Equal(XMLoadFloat3(&m_arrPoints[POINT_A]), XMLoadFloat3(&vPointB)) || DirectX::XMVector3Equal(XMLoadFloat3(&m_arrPoints[POINT_B]), XMLoadFloat3(&vPointB)))
			return true;
	}
	return false;
}

bool NavigationCell::ComparePoint(const shared_ptr<NavigationCell>& pCell)
{
	if (nullptr == pCell)
		return false;

	using namespace DirectX;
	for (int i = 0; i < POINT_END; ++i) {
		XMVECTOR arrPointsi = XMLoadFloat3(&m_arrPoints[i]);
		XMVECTOR pointA = XMLoadFloat3(&pCell->GetPoint(POINT_A));
		XMVECTOR pointB = XMLoadFloat3(&pCell->GetPoint(POINT_B));
		XMVECTOR pointC = XMLoadFloat3(&pCell->GetPoint(POINT_C));

		if (XMVector3Equal(arrPointsi, pointA))
		{
			for (int j = 0; j < POINT_END; ++j) {
				if (i == j)
					continue;
				XMVECTOR arrPointsj = XMLoadFloat3(&m_arrPoints[j]);
				if (XMVector3Equal(arrPointsj, pointB) || XMVector3Equal(arrPointsj, pointC))
					return true;
			}
		}
		if (XMVector3Equal(arrPointsi, pointB))
		{
			for (int j = 0; j < POINT_END; ++j) {
				if (i == j)
					continue;
				XMVECTOR arrPointsj = XMLoadFloat3(&m_arrPoints[j]);
				if (XMVector3Equal(arrPointsj, pointA) || XMVector3Equal(arrPointsj, pointC))
					return true;
			}
		}
		if (XMVector3Equal(arrPointsi, pointC))
		{
			for (int j = 0; j < POINT_END; ++j) {
				if (i == j)
					continue;
				XMVECTOR arrPointsj = XMLoadFloat3(&m_arrPoints[j]);
				if (XMVector3Equal(arrPointsj, pointB) || XMVector3Equal(arrPointsj, pointA))
					return true;
			}
		}
	}
	return false;
}

const XMFLOAT3 NavigationCell::CalculateHeight(const XMFLOAT3& vPosition)
{
	DirectX::XMVECTOR planeVector = DirectX::XMLoadFloat4(&m_xmf4Plane);

	float dotResult = DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(planeVector, DirectX::XMLoadFloat3(&vPosition)));

	XMFLOAT3 heightResult;
	heightResult.x = dotResult;
	heightResult.y = vPosition.y;  
	heightResult.z = dotResult;    

	return heightResult;
}

void NavigationCell::CalculateCrossResult(array<XMFLOAT3, POINT_END>& arrPointsEnd)
{
	XMFLOAT3 p1{ m_arrPoints[POINT_A].x, 0.f, m_arrPoints[POINT_A].z };
	XMFLOAT3 p2{ m_arrPoints[POINT_B].x, 0.f, m_arrPoints[POINT_B].z };
	XMFLOAT3 p3{ m_arrPoints[POINT_C].x, 0.f, m_arrPoints[POINT_C].z };

	XMFLOAT3 L1 = { p2.x - p1.x, p2.y - p1.y, p2.z - p1.z };
	XMFLOAT3 L2 = { p3.x - p2.x, p3.y - p2.y, p3.z - p2.z };
	XMFLOAT3 L3 = { p1.x - p3.x, p1.y - p3.y, p1.z - p3.z };

	XMFLOAT3 vCross1;
	XMFLOAT3 vCross2;
	XMFLOAT3 vCross3;

	vCross1.x = L1.y * L2.z - L1.z * L2.y;
	vCross1.y = L1.z * L2.x - L1.x * L2.z;
	vCross1.z = L1.x * L2.y - L1.y * L2.x;

	vCross2.x = L2.y * L3.z - L2.z * L3.y;
	vCross2.y = L2.z * L3.x - L2.x * L3.z;
	vCross2.z = L2.x * L3.y - L2.y * L3.x;

	vCross3.x = L3.y * L1.z - L3.z * L1.y;
	vCross3.y = L3.z * L1.x - L3.x * L1.z;
	vCross3.z = L3.x * L1.y - L3.y * L1.x;

	arrPointsEnd[POINT_A] = vCross1;
	arrPointsEnd[POINT_B] = vCross2;
	arrPointsEnd[POINT_C] = vCross3;
}

void NavigationCell::ResortPointsClockwise()
{
	// 같은 시계 방향으로 점들이 배치되도록 하는 것
	bool bIsTrue = true;
	while (bIsTrue) {
		array<XMFLOAT3, POINT_END> arrPosList;
		//CalculateCrossResult(arrPosList);

		if (arrPosList[0].y < 0)
		{
			std::swap(m_arrPoints[POINT_A], m_arrPoints[POINT_B]);
			continue;
		}
		if (arrPosList[1].y < 0)
		{
			std::swap(m_arrPoints[POINT_B], m_arrPoints[POINT_C]);
			continue;
		}
		if (arrPosList[2].y < 0)
		{
			std::swap(m_arrPoints[POINT_C], m_arrPoints[POINT_A]);
			continue;
		}
		bIsTrue = false;
	}
	MakeLineAndNormal();
}

void NavigationCell::MakeLineAndNormal()
{


	XMVECTOR planeVec;
	planeVec = DirectX::XMPlaneFromPoints(XMLoadFloat3(&m_arrPoints[POINT_B]), XMLoadFloat3(&m_arrPoints[POINT_A]), XMLoadFloat3(&m_arrPoints[POINT_C]));
	XMStoreFloat4(&m_xmf4Plane, planeVec);
}

CCellMesh::CCellMesh(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, array<XMFLOAT3, 3>& Points) : CMesh(_Device, _CommandList)
{
	m_nVertices = 3;
	m_nStride = sizeof(CDiffusedVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	CDiffusedVertex pVertices[3];
	pVertices[0] = CDiffusedVertex(Points[0], XMFLOAT4(1.0f, 0.0f, 0.0f,
		1.0f));
	pVertices[1] = CDiffusedVertex(Points[1], XMFLOAT4(1.0f, 0.0f, 0.0f,
		1.0f));
	pVertices[2] = CDiffusedVertex(Points[2], XMFLOAT4(1.0f, 0.0f, 0.0f,
		1.0f));

	m_pd3dPositionBuffer = Util::CreateBufferResource(_Device, _CommandList, pVertices, m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);
	//정점 버퍼 뷰를 생성한다. 

	D3D12_VERTEX_BUFFER_VIEW m_pd3dPositionBufferView;
	m_pd3dPositionBufferView.BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_pd3dPositionBufferView.StrideInBytes = m_nStride;
	m_pd3dPositionBufferView.SizeInBytes = m_nStride * m_nVertices;

	m_pd3dVertexBufferViews.emplace_back(m_pd3dPositionBufferView);
}
