// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "Winmm.lib")
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include<WS2tcpip.h>
#include<winsock2.h>
#include<mswsock.h>
#include<mmsystem.h>
#include<WS2spi.h>
#include<mstcpip.h>

#include<windows.h>
#include<iostream>

#include<cstdlib>
#include<cstdint>

#include<thread>
#include<assert.h>
#include<mutex>
#include<memory>
#include<functional>
#include<algorithm>

#include<chrono>
#include <ctime>
#include<cmath>
#include<atomic>

#include<tbb/tbb.h>
#include<array>
#include<unordered_set>
#include<vector>

#include<string>
#include<tchar.h>
#include<typeinfo>


//#include"./include/tbb/tbb.h"

using namespace std;

using namespace chrono;

using namespace tbb;

#include"Network\Server\Server.h"
#include"Util\Vector2D.h"
#include"Util\Metrix.h"




// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
