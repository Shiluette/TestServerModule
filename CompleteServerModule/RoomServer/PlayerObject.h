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
	STOP_STATE,				// 멈춰 있는 상태(아무 이동키도 누르지 않은 상태)
	RUN_STATE,				// 달리는 상태(일반적)
	WALK_STATE,				// 걷는 상태(특수 키 누르고 이동시)
	CROUCH_STATE,			// 숙여있는 상태(특수 키누르고 있을 시)
	EMPLACETRAP_STATE,		// 트랩을 설치하는 상태(이동하지못함)
}MOVE_STATE;
typedef enum
{
	IDLE_STATE,				// 보통상태
	WOUNDTRAP_STATE,		// 트랩에 걸린 상태
	EMERGENCY_STATE,		// 체력이 얼마 안 남았을때 상태
	DEAD_STATE,				// 죽은 상태
}STATE_BAR;


// 기능 설정
// 걷기 뛰에 따른 상태 변화
// 체력 변화
// 
class PlayerObject : public NameObject, public Work
{
private:
	INT64		_uid;					// 유저 넘버
	USER_ROLE	_role;					// 유저의 역할
private:
	D3DXVECTOR3 _position;				// 월드 내에 좌표
	D3DXVECTOR3 _direction;				// defualt value
	MOVE_STATE _moveState;				// 이동상태
	STATE_BAR _statusBar;				// 상태 창
	FLOAT _degree;						// 바라보는 방향
	FLOAT _speed;						// 이동 속도
	INT32 _hp;							// 체력 
	INT32 _TrapCount;					// 트랩 수
	UINT64 _latency;					// 레이턴시 count;
private:
	static const float Killer_speed;	// 살인마 이동속도
	static const float Sur_run;			// 생존자 뛰는속도
	static const float Sur_walk;		// 생존자 걷는속도
	static const float Sur_CrouchWalk;		// 생존자 기는속도
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
	void setLatency(UINT64 time);		// 레이턴시 수정
	void useTrap();						// 트렙 사용시
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