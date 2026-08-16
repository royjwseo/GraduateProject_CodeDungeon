#include "ClientDefine.h"
#include "CBullet.h"
#include "UTexGroup.h"
#include "UMethod.h"
#include "UModel.h"
#include "UTransform.h"
#include "UGameInstance.h"
#include "UCollider.h"
#include "CMonster.h"

CBullet::CBullet(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) : 
	UPawn(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::DYNAMIC),
	m_spModel{ nullptr }, m_BulletDesc{}, m_CusTimer {
	m_BulletDesc.fLifeTime
}, m_vDir{}
{
}

CBullet::CBullet(const CBullet& _rhs) : 
	UPawn(_rhs),
	m_spModel{ nullptr }, m_vDir{}, m_BulletDesc{}, m_CusTimer{ m_BulletDesc.fLifeTime }
{
}

void CBullet::Free()
{
}

HRESULT CBullet::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CBullet::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	if (FAILED(__super::NativeConstructClone(_vecDatas)))
		return E_FAIL;

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spModel = static_pointer_cast<UModel>(spGameInstance->CloneResource(PROTO_RES_BULLETMODEL));
	AddShader(PROTO_RES_MODELSHADER, RES_SHADER);

	UCollider::COLLIDERDESC tDesc;
	{
		tDesc.vScale = _float3{ 10.f, 10.f, 50.f };
		tDesc.vTranslation = _float3{ 0.f, 0.f, 26.f };
	}
	m_spCollider = AddComponent<UCollider>(PROTO_COMP_OBBCOLLIDER, L"OBBCollider", { &tDesc });
	{
		tDesc.vScale = _float3{ 50.f, 50.f, 50.f };
		tDesc.vTranslation = _float3{ 0.f, 0.f, 26.f };
	}
	m_spFirstCheckCollider = AddComponent<UCollider>(PROTO_COMP_SPHERECOLLIDER, L"SphereCollider", { &tDesc });
	return S_OK;
}

bool CBullet::Hit(CSHPTRREF<CMonster> _pEnemy)
{
	RETURN_CHECK(nullptr == _pEnemy, false);
	RETURN_CHECK(false == IsActive(), false);
	RETURN_CHECK(false == _pEnemy->GetFirstCheckCollider()->IsCollision(m_spFirstCheckCollider), false);
	RETURN_CHECK( false  == _pEnemy->GetCollider()->IsCollision(m_spCollider), false);
	Collision(_pEnemy);
	_pEnemy->IsHit(ThisShared<UPawn>());
	Reset();
	return true;
}

void CBullet::Reset()
{
	SetActive(false);
	m_CusTimer.ResetTimer();
}

void CBullet::TickActive(const _double& _dTimeDelta)
{
	if (m_CusTimer.IsOver(_dTimeDelta))
	{
		m_CusTimer.ResetTimer();
		SetActive(false);
	}
	else
	{
		GetTransform()->TranslateDir(m_vDir, _dTimeDelta, m_BulletDesc.fSpeed);
		m_spCollider->SetTransform(GetTransform());
		m_spFirstCheckCollider->SetTransform(GetTransform());
	}
}

void CBullet::LateTickActive(const _double& _dTimeDelta)
{
	if (nullptr != m_spModel)
	{
		AddRenderGroup(RI_NONALPHA_MIDDLE);

#ifdef _USE_DEBUGGING
		m_spCollider->AddRenderer(RI_NONALPHA_LAST);
		m_spFirstCheckCollider->AddRenderer(RI_NONALPHA_LAST);
#endif 
	}
}

HRESULT CBullet::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	if (nullptr != m_spModel)
	{
		__super::RenderActive(_spCommand, _spTableDescriptor);

		for (_uint i = 0; i < m_spModel->GetMeshContainerCnt(); ++i)
		{
			// Bind Transform 
			GetTransform()->BindTransformData(GetShader());

			m_spModel->BindTexture(i, SRV_REGISTER::T0, TEXTYPE::TextureType_DIFFUSE, GetShader());
			m_spModel->BindTexture(i, SRV_REGISTER::T1, TEXTYPE::TextureType_NORMALS, GetShader());
			// Render
			m_spModel->Render(i, GetShader(), _spCommand);
		}
	}
	return S_OK;
}

void CBullet::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}
