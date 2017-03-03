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
	_workCount = DataBase.get("ThreadCount", 0).asInt();
	if (_workCount == 0) {
		SLog(L" ** not to do this Thread create, input zero count **");
		return;
	}
	std::array<wchar_t, SIZE_256> temp;
	temp.assign(NULL);
	str_t serverName = DataBase.get("ServerName", "").asString();
	StrConvA2W((char *)serverName.c_str(), temp.data(), serverName.size()+1);
	_serverName.append(temp.data());//_serverName = temp.data();
	temp.assign(NULL);
	str_t db = DataBase.get("DB", "").asString();
	StrConvA2W(&db.front(), temp.data(), db.size()+1);
	_dbName.append(temp.data()); //_dbName = temp.data();
	temp.assign(NULL);
	str_t login = DataBase.get("Login", "").asString();
	StrConvA2W(&login.front(), temp.data(), login.size()+1);
	_login.append(temp.data());//_login = temp.data();
	temp.assign(NULL);
	str_t pw = DataBase.get("Password", "").asString();
	StrConvA2W(&pw.front(), temp.data(), pw.size()+1);
	_password.append(temp.data()); //_password = temp.data();

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
