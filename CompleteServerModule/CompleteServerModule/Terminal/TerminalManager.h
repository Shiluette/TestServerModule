#pragma once
#include"stdafx.h"
class TerminalManager : public Singleton<TerminalManager>
{
private:
	unordered_map<wstr_t, Terminal *> _terminalPool;
	Server * _server;

public:
	TerminalManager();
	virtual ~TerminalManager();

	void initialize(jsonValue_t * config);

	void put(wstr_t serverName, Terminal * terminal);
	Terminal* terminal(wstr_t name);
	
	bool isTerminal(const char *ip);
	void run(Server *server);

};