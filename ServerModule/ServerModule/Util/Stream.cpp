#include"stdafx.h"
#include "Stream.h"

Stream::Stream()
{
	this->init();
}

Stream::Stream(UCHAR * stream, size_t size)
{
	this->init();
	this->set(stream, size);
}

void Stream::init()
{
	readPt_ = 0;
	offset_ = 0;
	ZeroMemory(&stream_, sizeof(stream_));
}

UCHAR * Stream::data()
{
	return stream_.data();
}

size_t Stream::size()
{
	return offset_;
}

void Stream::operator=(Stream & stream)
{
	this->set(stream.data(), stream.size());
}

void Stream::set(UCHAR * data, size_t size)
{
	this->offset_ = size;
	memcpy_s(this->stream_.data(), stream_.size(), (void *)data, size);
}

bool Stream::checkWriteBound(size_t len)
{
	if (offset_ + len > sizeof(stream_)) {
		/*SLog(L"! socket stream over.");
		ASSERT(FALSE);*/
		return false;
	}
	return true;
}
#define STREAM_WRITE(value)						\
	INT32 size = sizeof(value);					\
	if (this->checkWriteBound(size) == false) {	\
		return;									\
	}											\
	memcpy_s((void *)(stream_.data() + offset_), stream_.size() - offset_, (const void *)&value, size);\
	offset_ += size;


template<class T>
void Stream::operator<<(const T & value)
{
	STREAM_WRITE(value);
}


void Stream::operator<<(const bool & value)
{
	STREAM_WRITE(value);
}

void Stream::operator<<(const INT8 & value)
{
	STREAM_WRITE(value);
}

void Stream::operator<<(const UINT8 & value)
{
	STREAM_WRITE(value);
}

void Stream::operator<<(const INT16 & value)
{
	STREAM_WRITE(value);
}

void Stream::operator<<(const UINT16 & value)
{
	STREAM_WRITE(value);
}

void Stream::operator<<(const INT32 & value)
{
	STREAM_WRITE(value);
}

void Stream::operator<<(const UINT32 & value)
{
	STREAM_WRITE(value);
}

void Stream::operator<<(const INT64 & value)
{
	STREAM_WRITE(value);
}

void Stream::operator<<(const UINT64 & value)
{
	STREAM_WRITE(value);
}
template<class T>
void Stream::operator<<(const std::vector<T>& value)
{
	*this << value.size();
	for (auto i : value) {
		*this << i;
	}
}



void Stream::operator<<(const str_t & value)
{
	*this << (INT32)value.length();
	for (auto i : value) {
		*this << i;
	}
}

void Stream::operator<<(const wstr_t & value)
{
	*this << (INT32)value.length();
	for (auto i : value) {
		*this << i;
	}
}

bool Stream::checkReadBound(size_t len)
{
	if (readPt_ + len > offset_) {
		/*SLog(L"! readOffset : %d, size : %d, totalOffset = %d", readPt_, len, offset_);
		SLog(L"! socket stream has not memory.");
		ASSERT(FALSE);*/
		return false;
	}
	return true;
}

void Stream::read(void * retval, size_t len)
{
	memcpy_s(retval, len, (void *)(stream_.data() + readPt_), len);
	readPt_ += len;
}

#define STREAM_READ(type, retval)			\
	size_t size = sizeof(type);				\
	if(this->checkReadBound(size) == false){\
		return;								\
	}										\
	this->read((void *)(retval), size);


template<class T>
void Stream::operator >> (T * retval)
{
	STREAM_READ(T, retval);
}


void Stream::operator >> (bool * retval)
{
	STREAM_READ(bool, retval);
}

void Stream::operator >> (INT8 * retval)
{
	STREAM_READ(INT8, retval);
}

void Stream::operator >> (UINT8 * retval)
{
	STREAM_READ(UINT8, retval);
}

void Stream::operator >> (INT16 * retval)
{
	STREAM_READ(INT16, retval);
}

void Stream::operator >> (UINT16 * retval)
{
	STREAM_READ(UINT16, retval);
}

void Stream::operator >> (INT32 * retval)
{
	STREAM_READ(INT32, retval);
}

void Stream::operator >> (UINT32 * retval)
{
	STREAM_READ(UINT32, retval);
}

void Stream::operator >> (INT64 * retval)
{
	STREAM_READ(INT64, retval);
}

void Stream::operator >> (UINT64 * retval)
{
	STREAM_READ(UINT64, retval);
}

template<class T>
void Stream::operator >> (std::vector<T>* retval)
{
	size_t size;
	*this >> &size;

	for (size_t i = 0; i < size; ++i) {
		T tmo;
		*this >> &tme;
		retval->push_back(tmp);
	}
}

void Stream::operator >> (str_t * retval)
{
	INT32 size;
	*this >> &size;
	if (this->checkReadBound(size) == false) {
		return;
	}

	char *buf = new char[size + 1];
	this->read((void *)(buf), size * sizeof(CHAR));
	buf[size] = '\0';

	retval->clear();
	*retval = buf;

	delete buf;
}

void Stream::operator >> (wstr_t * retval)
{
	INT32 size;
	*this >> &size;
	if (this->checkReadBound(size) == false) {
		return;
	}

	WCHAR *buf = new WCHAR[size + 1];
	this->read((void *)(buf), size * sizeof(WCHAR));
	buf[size] = '\0';

	retval->clear();
	*retval = buf;

	delete buf;
}
