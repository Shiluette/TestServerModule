#pragma once
#include"stdafx.h"

typedef UINT64 object_t;
typedef UINT64 oid_t;
typedef int32_t packet_size_t;

typedef time_t tick_t;
typedef thread thread_t;
typedef thread::id  threadId_t;

typedef std::recursive_mutex lock_t;

typedef std::string str_t;
typedef std::wstring wstr_t;

enum {
	SIZE_8 = 8,
	SIZE_64 = 64,
	SIZE_128 = 128,
	SIZE_256 = 256,
	SIZE_1028 = 1024,
	SIZE_4096 = 4096,
	SIZE_8192 = 8192,
	DB_PARAM_SIZE = 8192,
	SOCKET_BUF_SIZE = 10240
};