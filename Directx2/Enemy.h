#pragma once
#include "DX_info.h"
#include "Transform.h"
#include "Network.h"

class Enemy :
	public Transform
{
public:
	Enemy(IScene*, INoise*, Vector2);

	void Update();
	void Render();

	bool IsChasing() {return isChasing;}
	float AreaAlpha() {return areaAlpha;}

private:

	void CheckWave();
	void CheckPlayer();

	IScene* sceneEvent;
	INoise* noiseEvent;

	float curAppearanceFrequency=0;

	bool isChasing = false;
	Vector2 chaseDest;
	float chaseIntensity;
	float areaAlpha = 1;

	float curTime=0;
};

