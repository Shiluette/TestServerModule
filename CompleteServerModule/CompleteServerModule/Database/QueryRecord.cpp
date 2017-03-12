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
	//SQLFetch(*hstm);
	SQLSMALLINT ColumCount = 999;
	SQLWCHAR columname[30] = {};
	SQLSMALLINT columnNameLength;
	SQLRETURN ret = SQLNumResultCols(*hstm, &ColumCount);
	/*if (ret == SQL_SUCCESS) {
		MessageBox(NULL, L"suc", L"", NULL);
	}
	if (ret == SQL_ERROR) {
		MessageBox(NULL, L"err", L"", NULL);
	}
	if (ret == SQL_STILL_EXECUTING) {
		MessageBox(NULL, L"still", L"", NULL);
	}
	if (ret == SQL_INVALID_HANDLE) {
		MessageBox(NULL, L"invalid", L"", NULL);
	}*/
	for (auto i = 0; i < ColumCount; ++i) {
		SQLColAttribute(*hstm, i + 1, SQL_DESC_NAME, columname, sizeof(SQLWCHAR) * 30, &columnNameLength, NULL);
		_name.emplace_back(columname);
		SQLColAttribute(*hstm, i + 1, SQL_DESC_TYPE_NAME, columname, sizeof(SQLWCHAR) * 30, &columnNameLength, NULL);
		_type.emplace_back(columname);
	}
}


bool QueryRecord::get(wchar_t * fieldName, char * fieldValue)
{
	array<char, SIZE_128> value;
	for (int num = 0; num < _name.size(); ++num) {
		if (lstrcmpW(_name[num].data(), fieldName) != 0) continue;
		SQLBindCol(*_hstmPtr, num + 1, SQL_C_CHAR, (SQLPOINTER)value.data(), SIZE_128, NULL);
		moveNext();
		sprintf_s(fieldValue, SIZE_128, "%s", (LPCSTR)value.data());
		SQLBindCol(*_hstmPtr, num + 1, SQL_C_WCHAR, (SQLPOINTER)NULL, sizeof(value), NULL);
		moveBefore();
		return true;
	}
	return false;
}

bool QueryRecord::get(wchar_t * fieldName, wchar_t * fieldValue)
{
	array<wchar_t, SIZE_128> value;
	for (int num = 0; num < _name.size(); ++num) {
		if (lstrcmpW(_name[num].data(), fieldName) != 0) continue;
		SQLBindCol(*_hstmPtr, num + 1, SQL_C_WCHAR, (SQLPOINTER)value.data(), SIZE_128, NULL);
		moveNext();
		swprintf_s(fieldValue, SIZE_128, L"%s", (LPCWSTR)value.data());
		SQLBindCol(*_hstmPtr, num + 1, SQL_C_WCHAR, (SQLPOINTER)NULL, sizeof(value), NULL);
		moveBefore();
		return true;
	}
	return false;
}

bool QueryRecord::get(wchar_t * fieldName, int * fieldValue)
{
	int copyvalue = 0;
	SQLRETURN ret;
	for (int num = 0; num < _name.size(); ++num) {
		if (lstrcmpW(_name[num].data(), fieldName) != 0) continue;
		ret = SQLBindCol(*_hstmPtr, num + 1, SQL_C_SLONG, (SQLPOINTER)&copyvalue, sizeof(copyvalue), NULL);
		if(SQL_INVALID_HANDLE == ret){}
		moveNext();
		CopyMemory(fieldValue,&copyvalue, sizeof(int));
		SQLBindCol(*_hstmPtr, num + 1, SQL_C_SLONG, (SQLPOINTER)NULL, sizeof(copyvalue), NULL);
		moveBefore();
		return true;
	}
	return false;
}

bool QueryRecord::get(wchar_t * fieldName, float * fieldValue)
{
	float value = 0;
	for (int num = 0; num < _name.size(); ++num) {
		if (lstrcmpW(_name[num].data(), fieldName) != 0) continue;
		SQLBindCol(*_hstmPtr, num + 1, SQL_C_FLOAT, (SQLPOINTER)&value, sizeof(value), NULL);
		//fieldValue = value;
		moveNext();
		CopyMemory(fieldValue, &value, sizeof(float));
		SQLBindCol(*_hstmPtr, num + 1, SQL_C_FLOAT, (SQLPOINTER)NULL, sizeof(value), NULL);
		moveBefore();
		return true;
	}
	return false;
}

bool QueryRecord::get(wchar_t * fieldName, double * fieldValue)
{
	double value = 0;
	for (int num = 0; num < _name.size(); ++num) {
		if (lstrcmpW(_name[num].data(), fieldName) != 0) continue;
		SQLBindCol(_hstmPtr, num + 1, SQL_C_DOUBLE, (SQLPOINTER)&value, sizeof(value), NULL);
		//fieldValue = value;
		moveNext();
		CopyMemory(fieldValue, &value, sizeof(double));
		SQLBindCol(*_hstmPtr, num + 1, SQL_C_DOUBLE, (SQLPOINTER)NULL, sizeof(value), NULL);
		moveBefore();
		return true;
	}
	return false;
}

bool QueryRecord::get(wchar_t * fieldName, long * fieldValue)
{
	long value = 0;
	for (int num = 0; num < _name.size(); ++num) {
		if (lstrcmpW(_name[num].data(), fieldName) != 0) continue;
		SQLBindCol(_hstmPtr, num + 1, SQL_C_LONG, (SQLPOINTER)&value, sizeof(value), NULL);
		moveNext();
		CopyMemory(fieldValue, &value, sizeof(LONG));
		SQLBindCol(*_hstmPtr, num + 1, SQL_C_LONG, (SQLPOINTER)NULL, sizeof(value), NULL);
		moveBefore();
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

bool QueryRecord::moveBefore()
{
	auto ret = SQLFetchScroll(*_hstmPtr, SQL_FETCH_PRIOR, NULL);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		return false;
	}
	return true;
}

bool QueryRecord::moveFirst() 
{
	auto ret = SQLFetchScroll(*_hstmPtr, SQL_FETCH_FIRST, NULL);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		return false;
	}
	return true;

}

bool QueryRecord::moveLast()
{
	 auto ret = SQLFetchScroll(*_hstmPtr, SQL_FETCH_LAST, NULL);
	 if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		 return false;
	 }
	 return true;
}
