#pragma once
#include"stdafx.h"
#include<sql.h>
#include<sqlext.h>
typedef enum
{
	DB_STOP,
	DB_STANDBY,
	DB_RUNING
}DB_STATE;
class Database
{
protected:
	DB_STATE	_state;
public:
	Database() {}
	virtual ~Database() {}
	virtual bool connect(const wchar_t * dbName, const wchar_t * id, const wchar_t *pw) = 0;
	virtual bool connect() = 0;
	virtual bool connected() = 0;
	virtual bool disconnect() = 0;

	virtual void run() = 0;
	DB_STATE &state() { return _state; }
};

class ODBCDatabase : public Database
{
private:
	SQLRETURN _retcode;			// °á°ú °ª
	SQLHSTMT _hstmt;			// statement Handle
	SQLHDBC _hdbc;				//  connection handle
	SQLHENV _henv;				// environment handle
	wstr_t _dbName;				// DBName
	wstr_t _connectionStr;		// ConnectionStr;
	bool _connected;			// true = connnetion, false = disconnection

	Thread * _thread;
private:

	void HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);

	void execute();
	void process();
	void run();
public:
	ODBCDatabase();
	virtual ~ODBCDatabase();
	virtual bool connect();
	bool connect(const wchar_t * drive, const wchar_t * server, const wchar_t * dataBase, const wchar_t * uid, const wchar_t * pw);
	virtual bool connect(const wchar_t * dbName, const wchar_t * id, const wchar_t *pw);
	virtual bool connected();
	virtual bool disconnect();

	
};