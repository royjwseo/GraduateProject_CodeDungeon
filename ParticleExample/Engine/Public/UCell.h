#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCELL_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCELL_H

#include "UObject.h"

BEGIN(Engine)

class ENGINE_DLL DEF_CACHE_ALGIN  UCell : public UObject {
public:
	enum POINT : _uint { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE : _uint { LINE_AB, LINE_BC, LINE_CA, LINE_END };
	// CellDecs
	typedef struct tagCellDesc {
		ARRAY<_float3, POINT_END>	vPoints{};
		ARRAY<_float3, LINE_END>		vLine{};
		ARRAY<_float3, LINE_END>		vNormal{};
		ARRAY<_int, LINE_END>				iNeighbor{-1, -1, -1};
		_int													iIndex{ -1 };
	}CELLDECS;
public:
	UCell(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(UCell)
	DESTRUCTOR(UCell)
public:
	const _float3& GetPoint(const POINT& _ePoint) const { return m_arrPoints[_ePoint]; }
	const _float3& GetLine(const LINE& _eLine) const { return m_arrLines[_eLine]; }
	const ARRAY<_float3, POINT_END>& GetPoints() const { return m_arrPoints; }
	const ARRAY<_float3, LINE_END>& GetLines() const { return m_arrLines; }
	const ARRAY<_float3, LINE_END>& GetNormal() const { return m_arrNormals; }
	const ARRAY<_int, LINE_END>& GetNeighbor() const { return m_arrNeighbors; }
	const _float3& GetCenterPos() const { return m_vCenterPos; }
	const _int& GetIndex() const { return m_iIndex; }

	void SetPoint(const POINT& _ePoint, const _float3& _vPoint) { m_arrPoints[_ePoint] = _vPoint; }
	void SetCenterPos(const _float3& _vCenterPos) { this->m_vCenterPos = _vCenterPos; }
	void SetNeighbor(const LINE& _eLine, const _uint& _iIndex) { m_arrNeighbors[_eLine] = _iIndex; }
public:
	virtual void Free() override;
	virtual  HRESULT NativeConstruct() override;
	// Points
	HRESULT NativeConstruct(ARRAY<_float3, POINT_END>& _Points, const _uint _iIndex);
public:
	_bool IsIn(const _float3& _vPos, _int& _iNeightborIndex, _float3& _vLine);
	_bool IsComparePoints(const _float3& _vPointA, const _float3& _vPointB);
	_bool IsComparePoint(CSHPTRREF<UCell> _pCell);
	const _float ComputeHeight(const _float3& _vPosition);
private:
	// CrossResult
	void CalculateCrossResult(ARRAY<_float3, POINT_END>& _arrPointsEnd);
	void ResortPoints();
	void MakeLineAndNormal();
private:
	// Points
	ARRAY<_float3, POINT_END>		m_arrPoints;
	// Line
	ARRAY<_float3, LINE_END>			m_arrLines;
	// Normal
	ARRAY<_float3, LINE_END>			m_arrNormals;
	// Neighbor
	ARRAY<_int, LINE_END>					m_arrNeighbors;
	// CenterPos
	_float3												m_vCenterPos;
	// Index
	_int														m_iIndex;
	// Planes
	_float4												m_vPlane;
};

END


#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCELL_H