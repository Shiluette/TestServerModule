#pragma once
#include"stdafx.h"

class Packet;
class Package;
struct SOCKET_DATA {
	SOCKET _socket;
	SOCKADDR_IN _addrInfo;
};
class Session
{
protected:
	SOCKET_DATA _socketData;
	oid_t _id;
	int8_t _type;

	bool setsocketOption();
	tick_t _lastHeartBeat; // 클라이언트와 신호가 일정 시간 이하로 없을 경우 확인용

public:
	Session();
	virtual ~Session();

	virtual bool onAccept(SOCKET socket, SOCKADDR_IN addrinfo);
	virtual void onSend(size_t transferSzie) = 0;
	virtual void sendPacket(Packet * packet) = 0;
	virtual Package * onRecv(size_t transfersize) = 0;
	virtual void recvStandBy() {};
	virtual void onClose();

	SOCKET & getSocket();
	str_t clientAddress();

	oid_t getId();
	void setId(oid_t id);

	int8_t getType();
	void settype(int8_t type);

	tick_t heartBeat();
	void updateHeartBeat();

	
};