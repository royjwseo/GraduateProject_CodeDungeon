#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERTERRAIN_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERTERRAIN_H

#include "UVIBuffer.h"

BEGIN(Engine)
class URawData;

// Terrain 
typedef struct tagTerrainDesc {
	_uint iNumVerticiesX{ 0 }, iNumVerticiesZ{ 0 };
}TERRAINDESC;

class ENGINE_DLL DEF_CACHE_ALGIN UVIBufferTerrain : public UVIBuffer {
public:
	UVIBufferTerrain(CSHPTRREF <UDevice> _spDevice);
	UVIBufferTerrain(const UVIBufferTerrain& _rhs);
	DESTRUCTOR(UVIBufferTerrain)
public:
	const TERRAINDESC& GetTerrainDesc() const { return m_stTerrainDesc; }
public:
	CLONE_THIS(UVIBufferTerrain)
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	HRESULT NativeConstruct(const TERRAINDESC& _stTerrainDesc, const _float& _fHeightSize = 1.f);
	HRESULT NativeConstruct(const _wstring& _wstrMapFilePath,  const _float& _fHeightSize = 1.f);
	HRESULT NativeConstruct(CSHPTRREF<URawData> _spRawData, const _float& _fHeightSize = 1.f);
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas);

	bool TerrainHeightPos(_float3& _vPosition, const _float4x4& _mWorldMatirx);
protected:
	HRESULT CreateIndex(VECTOR<VTXNORMAL>& _vecVertex);
	HRESULT ComputeNormal(VECTOR<VTXNORMAL>& _vecVertex, INDICIES32* _pIndices, _uint& _iIndex);
private:
	// Terrain 
	TERRAINDESC		m_stTerrainDesc;
	_uint						m_iMaxIndex;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UVIBUFFERTERRAIN_H