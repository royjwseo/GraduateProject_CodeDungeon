#include "ClientDefine.h"
#include "CMonster.h"
#include "UTexGroup.h"
#include "UMethod.h"
#include "UShaderConstantBuffer.h"
#include "UTerrain.h"
#include "UTransform.h"
#include "UShader.h"
#include "UGameInstance.h"
#include "CPlayer.h"
#include "UModel.h"
#include "UCollider.h"
#include "UAnimatedParticle.h"

CMonster::CMonster(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) :
	UCharacter(_spDevice, _wstrLayer, _eCloneType),
	m_spModel{nullptr},
	m_spTarget{nullptr}, 
	m_spTerrain{nullptr},
	m_vPrevPos{},
	m_fSpeed{ 0.f },
	m_vForwardPos{},
	m_vBackPos{},
	m_iHitCount{0}
{
}

CMonster::CMonster(const CMonster& _rhs) :
	UCharacter(_rhs),
	m_spModel{ nullptr },
	m_spTarget{ nullptr },
	m_spTerrain{ nullptr },
	m_vPrevPos{},
	m_fSpeed{ 0.f },
	m_vForwardPos{},
	m_vBackPos{},
	m_iHitCount{ 0 }
{
}

void CMonster::Free()
{
}

HRESULT CMonster::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CMonster::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK(_vecDatas.size() <= 0, E_FAIL);
	m_spTerrain = UMethod::ConvertTemplate_Index<SHPTR<UTerrain>>(_vecDatas, 0);
	m_spTarget = UMethod::ConvertTemplate_Index < SHPTR<CPlayer>>(_vecDatas, 1);
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	_uint iRandCount = spGameInstance->GetRandomNumber(3);
	m_fSpeed =  200;
	if (iRandCount == 0)
	{
		m_spModel = static_pointer_cast<UModel>(spGameInstance->CloneResource(PROTO_RES_MONSTERMODE_01));
		m_iHitCount = 1;
	}
	else if (iRandCount == 1)
	{
		m_spModel = static_pointer_cast<UModel>(spGameInstance->CloneResource(PROTO_RES_MONSTERMODE_02));
		m_iHitCount = 2;
		m_fSpeed += 50.f;
	}
	else
	{
		m_spModel = static_pointer_cast<UModel>(spGameInstance->CloneResource(PROTO_RES_MONSTERMODE_03));
		m_iHitCount = 3;
		m_fSpeed += 85.f;
	}
	AddShader(PROTO_RES_MODELSHADER, RES_SHADER);
	//GetTransform()->SetPos({ 0.f, 0.f, 0.f });
	UCollider::COLLIDERDESC tDesc;
	{
		tDesc.vScale = _float3{ 500.f, 250.f, 800.f };
		tDesc.vTranslation = _float3{ 0.f, 150.f, 26.f };
	}
	m_spCollider = AddComponent<UCollider>(PROTO_COMP_OBBCOLLIDER, L"OBBCollider", { &tDesc });
	{
		tDesc.vScale = _float3{ 500.f, 50.f, 50.f };
		tDesc.vTranslation = _float3{ 0.f, 100.f, 26.f };
	}
	m_spFirstCheckCollider = AddComponent<UCollider>(PROTO_COMP_SPHERECOLLIDER, L"SphereCollider", { &tDesc });

	{
		UParticle::PARTICLEDESC tDesc;
		tDesc.wstrParticleComputeShader = PROTO_RES_COMPUTEPARTICLE2DSHADER;
		tDesc.wstrParticleShader = PROTO_RES_2DANIMATEPARTICLESHADER;
		tDesc.wstrParticleTextureName = L"Flare_Sparks_TexS";

		tDesc.ParticleParam.stGrobalParticleInfo.fAccTime = 2.f;
		tDesc.ParticleParam.stGrobalParticleInfo.fDeltaTime = 2.f;
		tDesc.ParticleParam.stGrobalParticleInfo.fEndScaleParticle = 5.f;
		tDesc.ParticleParam.stGrobalParticleInfo.fMaxLifeTime = 10.f;
		tDesc.ParticleParam.stGrobalParticleInfo.fMaxSpeed = 5000.f;
		tDesc.ParticleParam.stGrobalParticleInfo.fMinSpeed = 2000.f;
		tDesc.ParticleParam.stGrobalParticleInfo.fStartScaleParticle = 10.f;
		tDesc.ParticleParam.stGrobalParticleInfo.iMaxCount = 300;
		UAnimatedParticle::ANIMPARTICLEDESC tAnimDesc;
		tAnimDesc.vTextureSize = _float2{ 2.f, 1.f };
		tAnimDesc.fNextAnimTime = 0.1f;

		m_spAnimateParticle = std::static_pointer_cast<UAnimatedParticle>(spGameInstance->CloneActorAdd(PROTO_ACTOR_ANIMATEPARTICLE,
			{ &tDesc, &tAnimDesc }));
		m_spAnimateParticle->SetActive(false);
	}
	return S_OK;
}

bool CMonster::IsHit(CSHPTRREF<UPawn> _spEnemy)
{
	--m_iHitCount;
	if (m_iHitCount <= 0)
	{
		m_spAnimateParticle->GetTransform()->SetPos(GetTransform()->GetPos() + _float3{0.f, 50.f, 0.f});
		m_spAnimateParticle->SetActive(true);
		SetActive(false);
	}
	return true;
}

void CMonster::TickActive(const _double& _dTimeDelta)
{
	m_vPrevPos = GetTransform()->GetPos();

	GetTransform()->LookAt(m_spTarget->GetTransform()->GetPos());

	if (nullptr != m_spTarget)
	{
		GetTransform()->MoveForward(_dTimeDelta, m_fSpeed);

		if (nullptr != m_spTerrain)
		{
			_float3 vPosition = GetTransform()->GetPos();
			m_spTerrain->TerrainHeightPos(vPosition);
			GetTransform()->SetPos(vPosition);

			 vPosition = GetTransform()->GetPos();
			_float3 vLook = GetTransform()->GetLook();
			m_vForwardPos = vPosition + GetTransform()->GetLook() * 200.f;
			m_vBackPos = vPosition - GetTransform()->GetLook() * 200.f;
			m_spTerrain->TerrainHeightPos(m_vForwardPos);
			m_spTerrain->TerrainHeightPos(m_vBackPos);
			// 벡터 사이의 각도를 계산
			_float3 v1 = _float3::NormalizeValue(m_vBackPos - m_vForwardPos);
			_float3 v2 = _float3::Up;
			_float dot = _float3::Dot(v1, v2);

			_float3 vAngle = GetTransform()->GetAngles();
			vAngle.x = DirectX::XMConvertToDegrees(dot);
			GetTransform()->RotateFix(vAngle);
		}
	}
}

void CMonster::LateTickActive(const _double& _dTimeDelta)
{
	m_spCollider->SetTransform(GetTransform());
	m_spFirstCheckCollider->SetTransform(GetTransform());

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	if (false == spGameInstance->IsFrustomContains(GetTransform()->GetPos(), GetTransform()->GetScale().x * GetTransform()->GetScale().y))
		return;

	if (nullptr != m_spModel)
	{
		AddRenderGroup(RI_NONALPHA_MIDDLE);

#ifdef _USE_DEBUGGING
		m_spCollider->AddRenderer(RI_NONALPHA_LAST);
		m_spFirstCheckCollider->AddRenderer(RI_NONALPHA_LAST);
#endif 
	}

}

HRESULT CMonster::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
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

void CMonster::Collision(CSHPTRREF<UPawn> _pEnemy)
{
	
}
