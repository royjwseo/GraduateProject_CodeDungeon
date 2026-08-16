#include "ClientDefines.h"
#include "CClientAdiminstor.h"
#include "ACoreInstance.h"
#include "CClientSession.h"

CClientAdiminstor::CClientAdiminstor(OBJCON_CONSTRUCTOR) : 
	AStressClientService(OBJCON_CONDATA), m_iMaxSessionCount{1000}
{
}

bool CClientAdiminstor::NativeConstruct()
{
	Connect();

	return true;
}

_bool CClientAdiminstor::Start()
{
	__super::Start();
	SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();

	for (_uint i = 0; i < 2; ++i)
	{
		spCoreInstance->RegisterFunc(CClientAdiminstor::ThreadFunc, this);
	}
	spCoreInstance->Join();
	return true;
}



void CClientAdiminstor::Connect()
{
	SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
	SHPTR<CClientAdiminstor> spClientAdiminstor = ThisShared<CClientAdiminstor>();
	for (_uint j = 0; j < 4; ++j)
	{
		for (_uint i = 0; i < m_iMaxSessionCount; ++i)
		{
			TCPSOCKET tcpSocket(GetIOContext());
			SESSIONID SessionID = GiveID();
			SHPTR<CClientSession> spClientSession = Create<CClientSession>(spCoreInstance, std::move(tcpSocket), SessionID);
			InsertSession(SessionID, spClientSession);
			spClientSession->ConnectTcpSocket();
			vecSessions[j].push_back(spClientSession);
		}
	}
}

void CClientAdiminstor::Tick()
{
	static std::vector<std::string> wordList = {
"apgdple", "bananafdsafasdf", "cholate", "dgdg", "ephant",
"flfgsdfgfdsgr", "itar", "happfsass", "intnet", "jaz"
	};

	CS_LOGIN Login;
	Login.set_time_test(CurrentMilliseconds());
	for (auto& iter : vecSessions[TLS::g_ThreadID])
	{
		Login.set_user_name(wordList[rand() % 10]);
		iter->SendProtoData(Login, TAG_CS_LOGIN);
	}
}

void CClientAdiminstor::ThreadFunc(void* _Service)
{
	static _uint i = 0;
	TLS::g_ThreadID = i++;
	RETURN_CHECK(nullptr == _Service, ;);
	CClientAdiminstor* pService = static_cast<CClientAdiminstor*>(_Service);
	while (true)
	{
		pService->Tick();
	}
}

void CClientAdiminstor::Free()
{
}
