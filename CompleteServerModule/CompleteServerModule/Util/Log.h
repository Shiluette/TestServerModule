#pragma once
#include"stdafx.h"
#include<fstream>
#include"Singleton.h"
#include"Type.h"


#define SLog(arg, ...)		SystemLog::getInstance().log(arg, __VA_ARGS__);
#define SErrLog(arg, ...)	SystemLog::getInstance().log(arg, __VA_ARGS__); ::ExitProcess(0);


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
	LogFile(jsonValue_t * config);
	virtual ~LogFile();

	void initialize() {}
	void initialize(WCHAR * logFileName);
	void log(WCHAR * logStr);
};

class LogWriter
{
private:
	BaseLog	* _base;
	wstr_t _prefix;
public:
	LogWriter();
	virtual ~LogWriter();

	void setLogger(BaseLog *base, const WCHAR *logPrefix);
	BaseLog *logger();

	void log(WCHAR *fmt, ...);
	void log(WCHAR *fmt, va_list args);
};


typedef LogWriter* LogWriterPtr;

class SystemLog : public Singleton<SystemLog>
{
private:
	LogWriter _logWrite;
public:
	SystemLog();
	virtual ~SystemLog();

	void initialize(jsonValue_t * config);
	void log(WCHAR *fmt, ...);
};