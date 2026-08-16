#include "ASession.h"

BEGIN(Core)
class AService;
END

class  CClientSession final : public ASession {
public:
	CClientSession(SESSION_CONSTRUCTOR);
	NO_COPY(CClientSession)
	DESTRUCTOR(CClientSession)
public:
	virtual _bool Start() override;
	// 클라이언트에서 전송된 버퍼를 읽는 함수
	virtual void RecvData() override;
	// 클라이언트에게 버퍼를 조합하여 전송하는 함수
	virtual _bool SendData(_char* _pPacket, const Core::PACKETHEAD& _PacketHead) override;
	virtual void Disconnect() override;
	virtual void ConnectTcpSocket() override;

	void SendMsg();
protected:
	virtual _bool ProcessPacket(_char* _pPacket, const Core::PACKETHEAD& _PacketHead) override;
private:
	virtual void Free() override;
};

