#pragma once
#include "GameObject.h"

class NavigationCell : public CGameObject
{
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
	typedef struct CellDesc {
		array<XMFLOAT3, POINT_END>	vPoints{};
		array<XMFLOAT3, LINE_END>	vLine{};
		array<XMFLOAT3, LINE_END>	vNormal{};
		array<int, LINE_END>		iNeighbor{ -1, -1, -1 };
		int							iIndex{ -1 };
	}CELLDESC;
public:
	NavigationCell(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, const ComPtr<ID3D12RootSignature>& _RootSignature, array<XMFLOAT3, POINT_END>& Points, const int iIndex);
	virtual ~NavigationCell();

	
public:
	const XMFLOAT3& GetPoint(const POINT& _ePoint) const { return m_arrPoints[_ePoint]; }
	const array<XMFLOAT3, POINT_END>& GetPoints() const { return m_arrPoints; }
	const array<XMFLOAT3, LINE_END>& GetNormal() const { return m_arrNormals; }
	const array<int, LINE_END>& GetNeighbor() const { return m_arrNeighbors; }
	const XMFLOAT3& GetCenterPos() const { return m_xmf3Center; }
	const int& GetIndex() const { return m_iIndex; }

	void SetPoint(const POINT& ePoint, const XMFLOAT3& xmf3Point) { m_arrPoints[ePoint] = xmf3Point; }
	void SetCenterPos(const XMFLOAT3& xmf3Center) { this->m_xmf3Center = xmf3Center; }
	void SetNeighbor(const LINE& eLine, const int& iIndex) { m_arrNeighbors[eLine] = iIndex; }

	bool IsInNeighbor(const XMFLOAT3& vPos, int& iNeighborIndex, XMFLOAT3& vLine);
	bool ComparePoints(const XMFLOAT3& _PointA, const XMFLOAT3& vPointB);
	bool ComparePoint(const shared_ptr<NavigationCell>& pCell);
	const XMFLOAT3 CalculateHeight(const XMFLOAT3& vPosition);

	void CalculateCrossResult(array<XMFLOAT3, POINT_END>& arrPointsEnd);
	void ResortPointsClockwise();
	void MakeLineAndNormal();
private:
	array<XMFLOAT3, POINT_END>	m_arrPoints{};
	array<XMFLOAT3, LINE_END>	m_arrNormals{};

	array<int, LINE_END>	m_arrNeighbors{};

	XMFLOAT3 m_xmf3Center;
	int		 m_iIndex;
	XMFLOAT4 m_xmf4Plane;
};

class CCellMesh : public CMesh
{
public:
	CCellMesh(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, array<XMFLOAT3, 3>& Points);
	virtual ~CCellMesh() { }
};

