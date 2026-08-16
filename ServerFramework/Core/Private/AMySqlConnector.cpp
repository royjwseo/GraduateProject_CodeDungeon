#include "CoreDefines.h"
#include "AMySqlConnector.h"

namespace Core {

	AMySqlConnector::AMySqlConnector() : m_pConnection{ nullptr }, m_iThreadIndex{ 0 } , 
		m_spStatement{ nullptr }, m_isUseConnector{ false } {
	}

	_bool AMySqlConnector::NativeConstruct(sql::mysql::MySQL_Driver* _pDriver, const _string& _strAddress, 
		const _string& _strName, const _string& _strPassward, _int _iThreadID)
	{
		assert(nullptr != _pDriver);
		m_pConnection = _pDriver->connect(_strAddress.c_str(), _strName.c_str(), _strPassward.c_str());
		m_iThreadIndex = _iThreadID;

		if (nullptr == m_pConnection)
			return false;

		m_pConnection->setSchema(SQL_DATABASE_NAME);


		m_spStatement = std::move(std::shared_ptr<sql::Statement>(m_pConnection->createStatement()));
		return true;
	}

	sql::PreparedStatement* AMySqlConnector::MakePrepareStatement(const _string& _strStateString){
		return m_pConnection->prepareStatement(_strStateString.c_str());
	}

	void AMySqlConnector::UseConnector()
	{
		ChangeUseConnectorValueToCas(true);
	}

	void AMySqlConnector::ReturnConnector()
	{
		ChangeUseConnectorValueToCas(false);
	}

	void AMySqlConnector::ChangeUseConnectorValueToCas(_bool _isValue)
	{
		while (true)
		{
			_bool isUse = m_isUseConnector.load(std::memory_order_release);
			_bool isValue = _isValue;
			if (true == m_isUseConnector.compare_exchange_strong(isUse, isValue))
				break;
		}
	}

	void AMySqlConnector::Free()
	{
		delete m_pConnection;
	}
}