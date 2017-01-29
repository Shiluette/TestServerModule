#pragma once
#include"stdafx.h"

class Session;
class SessionManager;
class Package;

typedef enum {
	IO_READ,
	IO_WRITE,
	IO_ERROR,
}IO_OPERATION;

class IoData {
	OVERLAPPED _overlapped; // overlapped
	IO_OPERATION _ioType; // type  0 : Read , 1 : Write , 2 : Error
	size_t _totalBytes; // Read Total byte
	size_t _currentBytes; // current Total byte
	array<char, SOCKET_BUF_SIZE> _buffer; // buf
public:
	IoData();
	void clear();

	bool lackIOBuf(size_t size);
	int32_t setupTotalBytes();
	size_t totalByte();

	IO_OPERATION &type();
	void setType(IO_OPERATION type);

	WSABUF wsaBuf();
	char * data();
	bool setData();// TODO :: buf
	LPWSAOVERLAPPED overlapped();
};


#define IO_DATA_COUNT 2


class IOCPSession : public Session
{
public:
	array<IoData, IO_DATA_COUNT> _ioData;
private:
	void initialize();

	void checkErrorIo(DWORD ret);
	void recv(WSABUF wsaBuf);
	bool isRecving(size_t size);

	void send(WSABUF wsaBuf);
public:
	IOCPSession();

	void onSend(size_t size);
	void sendPacket(/*TODO : Packet *  */);
	/*Package * onRecv(size_t size)*/ // TODO ::
	void reccvStandBy();
};