#include"stdafx.h"
#include"Terminal.h"

Terminal::Terminal(Server * server, wstr_t name)
{
	_server = server;
	_name = name;
}

Terminal::~Terminal()
{
	_state = TERMINAL_STOP;
}

TERMINAL_STATE & Terminal::status()
{
	return _state;
}

void Terminal::initialize(jsonValue_t * config, int index)
{
	jsonValue_t ip = config->get("IP", "");
	jsonValue_t port = config->get("PORT", "");
	strcpy_s(_ip, ip[index].asString().c_str());
	_port = port[index].asInt();
	this->run();
}

void Terminal::sendPacket(Packet * packet)
{
	if (_state == TERMINAL_READY) {
		_session.sendPacket(packet);
	}
}

const char * Terminal::ip()
{
	return _ip;
}

int Terminal::port()
{
	return _port;
}

void Terminal::connectProcess()
{
CONNECT_START:
	int connectTrycnt = 0;
	while (_shutdown == false) {
		if (_session.connectTo(_ip, _port)) {
			break;
		}
		SLog(L"** try connect [%s], server[%S]:[%d]... [%d]", _name.c_str(), _ip, _port, connectTrycnt++);
		// 5�ʿ� 1���� ���� �õ�
		Sleep(5000);
	}
	// ���� �Ϸ� ���� ����
	_state = TERMINAL_READY;
	// TODO : �͹̳� �������� �˸��� ��Ŷ ���� �ʿ�.

	SLog(L"**** [%s]terminal connect [%S]:[%d] ready****", _name.c_str(), _ip, _port);

	while (_shutdown == false) {
		Package * Tpackage = _session.onRecv(0);

		if (Tpackage == nullptr) {
			SLog(L"!! terminal [%s] disconnected !!", _name.c_str());
			_session.onClose();
			goto CONNECT_START;
			// ������ ������ ������� ����Ͽ� �ٽ� ����õ��ϰ�...
		}
		_server->putPackage(Tpackage);
	}
	
}

void Terminal::run()
{
	_terminalthread = MAKE_THREAD(Terminal, connectProcess);
}
