#pragma once
#include"stdafx.h"
#include"ODBCDatabase.h"

class QueryRecord
{
private:
	SQLHSTMT * _hstmPtr;
	int	_resultCount;
	std::vector<wstr_t> _name;
	std::vector<wstr_t> _type;
public:
	QueryRecord();
	~QueryRecord();
	void setstate(SQLHSTMT * hstm);
	void setColumParam(SQLHSTMT * hstm);
	bool get(wchar_t * fieldName, char * fieldValue);
	bool get(wchar_t * fieldName, wchar_t * fieldValue);
	bool get(wchar_t * fieldName, int & fieldValue);
	bool get(wchar_t * fieldName, float & fieldValue);
	bool get(wchar_t * fieldName, double & fieldValue);
	bool get(wchar_t * fieldName, long & fieldValue);
	bool moveNext();
};