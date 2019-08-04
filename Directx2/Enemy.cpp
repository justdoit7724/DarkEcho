#include "Enemy.h"
#include "WaveMgr.h"


#define ENEMY_APPEARANCE_FREQUENCY 0.0275f
#define ENEMY_APPEARANCE_NUM 5
#define ENEMY_APPEARANCE_LIFETIME 0.15f
#define ENEMY_SPEED 130
#define ENEMY_ACT_RAD 45
#define ENEMY_KILL_RAD 40


Enemy::Enemy(IScene* _sceneEvent, INoise* _noiseEvent, Vector2 firstPos)
{
	sceneEvent = _sceneEvent;
	noiseEvent = _noiseEvent;
	pos = firstPos;
}

void Enemy::Update()
{
	CheckWave();
	CheckPlayer();

	if (isChasing)
	{
		pos += dir * ENEMY_SPEED * Timer::SPF();

		Vector2 distV = pos - chaseDest;
		float sDist = Vector2::Dot(distV, distV);
		if (sDist < 10.0f) {
			isChasing = false;
		}
	}
	else { // fading in the area alpha (RED)

		curTime += Timer::SPF();
		areaAlpha = min(1, curTime / 5.0f);
	}
}

void Enemy::Render()
{
	if (!isChasing)
		return;

	curAppearanceFrequency += Timer::SPF();

	if (curAppearanceFrequency >= ENEMY_APPEARANCE_FREQUENCY)
	{
		curAppearanceFrequency = 0;

		// 0~1
		float nRand = float(rand()) / (float(RAND_MAX));
		// 1~2
		float nRand2 = (float(rand()) / (float(RAND_MAX))+1.0f)/2.0f;
		float curAngle = ANGLE360* + nRand;
		Vector2 curNDir = Vector2(cos(curAngle), -sin(curAngle));
		Vector2 curNPos = pos + curNDir * nRand2 * 15.0f;

		noiseEvent->Generate(curNPos, curNDir, ENEMY_APPEARANCE_LIFETIME, WAVE_KINEMATIC, RED);
	}
}

void Enemy::CheckWave()
{
	const int pX = pos.x / MAP_DIVISION_LENGTH_WIDTH;
	const int pY = pos.y / MAP_DIVISION_LENGTH_HEIGHT;
	const int minX = max(0, pX - 1);
	const int maxX = min(MAP_DIVISION_NUM_WIDTH, pX + 1);
	const int minY = max(0, pY - 1);
	const int maxY = min(MAP_DIVISION_NUM_HEIGHT, pY + 1);
	const float sActRad = ENEMY_ACT_RAD * ENEMY_ACT_RAD;
	for (int y = minY; y <= maxY; ++y)
		for (int x = minX; x <= maxX; x++)
		{
			const WavePartition& p = noiseEvent->Partition(y, x);
			for (int i = 0; i < p.size; ++i)
			{
				UINT curWaveIdx = p.idx[i];
				const Wave& curWave = noiseEvent->WaveInfo(curWaveIdx);
				Vector2 distV = pos - curWave.pos;
				float sDist = Vector2::Dot(distV, distV);
				if (sDist < sActRad)
				{
					if (curWave.kind != WAVE_NORMAL)
						continue;
					noiseEvent->Remove(curWaveIdx);

					if (curWave.curIntensity >= chaseIntensity)
					{
						isChasing = true;
						Vector2 distV = Vector2(
							curWave.origin.x - pos.x,
							curWave.origin.y - pos.y);
						float nRand = ((float(rand()) / float(RAND_MAX)) +1)*0.5f; // 0.5 ~ 1
						const float moveOver = 20 * nRand;
						chaseDest = Vector2::Normalize(distV)*moveOver + curWave.origin;
						chaseIntensity = curWave.curIntensity;
						dir = Vector2::Normalize(chaseDest - pos);
						areaAlpha = 0;
						curTime = 0;
					}
				}
			}
		}

	chaseIntensity -= Timer::SPF()*1.1f;
}

void Enemy::CheckPlayer()
{
	Vector2 playerPos;
	if (!sceneEvent->PlayerPos(playerPos))
		return;
	Vector2 distV = pos - playerPos;
	float sDist = Vector2::Dot(distV, distV);
	float sKillRad = ENEMY_KILL_RAD * ENEMY_KILL_RAD;

	if (sDist <= sKillRad)
	{
		sceneEvent->KillPlayer();
	}
}
