#pragma once
#include"stdafx.h"
#include"ODBCDatabase.h"

typedef enum {
	QUERY_NOT_RETURN,	// 리턴 없는 쿼리
	QUERY_WAIT_RETURN,	// db응답을 기다린뒤 처리 하는 쿼리
	QUERY_CALL_BACK,	// DB응답을 기다리지 않지만 뒤처리는 있는 쿼리
}QUERY_TYPE;

class QueryStatement
{
private:
	wstr_t _query;
	int _parameterCount;
	QUERY_TYPE _type;

public:
	QueryStatement();
	~QueryStatement();

	void setQuery(wchar_t *query, QUERY_TYPE type = QUERY_NOT_RETURN);
	
	const WCHAR * query();
	QUERY_TYPE type();
	template<class T>
	void addArg(WCHAR * pmt, T value);
	void addParam(CHAR * value);
	void addParam(WCHAR * value);
	void addParam(INT32 * value);
	void addParam(UINT32 * value);
	void addParam(INT64 * value);
	void addParam(UINT64 * value);
	void addParam(FLOAT* value);
	void addParam(DOUBLE* value);
};

