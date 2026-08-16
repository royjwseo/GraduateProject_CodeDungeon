#include "ClientDefine.h"
#include "CPlayer.h"
#include "UTexGroup.h"
#include "UMethod.h"
#include "UShaderConstantBuffer.h"
#include "UTerrain.h"
#include "UTransform.h"
#include "UShader.h"
#include "UVIBufferCube.h"
#include "CMainCamera.h"
#include "UGameInstance.h"
#include "CPlayerGun.h"
#include "UCollider.h"
#include "CMonster.h"
#include "CBullet.h"

CPlayer::CPlayer(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) : 
	UCharacter(_spDevice, _wstrLayer, _eCloneType),
	m_stNormalCubeParam{},
	m_iNormalCubeParamSize{ sizeof(NORMALCUBEPARAM) },
	m_spNormalCubeShaderConstnatBuffer{ nullptr },
	m_spVIBufferCube{ nullptr },
	m_spTexGroup{ nullptr },
	m_spTerrain{ nullptr },
	m_spMainCamera{nullptr}
{
}

CPlayer::CPlayer(const CPlayer& _rhs) : 
	UCharacter(_rhs),
	m_stNormalCubeParam{},
	m_iNormalCubeParamSize{ sizeof(NORMALCUBEPARAM) },
	m_spNormalCubeShaderConstnatBuffer{ nullptr },
	m_spVIBufferCube{ nullptr },
	m_spTexGroup{ nullptr },
	m_spTerrain{ nullptr },
	m_spMainCamera{ nullptr }
{
}

void CPlayer::Free()
{
}

HRESULT CPlayer::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CPlayer::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK(_vecDatas.size() <= 0, E_FAIL);
	m_spTerrain = UMethod::ConvertTemplate_Index<SHPTR<UTerrain>>(_vecDatas, 0);
	m_spMainCamera = UMethod::ConvertTemplate_Index < SHPTR<CMainCamera>>(_vecDatas, 1);

	if (FAILED(__super::NativeConstructClone(_vecDatas)))
		return E_FAIL;

	m_spVIBufferCube = AddResource<UVIBufferCube>(PROTO_RES_VIBUFFERNORMALCUBE, RES_VIBUFFER);
	AddShader(PROTO_RES_3DNORMALCUBESHADER);
	m_spTexGroup = AddResource<UTexGroup>(PROTO_RES_CUBETEXTUREGROUP, RES_TEXGROUP);

	m_spNormalCubeShaderConstnatBuffer = CreateNative<UShaderConstantBuffer>(GetDevice(), CBV_REGISTER::NORMALCUBEPARAM, m_iNormalCubeParamSize);

	m_stNormalCubeParam.g_iNormalCubeTextureBind =0;
	m_stNormalCubeParam.g_vNormalCubeColor = _float4(1.f, 0.f, 0.f, 1.f);

	GetTransform()->SetPos(_float3{ 10, 1, 10 });
	//GetTransform()->SetScale(_float3{ 10.f, 40.f, 10.f });


	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_wpPlayerGun = std::static_pointer_cast<CPlayerGun>(spGameInstance->CloneActorAdd(PROTO_ACTOR_PLAYERGUN));
	m_wpPlayerGun.lock()->SetOwner(ThisShared<UPawn>());

	UCollider::COLLIDERDESC tDesc;
	{
		tDesc.vScale = _float3{ 10.f, 50.f, 10.f };
		tDesc.vTranslation = _float3{ 0.f, 50.f, 0.f };
	}
	m_spCollider = AddComponent<UCollider>(PROTO_COMP_OBBCOLLIDER, L"OBBCollider", { &tDesc });
	{
		tDesc.vScale = _float3{ 50.f, 50.f, 50.f };
		tDesc.vTranslation = _float3{ 0.f, 50.f, 0.f };
	}
	m_spFirstCheckCollider 	= AddComponent<UCollider>(PROTO_COMP_SPHERECOLLIDER, L"SphereCollider", { &tDesc });
	return S_OK;
}

