#pragma once
#include"stdafx.h"

typedef enum {
	TERMINAL_STOP,
	TERMINAL_READY,
}TERMINAL_STATE;

class Server;
class Terminal
{
protected:
	Server *			_server;
	TERMINAL_STATE		_state;
	wstr_t				_name;

	char				_ip[16];
	int					_port;

	TerminalSession		_session;
	Thread				* _terminalthread;
public:
	Terminal(Server *server, wstr_t name);
	virtual ~Terminal();
	TERMINAL_STATE &status();

	void		initialize(jsonValue_t *config, int index);
	void		sendPacket(Packet *packet);
	const char *ip();
	int			port();

private:
	void		connectProcess();
	void		run();

};