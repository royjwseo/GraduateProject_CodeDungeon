#include "EngineDefines.h"
#include "UParticle.h"
#include "UParticleSystem.h"
#include "UShader.h"
#include "UTexGroup.h"
#include "UTexture.h"
#include "UMethod.h"
#include "UGameInstance.h"
#include "UTransform.h"
#include "UVIBufferPoint.h"

UParticle::UParticle(CSHPTRREF<UDevice> _spDevice,  const _wstring& _wstrLayer,
	const CLONETYPE& _eCloneType)
	: UPawn(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::NON),
	m_spTexGroup{ nullptr }, m_spParticleSystem { nullptr }, m_spVIBufferPoint{nullptr}
{
}

UParticle::UParticle(const UParticle& _rhs) : 
	UPawn(_rhs), 
	m_spTexGroup{ nullptr }, m_spParticleSystem{ nullptr }, m_spVIBufferPoint{ nullptr }
{
}

void UParticle::Free()
{
}

HRESULT UParticle::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UParticle::NativeConstructClone(const VOIDDATAS& _convecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_convecDatas), E_FAIL);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	if (_convecDatas.size() > 0)
	{
		PARTICLEDESC stParticleDesc = UMethod::ConvertTemplate_Index<PARTICLEDESC>(_convecDatas, 0);

		m_spParticleSystem = static_pointer_cast<UParticleSystem>(spGameInstance->CloneResource(PROTO_RES_PARTICLESYSTEM, {&stParticleDesc.ParticleParam}));
		m_spTexGroup = static_pointer_cast<UTexGroup>(spGameInstance->CloneResource(PROTO_RES_PARTICLETEXTUREGROUP));
		m_spVIBufferPoint = static_pointer_cast<UVIBufferPoint>(spGameInstance->CloneResource(PROTO_RES_VIBUFFERPOINT));

		m_spParticleSystem->SettingComputeShader(stParticleDesc.wstrParticleComputeShader);
		AddShader(stParticleDesc.wstrParticleShader);

		_uint iIndex = m_spTexGroup->GetTextureIndex(stParticleDesc.wstrParticleTextureName);
		m_TextureIndexContainer.insert(std::pair<_uint, SRV_REGISTER>(iIndex, SRV_REGISTER::T0));

		m_LifeTimer = CUSTIMER{ stParticleDesc.ParticleParam.stGrobalParticleInfo.fMaxLifeTime };

		SetActive(false);
	}
	return S_OK;
}

void UParticle::TickActive(const _double& _dTimeDelta)
{
	if (true == m_LifeTimer.IsOver(_dTimeDelta))
	{
		SetActive(false);
		m_LifeTimer.ResetTimer();
	}

	m_spParticleSystem->Update(_dTimeDelta);
}

void UParticle::LateTickActive(const _double& _dTimeDelta)
{
	AddRenderGroup(RENDERID::RI_ALPHA);
}

HRESULT UParticle::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{

	__super::RenderActive(_spCommand, _spTableDescriptor);
	m_spParticleSystem->Render();
	BindShaderBuffer();
	// Bind Transform
	GetTransform()->BindTransformData(GetShader());
	// Bind Particle System Buffer
	m_spParticleSystem->BindShaderParams(GetShader());
	// bind Srv Buffer 
	for (auto& iter : m_TextureIndexContainer)
	{
		GetShader()->BindSRVBuffer(iter.second, m_spTexGroup->GetTexture(iter.first));
	}

	m_spVIBufferPoint->Render(GetShader(), _spCommand, m_spParticleSystem->GetMaxParticleCnt());
	return S_OK;
}

void UParticle::BindShaderBuffer()
{
}

void UParticle::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}

_bool UParticle::Save(const _wstring& _wstrPath)
{
	return _bool();
}

_bool UParticle::Load(const _wstring& _wstrPath)
{
	return _bool();
}

#ifdef _USE_IMGUI
void UParticle::ShowObjectInfo()
{

}
#endif 