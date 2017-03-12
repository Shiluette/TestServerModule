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
			// user uid get... userManager���� User ���� �Ϸ�.
			sPacket.uid = UserManager::getInstance().login(_oid, _id);
			SLog(L"** User Login   - id : [ %s ]  oid : [%I64u]  , uid : [%I64d]", _id.c_str() ,_oid, _uid);
			// TODO : SendPacket�� ����� �Ѵ�. -> Contents��? �ƴϸ� ���⼭? 
			// ��� ��? ������ Send�� ����ŷ�̶� return value �ٷ� ���´�.
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


// �α��� ��û
void RobbyProcess::CPacket_LOGIN(Session * session, Packet * rowPacket)
{
	PK_C_REQ_LOGIN * packet = (PK_C_REQ_LOGIN *)rowPacket;
	Query_Login * query = new Query_Login();
	query->_oid = session->getId();
	// ���� ���̵� User����� ���� ����......
	query->_id.assign(packet->id.begin(), packet->id.end());

	QueryStatement *statement = query->statement();
	statement->addParam((WCHAR *)packet->id.c_str());
	statement->addParam((WCHAR *)packet->pw.c_str());
	DBManager::getInstance().pushQuery(query);
}
// ȸ�� ���� ��û
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
// ���� ����
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
// �� ã��
// TODO : ������ �������� ���� �÷��̾� �� ��� �� ã�Ⱑ Ȱ��ȭ �ȵǰ� ���� (�Ϸ�)
// Packet�� ������ �÷��̾ ������ ���� �濡 ���� �� �� �ְ� ����!
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


// �غ���� ON
// TODO : �غ� ���·� ������ ������ �� �ٸ� �����鿡�� ���� ������ �۽��ؾ��Ѵ�.
void RobbyProcess::CPacket_GAMEREADYON(Session * session, Packet * rowPacket)
{
	PK_C_REQ_GAMEREADYON * packet = (PK_C_REQ_GAMEREADYON *)rowPacket;
	User * user = UserManager::getInstance().getUser(packet->uid);
	user->ready();
	PK_S_BRD_ROOMSTATE sPacket;
	sPacket.update = 3;
	sPacket.uid = packet->uid;
	Session * pSession = nullptr;
	// ������ ã�� Ready ���·� ����
	vector <User *> * userinfo = RoomManager::getInstance().findRoom(user->roomNumber())->UserInfo();
	for (auto i : *userinfo) {
		pSession = SessionManager::getInstance().session(i->oid());
		pSession->sendPacket(&sPacket);
	}																						
}


// �غ� ���� OFF
// TODO : �غ����� ���·� ������ ������ �� �ٸ� �����鿡�� ���� ������ �۽��ؾ��Ѵ�.
void RobbyProcess::CPacket_GAMEREADYOFF(Session * session, Packet * rowPacket)
{
	PK_C_REQ_GAMEREADYOFF * packet = (PK_C_REQ_GAMEREADYOFF *)rowPacket;
	User * user = UserManager::getInstance().getUser(packet->uid);
	user->unready();
	PK_S_BRD_ROOMSTATE sPacket;
	sPacket.update = 4;
	sPacket.uid = packet->uid;
	Session * pSession = nullptr;
	// ������ ã�� UnReady ���·� ����
	vector <User *> * userinfo = RoomManager::getInstance().findRoom(user->roomNumber())->UserInfo();
	for (auto i : *userinfo) {
		pSession = SessionManager::getInstance().session(i->oid());
		pSession->sendPacket(&sPacket);
	}
}

// �� ������
// TODO : ������ �����͸� �Է��Ͽ� RoomManager���� ���� �ؾ� �Ѵ�.(�Ϸ�)
// ���� ���� ������ �۾��� RoomManager����(ContentsThread�� ���� ������� Lock ����)
void RobbyProcess::CPacket_EXITROOM(Session * session, Packet * rowPacket)
{
	PK_C_REQ_EXITROOM * packet = (PK_C_REQ_EXITROOM *)rowPacket;

	User * pUser = UserManager::getInstance().getUser(packet->uid);
																				
	RoomManager::getInstance().LeaveUser(pUser);
}

// ���� ������
// TODO : ������ ��������� ������ �ִ� �����͵��� �⺻��(default)���� �����ʿ�
// DB�� �α׿��� �ð� �ۼ�.
void RobbyProcess::CPacket_GAMEOUT(Session * session, Packet * rowPacket)
{

	Query_Logout * query = new Query_Logout();
	PK_C_REQ_GAMEOUT * packet = (PK_C_REQ_GAMEOUT *)rowPacket;
	User * pUser = UserManager::getInstance().getUser(packet->uid);
	query->_id.assign(pUser->id());
	
	DBManager::getInstance().pushQuery(query);									// DB �ۼ� ��û
	
	UserManager::getInstance().logout(packet->uid);								// User �α׿��� �۾�


	SessionManager::getInstance().closeSession(session);						// Session �ݱ�(���ϴݰ� ��Ÿ ����)
}