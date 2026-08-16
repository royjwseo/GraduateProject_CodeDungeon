#include "ClientDefine.h"
#include "CStage1Scene.h"
#include "USkyBox.h"
#include "UGameInstance.h"
#include "CMainCamera.h"
#include "UTransform.h"
#include "UTerrain.h"
#include"CProtoMaker.h"
#include "UGpuCommand.h"
#include "CPlayer.h"
#include "UActor.h"
#include "UParticle.h"
#include "CTree.h"
#include "UVIBufferTerrain.h"
#include "CWater.h"
#include "CAimingPoint.h"
#include "CMonster.h"
#include "UAnimatedParticle.h"
#include "UFont.h"


CStage1Scene::CStage1Scene(CSHPTRREF<UDevice> _spDevice)
	: UScene(_spDevice, SCENE::SCENE_STAGE1),
	m_spMainCamera{ nullptr },
	m_spSkyBox{ nullptr },
	m_spTerrainObject{nullptr},
	m_spPlayer{nullptr},
	m_spParticle{nullptr}, 
	m_WinTimer{5.f}
{
}

void CStage1Scene::Free()
{
}

HRESULT CStage1Scene::LoadSceneData()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	{
		SHPTR<UCommand> spCommand = static_pointer_cast<UCommand>(spGameInstance->GetGpuCommand());

		CProtoMaker::LoadStageScene1(spGameInstance, GetDevice(), spCommand);
	}
	{
		USkyBox::SKYBOXDESC tDesc{L"SkyBox", L"SkyBox_Cloud", true};
		VOIDDATAS vecDatas;
		{
			vecDatas.push_back(&tDesc);
		}
		m_spSkyBox = static_pointer_cast<USkyBox>(spGameInstance->CloneActorAdd(PROTO_ACTOR_SKYBOX, vecDatas));
	}
	{
		UCamera::CAMDESC tDesc;
		tDesc.stCamProj = UCamera::CAMPROJ(UCamera::PROJECTION_TYPE::PERSPECTIVE, _float3(0.f, 0.f, 0.f),
			_float3(0.f, 0.f, 1.f),
			DirectX::XMConvertToRadians(60.0f), WINDOW_WIDTH, WINDOW_HEIGHT, 0.2f, 10000.f);
		tDesc.stCamValue = UCamera::CAMVALUE(5.f, DirectX::XMConvertToRadians(90.f));
		tDesc.eCamType = CAMERATYPE::MAIN;
		// Actor Add Main Camera
		VOIDDATAS vecDatas;
		{
			vecDatas.push_back(&tDesc);
		}
		m_spMainCamera = static_pointer_cast<CMainCamera>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MAINCAMERA, vecDatas));
		m_spMainCamera->GetTransform()->SetPos({ 0.f, 100.f,  0.f });
		m_spMainCamera->GetTransform()->LookAt({ 100.f, 0.f, 100.f });
	}
	{
		{
			// Default로 CloneOnce니까 주의
			UTerrain::TERRAINDESC tDesc;
			tDesc.wstrBufferProtoName = PROTO_RES_VIBUFFERTERRAIN;
			tDesc.wstrTextureName = L"Detail_Texture_7";
			tDesc.wstrTextureProtoName = PROTO_RES_TERRAINGROUP;
			// Terrain 생성
			VOIDDATAS vecDatas;
			{
				vecDatas.push_back(&tDesc);
			}

			m_spTerrainObject = std::static_pointer_cast<UTerrain>(spGameInstance->CloneActorAdd(PROTO_ACTOR_TERRAIN, vecDatas));
			m_spTerrainObject->GetTransform()->SetScale(_float3{ 100.f, 100.f, 100.f });
		}
	}
	{
		VOIDDATAS vecDatas;
		{
			vecDatas.push_back(&m_spTerrainObject);
			vecDatas.push_back(&m_spMainCamera);
		}
		m_spPlayer = static_pointer_cast<CPlayer>(spGameInstance->CloneActorAdd(PROTO_ACTOR_PLAYER, vecDatas));

		m_TreeContainer.reserve(MAX_TREE_CNT);
		SHPTR<UVIBufferTerrain> spBufferTerrain = m_spTerrainObject->GetVIBufferTerrain();
		for (_uint i = 0; i < MAX_TREE_CNT; ++i)
		{
			SHPTR<CTree> spTree = static_pointer_cast<CTree>(spGameInstance->CloneActorAdd(PROTO_ACTOR_TREE, vecDatas));
			m_TreeContainer.push_back(spTree);

			_float3 vPos = (*spBufferTerrain->GetVertexPos())[spGameInstance->GetRandomNumber(spBufferTerrain->GetVertexCnt())];

			vPos = _float3::TransformCoord(vPos, m_spTerrainObject->GetTransform()->GetWorldMatrix());
			vPos.y -= 3.f;
			spTree->GetTransform()->SetPos(vPos);
		}
	}
	{
		VOIDDATAS vecDatas;
		{
			vecDatas.push_back(&m_spTerrainObject);
			vecDatas.push_back(&m_spPlayer);
		}
		SHPTR<UVIBufferTerrain> spBufferTerrain = m_spTerrainObject->GetVIBufferTerrain();
		for (_uint i = 0; i < MAX_MONSTER_CNT;)
		{
			_float3 vPos = (*spBufferTerrain->GetVertexPos())[spGameInstance->GetRandomNumber(spBufferTerrain->GetVertexCnt())];

			vPos = _float3::TransformCoord(vPos, m_spTerrainObject->GetTransform()->GetWorldMatrix());

			if (m_spPlayer->GetTransform()->ComputeDistance(vPos) <= 15000.f)
				continue;

			SHPTR<CMonster> spMonster = std::static_pointer_cast<CMonster>(spGameInstance->CloneActorAdd(PROTO_ACTOR_MONSTER, vecDatas));
			m_MonsterList.push_back(spMonster);
			spMonster->GetTransform()->SetPos(vPos);
			++i;
		}
	}
	/* Water */
	{
		CWater::BINDTEXTURENAME Textures;
		Textures.wstrDiffuseTextureName = L"Water_Base_Texture_0";
		Textures.wstrDetailTexture01Name = L"Water_Detail_Texture_0";
		Textures.wstrDetailTexture02Name = L"Water_Texture_Alpha";
		m_spWater = static_pointer_cast<CWater>(spGameInstance->CloneActorAdd(PROTO_ACTOR_WATER, {&Textures}));
	}
	{
		AddLight(LIGHTINFO{ LIGHTTYPE::TYPE_DIRECTIONAL, {1.f, 1.f, 1.f, 1.f}, {0.05f, 0.05f, 0.05f, 0.05f}, {1.f, 1.f, 1.f, 1.f}, {0.f, -1.f, -1.f,},
			{0.f, 100.f, 0.f}, 0.f, 0.f, 1.f, 200.f }, 
			LIGHTCONTROL{RIMLIGHT{}});
	}

	{
		CAimingPoint::UIDESC tRectDesc;
		tRectDesc.v2Pos = _float2(_float(WINDOW_HALF_WIDTH) , (_float)(WINDOW_HALF_HEIGHT));
		tRectDesc.v2Size = _float2(20.f, 20.f);
		tRectDesc.fZBufferOrder = 0.5f;
		m_spAimingPoint = std::static_pointer_cast<CAimingPoint>(spGameInstance->CloneActorAdd(PROTO_ACTOR_AIMPOINT_UI, {&tRectDesc }));
	}
	{
		m_spScoreFont = spGameInstance->AddFont(FONT_NAMUSQAURE);
		m_spScoreFont->SetPos(_float2{ 0.f, 0.f });
		m_spScoreFont->SetText(L"Alive Enemy : ");
	}
	{
		m_spWinFont = spGameInstance->AddFont(FONT_NAMUSQAURE);
		m_spWinFont->SetPos(_float2{ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
	}
	m_WinTimer.ResetTimer();
	return S_OK;
}

void CStage1Scene::Tick(const _double& _dTimeDelta)
{
	_uint iAliveEnemy{ 0 };

	for (auto& iter : m_MonsterList)
	{
		if (true == iter->IsActive())
		{
			++iAliveEnemy;
		}
	}
	_wstring strText = L"Alive Enemy : ";
	strText += std::to_wstring(iAliveEnemy);
	m_spScoreFont->SetText(strText);

	if (0 == iAliveEnemy)
	{
		m_spWinFont->SetText(L"YouWin!");
		if (true == m_WinTimer.IsOver(_dTimeDelta))
		{
			SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
			spGameInstance->SetGamming(false);
		}
	}
}

void CStage1Scene::LateTick(const _double& _dTimeDelta)
{

}

void CStage1Scene::CollisionTick(const _double& _dTimedelta)
{
	for (auto& iter : m_MonsterList)
	{
		m_spPlayer->CollisionOthers(iter);
	}
}
