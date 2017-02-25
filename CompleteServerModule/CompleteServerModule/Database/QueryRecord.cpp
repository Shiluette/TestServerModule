#include"stdafx.h"
#include "QueryRecord.h"

QueryRecord::QueryRecord()
{
	_hstmPtr = nullptr;
	_resultCount = 0;
	_name.clear();
	_type.clear();
}

QueryRecord::~QueryRecord()
{
	_hstmPtr = nullptr;
	_name.clear();
	_type.clear();
}

void QueryRecord::setstate(SQLHSTMT * hstm)
{
	_hstmPtr = hstm;
}

void QueryRecord::setColumParam(SQLHSTMT * hstm)
{
	if (_hstmPtr == nullptr)
		this->setstate(hstm);
	SQLSMALLINT ColumCount = 0;
	SQLWCHAR columname[30];
	SQLSMALLINT columnNameLength;
	SQLNumResultCols(hstm, &ColumCount);
	for (auto i = 0; i < ColumCount; ++i) {
		SQLColAttribute(*hstm, SQL_DESC_NAME, i + 1, columname, sizeof(SQLWCHAR) * 30, &columnNameLength, NULL);
		_name.emplace_back(columname);
		SQLColAttribute(*hstm, SQL_DESC_TYPE_NAME, i + 1, columname, sizeof(SQLWCHAR) * 30, &columnNameLength, NULL);
		_type.emplace_back(columname);
	}
}


bool QueryRecord::get(wchar_t * fieldName, char * fieldValue)
{
	array<char, SIZE_128> value;
	for (int num = 0; num < _name.size(); ++num) {
		if (lstrcmpW(_name[num].data(), fieldName) != 0) continue;
		SQLBindCol(_hstmPtr, num + 1, SQL_C_CHAR, (SQLPOINTER)value.data(), SIZE_128, NULL);
		sprintf_s(fieldValue, SIZE_128, "%s", (LPCSTR)value.data());
		return true;
	}
	return false;
}

bool QueryRecord::get(wchar_t * fieldName, wchar_t * fieldValue)
{
	array<wchar_t, SIZE_128> value;
	for (int num = 0; num < _name.size(); ++num) {
		if (lstrcmpW(_name[num].data(), fieldName) != 0) continue;
		SQLBindCol(_hstmPtr, num + 1, SQL_C_WCHAR, (SQLPOINTER)value.data(), SIZE_128, NULL);
		swprintf_s(fieldValue, SIZE_128, L"%s", (LPCWSTR)value.data());
		return true;
	}
	return false;
}

bool QueryRecord::get(wchar_t * fieldName, int & fieldValue)
{
	int value = 0;
	for (int num = 0; num < _name.size(); ++num) {
		if (lstrcmpW(_name[num].data(), fieldName) != 0) continue;
		SQLBindCol(_hstmPtr, num + 1, SQL_C_SBIGINT, (SQLPOINTER)&value, sizeof(value), NULL);
		fieldValue = value;
		return true;
	}
	return false;
}

bool QueryRecord::get(wchar_t * fieldName, float & fieldValue)
{
	float value = 0;
	for (int num = 0; num < _name.size(); ++num) {
		if (lstrcmpW(_name[num].data(), fieldName) != 0) continue;
		SQLBindCol(_hstmPtr, num + 1, SQL_C_FLOAT, (SQLPOINTER)&value, sizeof(value), NULL);
		fieldValue = value;
		return true;
	}
	return false;
}

bool QueryRecord::get(wchar_t * fieldName, double & fieldValue)
{
	double value = 0;
	for (int num = 0; num < _name.size(); ++num) {
		if (lstrcmpW(_name[num].data(), fieldName) != 0) continue;
		SQLBindCol(_hstmPtr, num + 1, SQL_C_DOUBLE, (SQLPOINTER)&value, sizeof(value), NULL);
		fieldValue = value;
		return true;
	}
	return false;
}

bool QueryRecord::get(wchar_t * fieldName, long & fieldValue)
{
	long value = 0;
	for (int num = 0; num < _name.size(); ++num) {
		if (lstrcmpW(_name[num].data(), fieldName) != 0) continue;
		SQLBindCol(_hstmPtr, num + 1, SQL_C_LONG, (SQLPOINTER)&value, sizeof(value), NULL);
		fieldValue = value;
		return true;
	}
	return false;
}

bool QueryRecord::moveNext()
{
	if (SQLFetch(*_hstmPtr) != SQL_SUCCESS)
		return false;
	return true;
}
