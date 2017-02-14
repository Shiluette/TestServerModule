#include"stdafx.h"
#include"Session.h"

bool Session::setsocketOption()
{
	// 좀비 세션에 대한 대처 방안
	// SIO_KEEPALIVE_VALS 사용 권유, SIO_KEEPALIVE는 비추천
	// 이유 : SO_KEEPALIVE 옵션을 적용하면 시스템 레지스트리 값을 변경하므로 그 시스템의 모든 소켓의 KEEPALIVE 속성이 변경하게 된다
	tcp_keepalive keepAliveSet = { 0 }, returned = { 0 };
	keepAliveSet.onoff = 1;
	keepAliveSet.keepalivetime = 10000;				// 10초마다 한번씩 연결되있는지 확인
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
	//getpeername : 연결된 상대측 소켓 주소 정보를 가져온다.
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
