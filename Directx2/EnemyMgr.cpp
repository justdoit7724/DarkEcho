#include "EnemyMgr.h"
#include "Camera.h"
#include "Game_info.h"

EnemyMgr::EnemyMgr(IScene* sceneEvent, INoise* noiseEvent)
{
	enemies[0] = new Enemy(sceneEvent, noiseEvent, Vector2(2300, 140));
	enemies[1] = new Enemy(sceneEvent, noiseEvent, Vector2(2570, 1750));
	enemies[2] = new Enemy(sceneEvent, noiseEvent, Vector2(60, 1330));
	enemies[3] = new Enemy(sceneEvent, noiseEvent, Vector2(120, 2740));
}

EnemyMgr::~EnemyMgr()
{
	delete[ENEMY_MAX] enemies;
}

void EnemyMgr::Update()
{
	const float divHeight = MAP_DIVISION_LENGTH_HEIGHT;
	const float divWidth = MAP_DIVISION_LENGTH_WIDTH;

	for (int i = 0; i < ENEMY_MAX; ++i){

		if (!enemies[i])
		{
			cbInfo.data[i].w = ENEMY_NOT_EXIST;
			break;
		}

		enemies[i]->Update();

		cbInfo.data[i].z = enemies[i]->AreaAlpha();

		Vector2 lPos = enemies[i]->pos - Camera::Pos();
		cbInfo.data[i].x = lPos.x;
		cbInfo.data[i].y = lPos.y;

		float hWidth = SCREEN_WIDTH / 2.0f;
		float hHeight = SCREEN_HEIGHT / 2.0f;
		if (lPos.x < -hWidth || hWidth < lPos.x ||
			lPos.y < -hHeight || hHeight < lPos.y) {
			cbInfo.data[i].w = ENEMY_OUTSCREEN;
		}
		else {
			cbInfo.data[i].w = ENEMY_INSCREEN;
		}
	}
}

void EnemyMgr::Render()
{
	for (Enemy* e : enemies) {
		if (!e)
			break;
		e->Render();
	}
}
