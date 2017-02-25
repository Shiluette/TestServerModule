#pragma once
#include"stdafx.h"
#include"ODBCDatabase.h"
#include"QueryRecord.h"
#include"QueryStatement.h"

class Query
{
protected:
	QueryStatement * _statement;
	QueryRecord _recode;
public:
	Query();
	virtual ~Query();
	QueryRecord & recode();
	void setStatement(QueryStatement * statement);
	QueryStatement * statement();
};