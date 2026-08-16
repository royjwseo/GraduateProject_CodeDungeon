#include "CoreDefines.h"
#include "ACollisionManager.h"
#include "APawn.h"
#include "ASession.h"

namespace Core {


	ACollisionManager::ACollisionManager() : m_MonsterPawnList{}
	{
	}

	void ACollisionManager::AddMonsterPawnList(APawn* _pPawn)
	{
		assert(nullptr != _pPawn);
		m_MonsterPawnList.insert(_pPawn);
	}

	void ACollisionManager::CollisionSituation(const _double _dTimeDelta)
	{
		for (auto& iter : m_MonsterPawnList)
		{
			if (nullptr == iter)
				continue;

			if (true == iter->IsPermanentDisable())
				continue;

			if (false == iter->IsActive())
				continue;

			for (auto& Monster : m_MonsterPawnList)
			{
				if (nullptr == Monster)
					continue;

				if (true == Monster->IsPermanentDisable())
					continue;

				if (false == Monster->IsActive())
					continue;

				iter->IsHit(Monster, _dTimeDelta);
			}
		}
	}

	void ACollisionManager::CollisionSituationToPlayer(ASession* _pSession, const _double _dTimeDelta)
	{
		for (auto& iter : m_MonsterPawnList)
		{
			if (nullptr == iter)
				continue;

			if (true == iter->IsPermanentDisable())
				continue;

			if (false == iter->IsActive())
				continue;

			_pSession->IsHit(iter, _dTimeDelta);
		}
	}

	void ACollisionManager::Free()
	{
	}

}