#pragma once
#include"stdafx.h"

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

#include<array>
#include<unordered_set>
#include<vector>
#include<map>
#include<unordered_map>
#include<set>

#include<string>
#include<tchar.h>
#include<typeinfo>
//#include<inttypes.h>

#if _DEBUG
#define CONTEXT_SWITCH  Sleep(1)
#else;
#define CONTEXT_SWITCH  ::SwitchToThread()
#endif


//#include"./include/tbb/tbb.h"

using namespace std;

#include"Network\Winsocket.h"

#include"Json\json.h"
#include"Shutdown.h"
#include"Util\Type.h"
#include"Util\Util.h"

#include"Util\Singleton.h"

#include"Util\Clock.h"
#include"Util\Log.h"
#include"util\Assert.h"

#include"Util\Thread.h"
#include"Util\Lock.h"
#include"Util\ThreadJobQueue.h"

#include"Util\Config.h"

#include"Util\Minidump.h"
#include"Util\Gameobject.h"
#include"Util\Task.h"

#include"Network\Session\Stream.h"

#include"Network\Session\PacketHeader.h"
#include"Network\Session\Packet.h"
#include"Network\Session\PacketAnalyzer.h"
#include"Network\Session\PacketFactory.h"
#include"Network\Session\Package.h"


#include"Network\Contents\ContentsProcess.h"
#include"Network\Session\SessionManager.h"
#include"Network\Session\Session.h"

#include"Network\Server\Server.h"
#include"Network\Server\IOCPServer.h"

#include"Terminal\TerminalSession.h"
#include"Terminal\Terminal.h"
#include"Terminal\TerminalManager.h"

#include"Util\Vector2D.h"
#include"Util\Metrix.h"


#include"Database\ODBCDatabase.h"
#include"Database\Query.h"
#include"Database\DBManager.h"