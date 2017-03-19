#include"stdafx.h"
#include"TerminalManager.h"

TerminalManager::TerminalManager()
{
}

TerminalManager::~TerminalManager()
{
	for (auto iter : _terminalPool) {
		auto terminal = iter.second;
		SAFE_DELETE(terminal);
	}
}

void TerminalManager::initialize(jsonValue_t * config)
{
	jsonValue_t tervalue = (*config)["Terminal"];
	if (tervalue == NULL) {
		SErrLog(L"** not exist terminal setting **");
		return;
	}
	
	int terminalcount = tervalue.get("TerminalCount", 0).asInt();
	auto nameList = tervalue["NAME"];
	for (auto cnt = 0; cnt < terminalcount; ++cnt) {
		str_t temp;
		array<WCHAR, _MAX_PATH> terminalName;
		temp = nameList[cnt].asString();
		StrConvA2W((char *)temp.c_str(),terminalName.data(), terminalName.max_size());
		Terminal * terminal = new Terminal(_server, terminalName.data());
		terminal->initialize(&tervalue, cnt);

		this->put(terminalName.data(), terminal);
	}

	SLog(L"### Terminal Setting complete ~! ###");
}

void TerminalManager::put(wstr_t serverName, Terminal * terminal)
{
	_terminalPool.insert(make_pair(serverName, terminal));
}

Terminal * TerminalManager::terminal(wstr_t name)
{
	return _terminalPool.at(name);
}

bool TerminalManager::isTerminal(const char * ip)
{
	for (auto iter : _terminalPool) {
		if (!strcmp(ip, iter.second->ip())) {
			return true;
		}
	}
	return false;
}

void TerminalManager::run(Server * server)
{
	_server = server;


	jsonValue_t config;
	if (!loadConfig(&config)) {
		return;
	}
	this->initialize(&config);
}
