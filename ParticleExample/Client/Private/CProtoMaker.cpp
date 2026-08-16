#include "ClientDefine.h"
#include "CProtoMaker.h"
#include "UGameInstance.h"

#include "UDevice.h"
#include "UCommand.h"

#include "CMainCamera.h"
#include "UVIBufferTerrain.h"
#include "UTexGroup.h"
#include "URawData.h"

#include "CPlayer.h"
#include "UModel.h"
#include "CTree.h"
#include "CWater.h"
#include "CPlayerGun.h"
#include "CAimingPoint.h"
#include "CMonster.h"
#include "CBullet.h"

HRESULT CProtoMaker::CreateProtoData(CSHPTRREF<UGameInstance> _spGameInstance, CSHPTRREF<UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand)
{
	_spGameInstance->AddPrototype(PROTO_ACTOR_MAINCAMERA, CreateConstructorToNative<CMainCamera>(_spDevice, LAYTER_CAM, CLONETYPE::CLONE_ONCE));
	_spGameInstance->AddPrototype(PROTO_ACTOR_PLAYER, CreateConstructorToNative<CPlayer>(_spDevice, LAYER_PLAYER, CLONETYPE::CLONE_ONCE));
	_spGameInstance->AddPrototype(PROTO_ACTOR_TREE, CreateConstructorToNative<CTree>(_spDevice, LAYER_PLAYER, CLONETYPE::CLONE_ONCE));
	_spGameInstance->AddPrototype(PROTO_ACTOR_WATER, CreateConstructorToNative<CWater>(_spDevice, LAYER_WATER, CLONETYPE::CLONE_ONCE));
	_spGameInstance->AddPrototype(PROTO_ACTOR_PLAYERGUN, CreateConstructorToNative<CPlayerGun>(_spDevice, LAYER_ITEM, CLONETYPE::CLONE_ONCE));
	_spGameInstance->AddPrototype(PROTO_ACTOR_AIMPOINT_UI, CreateConstructorToNative<CAimingPoint>(_spDevice, LAYER_UI, CLONETYPE::CLONE_ONCE));
	_spGameInstance->AddPrototype(PROTO_ACTOR_MONSTER, CreateConstructorToNative<CMonster>(_spDevice, LAYER_MONSTER, CLONETYPE::CLONE_ONCE));
	_spGameInstance->AddPrototype(PROTO_ACTOR_BULLET, CreateConstructorToNative<CBullet>(_spDevice, LAYER_BULLET, CLONETYPE::CLONE_ONCE));

	_spGameInstance->FontCreate(FONT_NAMUSQAURE, L"..\\..\\Resource\\Font\\NanumSquare.spritefont");
	return S_OK;
}

HRESULT CProtoMaker::LoadStageScene1(CSHPTRREF<UGameInstance> _spGameInstance, CSHPTRREF<UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand)
{
	SHPTR<UVIBufferTerrain> pTerrain = CreateConstructorNative<UVIBufferTerrain>(_spDevice,
		L"..\\..\\Resource\\Terrain\\HeightMap\\Height.bmp", 10.f);

	_spGameInstance->AddPrototype(PROTO_RES_VIBUFFERTERRAIN, CLONETYPE::CLONE_ONCE, pTerrain);

	_spGameInstance->AddPrototype(PROTO_RES_TERRAINGROUP, CLONETYPE::CLONE_ONCE,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\Terrain\\Diffuse", true));

	_spGameInstance->AddPrototype(PROTO_RES_WATERTEXTUREGROUP, CLONETYPE::CLONE_ONCE,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\Water\\", true));

	_spGameInstance->AddPrototype(PROTO_RES_CUBETEXTUREGROUP, CLONETYPE::CLONE_ONCE,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\SkyBox", true));

	_spGameInstance->AddPrototype(PROTO_RES_PARTICLETEXTUREGROUP, CLONETYPE::CLONE_ONCE,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\Particle", true));

	_spGameInstance->AddPrototype(PROTO_RES_UITEXUREGROUP, CLONETYPE::CLONE_ONCE,
		CreateConstructorNative<UTexGroup>(_spDevice, L"..\\..\\Resource\\UI", true));

	{
		_wstring strPath = L"..\\..\\Resource\\Model\\Eviornmemt\\Tree01\\Convert\\tree01_FBX.bin";
		SHPTR<UModel> spModel = CreateConstructorNative < UModel >(_spDevice, strPath);
		_spGameInstance->AddPrototype(PROTO_RES_TREEMODEL_01, CLONETYPE::CLONE_ONCE, spModel);
	}
	{
		_wstring strPath = L"..\\..\\Resource\\Model\\Eviornmemt\\Tree02\\Convert\\tree02_FBX.bin";
		SHPTR<UModel> spModel = CreateConstructorNative < UModel >(_spDevice, strPath);
		_spGameInstance->AddPrototype(PROTO_RES_TREEMODEL_02, CLONETYPE::CLONE_ONCE, spModel);
	}
	{
		_wstring strPath = L"..\\..\\Resource\\Model\\Object\\SciGun\\Convert\\mauler_FBX.bin";
		SHPTR<UModel> spModel = CreateConstructorNative<UModel>(_spDevice, strPath);
		_spGameInstance->AddPrototype(PROTO_RES_GUNMODEL_01, CLONETYPE::CLONE_ONCE, spModel);
	}
	{
		_wstring strPath = L"..\\..\\Resource\\Model\\Object\\T90LP\\Convert\\T90LP Desert Camo_FBX.bin";
		SHPTR<UModel> spModel = CreateConstructorNative<UModel>(_spDevice, strPath);
		_spGameInstance->AddPrototype(PROTO_RES_MONSTERMODE_01, CLONETYPE::CLONE_ONCE, spModel);
	}
	{
		_wstring strPath = L"..\\..\\Resource\\Model\\Object\\T90LP\\Convert\\T90LP ForrestCamo_FBX.bin";
		SHPTR<UModel> spModel = CreateConstructorNative<UModel>(_spDevice, strPath);
		_spGameInstance->AddPrototype(PROTO_RES_MONSTERMODE_02, CLONETYPE::CLONE_ONCE, spModel);
	}
	{
		_wstring strPath = L"..\\..\\Resource\\Model\\Object\\T90LP\\Convert\\T90LP ForrestWavyCamo_FBX.bin";
		SHPTR<UModel> spModel = CreateConstructorNative<UModel>(_spDevice, strPath);
		_spGameInstance->AddPrototype(PROTO_RES_MONSTERMODE_03, CLONETYPE::CLONE_ONCE, spModel);
	}
	{
		_wstring strPath = L"..\\..\\Resource\\Model\\Object\\Bullet\\Convert\\Bullet1_FBX.bin";
		SHPTR<UModel> spModel = CreateConstructorNative<UModel>(_spDevice, strPath);
		_spGameInstance->AddPrototype(PROTO_RES_BULLETMODEL, CLONETYPE::CLONE_ONCE, spModel);
	}

	return S_OK;
}
