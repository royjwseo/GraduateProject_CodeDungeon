#include "EngineDefines.h"
#include "UTerrain.h"
#include "UVIBufferTerrain.h"
#include "UTexGroup.h"
#include "UTransform.h"
#include "UShader.h"
#include "UGameInstance.h"
#include "UMethod.h"

UTerrain::UTerrain(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) :
	UPawn(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::STATIC), 
	m_spVIBufferTerrain{ nullptr }, m_spTextureGroup{ nullptr }, m_wstrTextureName{ L"" }
{
}

UTerrain::UTerrain(const UTerrain& _rhs) : UPawn(_rhs), 
m_spVIBufferTerrain{ _rhs.m_spVIBufferTerrain }, m_spTextureGroup{ _rhs.m_spTextureGroup }
, m_wstrTextureName{ _rhs.m_wstrTextureName }
{
}

void UTerrain::Free()
{
}

HRESULT UTerrain::NativeConstruct()
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(), E_FAIL);
	return S_OK;
}

HRESULT UTerrain::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	RETURN_CHECK(_vecDatas.size() <= 0, E_FAIL);

	TERRAINDESC tTerrainDesc = UMethod::ConvertTemplate_Index<TERRAINDESC>(_vecDatas, 0);

	SHPTR<UGameInstance> pGameInstance = GET_INSTANCE(UGameInstance);
	// Clone To Resource
	m_spVIBufferTerrain = AddResource<UVIBufferTerrain>(tTerrainDesc.wstrBufferProtoName, RES_VIBUFFER);
	// Texture Proto Name
	if (L"" != tTerrainDesc.wstrTextureProtoName) {
		m_spTextureGroup = AddResource<UTexGroup>(tTerrainDesc.wstrTextureProtoName, RES_TEXGROUP);
	}
	AddShader(PROTO_RES_NORMALOBJECTSHADER);
	return S_OK;
}

_bool UTerrain::TerrainHeightPos(_float3& _vPosition)
{
	RETURN_CHECK(nullptr == m_spVIBufferTerrain, false);
	return m_spVIBufferTerrain->TerrainHeightPos(_vPosition, GetTransform()->GetWorldMatrix());
}

void UTerrain::TickActive(const _double& _dTimeDelta)
{
}

void UTerrain::LateTickActive(const _double& _dTimeDelta)
{
	AddRenderGroup(RENDERID::RI_NONALPHA_FIRST);
}

HRESULT UTerrain::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);

	if (nullptr != m_spTextureGroup)
	{
		m_spTextureGroup->SetUpTextureName(GetShader(), SRV_REGISTER::T0, m_wstrTextureName);
	}
	{
		GetTransform()->BindTransformData(GetShader());
	}
	m_spVIBufferTerrain->Render(GetShader(), _spCommand);
	return S_OK;
}

void UTerrain::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}
