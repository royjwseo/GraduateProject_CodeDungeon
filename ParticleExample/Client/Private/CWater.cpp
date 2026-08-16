#include "ClientDefine.h"
#include "CWater.h"
#include "UTexGroup.h"
#include "UMethod.h"
#include "UShaderConstantBuffer.h"
#include "UTerrain.h"
#include "UTransform.h"
#include "UShader.h"
#include "UVIBufferCube.h"
#include "CMainCamera.h"
#include "UGameInstance.h"

CWater::CWater(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) : 
	UPawn(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::NON), 
	m_spVIBuffer{nullptr}, 
	m_spWaterTexGroup{nullptr}
{
}

CWater::CWater(const CWater& _rhs) : 
	UPawn(_rhs),
	m_spVIBuffer{ nullptr },
	m_spWaterTexGroup{ nullptr }
{
}

void CWater::Free()
{
}

HRESULT CWater::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CWater::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK(_vecDatas.size() <= 0, E_FAIL);
	// Constructor 
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);

	m_BindTextureName = UMethod::ConvertTemplate_Index<BINDTEXTURENAME>(_vecDatas, 0);
	RETURN_CHECK(m_BindTextureName.wstrDiffuseTextureName.empty(), E_FAIL);
	AddShader(PROTO_RES_WATERSHADER);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spVIBuffer = std::static_pointer_cast<UVIBufferCube>(spGameInstance->CloneResource(PROTO_RES_VIBUFFERCUBE));
	m_spWaterTexGroup = std::static_pointer_cast<UTexGroup>(spGameInstance->CloneResource(PROTO_RES_WATERTEXTUREGROUP));

	m_spWaterShaderBuffer = CreateNative<UShaderConstantBuffer>(GetDevice(), CBV_REGISTER::WATERPARAM, WATEROBJECTPARAM_SIZE);
	GetTransform()->SetScale(_float3{ 20000.f, 250.f, 20000.f });
	GetTransform()->SetPos(_float3{ 10000.f, 00.f, 10000.f });
	return S_OK;
}

void CWater::TickActive(const _double& _dTimeDelta)
{

}

void CWater::LateTickActive(const _double& _dTimeDelta)
{
	if (nullptr != m_spVIBuffer)
		AddRenderGroup(RENDERID::RI_NONALPHA_LAST);
}

HRESULT CWater::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);
	GetTransform()->BindTransformData(GetShader());
	GetShader()->BindCBVBuffer(m_spWaterShaderBuffer, &m_WaterParam, WATEROBJECTPARAM_SIZE);
	GetShader()->BindSRVBuffer(SRV_REGISTER::T0, m_spWaterTexGroup->GetTexture(m_BindTextureName.wstrDiffuseTextureName));
	GetShader()->BindSRVBuffer(SRV_REGISTER::T1, m_spWaterTexGroup->GetTexture(m_BindTextureName.wstrDetailTexture01Name));
	GetShader()->BindSRVBuffer(SRV_REGISTER::T2, m_spWaterTexGroup->GetTexture(m_BindTextureName.wstrDetailTexture02Name));

	m_spVIBuffer->Render(GetShader(), _spCommand);
	return S_OK;
}

void CWater::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}
