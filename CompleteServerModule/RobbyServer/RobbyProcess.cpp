#include"stdafx.h"
#include"User.h"
#include"RobbyProcess.h"

class Query_Login : public Query
{
public:
	uint64_t _oid;
	int64_t _uid;
	wstr_t _id;

	Query_Login() {
		_statement->setQuery(this->procedure(), QUERY_WAIT_RETURN);
	}
	virtual ~Query_Login() {
		int result = 100;
		_recode.get(L"result", &result);
		Session * session = SessionManager::getInstance().session(_oid);
		if (result == 0) {
			// Packet create
			PK_S_ANS_LOGINSUCC sPacket;
			// user uid get... userManager으로 User 설정 완료.
			sPacket.uid = UserManager::getInstance().login(_oid, _id);
			SLog(L"** User Login   - id : [ %s ]  oid : [%I64u]  , uid : [%I64d]", _id.c_str() ,_oid, _uid);
			// TODO : SendPacket을 해줘야 한다. -> Contents로? 아니면 여기서? 
			// 어떻게 함? 어차피 Send가 논블로킹이라서 return value 바로 나온다.
			session->sendPacket(&sPacket);
		}
		else {
			PK_S_ANS_LOGINFAIL sPacket;
			sPacket.errcode = result;
			session->sendPacket(&sPacket);
		}
	}
	WCHAR * procedure()
	{
		return L"dbo.p_USERLOGIN";
	}
};
class Query_Logout : public Query
{
public:
	wstr_t _id;
	Query_Logout() {
		_statement->setQuery(this->procedure(), QUERY_NOT_RETURN);
	}
	virtual ~Query_Logout(){}
	WCHAR * procedure()
	{
		return L"dbo.p_USERLOGOUT";
	}
};
class Query_Join : public Query
{
public:
	uint64_t _oid;


	Query_Join() {
		_statement->setQuery(this->procedure(), QUERY_WAIT_RETURN);
	}
	virtual ~Query_Join() {
		int result = 100;
		_recode.get(L"result", &result);
		Session * session = SessionManager::getInstance().session(_oid);
		if (result == 100) {
			PK_S_ANS_JOINSUCC sPacket;
			session->sendPacket(&sPacket);
		}
		else {
			PK_S_ANS_JOINFAIL sPacket;
			sPacket.errcode = result;
			session->sendPacket(&sPacket);
		}
		
	}
	WCHAR * procedure()
	{
		return L"dbo.p_USERLOGIN_INSERT";
	}
};

RobbyProcess::RobbyProcess()
{
	this->registSubPacketFunc();
}

void RobbyProcess::registSubPacketFunc()
{
	_runFuncTable.insert(make_pair(PE_C_REQ_LOGIN, &RobbyProcess::CPacket_LOGIN));
	_runFuncTable.insert(make_pair(PE_C_REQ_JOIN, &RobbyProcess::CPacket_JOIN));
	_runFuncTable.insert(make_pair(PE_C_REQ_SELECTPART, &RobbyProcess::CPacket_SELECTPART));
	_runFuncTable.insert(make_pair(PE_C_REQ_SEARCHROOM, &RobbyProcess::CPacket_SEARCHROOM));
	_runFuncTable.insert(make_pair(PE_C_REQ_GAMEREADYON, &RobbyProcess::CPacket_GAMEREADYON));
	_runFuncTable.insert(make_pair(PE_C_REQ_GAMEREADYOFF, &RobbyProcess::CPacket_GAMEREADYOFF));
	_runFuncTable.insert(make_pair(PE_C_REQ_EXITROOM, &RobbyProcess::CPacket_EXITROOM));
	_runFuncTable.insert(make_pair(PE_C_REQ_GAMEOUT, &RobbyProcess::CPacket_GAMEOUT));

}


