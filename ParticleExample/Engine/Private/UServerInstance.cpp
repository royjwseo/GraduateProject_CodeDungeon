#include "EngineDefines.h"
#include "boost/asio.hpp"
// OutPutSeverData Core Enum 
#include "..\..\..\OutputServerData\CoreProtocol.h"

using IOSERVICE = boost::asio::io_service;
using TCPSOCKET = boost::asio::ip::tcp::socket;

#include "UServerInstance.h"
#include "UGameInstance.h"

UServerInstance::UServerInstance() : m_isServerThreadRunning{false}, m_TcpSocket(m_IoService)
{
}

void UServerInstance::Free()
{
	m_TcpSocket.close();
}

HRESULT UServerInstance::ReadyConnectServer()
{
	// Register Thterad 
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	spGameInstance->RegisterOutSideWorkThread(ServerThread, this);
	return S_OK;
}
// Is Connect Server
_bool UServerInstance::IsConnectServer()
{
	// EndPoint
	boost::asio::ip::tcp::endpoint EndPoint(boost::asio::ip::address::from_string(Core::IP_ADDRESS), PORT_NUM);

	try {
		m_TcpSocket.connect(EndPoint);
	}
	catch (const std::exception& e) {
		DEBUG_MSG("ServerConnect Failed");
		return false;
	}

	_string UserName{ "PTH" };

	CS_LOGIN LoginPacket;
	MakeCsLoginPacket(UserName, LoginPacket);
	// Write Buffer 
	m_TcpSocket.write_some(boost::asio::buffer(&LoginPacket, LoginPacket.size()));
	m_TcpSocket.non_blocking(true);
	return true;
}


void UServerInstance::ServerThread(void* _pServerThread)
{
	UServerInstance* pInstance = static_cast<UServerInstance*>(_pServerThread);
	RETURN_CHECK(nullptr == pInstance, ;);
	pInstance->RunThread();
}

void UServerInstance::RunThread()
{
	while (m_isServerThreadRunning)
	{

	}
}