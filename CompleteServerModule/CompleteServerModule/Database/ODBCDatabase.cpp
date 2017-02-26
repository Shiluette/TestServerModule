#include "stdafx.h"
#include "Query.h"
#include "ODBCDatabase.h"
#include "DBManager.h"

ODBCDatabase::ODBCDatabase()
{
	_hstmt = SQL_NULL_HSTMT;
	_hdbc = SQL_NULL_HDBC;
	_henv = SQL_NULL_HENV;
	_state = DB_STOP;
	_connected = false;
}

ODBCDatabase::~ODBCDatabase()
{
	if (connected())
		this->disconnect();
	SAFE_DELETE(_thread);
}

bool ODBCDatabase::connect()
{
	short short_result = 0;
	SQLWCHAR out_connect_str[1024] = L"";
	try
	{
		_retcode = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &_henv);
		if (_retcode != SQL_SUCCESS && _retcode != SQL_SUCCESS_WITH_INFO) throw 0;
		_retcode = SQLSetEnvAttr(_henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
		if (_retcode != SQL_SUCCESS && _retcode != SQL_SUCCESS_WITH_INFO) throw 1;
		_retcode = SQLAllocHandle(SQL_HANDLE_DBC, _henv, &_hdbc);
		if (_retcode != SQL_SUCCESS && _retcode != SQL_SUCCESS_WITH_INFO) throw 2;
		SQLSetConnectAttr(_hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
		
		_retcode = SQLDriverConnect(_hdbc, NULL, (SQLWCHAR *)&_connectionStr.front(), lstrlenW(_connectionStr.data()), out_connect_str, sizeof(wchar_t) * 1024, &short_result, SQL_DRIVER_NOPROMPT);
		if (_retcode != SQL_SUCCESS && _retcode != SQL_SUCCESS_WITH_INFO) throw 3;
		_retcode = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &_hstmt);
		if (_retcode != SQL_SUCCESS && _retcode != SQL_SUCCESS_WITH_INFO) throw 4;
	}
	catch (int thrNum)
	{
		switch (thrNum) {
		case 0:
			SLog(L"!! ODBC ERROR(0) :  SQLAllocHandle(SQL_HANDLE_ENV, NULL, &_henv) !!");
			break;
		case 1:
			SLog(L"!! ODBC ERROR(1) :  SQLSetEnvAttr(_henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0) !!");
			SQLFreeHandle(SQL_HANDLE_ENV, _henv);
			break;
		case 2:
			SLog(L"!! ODBC ERROR(2) :  SQLAllocHandle(SQL_HANDLE_DBC, _henv, &_hdbc) !!");
			SQLFreeHandle(SQL_HANDLE_ENV, _henv);
			break;
		case 3:
			SLog(L"!! ODBC ERROR(3) :  SQLDriverConnect() !!");
			HandleDiagnosticRecord(_hdbc, SQL_HANDLE_DBC, _retcode);
			SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
			SQLFreeHandle(SQL_HANDLE_ENV, _henv);
			break;
		case 4:
			SLog(L"!! ODBC ERROR(4) :  SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &_hstmt) !!");
			SQLDisconnect(_hdbc);
			SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
			SQLFreeHandle(SQL_HANDLE_ENV, _henv);
			break;
		default:
			break;
		}
		return false;
	}
	SLog(L"ODBC DataBase Connection Success");
	_connected = true;
	_state = DB_STANDBY;
	return true;;
}

bool ODBCDatabase::connect(const wchar_t * drive, const wchar_t * server, const wchar_t * dataBase, const wchar_t * uid, const wchar_t * pw)
{
	// Drive : ���� ���� ��ǻ�Ϳ� ��ġ�� ���� Ȥ�� Ŭ���̾�Ʈ Ʈ���̹����� ��Ȯ�ϰ� ����Ѵ�.
	// Server : ������ TNS �̸��� "IP"�������� ����Ѵ�.
	// DataBase : "������ ���̽��� �̸��� ����Ѵ�."
	// Uid : ������ ������ ID�� ����Ѵ�.
	// PWD : UID�� ����� ������ �н����带 ����Ѵ�.
	// ex : char * connect_std = "DRIVE = {SQL Server};SERVER=xxx.xxx.xxx.xxx; DATABASE=TESTSOFT;UID=user_id;PWD=user_password;"
	std::array<wchar_t, SIZE_128> connect_std;

	snwprintf(connect_std, L"DRIVE={%s};SERVER=%s;DATABASE=%s;UID=%s;PWD=%s", drive, server, dataBase, uid, pw);
	SLog(L"Input connected Data : %s", connect_std.data());
	std::copy(connect_std.begin(), connect_std.end(), _connectionStr.begin());

	return this->connect();
}

