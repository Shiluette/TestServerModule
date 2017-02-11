#pragma once
#include"stdafx.h"
#include<fstream>
#include"Singleton.h"
#include"Type.h"


//#define SLog(arg, ...)


class BaseLog
{
public:
	BaseLog() {}
	virtual ~BaseLog() {}
	virtual void initialize() {}
	virtual void unInitalize() {}
	virtual void log(WCHAR * logStr) = 0;
};


class LogPrintf : public BaseLog
{
public:
	LogPrintf();
	void log(WCHAR *logString);
};

class LogFile : public BaseLog
{
	std::wfstream _file;
	wstr_t _fileName;
public:
	LogFile();
	virtual ~LogFile();

	void initialize() {}
	void initialize(WCHAR * logFileName);
	void log(WCHAR * logStr);
};