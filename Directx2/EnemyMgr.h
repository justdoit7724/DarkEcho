#pragma once
#include "DX_info.h"
#include "Enemy.h"

class INoise;

#define ENEMY_MAX 7
#define ENEMY_NOT_EXIST 0
#define ENEMY_OUTSCREEN 1
#define ENEMY_INSCREEN 2

struct CB_Enemy {
	// float2(pos) - float(area alpha) - float(exist)
	Vector4 data[ENEMY_MAX];

	CB_Enemy() {}
};

class EnemyMgr : public IEnemyInfo
{
public:
	EnemyMgr(IScene*, INoise*);
	~EnemyMgr();

	void Update();
	void Render();


	CB_Enemy Info() override
	{
		return cbInfo;
	}

private:
	Enemy* enemies[ENEMY_MAX] = { nullptr };

	CB_Enemy cbInfo;
};

