#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UNAVIGATION_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UNAVIGATION_H

#include "UResource.h"
#include "UCell.h"
#include "UVIBufferTerrain.h"

BEGIN(Engine)
class UTransform;

using CELLCONTAINER = std::vector<SHPTR<UCell>>;

class ENGINE_DLL DEF_CACHE_ALGIN  UNavigation : public UResource {
public:
	typedef struct tagNavDesc {
		_uint iCurIndex{ 0 };
	}NAVDESC;
public:
	UNavigation(CSHPTRREF <UDevice> _spDevice);
	UNavigation(const UNavigation& _rhs);
	DESTRUCTOR(UNavigation)
public:
	CSHPTRREF<CELLCONTAINER> GetCellContainer() const { return m_spCellContainer; }
public:
	CLONE_MACRO(UNavigation, "Navigation::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	HRESULT NativeConstruct(const VECTOR<SHPTR< UVIBufferTerrain>>& _vecTerrain);
	HRESULT NativeConstruct(const VECTOR<_float3>& _vecPosList);
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
	// Compute Height
	const _float  ComputeHeight(const _float3& _vPosition);
	void ComputeHeight(CSHPTRREF<UTransform> _spTransform);
	// Move
	_bool IsMove(const _float3& _vPosition, SHPTR<UCell>& _spCell);
	// Find
	SHPTR<UCell> FindCell(const _float3& _vPosition);
	// Insert Cell
	void InsertCell(const _float3& _vCellPos);
	// SaveLoada
	_bool Load(const _wstring& _wstrPath);
	_bool Save(const _wstring& _wstrPath);
private:
	// Ready Neighbor
	HRESULT ReadyNeighbor();
private:private:
	SHPTR<CELLCONTAINER>		m_spCellContainer;
	SHPTR<UCell>							m_spCurCell;
	_int												m_iCurIndex;

};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UNAVIGATION_H