#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AMYSQLDRIVER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AMYSQLDRIVER_H

#include "ACoreObject.h"

BEGIN(Core)
class AMySqlConnector;
class AMySqlTable;

/*
@ Date: 2024-01-28, Writer: 박태현
@ Explain
-  DataBase와 연결을 하기 위한 클래스이다. 
연결할 데이터베이스는 mysql, redis
*/
class CORE_DLL AMySqlDriver final : public ACoreObject {
public:
	using SQLARRAY = ARRAY<SHPTR<AMySqlConnector>, TLS::MAX_WORKTHREAD>;
	using SQLTABLE = ARRAY<SHPTR<AMySqlTable>, MAX_SQLTABLE_LENGTH>;
public:
	AMySqlDriver(OBJCON_CONSTRUCTOR, const _string& _strAddress, const _string& _strName, const _string& _strPassward);
	NO_COPY(AMySqlDriver)
	DESTRUCTOR(AMySqlDriver)
public:
	_bool	NativeConstruct();

	// ExcuteQueryMessage
	_bool ExcuteQueryMessage(SQLTABLETYPE _TableType, SQLQUERYTYPE _sqlQueryType, const _string& _strQueryData);

	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _bool _Value);
	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _float _Value);
	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _double _Value);
	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _short _Value);
	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _int _Value);
	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _llong _Value);
	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, const _string& _Value);
public: /* get set*/
	const _string& GetAddress() const { return m_strAddress; }
	const _string& GetSqlName() const { return m_strSqlName; }
	const _string& GetPassword() const { return m_strPassword; }
protected:

private:
	virtual void Free() override;
private:
	sql::mysql::MySQL_Driver*	 m_pDriver;
	SQLARRAY									 m_MySqlConnectContainer;
	SQLTABLE									 m_MySqlTableContainer;
	_string										 m_strAddress;
	_string										 m_strSqlName;
	_string										 m_strPassword;
};

END


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AMYSQLDRIVER_H