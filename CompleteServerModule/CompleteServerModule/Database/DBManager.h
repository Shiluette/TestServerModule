#pragma once
#include"stdafx.h"
#include"ODBCDatabase.h"

class Query;
class DBManager : public Singleton<DBManager>
{
private:
	int _workCount;
	std::vector<Database *> _dbPool;

	wstr_t _serverName;
	wstr_t _dbName;
	wstr_t _login;
	wstr_t _password;
	ThreadJobQueue<Query *> *_queryQueue;
public:
	DBManager();
	virtual ~DBManager();
	void initialize(jsonValue_t * config);

	size_t runQueryCount();
	void pushQuery(Query * );
	bool popQuery(Query **);

	void run();
};