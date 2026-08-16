#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UTERRAIN_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UTERRAIN_H

#include "UPawn.h"


BEGIN(Engine)
class UVIBufferTerrain;
class UTexGroup;
class UNavigation;

class ENGINE_DLL  DEF_CACHE_ALGIN  UTerrain : public UPawn {
public:
	typedef struct tagTerrainDesc {
		_wstring				wstrBufferProtoName{};
		_wstring				wstrTextureProtoName{};
		_wstring				wstrTextureName{ L"" };
	}TERRAINDESC;
public:
	UTerrain(CSHPTRREF<UDevice> _spDevice, 	const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UTerrain(const UTerrain& _rhs);
	DESTRUCTOR(UTerrain)
public:
	CSHPTRREF<UVIBufferTerrain> GetVIBufferTerrain() const { return m_spVIBufferTerrain; }
	CSHPTRREF<UTexGroup> GetTextureGroup() const { return m_spTextureGroup; }
	const _wstring& GetTextureName() const { return m_wstrTextureName; }
public:
	CLONE_MACRO(UTerrain, "UTerrain::Clone To Failed")
	virtual void Free() override;
	// Native Construct
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;

	_bool TerrainHeightPos(_float3& _vPosition);
protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;
private:
	SHPTR<UVIBufferTerrain>			m_spVIBufferTerrain;
	SHPTR<UTexGroup>					m_spTextureGroup;
	DEF_CACHE_ALGIN _wstring		m_wstrTextureName;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UTERRAIN_H

