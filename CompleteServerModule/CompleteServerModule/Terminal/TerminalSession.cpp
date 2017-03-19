#include"stdafx.h"
#include"TerminalSession.h"

bool TerminalSession::connectTo(char * ip, int port)
{
	// Ŭ���̾�Ʈ�� ���� �ϴ°Ͱ� ����....
	_socketData._socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_socketData._socket == INVALID_SOCKET) {
		SLog(L"!!! Terminal socket fail !!");
		return false;
	}
	ZeroMemory(&_socketData._addrInfo, sizeof(_socketData._addrInfo));
	_socketData._addrInfo.sin_family = AF_INET;
	_socketData._addrInfo.sin_port = htons(port);
	inet_pton(AF_INET, ip, &(_socketData._addrInfo.sin_addr));

	this->setsocketOption();

	int retval = ::connect(_socketData._socket, (sockaddr *)&_socketData._addrInfo, sizeof(_socketData._addrInfo));
	if (retval == SOCKET_ERROR) {
		SLog(L"!! Terminal socket connect fail... !!");
		return false;
	}
	return true;
}

void TerminalSession::onSend(size_t transferSize)
{
	// NOTYFY : �͹̳ο��� �� ����Ȯ�� �ʿ� ����. ������ TCP�� !!!
}

void TerminalSession::sendPacket(Packet * packet)
{
	Stream stream;
	packet->encode(stream);

	packet_size_t offset = 0;

	array<char, SOCKET_BUF_SIZE> buffer;
	// Headersize : ��Ŷ�� �� ũ�� ǥ�� ũ��
	const size_t packetHeadersize = sizeof(packet_size_t);
	packet_size_t packetLen = packetHeadersize + (packet_size_t)stream.size();

	memcpy_s(buffer.data() + offset, buffer.max_size(), (void*)&packetLen, packetHeadersize);

	offset += packetHeadersize;
	memcpy_s(buffer.data() + offset, buffer.max_size(), (void*)stream.data(), (int32_t)stream.size());

	offset += (packet_size_t)stream.size();

	::send(_socketData._socket, buffer.data(), offset, 0);
}

Package * TerminalSession::onRecv(size_t transferSize)
{
	array<char, SOCKET_BUF_SIZE> data;
	int retval = ::recv(_socketData._socket, data.data(), (int)data.size(), 0);
	if (retval <= 0) {
		return nullptr;
	}

	packet_size_t offset = 0;
	packet_size_t packetLen = 0;
	
	// �� ��Ŷ ������ ũ�� 
	memcpy_s((void *)packetLen, sizeof(packetLen), (void *)data.data(), sizeof(packetLen));
	
	offset += sizeof(packetLen);

	Packet * packet = PacketAnalyzer::getInstance().analyzer(data.data() + offset, packetLen);
	if (packet == nullptr) {
		return nullptr;
	}
	Package * package = new Package(this, packet);
	return package;
}
