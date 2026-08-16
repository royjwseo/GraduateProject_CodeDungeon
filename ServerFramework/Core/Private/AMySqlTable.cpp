#include "CoreDefines.h"
#include "AMySqlTable.h"
#include "AMySqlConnector.h"

namespace Core
{
	AMySqlTable::AMySqlTable(SQLTABLETYPE _eTableType) :
		 m_eSqlTableType{ _eTableType }, m_strTableName { "" }, m_upProcedureStatement{nullptr}, 
		m_spSqlConnector{nullptr}
	{
	}

	_bool AMySqlTable::NativeConstruct(SHPTR<AMySqlConnector> _spMySqlConnector, const _string& _strTableName)
	{
		assert(nullptr != _spMySqlConnector);

		std::shared_ptr<sql::Statement> State(_spMySqlConnector->GetStatement());

		_string tableName = _strTableName;
		_string dbName = SQL_DATABASE_NAME;
		_string checkTableExistsQuery = "SELECT COUNT(*) FROM information_schema.tables WHERE table_schema = '"
			+ dbName + "' AND table_name = '" + tableName + "'";

		std::unique_ptr<sql::ResultSet> Res(State->executeQuery(checkTableExistsQuery.c_str()));
		assert(Res != nullptr);
		// 만약 데이터베이스에 테이블이 존재하지 않는다면
		if (false == Res->next() || Res->getInt(1) < 0)
		{
			// Table을 생성한다. 
			return CreateTable(State.get());
		}
		return false;
	}

	void AMySqlTable::BindParam(_int _ParamIndex, _bool _Value)
	{
		assert(nullptr != m_upProcedureStatement);
		m_upProcedureStatement->setBoolean(_ParamIndex, _Value);
	}

	void AMySqlTable::BindParam(_int _ParamIndex, _float _Value)
	{
		assert(nullptr != m_upProcedureStatement);
		m_upProcedureStatement->setDouble(_ParamIndex, static_cast<_double>(_Value));
	}

	void AMySqlTable::BindParam(_int _ParamIndex, _double _Value)
	{
		assert(nullptr != m_upProcedureStatement);
		m_upProcedureStatement->setDouble(_ParamIndex, _Value);
	}

	void AMySqlTable::BindParam(_int _ParamIndex, _short _Value)
	{
		assert(nullptr != m_upProcedureStatement);
		m_upProcedureStatement->setInt(_ParamIndex, static_cast<_int>(_Value));
	}

	void AMySqlTable::BindParam(_int _ParamIndex, _int _Value)
	{
		assert(nullptr != m_upProcedureStatement);
		m_upProcedureStatement->setInt(_ParamIndex, _Value);
	}

	void AMySqlTable::BindParam(_int _ParamIndex, _llong _Value)
	{
		assert(nullptr != m_upProcedureStatement);
		m_upProcedureStatement->setInt64(_ParamIndex, _Value);
	}

	void AMySqlTable::BindParam(_int _ParamIndex, const _string& _Value)
	{
		assert(nullptr != m_upProcedureStatement);
		m_upProcedureStatement->setString(_ParamIndex, _Value.c_str());
	}

	void AMySqlTable::UpdateProcedureValue(CSHPTRREF<AMySqlConnector> _spMySqlConnector, std::unique_ptr<sql::PreparedStatement>&& _upProcedureStatement)
	{
		assert(nullptr != m_spSqlConnector && nullptr != m_upProcedureStatement.get());
		m_spSqlConnector = _spMySqlConnector;
		m_upProcedureStatement = std::move(_upProcedureStatement);
		m_spSqlConnector->UseConnector();
	}

	void AMySqlTable::Free()
	{
	}

}