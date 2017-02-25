#include"stdafx.h"
#include"DBManager.h"

DBManager::DBManager()
{
	jsonValue_t config;
	if (!loadConfig(&config)) {
		return;
	}

	this->initialize(&config);
}

DBManager::~DBManager()
{
	SAFE_DELETE(_queryQueue);
	for (auto db : _dbPool) {
		db->disconnect();
		SAFE_DELETE(db);
	}
}

void DBManager::initialize(jsonValue_t * config)
{
	jsonValue_t DataBase = (*config)["DataBase"];
	if (DataBase == NULL) {
		SErrLog(L"** not exist process setting **");
		return;
	}
	int processCount = DataBase.get("ThreadCount", 0).asInt();
	if (processCount == 0) {
		SLog(L" ** not to do this Thread create, input zero count **");
		return;
	}
	std::array<wchar_t, SIZE_256> temp;
	str_t serverName = DataBase.get("ServerName", "").asString();
	StrConvA2W(&serverName.front(), temp.data(), serverName.size());
	_serverName = temp.data();

	str_t db = DataBase.get("DB", "").asString();
	StrConvA2W(&db.front(), temp.data(), db.size());
	_dbName = temp.data();

	str_t login = DataBase.get("Login", "").asString();
	StrConvA2W(&login.front(), temp.data(), login.size());
	_login = temp.data();

	str_t pw = DataBase.get("Password", "").asString();
	StrConvA2W(&pw.front(), temp.data(), pw.size());
	_password = temp.data();

	_queryQueue = new ThreadJobQueue<Query *>(L"DBQueueJob");

	for (auto i = 0; i < _workCount; ++i) {
		ODBCDatabase * odbcdb = new ODBCDatabase();
		_dbPool.push_back(odbcdb);
	}

}

size_t DBManager::runQueryCount()
{
	return _queryQueue->size();
}

void DBManager::pushQuery(Query * qy)
{
	_queryQueue->push(qy);
}

bool DBManager::popQuery(Query ** qy)
{
	return _queryQueue->pop(*qy);
}

void DBManager::run()
{
	for (auto db : _dbPool) {
		if (db->state() != DB_STOP)continue;
		if (!db->connect(_dbName.c_str(), _login.c_str(), _password.c_str())) {
			SErrLog(L"!! db[%s] connection error", _dbName.c_str());
		}
		db->run();
	}
}
