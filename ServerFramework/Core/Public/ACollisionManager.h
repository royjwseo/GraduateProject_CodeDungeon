#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ACOLLISIONMANAGER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ACOLLISIONMANAGER_H

#include "ACoreBase.h"

BEGIN(Core)
class ACollider;
class APawn;
class ASession;

using COLLISIONPAWNLIST = CONSET<APawn*>;

class ACollisionManager final : public ACoreBase {
public:
	ACollisionManager();
	DESTRUCTOR(ACollisionManager)
public:
	void AddMonsterPawnList(APawn* _pPawn);
	void CollisionSituation(const _double _dTimeDelta);
	void CollisionSituationToPlayer(ASession* _pSession, const _double _dTimeDelta);
private:
	virtual void Free() override;
private:
	COLLISIONPAWNLIST		m_MonsterPawnList;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ACOLLISIONMANAGER_H