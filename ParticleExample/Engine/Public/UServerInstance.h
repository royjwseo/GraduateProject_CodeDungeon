#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_USERVERINSTANCE_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_USERVERINSTANCE_H

#include "UBase.h"

BEGIN(Engine)

// Server Manager 
class UServerInstance : public UBase {
	DECLARE_SINGLETON(UServerInstance)
public:
	UServerInstance();
	DESTRUCTOR(UServerInstance)
public:
	virtual void Free() override;
public:
	// Ready Connect Server 
	HRESULT ReadyConnectServer();

	_bool IsConnectServer();
private:
	// Server Thread 
	static void ServerThread(void* _pServerThread);

	void RunThread();
private:
	_bool					m_isServerThreadRunning;

	IOSERVICE			m_IoService;
	TCPSOCKET		m_TcpSocket;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_USERVERINSTANCE_H