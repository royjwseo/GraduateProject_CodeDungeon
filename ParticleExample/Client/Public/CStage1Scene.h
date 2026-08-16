#ifndef _PTH_FRAMEWORK_CLIENT_PUBLIC_CSTAGE1SCENE_H
#define _PTH_FRAMEWORK_CLIENT_PUBLIC_CSTAGE1SCENE_H

#include "UScene.h"

BEGIN(Engine)
class USkyBox;
class UTerrain;
class UParticle;
class UAnimateParticle;
class UFont;
END

BEGIN(Client)
class CMainCamera;
class CPlayer;
class CTree;
class CWater;
class CAimingPoint;
class CMonster;

class CStage1Scene final  : public UScene {
public:
	CStage1Scene(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(CStage1Scene)
	DESTRUCTOR(CStage1Scene)
public:
	// UScene을(를) 통해 상속됨
	virtual void Free() override;
	virtual HRESULT LoadSceneData() override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void LateTick(const _double& _dTimeDelta) override;
	virtual void CollisionTick(const _double& _dTimedelta) override;
private:
	SHPTR<CMainCamera>			m_spMainCamera;
	SHPTR<USkyBox>					m_spSkyBox;
	SHPTR< UTerrain>					m_spTerrainObject;
	SHPTR<CPlayer>						m_spPlayer;
	SHPTR<UParticle>					m_spParticle;
	SHPTR<CWater>						m_spWater;
	SHPTR< CAimingPoint>			m_spAimingPoint;

	SHPTR<UFont>						m_spScoreFont;
	SHPTR<UFont>						m_spWinFont;
	_uint											m_iEnemyDieCount;

	CUSTIMER									m_WinTimer;
	// TreeList
	VECTOR<SHPTR<CTree>>	m_TreeContainer;
	constexpr static _uint			MAX_TREE_CNT{ 100 };

	LIST<SHPTR<CMonster>> m_MonsterList;
	constexpr static _uint			MAX_MONSTER_CNT{ 30 };
};

END

#endif // _PTH_FRAMEWORK_CLIENT_PUBLIC_CSTAGE1SCENE_H