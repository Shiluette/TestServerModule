#include"stdafx.h"
#include"QueryStatement.h"
#include"QueryRecord.h"
#include"Query.h"
#include"ODBCDatabase.h"
#include"DBManager.h"

Query::Query()
{
	_statement = new QueryStatement();
}

Query::~Query()
{
	SAFE_DELETE(_statement);
}

QueryRecord & Query::recode()
{
	return _recode;
}

void Query::setStatement(QueryStatement * statement)
{
	if(_statement != nullptr)
		SAFE_DELETE(_statement);
	_statement = statement;
}

QueryStatement * Query::statement()
{
	return _statement;
}
