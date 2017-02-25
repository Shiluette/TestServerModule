#pragma once
#include"stdafx.h"

class Stream
{
	size_t offset_;
	size_t readPt_;
	array<UCHAR, SOCKET_BUF_SIZE> stream_;

public:
	Stream();
	Stream(UCHAR *stream, size_t size);
	void init();

	UCHAR *data();
	size_t size();

	void operator = (Stream &stream);
	void set(UCHAR *data, size_t size);

	// write
	//------------------------------------------------------------------------------------//

	bool checkWriteBound(size_t len);


	template<class T>
	void operator << (const T &value);

	void operator << (const bool &value);
	void operator << (const INT8 &value);
	void operator << (const UINT8 &value);
	void operator << (const INT16 &value);
	void operator << (const UINT16 &value);
	void operator << (const INT32 &value);
	void operator << (const UINT32 &value);
	void operator << (const INT64 &value);
	void operator << (const UINT64 &value);


	template<class T>
	void operator << (const std::vector<T> &value);

	void operator << (const str_t &value);
	void operator << (const wstr_t &value);

	//read
	//------------------------------------------------------------------------------------//

	bool checkReadBound(size_t len);
	void read(void *retval, size_t len);

	template<class T>
	void operator >> (T *retval);

	void operator >> (bool *retval);
	void operator >> (INT8 *retval);
	void operator >> (UINT8 *retval);
	void operator >> (INT16 *retval);
	void operator >> (UINT16 *retval);
	void operator >> (INT32 *retval);
	void operator >> (UINT32*retval);
	void operator >> (INT64  *retval);
	void operator >> (UINT64 *retval);


	template<class T>
	void operator >> (std::vector<T> *retval);

	void operator >> (str_t *retval);
	void operator >> (wstr_t *retval);
};
