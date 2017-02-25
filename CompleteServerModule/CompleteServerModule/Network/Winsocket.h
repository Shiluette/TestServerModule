#pragma once
#include"stdafx.h"
class Winsocket
{
private:
	WSADATA _wsa;
public:
	Winsocket() {
		static bool init = false;
		if (init) {
			return;
		}

		if (WSAStartup(MAKEWORD(2, 2), &_wsa ) != 0) {
			printf("! WINSOCK ERR : WSAStartup fail\n");
			exit(0);
		}
		init = true;
		printf(" ** WSA 2.2 Startup completeed. ** \n");
	}

	~Winsocket() {
		WSACleanup();
	}
};

static Winsocket winsocket;