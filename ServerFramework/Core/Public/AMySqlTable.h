#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AMYSQLTABLE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AMYSQLTABLE_H

#include "ACoreBase.h"

BEGIN(Core)
class AMySqlConnector;
/*
@ Date: 2024-01-28, Writer: 박태현
@ Explain
-  Table 별로 데이터를 관리하기 위한 클래스이다. 
*/
class AMySqlTable abstract : public ACoreBase {
public:
	AMySqlTable(SQLTABLETYPE _sqlTableType);
	DESTRUCTOR(AMySqlTable)
public:
	_bool NativeConstruct(SHPTR<AMySqlConnector> _spMySqlConnector, const _string& _strTableName);
	// ExcuteQueryMessage
	virtual _bool ExcuteQueryMessage(SHPTR<AMySqlConnector> _spMySqlConnector, 
		SQLQUERYTYPE _sqlQueryType, const _string& _strQueryData) PURE;

	void BindParam(_int _ParamIndex, _bool _Value);
	void BindParam(_int _ParamIndex, _float _Value);
	void BindParam(_int _ParamIndex, _double _Value);
	void BindParam(_int _ParamIndex, _short _Value);
	void BindParam(_int _ParamIndex, _int _Value);
	void BindParam(_int _ParamIndex, _llong _Value);
	void BindParam(_int _ParamIndex, const _string& _Value);

public: /* get set*/
	const SQLTABLETYPE GetTableType() const { return m_eSqlTableType; }
	const _string& GetTableName() const { return m_strTableName; }
	sql::PreparedStatement* GetProcedureStatement() const { return m_upProcedureStatement.get(); }
protected:
	// Create Table
	virtual _bool CreateTable(sql::Statement* _pStmt) PURE;
	virtual void CheckAndCreateProcedure(sql::Statement* _pStmt) PURE;

	void UpdateProcedureValue(CSHPTRREF<AMySqlConnector> _spMySqlConnector, 
		std::unique_ptr<sql::PreparedStatement>&& _upProcedureStatement);
private:
	virtual void Free() override;
private: // prepare
	SQLTABLETYPE													m_eSqlTableType;
	_string																	m_strTableName;

	SHPTR<AMySqlConnector>								m_spSqlConnector;
	std::unique_ptr<sql::PreparedStatement>	m_upProcedureStatement;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AMYSQLTABLE_H