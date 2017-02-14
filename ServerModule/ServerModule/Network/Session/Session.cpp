#include"stdafx.h"
#include"Session.h"

bool Session::setsocketOption()
{
	// ���� ���ǿ� ���� ��ó ���
	// SIO_KEEPALIVE_VALS ��� ����, SIO_KEEPALIVE�� ����õ
	// ���� : SO_KEEPALIVE �ɼ��� �����ϸ� �ý��� ������Ʈ�� ���� �����ϹǷ� �� �ý����� ��� ������ KEEPALIVE �Ӽ��� �����ϰ� �ȴ�
	tcp_keepalive keepAliveSet = { 0 }, returned = { 0 };
	keepAliveSet.onoff = 1;
	keepAliveSet.keepalivetime = 10000;				// 10�ʸ��� �ѹ��� ������ִ��� Ȯ��
	keepAliveSet.keepaliveinterval = 10000;			// resend if no-reply

	DWORD dwBytes;
	if (WSAIoctl(_socketData._socket, SIO_KEEPALIVE_VALS, &keepAliveSet, sizeof(keepAliveSet), &returned, sizeof(returned), &dwBytes, NULL, NULL) != 0) {
		return false;
	}
	return true;
}

Session::Session()
{
	this->updateHeartBeat();
}

Session::~Session()
{
	this->onClose();
}

bool Session::onAccept(SOCKET socket, SOCKADDR_IN addrinfo)
{
	_socketData._socket = socket;
	int addrLen;
	//getpeername : ����� ����� ���� �ּ� ������ �����´�.
	getpeername(_socketData._socket, (struct sockaddr *)&_socketData._addrInfo, &addrLen);
	_socketData._addrInfo = addrinfo;
	if (!this->setsocketOption()) {
		return false;
	}
	return true;
}

void Session::onClose()
{
	SessionManager::getInstance().closeSession(this);
}

SOCKET & Session::getSocket()
{
	return _socketData._socket;
}

str_t Session::clientAddress()
{
	array<char, SIZE_64> ip;
	inet_ntop(AF_INET, &(_socketData._addrInfo.sin_addr), ip.data(), ip.size());
	return ip.data();
}

oid_t Session::getId()
{
	return _id;
}

void Session::setId(oid_t id)
{
	_id = id;
}

int8_t Session::getType()
{
	return this->_type;
}

void Session::settype(int8_t type)
{
	_type = type;
}

tick_t Session::heartBeat()
{
	return _lastHeartBeat;
}

void Session::updateHeartBeat()
{
	_lastHeartBeat = CLOCK.systemTick();
}
