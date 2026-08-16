#include "ClientDefine.h"
#include "CPlayerGun.h"
#include "UTexGroup.h"
#include "UMethod.h"
#include "UModel.h"
#include "UTransform.h"
#include "UGameInstance.h"
#include "CBullet.h"

CPlayerGun::CPlayerGun(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) : 
	CGun(_spDevice, _wstrLayer, _eCloneType), m_spModel{nullptr}, m_iBulletIndex{0}, m_ShootTimer{ 0.1f }
{
}

CPlayerGun::CPlayerGun(const CPlayerGun& _rhs) :
	CGun(_rhs), m_spModel{nullptr}, m_iBulletIndex{0}, m_ShootTimer{0.1f}
{
}

void CPlayerGun::Free()
{
}

HRESULT CPlayerGun::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CPlayerGun::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spModel = static_pointer_cast<UModel>(spGameInstance->CloneResource(PROTO_RES_GUNMODEL_01));
	AddShader(PROTO_RES_MODELSHADER, RES_SHADER);

	for (int i = 0; i < BULLET_SIZE; ++i)
	{
		SHPTR<CBullet> spBullet = std::static_pointer_cast<CBullet>(spGameInstance->CloneActorAdd(PROTO_ACTOR_BULLET));
		spBullet->SetActive(false);
		m_BulletContainer.push_back(spBullet);
	}
	return S_OK;
}

void CPlayerGun::ShootBullet(const _float3& _vPos, const _float3& _vDir)
{
	if (m_ShootTimer.IsPass())
	{
		m_BulletContainer[m_iBulletIndex]->GetTransform()->SetPos(_vPos);
		m_BulletContainer[m_iBulletIndex]->GetTransform()->LookAt(_vPos + _vDir * 10.f);
		m_BulletContainer[m_iBulletIndex]->SetDir(_vDir);
		m_BulletContainer[m_iBulletIndex]->SetActive(true);
		m_ShootTimer.ResetTimer();

		m_iBulletIndex = (m_iBulletIndex + 1) % BULLET_SIZE;
	}
}

void CPlayerGun::TickActive(const _double& _dTimeDelta)
{
	m_ShootTimer.IsOver(_dTimeDelta);
}

void CPlayerGun::LateTickActive(const _double& _dTimeDelta)
{
	if (nullptr != m_spModel)
	{
		AddRenderGroup(RI_NONALPHA_MIDDLE);
	}
}

HRESULT CPlayerGun::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);

	SHPTR<UPawn> spPawn = GetOwner();
	if (nullptr != spPawn)
	{
		_float3 vPosition = DirectX::XMVector3Transform(_float3(10.f, 90.f, 2.f), DirectX::XMMatrixTranslationFromVector(
			spPawn->GetTransform()->GetPos()));

		GetTransform()->SetPos(vPosition);
		_float3 vAngle = spPawn->GetTransform()->GetAngles();
		vAngle.z = { 0.f };
		GetTransform()->RotateFix(vAngle);
	}

	for (_uint i = 0; i < m_spModel->GetMeshContainerCnt(); ++i)
	{
		// Bind Transform 
		GetTransform()->BindTransformData(GetShader());

		m_spModel->BindTexture(i, SRV_REGISTER::T0, TEXTYPE::TextureType_DIFFUSE, GetShader());
		m_spModel->BindTexture(i, SRV_REGISTER::T1, TEXTYPE::TextureType_NORMALS, GetShader());
		// Render
		m_spModel->Render(i, GetShader(), _spCommand);
	}
	return S_OK;
}

void CPlayerGun::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}
