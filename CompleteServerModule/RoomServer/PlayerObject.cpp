#include"stdafx.h"
#include "PlayerObject.h"


PlayerObject::PlayerObject(wstr_t name, USER_ROLE role, D3DXVECTOR3 position, FLOAT degree)
{
	this->setName(name);
	_role = role;
	_position = position;
	_degree = degree;
	_moveState = RUN_STATE;
	if (_role != ROLE_KILLER) {
		_hp = 3;
		_TrapCount = 10;
		_speed = Sur_run;
	}
	else {
		_hp = NULL;
		_TrapCount = NULL;
		_speed = Killer_speed;
	}
}

PlayerObject::PlayerObject(PlayerObject & instance)
{
	this->setName(instance.name());
	_uid = instance._uid;
	_role = instance._role;
	_position = instance._position;
	_direction = instance._direction;
	_hp = instance._hp;
	_TrapCount = instance._TrapCount;

}

PlayerObject::~PlayerObject()
{
}

void PlayerObject::setClear()
{
	_uid = 0;				
	_role = ROLE_NONE;		
	_position.x = NULL;		
	_position.y = NULL;
	_position.z = NULL;
	_direction.x = NULL;	
	_direction.y = NULL;
	_direction.z = NULL;
	_moveState = STOP_STATE;
	_statusBar = IDLE_STATE;
	_degree = 0.0f;
	_speed = 0.0f;
	_hp = 0;
	_TrapCount = 0;
	_latency = 0;
}

void PlayerObject::operator=(PlayerObject & ins)
{
	this->setName(ins.name());
	_uid = ins._uid;
	_role = ins._role;
	_position = ins._position;
	_direction = ins._direction;
	_moveState = ins._moveState;
	_statusBar = ins._statusBar;
	_degree = ins._degree;
	_speed = ins._speed;
	_hp = ins._hp;
	_TrapCount = ins._TrapCount;
	_latency = ins._latency;
}

void PlayerObject::setMoveState(MOVE_STATE & state)
{
	// Killer는 걷는 상태의 변화가 없으므로...
	if (_role == ROLE_KILLER) return;
	_moveState = state;
	switch (_moveState)
	{
	case RUN_STATE:
		_speed = Sur_run;
		break;
	case WALK_STATE:
		_speed = Sur_walk;
		break;
	case CROUCH_STATE:
		_speed = Sur_CrouchWalk;
		break;
	}
}

void PlayerObject::setLatency(UINT64 time)
{
	_latency = time;
}

void PlayerObject::useTrap()
{
	_TrapCount--;
}

void PlayerObject::woundTrap()
{
}

void PlayerObject::tick()
{
}

PlayerManager::PlayerManager()
{
	this->initalize();
}

PlayerManager::~PlayerManager()
{
	_playerPool.clear();
}

void PlayerManager::initalize()
{
	for (auto i = 0; i < 500; ++i) {
		_playerPool.insert(make_pair(i, NULL));
		_playerPool[i].push_back(PlayerObject(L"", ROLE_NONE, D3DXVECTOR3(0, 0, 0), 0.0f));
		_playerPool[i].push_back(PlayerObject(L"", ROLE_NONE, D3DXVECTOR3(0, 0, 0), 0.0f));
		_playerPool[i].push_back(PlayerObject(L"", ROLE_NONE, D3DXVECTOR3(0, 0, 0), 0.0f));
		_playerPool[i].push_back(PlayerObject(L"", ROLE_NONE, D3DXVECTOR3(0, 0, 0), 0.0f));
	}
}

void PlayerManager::insert(int roomNumber, int pos , PlayerObject & object)
{
	_playerPool[roomNumber][pos] = object;
}

void PlayerManager::clear(int roomNumber)
{
	_playerPool[roomNumber][0].setClear();
	_playerPool[roomNumber][1].setClear();
	_playerPool[roomNumber][2].setClear();
	_playerPool[roomNumber][3].setClear();
}
