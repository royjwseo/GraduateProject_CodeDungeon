#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UACTORGROUP_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UACTORGROUP_H

#include "UBase.h"

BEGIN(Engine)
class UActor;

class ENGINE_DLL UActorGroup : public UBase {
public:
	UActorGroup();
	NO_COPY(UActorGroup)
	DESTRUCTOR(UActorGroup)
public:
	using ACTORLIST = SET<SHPTR<UActor>>;
	const ACTORLIST& GetActiveActorList() const { return m_lsActiveActors; }
public:
	virtual void Free() override;
public:
	void Tick(const _double& _dTimeDelta);
	void LateTick(const _double& _dTimeDelta);
	void AddActive(CSHPTRREF<UActor> _pActor);
	void AddDelete(CSHPTRREF<UActor> _pActor);
private:
	ACTORLIST	m_lsActiveActors;
	ACTORLIST	m_lsDeleteActors;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UACTORGROUP_H