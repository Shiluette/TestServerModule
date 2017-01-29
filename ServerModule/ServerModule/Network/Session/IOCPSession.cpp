#include"stdafx.h"
#include"Session.h"
#include "IOCPSession.h"
#include"SessionManager.h"


IoData::IoData()
{
	ZeroMemory(&_overlapped, sizeof(_overlapped));
	_ioType = IO_ERROR;
	this->clear();
}

void IoData::clear()
{
	_buffer.fill(0);
	_totalBytes = 0;
	_currentBytes = 0;
}

int32_t IoData::setupTotalBytes()
{
	// TODO ::
	return int32_t();
}

size_t IoData::totalByte()
{
	return _totalBytes;
}

IO_OPERATION & IoData::type()
{
	return _ioType;
}

void IoData::setType(IO_OPERATION type)
{
	_ioType = type;
}

WSABUF IoData::wsaBuf()
{
	WSABUF wsaBuf;
	wsaBuf.buf = _buffer.data() + _currentBytes;
	wsaBuf.len = (ULONG)(_totalBytes - _currentBytes);
	return wsaBuf;
}

char * IoData::data()
{
	return _buffer.data();
}

bool IoData::setData()
{
	// TODO ::
	this->clear();

	//if (_buffer.max_size() <= data.size()) {
	//	// TODO : 버퍼보다 큰 데이터를 보낼려고 함. log 메시지 출력
	//	return false;
	//}
}

LPWSAOVERLAPPED IoData::overlapped()
{
	return &_overlapped;
}

bool IoData::lackIOBuf(size_t size)
{
	// TODO ::
	_currentBytes += size;
	if (_currentBytes < _totalBytes) {
		return true;
	}
	return false;
}

void IOCPSession::initialize()
{
	ZeroMemory(&_socketData, sizeof(SOCKET_DATA));
	_ioData[IO_READ].setType(IO_READ);
	_ioData[IO_WRITE].setType(IO_WRITE);
}

void IOCPSession::checkErrorIo(DWORD ret)
{
	if (ret == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING)) {
		// TODO :: Log 작성  "SOCKET err : getLasterror()
	}
}

void IOCPSession::recv(WSABUF wsaBuf)
{
	DWORD flag = 0;
	DWORD recvBytes = 0;
	DWORD ret = WSARecv(_socketData._socket, &wsaBuf, 1, &recvBytes, &flag, _ioData[IO_READ].overlapped(), NULL);
	this->checkErrorIo(ret);
}

bool IOCPSession::isRecving(size_t size)
{
	// recv buf 가 아직 다 안받아 졌는지 확인.
	if (_ioData[IO_READ].lackIOBuf(size)) {
		this->recv(_ioData[IO_READ].wsaBuf());
		return true;
	}
	return false;
}

void IOCPSession::send(WSABUF wsaBuf)
{
	DWORD flag = 0;
	DWORD sendBytes = 0;
	DWORD ret = WSASend(_socketData._socket, &wsaBuf, 1, &sendBytes, flag, _ioData[IO_WRITE].overlapped(), NULL);
	this->checkErrorIo(ret);
}

IOCPSession::IOCPSession()
{
	this->initialize();
}

void IOCPSession::onSend(size_t size)
{
	// TODO ::
}

void IOCPSession::sendPacket()
{
	// TODO ::
}

void IOCPSession::reccvStandBy()
{
	_ioData[IO_READ].clear();
}