// 로그인 요청
void RobbyProcess::CPacket_LOGIN(Session * session, Packet * rowPacket)
{
	PK_C_REQ_LOGIN * packet = (PK_C_REQ_LOGIN *)rowPacket;
	Query_Login * query = new Query_Login();
	query->_oid = session->getId();
	// 유저 아이디 User등록을 위해 지정......
	query->_id.assign(packet->id.begin(), packet->id.end());

	QueryStatement *statement = query->statement();
	statement->addParam((WCHAR *)packet->id.c_str());
	statement->addParam((WCHAR *)packet->pw.c_str());
	DBManager::getInstance().pushQuery(query);
}
// 회원 가입 요청
void RobbyProcess::CPacket_JOIN(Session * session, Packet * rowPacket)
{
	PK_C_REQ_JOIN * packet = (PK_C_REQ_JOIN *)rowPacket;
	Query_Join * query = new Query_Join();
	query->_oid = session->getId();

	QueryStatement * statement = query->statement();
	statement->addParam((WCHAR *)packet->id.c_str());
	statement->addParam((WCHAR *)packet->pw.c_str());
	statement->addParam((WCHAR *)packet->name.c_str());
	statement->addParam((WCHAR *)packet->email.c_str());
	DBManager::getInstance().pushQuery(query);
}
// 역할 설정
void RobbyProcess::CPacket_SELECTPART(Session * session, Packet * rowPacket)
{
	PK_C_REQ_SELECTPART * packet = (PK_C_REQ_SELECTPART *)rowPacket;
	User * pUser = UserManager::getInstance().getUser(packet->uid);
	if ((USER_ROLE)packet->role == pUser->setRole((USER_ROLE)packet->role)) {
		SLog(L" -- [Uid : %d ] , [SelectPart : %d ] --", pUser->uid(), pUser->role());
		PK_S_ANS_SELECTPARTSUCC sPacket;
		sPacket.role = packet->role;
		session->sendPacket(&sPacket);
	}
}
// 방 찾기
// TODO : 역할이 정해지지 않은 플레이어 일 경우 방 찾기가 활성화 안되게 제한 (완료)
// Packet을 보내서 플레이어가 역할을 고르고 방에 입장 할 수 있게 유도!
void RobbyProcess::CPacket_SEARCHROOM(Session * session, Packet * rowPacket)
{
	PK_C_REQ_SEARCHROOM * packet = (PK_C_REQ_SEARCHROOM *)rowPacket;
	User * pUser = UserManager::getInstance().getUser(packet->uid);
	if (pUser->role() == ROLE_NONE) {
																							
		PK_S_NTF_ROOMSEARCHNOTROLE sPacket;
		session->sendPacket(&sPacket);
		return;
	}

	RoomManager::getInstance().addEnterUser(pUser);
}


// 준비상태 ON
// TODO : 준비 상태로 변경을 시켜준 뒤 다른 유저들에게 상태 변경을 송신해야한다.
void RobbyProcess::CPacket_GAMEREADYON(Session * session, Packet * rowPacket)
{
	PK_C_REQ_GAMEREADYON * packet = (PK_C_REQ_GAMEREADYON *)rowPacket;
	User * user = UserManager::getInstance().getUser(packet->uid);
	user->ready();
	PK_S_BRD_ROOMSTATE sPacket;
	sPacket.update = 3;
	sPacket.uid = packet->uid;
	Session * pSession = nullptr;
	// 유저를 찾아 Ready 상태로 변경
	vector <User *> * userinfo = RoomManager::getInstance().findRoom(user->roomNumber())->UserInfo();
	for (auto i : *userinfo) {
		pSession = SessionManager::getInstance().session(i->oid());
		pSession->sendPacket(&sPacket);
	}																						
}


// 준비 상태 OFF
// TODO : 준비해제 상태로 변경을 시켜준 뒤 다른 유저들에게 상태 변경을 송신해야한다.
void RobbyProcess::CPacket_GAMEREADYOFF(Session * session, Packet * rowPacket)
{
	PK_C_REQ_GAMEREADYOFF * packet = (PK_C_REQ_GAMEREADYOFF *)rowPacket;
	User * user = UserManager::getInstance().getUser(packet->uid);
	user->unready();
	PK_S_BRD_ROOMSTATE sPacket;
	sPacket.update = 4;
	sPacket.uid = packet->uid;
	Session * pSession = nullptr;
	// 유저를 찾아 UnReady 상태로 변경
	vector <User *> * userinfo = RoomManager::getInstance().findRoom(user->roomNumber())->UserInfo();
	for (auto i : *userinfo) {
		pSession = SessionManager::getInstance().session(i->oid());
		pSession->sendPacket(&sPacket);
	}
}

// 방 나가기
// TODO : 나가기 데이터를 입력하여 RoomManager에게 전달 해야 한다.(완료)
// 실제 방을 나가는 작업은 RoomManager에서(ContentsThread의 단일 스레드로 Lock 없게)
void RobbyProcess::CPacket_EXITROOM(Session * session, Packet * rowPacket)
{
	PK_C_REQ_EXITROOM * packet = (PK_C_REQ_EXITROOM *)rowPacket;

	User * pUser = UserManager::getInstance().getUser(packet->uid);
																				
	RoomManager::getInstance().LeaveUser(pUser);
}

// 게임 나가기
// TODO : 유저의 게임종료로 지정되 있던 데이터들을 기본값(default)으로 정정필요
// DB에 로그오프 시간 작성.
void RobbyProcess::CPacket_GAMEOUT(Session * session, Packet * rowPacket)
{

	Query_Logout * query = new Query_Logout();
	PK_C_REQ_GAMEOUT * packet = (PK_C_REQ_GAMEOUT *)rowPacket;
	User * pUser = UserManager::getInstance().getUser(packet->uid);
	query->_id.assign(pUser->id());
	
	DBManager::getInstance().pushQuery(query);									// DB 작성 요청
	
	UserManager::getInstance().logout(packet->uid);								// User 로그오프 작업


	SessionManager::getInstance().closeSession(session);						// Session 닫기(소켓닫고 기타 등등등)
}