void CPlayer::CollisionOthers(CSHPTRREF<CMonster> _spMonster)
{
	RETURN_CHECK(nullptr == _spMonster, ;);

	if (true == _spMonster->GetFirstCheckCollider()->IsCollision(m_spFirstCheckCollider))
	{
		if (true == _spMonster->GetCollider()->IsCollision(m_spCollider))
		{
			SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
			spGameInstance->SetGamming(false);
			SetActive(false);
			return;
		}
	}

	SHPTR<CPlayerGun> spGun = m_wpPlayerGun.lock();
	if (nullptr != spGun)
	{
		for (auto& iter : spGun->GetBulletContainer())
		{
			if (iter->IsActive())
			{
				_float3 vPos = iter->GetTransform()->GetPos() + iter->GetTransform()->GetLook() * 50.f;
				if (false == m_spTerrain->TerrainHeightPos(vPos))
				{
					iter->Reset();
					continue;
				}
				iter->Hit(_spMonster);
			}
		}
	}
}

void CPlayer::TickActive(const _double& _dTimeDelta)
{
	SHPTR<UGameInstance> pGameInstance = GET_INSTANCE(UGameInstance);
	if (pGameInstance->GetDIKeyDown(DIK_LCONTROL))
	{
		m_spMainCamera->SetMoveState(!m_spMainCamera->IsMoveState());
	}

	if (false == m_spMainCamera->IsMoveState())
	{
		GetTransform()->LookAt(m_spMainCamera->GetTransform()->GetLook() + GetTransform()->GetPos());

		if (pGameInstance->GetDIKeyPressing(DIK_W))
			GetTransform()->MoveForward(_dTimeDelta, 300.f);
		if (pGameInstance->GetDIKeyPressing(DIK_A))
			GetTransform()->MoveLeft(_dTimeDelta, 300.f);
		if (pGameInstance->GetDIKeyPressing(DIK_D))
			GetTransform()->MoveRight(_dTimeDelta, 300.f);
		if (pGameInstance->GetDIKeyPressing(DIK_S))
			GetTransform()->MoveBack(_dTimeDelta, 300.f);
	}

	if (nullptr != m_spTerrain)
	{
		_float3 vPosition = GetTransform()->GetPos();
		bool bIsTrue = m_spTerrain->TerrainHeightPos(vPosition);
		if (true == bIsTrue)
		{
			GetTransform()->SetPos(vPosition);
			m_vPrevPos = vPosition;
		}
		else
		{
			GetTransform()->SetPos(m_vPrevPos);
		}
	}

	if (true == pGameInstance->GetDIKeyDown(DIK_SPACE))
	{
		m_wpPlayerGun.lock()->ShootBullet(m_spMainCamera->GetTransform()->GetPos(), m_spMainCamera->GetTransform()->GetLook());
	}
}

void CPlayer::LateTickActive(const _double& _dTimeDelta)
{
	if (false == m_spMainCamera->IsMoveState())
	{
		_float3 vPosition = DirectX::XMVector3Transform(_float3(0.f, 100.f, 0.5f), DirectX::XMMatrixTranslationFromVector(
		GetTransform()->GetPos()));

		m_spMainCamera->GetTransform()->SetPos(vPosition);

		m_spCollider->SetTransform(GetTransform());
		m_spFirstCheckCollider->SetTransform(GetTransform());
	}

#ifdef _USE_DEBUGGING
//	m_spCollider->AddRenderer(RI_NONALPHA_LAST);
//	m_spFirstCheckCollider->AddRenderer(RI_NONALPHA_LAST);
#endif 
}

HRESULT CPlayer::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);
	GetTransform()->BindTransformData(GetShader());
	GetShader()->BindCBVBuffer(m_spNormalCubeShaderConstnatBuffer, &m_stNormalCubeParam, m_iNormalCubeParamSize);
	m_spVIBufferCube->Render(GetShader(), _spCommand);
	return S_OK;
}

void CPlayer::Collision(CSHPTRREF<UPawn> _pEnemy)
{
	
}