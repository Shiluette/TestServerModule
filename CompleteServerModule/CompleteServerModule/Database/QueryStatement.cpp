#include"stdafx.h"
#include"ODBCDatabase.h"
#include"QueryRecord.h"
#include "QueryStatement.h"

QueryStatement::QueryStatement()
{
	_parameterCount = 0;
	_query.clear();
}

QueryStatement::~QueryStatement()
{
}

void QueryStatement::setQuery(wchar_t * query, QUERY_TYPE type)
{
	_query = L"EXEC ";
	_query += query;
	_type = type;
}

const WCHAR * QueryStatement::query()
{
	return _query.c_str();
}

QUERY_TYPE QueryStatement::type()
{
	return _type;
}
template<class T>
inline void QueryStatement::addArg(WCHAR * pmt, T value)
{
	// TODO : EXEC dbo.addUser id, pw 
	// 1. 생성시 setQuery 시 'EXEC + 쿼리' 이름 까지 붙고
	// 2. 인자 넣을시 addParam 이용하여 파라미터 넣기.
	std::array<WCHAR, DB_PARAM_SIZE>buffer;
	snwprintf(buffer, pmt, value);
	if (_parameterCount++) {
		_query += L", ";
	}
	else {
		_query += L" ";
	}
	_query += buffer.data();
}


void QueryStatement::addParam(CHAR * value)
{
	// this->addArg(L"%S", value); 시 pmt로 넘어갈시 snwprintf사용 불가.
	this->addArg(L"'%S'", value);
}

void QueryStatement::addParam(WCHAR * value)
{
	this->addArg(L"'%s'", value);
}

void QueryStatement::addParam(INT32 * value)
{
	this->addArg(L"'%d'", value);
}

void QueryStatement::addParam(UINT32 * value)
{
	this->addArg(L"'%u'", value);
}

void QueryStatement::addParam(INT64 * value)
{
	this->addArg(L"'%lld'", value);
}

void QueryStatement::addParam(UINT64 * value)
{
	this->addArg(L"'%llu'", value);
}

void QueryStatement::addParam(FLOAT * value)
{
	this->addArg(L"'%f'", value);
}

void QueryStatement::addParam(DOUBLE * value)
{
	this->addArg(L"'%lf'", value);
}
