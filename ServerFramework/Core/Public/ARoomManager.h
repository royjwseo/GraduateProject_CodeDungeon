#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AROOMMANAGER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AROOMMANAGER_H

#include "ACoreBase.h"

BEGIN(Core)
class ARoom;

class ARoomManager final : public ACoreBase {
public:
	using ROOMCONTAINER = CONHASHMAP<_llong, SHPTR<ARoom>>;
public:
	ARoomManager();
	NO_COPY(ARoomManager)
	DESTRUCTOR(ARoomManager)
public:


private:
	virtual void Free() override;
private:
	ROOMCONTAINER		m_RoomContainer;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AROOMMANAGER_H