#include"stdafx.h"
#include"Session.h"
#include"IOCPSession.h"
#include"SessionManager.h"
#include"PacketAnalyzer.h"


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
	packet_size_t offset = 0;
	packet_size_t packetLen = 0;
	if (_totalBytes == 0) {
		memcpy_s((void*)&packetLen, sizeof(packet_size_t), (void *)_buffer.data(), sizeof(packet_size_t));
		_totalBytes = (size_t)packetLen;
	}
	offset += sizeof(packetLen);
	return offset;
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

bool IoData::setData(Stream & stream)
{
	this->clear();

	if (_buffer.max_size() <= stream.size()) {
		SLog(L"!! Packet size very big. this size [%d] byte !!", stream.size());
		return false;
	}
	packet_size_t offset = 0;
	char *buf = _buffer.data();

	// 코딩 센스! memcpy_s 인자  포인터 값 넘겨야함
	// Packetlen = 인자를 넣을때 (void*)& packetLen으로 넣는 방안
	// 배열의 이름은 포인터니 packetLen[1] 으로 하여 (void*)PacketLen 이 둘의 차이점.
	packet_size_t packetLen = sizeof(packet_size_t) + (packet_size_t)stream.size();

	// 데이텅 앞부분에 데이터의 총 크기를 작성
	memcpy_s(buf + offset, _buffer.max_size(), (void *)&packetLen, sizeof(packetLen));

	// 앞 부분의 4바이트를 사용하였음 -> offset move
	offset += sizeof(packetLen);

	memcpy_s(buf + offset, _buffer.max_size(), (void *)stream.data(), (int32_t)stream.size());
	
	offset += (packet_size_t)stream.size();

	_totalBytes = offset;
	return true;
}

LPWSAOVERLAPPED IoData::overlapped()
{
	return &_overlapped;
}

bool IoData::lackIOBuf(size_t size)
{
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
		SLog(L"!! socket error : %d ", WSAGetLastError());
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
	if (_ioData[IO_WRITE].lackIOBuf(size)) {
		this->send(_ioData[IO_WRITE].wsaBuf());
	}
}

void IOCPSession::sendPacket(Packet * packet)
{
	Stream stream;
	packet->encode(stream);
	
	if (!_ioData[IO_WRITE].setData(stream)) {
		return;
	}
	WSABUF wsabuf;
	wsabuf.buf = _ioData[IO_WRITE].data();
	wsabuf.len = (ULONG)stream.size();

	this->send(wsabuf);
	this->recvStandBy();
}

Package * IOCPSession::onRecv(size_t size)
{
	packet_size_t offset = 0;
	offset += _ioData[IO_READ].setupTotalBytes();

	if (this->isRecving(size)) {
		return nullptr;
	}

	packet_size_t packetdataSize = _ioData[IO_READ].totalByte() - sizeof(packet_size_t);
	byte * packetData = (byte*)_ioData[IO_READ].data() + offset;

	Packet *packet = PacketAnalyzer::getInstance().analyzer((const char*)packetData, packetdataSize);
	if (packet == nullptr) {
		SLog(L"!! Not regist packet Type!!");
		this->onClose();
		return nullptr;
	}
	this->recvStandBy();
	Package * package = new Package(this, packet);
	return package;

}

void IOCPSession::recvStandBy()
{
	_ioData[IO_READ].clear();

	WSABUF wsabuf;
	wsabuf.buf = _ioData[IO_READ].data();
	wsabuf.len = SOCKET_BUF_SIZE;

	this->recv(wsabuf);
}
