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
#include<ctime>
#include<cmath>
#include<atomic>

#include<tbb/tbb.h>
#include<array>
#include<unordered_set>
#include<vector>
#include<map>

#include<string>
#include<tchar.h>
#include<typeinfo>


//#include"./include/tbb/tbb.h"

using namespace std;

using namespace chrono;

using namespace tbb;
#include"Shutdown.h"
#include"Util\Type.h"
#include"Util\Util.h"
#include"Util\ThreadJobQueue.h"
#include"Util\Singleton.h"
#include"Util\Thread.h"
#include"Util\Lock.h"

#include"Network\Contents\ContentsProcess.h"

#include"Session.h"

#include"Network\Server\Server.h"
#include"Network\Server\IOCPServer.h"
#include"Util\Vector2D.h"
#include"Util\Metrix.h"




// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
