#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AMYSQLCONNECTOR_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AMYSQLCONNECTOR_H

#include "ACoreBase.h"

BEGIN(Core)
class AMySqlDriver;

/*
@ Date: 2023-01-28, Writer: 박태현
@ Explain
-  MySqlConnector는 Connector를 여러개 쓰기 위한 클래스
그냥 connector 자체를 Lapping 해 놓은 것
*/
class CORE_DLL AMySqlConnector final : public ACoreBase {
public:
	AMySqlConnector();
	NO_COPY(AMySqlConnector)
	DESTRUCTOR(AMySqlConnector)
public:
	_bool NativeConstruct(sql::mysql::MySQL_Driver* _pDriver, const _string& _strAddress, const _string& _strName, 
		const _string& _strPassward, _int _iThreadID);
	sql::PreparedStatement* MakePrepareStatement(const _string& _strStateString);
	void UseConnector();
	void ReturnConnector();
public: /* get set */
	const std::shared_ptr<sql::Statement>& GetStatement() const { return m_spStatement; }
	_bool IsUseConnector() const { return m_isUseConnector.load(std::memory_order_seq_cst); }
protected:
	void ChangeUseConnectorValueToCas(_bool _isValue);
public: 
	// get Thread Index 
	const _int GetThreadIndex() const { return m_iThreadIndex; }
private:
	virtual void Free() override;
private:
	sql::Connection*													m_pConnection;
	_int																			m_iThreadIndex;
	std::shared_ptr<sql::Statement>						m_spStatement;
	std::atomic_bool													m_isUseConnector;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AMYSQLCONNECTOR_H