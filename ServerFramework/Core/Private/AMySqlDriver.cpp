#include "CoreDefines.h"
#include "AMySqlDriver.h"
#include "AMySqlConnector.h"
#include "AMySqlTable.h"
#include "ASqlLoginTable.h"

namespace Core {
	AMySqlDriver::AMySqlDriver(OBJCON_CONSTRUCTOR, const _string& _strAddress, 
		const _string& _strName, const _string& _strPassward) :
		ACoreObject(OBJCON_CONDATA), m_pDriver{ nullptr }, m_MySqlConnectContainer{}, 
		m_strAddress{_strAddress}, m_strSqlName {_strName}, m_strPassword{ _strPassward }
	{
	}

	_bool AMySqlDriver::NativeConstruct()
	{
		// Connect My Sql 
		m_pDriver = sql::mysql::get_driver_instance();
		assert(nullptr != m_pDriver);
		// 실제 데이터베이스가 존재하는지 확인 
		{
			// 데이터베이스 서버에 연결한다. 
			std::unique_ptr<sql::Connection> Conn(m_pDriver->connect(m_strAddress.c_str(), m_strSqlName.c_str(), m_strPassword.c_str()));
			// Statement 객체를 생성
			std::unique_ptr<sql::Statement> Stmt(Conn->createStatement());
			// Query문 작성 
			_string CheckDBExistQuery = "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = '";
			CheckDBExistQuery += SQL_DATABASE_NAME;
			CheckDBExistQuery += "'";
			// Make Result 
			std::unique_ptr<sql::ResultSet> res(Stmt->executeQuery(CheckDBExistQuery.c_str()));
			assert(res != nullptr);
			// 데이터베이스가 존재하지 않는다면
			if (false == res->next())
			{
				_string CreateDBQuery = "CREATE DATABASE ";
				CreateDBQuery.append(SQL_DATABASE_NAME);
				Stmt->executeUpdate(CreateDBQuery.c_str());
			}
		}
		// MySql Ready
		_int i = 0; 
		for (auto& iter : m_MySqlConnectContainer)
		{
			iter = CreateInitNative<AMySqlConnector>(m_pDriver, m_strAddress, m_strSqlName, m_strPassword, i++);
			assert(nullptr != iter);

		}

		m_MySqlTableContainer[SQLTABLETYPE::LOGIN] = CreateInitNative<ASqlLoginTable>(m_MySqlConnectContainer[0], SQL_LOGINTABLE_NAME);

		return true;
	}

	_bool AMySqlDriver::ExcuteQueryMessage(SQLTABLETYPE _TableType, SQLQUERYTYPE _sqlQueryType, const _string& _strQueryData)
	{
		return false;
	}

	void AMySqlDriver::BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _bool _Value)
	{
	}

	void AMySqlDriver::BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _float _Value)
	{
	}

	void AMySqlDriver::BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _double _Value)
	{
	}

	void AMySqlDriver::BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _short _Value)
	{
	}

	void AMySqlDriver::BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _int _Value)
	{
	}

	void AMySqlDriver::BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _llong _Value)
	{
	}

	void AMySqlDriver::BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, const _string& _Value)
	{
	}

	void AMySqlDriver::Free() {
		delete m_pDriver;
	}
}