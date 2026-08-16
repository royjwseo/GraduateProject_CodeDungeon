#include "EngineDefines.h"
#include "UNavigation.h"
#include "UTransform.h"
#include "UVIBufferTerrain.h"
#include "UDevice.h"
#include "UGpuCommand.h"
#include "UMethod.h"
#include <fstream>

UNavigation::UNavigation(CSHPTRREF<UDevice> _spDevice)
	: UResource(_spDevice),
	m_spCellContainer{},
	m_spCurCell{ nullptr },
	m_iCurIndex{0}
{
}

UNavigation::UNavigation(const UNavigation& _rhs) : UResource(_rhs), 
m_spCellContainer{ _rhs .m_spCellContainer}
{
}

void UNavigation::Free()
{
	m_spCellContainer.reset();
}

HRESULT UNavigation::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UNavigation::NativeConstruct(const VECTOR<SHPTR<UVIBufferTerrain>>& _vecTerrain)
{
	RETURN_CHECK_FAILED(NativeConstruct(), E_FAIL);
	RETURN_CHECK(0 == _vecTerrain.size(), E_FAIL);
	using POSPOINTER = SHPTR<const VECTOR<_float3>>;
	
	m_spCellContainer = std::make_shared<CELLCONTAINER>();
	for (auto& iter : _vecTerrain)
	{
		POSPOINTER spTerrainPos{nullptr};
		const void* pIndices = iter->GetIndices();
		const INDICIES32* pIndex32{ nullptr };
		_uint iIndiescCnt = iter->GetIndexCnt();
		{
			spTerrainPos = iter->GetVertexPos();
			RETURN_CHECK(nullptr == spTerrainPos, E_FAIL);
		}
		{
			pIndex32 = static_cast<const INDICIES32*>(pIndices);
			RETURN_CHECK(nullptr == pIndex32, E_FAIL);
		}
		for (_uint i = 0; i < iIndiescCnt; ++i) {
			// Pos Container
			ARRAY<_float3, UCell::POINT_END> PosContainer{
				(*spTerrainPos.get())[pIndex32[i]._1], (*spTerrainPos.get())[pIndex32[i]._2], (*spTerrainPos.get())[pIndex32[i]._3]
			};
			// Create Constructor
			SHPTR<UCell> pCell = CreateConstructorNative<UCell>(GetDevice(), PosContainer, i);
			m_spCellContainer->push_back(pCell);
		}
	}
	return S_OK;
}

HRESULT UNavigation::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK(_vecDatas.size() <= 0, E_FAIL);
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	NAVDESC tDesc = UMethod::ConvertTemplate_Index<NAVDESC>(_vecDatas, 0);
	m_iCurIndex = tDesc.iCurIndex;
	if ((*m_spCellContainer.get()).size() > m_iCurIndex)
		m_spCurCell = (*m_spCellContainer.get())[m_iCurIndex];
	else
		m_spCurCell = (*m_spCellContainer.get())[0];

	return S_OK;
}

const _float UNavigation::ComputeHeight(const _float3& _vPosition)
{
	RETURN_CHECK(nullptr == m_spCellContainer, false);

	if (nullptr != m_spCurCell) {
		return m_spCurCell->ComputeHeight(_vPosition);
	}
	else {
		_float3 vLine{};
		for (auto& iter : *m_spCellContainer.get()) {
			if (true == iter->IsIn(_vPosition, m_iCurIndex, vLine)) {
				m_spCurCell = iter;
				break;
			}
		}
		RETURN_CHECK(nullptr == m_spCurCell, _vPosition.y);
		return m_spCurCell->ComputeHeight(_vPosition);
	}
	return _vPosition.y;
}

void UNavigation::ComputeHeight(CSHPTRREF<UTransform> _spTransform)
{
	RETURN_CHECK(nullptr == _spTransform, ;);
	_float vValue = ComputeHeight(_spTransform->GetPos());

	_float3 vPos = _spTransform->GetPos();
	vPos.y = vValue;
	_spTransform->SetPos(vPos);
}

