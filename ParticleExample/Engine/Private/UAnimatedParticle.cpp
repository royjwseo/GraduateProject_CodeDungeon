#include "EngineDefines.h"
#include "UAnimatedParticle.h"
#include "UParticleSystem.h"
#include "UShader.h"
#include "UTexGroup.h"
#include "UTexture.h"
#include "UMethod.h"
#include "UGameInstance.h"
#include "UTransform.h"
#include "UVIBufferPoint.h"
#include "UShaderConstantBuffer.h"

UAnimatedParticle::UAnimatedParticle(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) : 
	UParticle(_spDevice, _wstrLayer, _eCloneType), m_vTextureSize{}, m_vUvPos{}, m_NextAnimTimer{}
{
}

UAnimatedParticle::UAnimatedParticle(const UParticle& _rhs) : 
	UParticle(_rhs), m_vTextureSize{}, m_vUvPos{}, m_NextAnimTimer{}
{
}

void UAnimatedParticle::Free()
{
}

HRESULT UAnimatedParticle::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UAnimatedParticle::NativeConstructClone(const VOIDDATAS& _convecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_convecDatas), E_FAIL);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	if (_convecDatas.size() > 1)
	{
		ANIMPARTICLEDESC stParticleDesc = UMethod::ConvertTemplate_Index<ANIMPARTICLEDESC>(_convecDatas, 1);
		m_vTextureSize = stParticleDesc.vTextureSize;

		m_spAnimPaticleBuffer = CreateNative<UShaderConstantBuffer>(GetDevice(), CBV_REGISTER::ANIMATEPARTICLEPARAM, ANIMATEPARTICLE_SIZE);

		m_NextAnimTimer = CUSTIMER(stParticleDesc.fNextAnimTime);
	}
	return S_OK;
}

void UAnimatedParticle::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
	Animated(_dTimeDelta);
}

void UAnimatedParticle::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);
}

HRESULT UAnimatedParticle::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderActive(_spCommand, _spTableDescriptor);
}

void UAnimatedParticle::BindShaderBuffer()
{
	m_AnimParticleParam.mAnimateMatrix = m_mTextureAnimation.Transpose();
	GetShader()->BindCBVBuffer(m_spAnimPaticleBuffer, &m_AnimParticleParam, ANIMATEPARTICLE_SIZE);
}

void UAnimatedParticle::Collision(CSHPTRREF<UPawn> _pEnemy)
{

}

_bool UAnimatedParticle::Save(const _wstring& _wstrPath)
{
	return _bool();
}

_bool UAnimatedParticle::Load(const _wstring& _wstrPath)
{
	return _bool();
}

void UAnimatedParticle::Animated(const _double& _dTimeDelta)
{
	m_mTextureAnimation._11 = 1.0f / m_vTextureSize.x;
	m_mTextureAnimation._22 = 1.0f / m_vTextureSize.y;
	m_mTextureAnimation._31 = m_vUvPos.x / m_vTextureSize.x;
	m_mTextureAnimation._32 = m_vUvPos.y / m_vTextureSize.y;
	if (true == m_NextAnimTimer.IsOver(_dTimeDelta))
	{
		m_NextAnimTimer.ResetTimer();
		if (++m_vUvPos.x== m_vTextureSize.x) { m_vUvPos.y++; m_vUvPos.x = 0; }
		if (m_vUvPos.y == m_vTextureSize.y) m_vUvPos.y = 0;
	}
}
#ifdef _USE_IMGUI
void UAnimatedParticle::ShowObjectInfo()
{
}
#endif 