#include "ClientDefines.h"
#include "CClientSession.h"
#include "AService.h"

CClientSession::CClientSession(SESSION_CONSTRUCTOR)
	: Core::ASession(SESSION_CONDATA(Core::SESSIONTYPE::PLAYER))
{
}

_bool CClientSession::Start()
{
	return __super::Start();
}

void CClientSession::RecvData()
{
	__super::RecvData();
}

_bool CClientSession::SendData(_char* _pPacket, const Core::PACKETHEAD& _PacketHead)
{
	CombineSendBuffer(_pPacket, _PacketHead);
	_bool result{ true };

	TCPSOCKET& Socket = GetTcpSocket(REF_RETURN);
	Socket.write_some(Asio::buffer(GetSendBuff(), _PacketHead.PacketSize + PACKETHEAD_SIZE));
	return result;
}

void CClientSession::Disconnect()
{		
	__super::Disconnect();
}

void CClientSession::ConnectTcpSocket()
{
	TCPSOCKET& TcpSocket = GetTcpSocket(REF_RETURN);
	//43.203.88.154
	// 127.0.0.1
	TcpSocket.connect(Asio::ip::tcp::endpoint(Asio::ip::address::from_string("127.0.0.1"),
		Core::TCP_PORT_NUM));
}

void CClientSession::SendMsg()
{
	static std::vector<std::string> wordList = {
	"apple", "banana", "chocolate", "dog", "elephant",
	"flower", "guitar", "happiness", "internet", "jazz"
	};

	CS_LOGIN Login;
	Login.set_time_test(CurrentMilliseconds());
	Login.set_user_name(wordList[rand() % wordList.size()]);
	SendProtoData(Login, TAG_CS_LOGIN);
	// Remove Object 조합 
	//CS_LOGOUT logout;
	//logout.set_id(GetID());
	//SendProtoData(logout, TAG_CS::TAG_CS_LOGOUT);

	//Disconnect();
}

_bool CClientSession::ProcessPacket(_char* _pPacket, const Core::PACKETHEAD& _PacketHead)
{
	switch (_PacketHead.PacketType)
	{
	case TAG_SERVER::TAG_SC_LOGIN:
	{
		SC_CHECKLOGIN Login;
		Login.ParseFromArray(_pPacket, static_cast<_int>(_PacketHead.PacketSize));
	//	std::cout << Login.id() << "\n";
	}
		break;
	}
	return true;
}

void CClientSession::Free()
{
}
