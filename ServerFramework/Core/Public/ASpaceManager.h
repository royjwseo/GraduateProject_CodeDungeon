#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ASPACEMANAGER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ASPACEMANAGER_H

#include "ACoreBase.h"

BEGIN(Core)
class ASpace;
class ASession;

/*
@ Date: 2023-01-22, Writer: 박태현
@ Explain
- Space를 관리하는 매니저 클래스
*/
class ASpaceManager final : public ACoreBase {
public:
	using SPACECONTAINER = CONVECTOR<SHPTR<ASpace>>;
public:
	ASpaceManager();
	NO_COPY(ASpaceManager)
	DESTRUCTOR(ASpaceManager)
public:
	void BuildGameSpace(const SPACEINFO& _SpaceInfo);
	void InsertSessionInSpace(SHPTR<ASession >_spSession);

protected:
	void BuildTreeRecursively(const SPACEINFO& _SpaceInfo, SHPTR<ASpace> _spParents = nullptr);

private:
	virtual void Free() override;
private:
	// Root 
	SHPTR<ASpace>		m_spRootSpace;	
	SPACECONTAINER	m_SpaceContainer;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ASPACEMANAGER_H