_bool UNavigation::IsMove(const _float3& _vPosition, SHPTR<UCell>& _spCell)
{
	RETURN_CHECK(nullptr == m_spCellContainer, false);

	if (-1 == m_iCurIndex || m_iCurIndex >= (*m_spCellContainer.get()).size()) {
		return false;
	}
	_float3 vLine{};
	_int iNeighBorIndex{ -1 };
	if (true == (*m_spCellContainer.get())[m_iCurIndex]->IsIn(_vPosition, iNeighBorIndex, vLine)) {
		m_spCurCell = _spCell = (*m_spCellContainer.get())[m_iCurIndex];
	}
	else {
		if (0 <= iNeighBorIndex) {
			_uint iValue{ 0 };
			while (true) {
				RETURN_CHECK(-1 == iNeighBorIndex, false);

				if (true == (*m_spCellContainer.get())[iNeighBorIndex]->IsIn(_vPosition, iNeighBorIndex, vLine)) {
					m_iCurIndex = iNeighBorIndex;
					break;
				}
				++iValue;
				if (iValue >= 10) {
					FindCell(_vPosition);
					return false;
				}
			}
			m_spCurCell = _spCell = (*m_spCellContainer.get())[m_iCurIndex];
			return true;
		}
	}
	return false;
}

SHPTR<UCell> UNavigation::FindCell(const _float3& _vPosition)
{
	RETURN_CHECK(nullptr == m_spCellContainer, nullptr);
	_int iNeighborIndex{ 0 };
	_float3 vLine{};
	for (auto& iter : (*m_spCellContainer.get())) {
		if (true == iter->IsIn(_vPosition, iNeighborIndex, vLine)) {
			m_iCurIndex = iter->GetIndex();
			return iter;
		}
	}
	return nullptr;
}

void UNavigation::InsertCell(const _float3& _vCellPos)
{
}

_bool UNavigation::Load(const _wstring& _wstrPath)
{
	std::ifstream load{_wstrPath, std::ios::binary};
	RETURN_CHECK(!load, false);

	_uint iNum{ 0 };
	load.read((char*)&iNum, sizeof(_uint));
	m_spCellContainer = std::make_shared<CELLCONTAINER>();
	m_spCellContainer->resize(iNum);
	// Num
	load.read((char*)m_spCellContainer->data(), sizeof(UCell) * iNum);
	return true;
}

_bool UNavigation::Save(const _wstring& _wstrPath)
{
	std::ofstream save{_wstrPath, std::ios::binary};
	RETURN_CHECK(!save, false);

	_uint iNum{ (_uint)(*m_spCellContainer.get()).size()};
	save.write((char*)&iNum, sizeof(_uint));
	save.write((char*)m_spCellContainer->data(), sizeof(UCell) * iNum);
	return true;
}

HRESULT UNavigation::ReadyNeighbor()
{
	RETURN_CHECK(nullptr == m_spCellContainer, E_FAIL);

	for (auto& pSourCell : (*m_spCellContainer.get())) {
		for (auto& pDestCell : (*m_spCellContainer.get())) {
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->IsComparePoints(pSourCell->GetPoint(UCell::POINT_A), pSourCell->GetPoint(UCell::POINT_B))) {
				pSourCell->SetNeighbor(UCell::LINE_AB, pDestCell->GetIndex());
				continue;
			}

			if (true == pDestCell->IsComparePoints(pSourCell->GetPoint(UCell::POINT_B), pSourCell->GetPoint(UCell::POINT_C))) {
				pSourCell->SetNeighbor(UCell::LINE_BC, pDestCell->GetIndex());
				continue;
			}

			if (true == pDestCell->IsComparePoints(pSourCell->GetPoint(UCell::POINT_C), pSourCell->GetPoint(UCell::POINT_A)))
				pSourCell->SetNeighbor(UCell::LINE_CA, pDestCell->GetIndex());
		}
	}

	return S_OK;
}