bool ODBCDatabase::connect(const wchar_t * dbName, const wchar_t * id, const wchar_t * pw)
{
	_dbName = dbName;
	// Allocate environment handle
	_retcode = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &_henv);
	if (_retcode != SQL_SUCCESS && _retcode != SQL_SUCCESS_WITH_INFO) 
		return false; 

	// Set the ODBC version environment attribute
	_retcode = SQLSetEnvAttr(_henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	if (_retcode != SQL_SUCCESS && _retcode != SQL_SUCCESS_WITH_INFO) {
		SQLFreeHandle(SQL_HANDLE_ENV, _henv);
		return false; 
	}

	// Allocate connection handle
	_retcode = SQLAllocHandle(SQL_HANDLE_DBC, _henv, &_hdbc);
	if (_retcode != SQL_SUCCESS && _retcode != SQL_SUCCESS_WITH_INFO) {
		SQLFreeHandle(SQL_HANDLE_ENV, _henv);
		return false;
	}

	// Login Timeout 5 Seconds
	SQLSetConnectAttr(_hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

	// Connect to data source
	_retcode = SQLConnect(_hdbc, (SQLWCHAR*)dbName, SQL_NTS, (SQLWCHAR*)id, SQL_NTS, (SQLWCHAR*)pw, SQL_NTS);
	if (_retcode != SQL_SUCCESS && _retcode != SQL_SUCCESS_WITH_INFO) {
		HandleDiagnosticRecord(_hdbc, SQL_HANDLE_DBC, _retcode);
		SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
		SQLFreeHandle(SQL_HANDLE_ENV, _henv);
		return false;
	}

	// Allocate statement handle
	_retcode = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &_hstmt);
	if (_retcode != SQL_SUCCESS && _retcode != SQL_SUCCESS_WITH_INFO) {
		SQLDisconnect(_hdbc);
		SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
		SQLFreeHandle(SQL_HANDLE_ENV, _henv);
		return false;
	}
	_connected = true;
	SLog(L"ODBC DataBase Connection Success");
	_state = DB_STANDBY;
	return true;
}

bool ODBCDatabase::connected()
{
	return _connected;
}

bool ODBCDatabase::disconnect()
{
	if (_state == DB_RUNING) {
		return false;
	}
	if(_hstmt)SQLFreeHandle(SQL_HANDLE_STMT, _hstmt);
	if(_hdbc)SQLDisconnect(_hdbc);
	if(_hdbc)SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
	if(_henv)SQLFreeHandle(SQL_HANDLE_ENV, _henv);
	_connected = false;
	_state = DB_STOP;
}

void ODBCDatabase::HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode)
{
	SQLSMALLINT iRec = 0;
	SQLINTEGER  iError;
	WCHAR       wszMessage[1000];
	WCHAR       wszState[SQL_SQLSTATE_SIZE + 1];


	if (RetCode == SQL_INVALID_HANDLE)
	{
		fwprintf(stderr, L"Invalid handle!\n");
		return;
	}

	while (SQLGetDiagRec(hType,
		hHandle,
		++iRec,
		wszState,
		&iError,
		wszMessage,
		(SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)),
		(SQLSMALLINT *)NULL) == SQL_SUCCESS)
	{
		// Hide data truncated..
		if (wcsncmp(wszState, L"01004", 5))
		{
			fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
		}
	}
}

void ODBCDatabase::execute()
{
	// 1. ť�� ���� ������� return
	if (DBManager::getInstance().runQueryCount() == 0) {
		return;
	}

	// 2. ť�� �����͸� ����
	Query * qy = nullptr;
	if (!DBManager::getInstance().popQuery(&qy)){
		SLog(L"!! DBQueue Pop is Not Completeed");
		return;
	}
	_state = DB_RUNING;
	QueryStatement * state = qy->statement();
	// 3. ���� �����͸� db�� ����
	try {
		_retcode = SQLExecDirect(_hstmt, (SQLWCHAR*)state->query(), SQL_NTS);
		if (_retcode != SQL_SUCCESS && _retcode != SQL_SUCCESS_WITH_INFO)throw;
		
	}
	catch (...) {
		DBManager::getInstance().pushQuery(qy);
		SQLFreeStmt(_hstmt, SQL_CLOSE);
		_state = DB_STANDBY;
		return;
	}
	switch (state->type())
	{
	case QUERY_NOT_RETURN:
		break;
	case QUERY_CALL_BACK:
		qy->recode().setColumParam(&_hstmt);
		break;
	case QUERY_WAIT_RETURN:
		qy->recode().setColumParam(&_hstmt);
		break;
	}
	// 4. �� ó�� �۾�.
	SAFE_DELETE(qy);
	SQLFreeStmt(_hstmt, SQL_CLOSE);
	_state = DB_STANDBY;
	return;
}

void ODBCDatabase::process()
{
	while (_shutdown == false) {
		if (!this->connected()) {
			SLog(L"!! db[%s] Connection is disconnection", _dbName.c_str());
			ASSERT(FALSE);
		}
		this->execute();
		CONTEXT_SWITCH;
	}
}

void ODBCDatabase::run()
{
	_thread = MAKE_THREAD(ODBCDatabase, process);
}
