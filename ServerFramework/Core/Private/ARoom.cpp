#include "CoreDefines.h"
#include "ARoom.h"
#include "ASession.h"

namespace Core {

	ARoom::ARoom(OBJCON_CONSTRUCTOR, _uint _MaxSessionCount) :
		ACoreObject(OBJCON_CONDATA), m_RoomID{ 0 }, m_MaxSessionCount {_MaxSessionCount}, m_isEmptyRoom{true}
	{
	}

	_bool ARoom::InsertSession(SHPTR<ASession> _spSession)
	{
		RETURN_CHECK(nullptr == _spSession, false);
		// 이미 있으면 리턴
		RETURN_CHECK(nullptr != FindSession(_spSession->GetSessionID()), false)
		SESSIONID id = _spSession->GetSessionID();
		m_SessionContainer.emplace(MakePair(id, _spSession));
		m_isEmptyRoom.store(false, std::memory_order_seq_cst);
		return true;
	}

	SHPTR<ASession> ARoom::FindSession(const SESSIONID _SessionID)
	{
		SESSIONCONTAINER::accessor access;
		m_SessionContainer.find(access, _SessionID);
		return access->second;
	}

	void ARoom::DeleteSession(const SESSIONID _SessionID)
	{
		SESSIONCONTAINER::accessor access;
		m_SessionContainer.find(access, _SessionID);
		RETURN_CHECK(true == access.empty(), ;);
		m_SessionContainer.erase(access);

		if (m_SessionContainer.size() <= 0)
		{
			m_isEmptyRoom.store(true, std::memory_order_seq_cst);
		}
	}

	void ARoom::BroadCastMessage(char* _pPacket, PACKETHEAD _PacketHead)
	{
		for (auto& iter : m_SessionContainer)
		{
			iter.second->SendData(_pPacket, _PacketHead);
		}
	}

	void ARoom::Free()
	{
	}
}
