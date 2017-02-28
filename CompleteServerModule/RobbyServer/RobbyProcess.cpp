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
		_recode.get(L"result", result);
		Session * session = SessionManager::getInstance().session(_oid);
		if (result == 0) {
			// Packet create
			PK_S_ANS_LOGINSUCC sPacket;
			// user uid get... userManager으로 User 설정 완료.
			sPacket.uid = UserManager::getInstance().login(_oid, _id);
			SLog(L"** User Login   - id : [ %s ]  oid : [%I64u]  , uid : [%I64d]", _id ,_oid, _uid);
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
		_recode.get(L"result", result);
		Session * session = SessionManager::getInstance().session(_oid);
		if (result == 1) {
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
}

RobbyProcess::~RobbyProcess()
{
}

void RobbyProcess::registSubPacketFunc()
{
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
		PK_S_ANS_SELECTPARTSUCC sPacket;
		sPacket.role = packet->role;
		session->sendPacket(&sPacket);
	}
}
// 방 찾기
void RobbyProcess::CPacket_SEARCHROOM(Session * session, Packet * rowPacket)
{
	PK_C_REQ_SEARCHROOM * packet = (PK_C_REQ_SEARCHROOM *)rowPacket;
	User * pUser = UserManager::getInstance().getUser(packet->uid);
	if (pUser->role() == ROLE_NONE) {
																							// TODO : 역할이 정해지지 않은 플레이어 일 경우 방 찾기가 활성화 안되게 제한 (완료)
																							// Packet을 보내서 플레이어가 역할을 고르고 방에 입장 할 수 있게 유도!
		PK_S_NTF_ROOMSEARCHNOTROLE sPacket;
		session->sendPacket(&sPacket);
		return;
	}

	RoomManager::getInstance().addEnterUser(pUser);
}

																							// TODO : 방에 들어 갔을 경우 방 입장 완료 패킷을 전송해야 한다.(추가 제작 필요)(완료)
// 준비상태 ON
void RobbyProcess::CPacket_GAMEREADYON(Session * session, Packet * rowPacket)
{

}
// 준비 상태 OFF
void RobbyProcess::CPacket_GAMEREADYOFF(Session * session, Packet * rowPacket)
{

}
// 방 나가기
void RobbyProcess::CPacket_EXITROOM(Session * session, Packet * rowPacket)
{
	PK_C_REQ_EXITROOM * packet = (PK_C_REQ_EXITROOM *)rowPacket;

	User * pUser = UserManager::getInstance().getUser(packet->uid);
																							// TODO : 나가기 데이터를 입력하여 RoomManager에게 전달 해야 한다.(완료)
																							// 실제 방을 나가는 작업은 RoomManager에서(ContentsThread의 단일 스레드로 Lock 없게)
	RoomManager::getInstance().LeaveUser(pUser);
}
// 게임 나가기
void RobbyProcess::CPacket_GAMEOUT(Session * session, Packet * rowPacket)
{
																							// TODO : 유저의 게임종료로 지정되 있던 데이터들을 기본값(default)으로 정정필요
																							// DB에 로그오프 시간 작성.
	PK_C_REQ_GAMEOUT * packet = (PK_C_REQ_GAMEOUT *)rowPacket;


	SessionManager::getInstance().closeSession(session);									// Session 닫기(소켓닫고 기타 등등등)
}