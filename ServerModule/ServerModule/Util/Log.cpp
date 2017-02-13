#include "stdafx.h"
#include "Log.h"

LogPrintf::LogPrintf()
{
	printf("- Log create : printf Log Mode\n");
}

void LogPrintf::log(WCHAR * logString)
{
	// �ް����� ���� %s , %S -> 
	// %s ����ϴ� �Լ��� ������ ���ڿ� Ÿ������ ���
	// %S ����ϴ� �Լ��� �ݴ�Ǵ� ���ڿ� Ÿ������ ���
	// wachr_t * str;
	// printf("%S", str); ����
	// %ws : �Լ��� ������� 'wchar_t *' �� ��ȯ
	// %hs : �Լ��� ������� 'char *' �� ��ȯ
	printf("%ws", logString);
}

LogFile::LogFile(jsonValue_t * config)
{
	jsonValue_t log = (*config)["Log"];

	std::array<WCHAR, SIZE_256> path;

	StrConvA2W(&log.get("Path", "").asString().front(), path.data(), path.max_size());
	printf("* Log is Create : [%ws]file Log mode.. \n", path.data());
	this->initialize(path.data());
}

LogFile::~LogFile()
{
	_file.close();
	_file.clear();

	size_t found = _fileName.find(L".log");
	if (found == wstr_t::npos) {
		return;
	}

	wstr_t closeFilename = _fileName.substr(0, found);
	closeFilename += CLOCK.nowTime(L"_%Y%m%d-%H%M%S.log");
	// LogFile name ���� �Լ�
	_wrename(_fileName.c_str(), closeFilename.c_str());
}

void LogFile::initialize(WCHAR * logFileName)
{
	_fileName = logFileName;
	_file.open(logFileName, std::ios::out | std::ios::trunc);
	if (_file.bad()) {
		printf("! LogFile error .... file open Failed.\n");
		// TODO : 
	}
}

void LogFile::log(WCHAR * logStr)
{
	printf("%ws1", logStr);
	_file << logStr;
	_file.flush();
}

SystemLog::SystemLog()
{
	jsonValue_t config;
	if (!loadConfig(&config)) {
		printf(" !!! have not config file !!! \n");
		exit(0);
		return;
	}
	this->initialize(&config);
}

SystemLog::~SystemLog()
{
}

void SystemLog::initialize(jsonValue_t * config)
{
	// FIXME : if�� ���� �ɸ��� �� �ɸ��� ��.... �׽�Ʈ�� �ʿ��ϴ�!
	if (strcmp(config->get("Log", "").asString().c_str(), "")) {
		printf("@ not exist Log setting @\n");
		BaseLog *base = new LogPrintf();
		_logWrite.setLogger(base, L"LiveServer");
		return;
	}

	jsonValue_t log = (*config)["Log"];
	
	std::array<WCHAR, SIZE_256> temp;

	StrConvA2W((char *)log.get("Prefix", "").asString().c_str(),temp.data(), temp.max_size());
	wstr_t prefix = temp.data();
	
	BaseLog * base;

	const char * type = log.get("Type", "").asString().c_str();
	if (!strcmp(type, "WithFile")) {
		base = new LogFile(config);
	}
	else {
		base = new LogPrintf();
	}
	_logWrite.setLogger(base, prefix.c_str());
}

void SystemLog::log(WCHAR * fmt, ...)
{
	// ���������Լ� ����.  va_list
	va_list arg;
	// ������ ���� �̵�
	va_start(arg, fmt);
	_logWrite.log(fmt, arg);
	va_end(arg);
}

LogWriter::LogWriter()
{
	_base = nullptr;
}

LogWriter::~LogWriter()
{
	_prefix.clear();
	SAFE_DELETE(_base);
}

void LogWriter::setLogger(BaseLog * base, const WCHAR * logPrefix)
{
	_prefix.clear();
	_prefix = logPrefix;

	if (_base) {
		BaseLog * old = _base;
		_base = nullptr;
		old->unInitalize();

		SAFE_DELETE(old);
	}
	_base = base;
	_base->initialize();
}

BaseLog * LogWriter::logger()
{
	return _base;
}

void LogWriter::log(WCHAR * fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);

	this->log(fmt, arg);

	va_end(arg);
}

void LogWriter::log(WCHAR * fmt, va_list args)
{
	wstr_t logMessage = CLOCK.nowTimeWithMilliSec();
	size_t threadId = 0;
	GET_CURRENT_THREAD_ID(&threadId);

	logMessage += L"\t";

	// ������ ���� ����
	// ������ �ڵ�... ���߿� Ȯ���ؾ���.
	Thread * pthread = ThreadManager::getInstance().at(threadId);
	if (pthread) {
		logMessage += pthread->GetName();
	}
	else {
		logMessage += _prefix;
	}

	array<WCHAR, SIZE_8 * 2> threadIdStr;
	snwprintf(threadIdStr, L"0x%X", threadId);

	logMessage += L" : ";
	logMessage += threadIdStr.data();
	logMessage += L"\t";

	array<WCHAR, SIZE_1024> logStr;

	vswprintf_s(logStr.data(), logStr.size(), fmt, args);

	logMessage += logStr.data();
	logMessage += L"\n";
	_base->log((WCHAR *)logMessage.c_str());
}
