#pragma once
#include"stdafx.h"
typedef enum
{
	ROLE_NONE,
	ROLE_KILLER,
	ROLE_SURVIORS,
}USER_ROLE;
typedef enum
{
	STOP_STATE,				// ���� �ִ� ����(�ƹ� �̵�Ű�� ������ ���� ����)
	RUN_STATE,				// �޸��� ����(�Ϲ���)
	WALK_STATE,				// �ȴ� ����(Ư�� Ű ������ �̵���)
	CROUCH_STATE,			// �����ִ� ����(Ư�� Ű������ ���� ��)
	EMPLACETRAP_STATE,		// Ʈ���� ��ġ�ϴ� ����(�̵���������)
}MOVE_STATE;
typedef enum
{
	IDLE_STATE,				// �������
	WOUNDTRAP_STATE,		// Ʈ���� �ɸ� ����
	EMERGENCY_STATE,		// ü���� �� �� �������� ����
	DEAD_STATE,				// ���� ����
}STATE_BAR;


// ��� ����
// �ȱ� �ٿ� ���� ���� ��ȭ
// ü�� ��ȭ
// 
class PlayerObject : public NameObject, public Work
{
private:
	INT64		_uid;					// ���� �ѹ�
	USER_ROLE	_role;					// ������ ����
private:
	D3DXVECTOR3 _position;				// ���� ���� ��ǥ
	D3DXVECTOR3 _direction;				// defualt value
	MOVE_STATE _moveState;				// �̵�����
	STATE_BAR _statusBar;				// ���� â
	FLOAT _degree;						// �ٶ󺸴� ����
	FLOAT _speed;						// �̵� �ӵ�
	INT32 _hp;							// ü�� 
	INT32 _TrapCount;					// Ʈ�� ��
	UINT64 _latency;					// �����Ͻ� count;
private:
	static const float Killer_speed;	// ���θ� �̵��ӵ�
	static const float Sur_run;			// ������ �ٴ¼ӵ�
	static const float Sur_walk;		// ������ �ȴ¼ӵ�
	static const float Sur_CrouchWalk;		// ������ ��¼ӵ�
public:
	PlayerObject() {
	}
	PlayerObject(wstr_t name, USER_ROLE role, 
		D3DXVECTOR3 position, FLOAT degree);
	PlayerObject(PlayerObject & instance);
	virtual ~PlayerObject();
	void setClear();
	void operator=(PlayerObject & ins);
	void setMoveState(MOVE_STATE & state);
	void setLatency(UINT64 time);		// �����Ͻ� ����
	void useTrap();						// Ʈ�� ����
	void woundTrap();
	void tick();
	void wakeup() {}
	void stop() {}
	void start() {}
};
// -- NOTY --
// Killer RunSpeed = 2.6m/s => 0.026 float/miliseconds
// Survivors RunSpeed = 2.083 m/s => 0.021 float/miliseconds
// Survivors WalkSpeed = 1.389 m/s => 0.014 float/miliseconds

const float PlayerObject::Killer_speed = 0.026f;
const float PlayerObject::Sur_run = 0.021f;
const float PlayerObject::Sur_walk = 0.014f;
const float PlayerObject::Sur_CrouchWalk = 0.014f;

class PlayerManager : public Singleton<PlayerManager>
{
	unordered_map<int, vector<PlayerObject>> _playerPool;
public:
	PlayerManager();
	virtual ~PlayerManager();
	void initalize();
	void insert(int roomNumber, int pos, PlayerObject & object);
	void clear(int roomNumber);
	
};