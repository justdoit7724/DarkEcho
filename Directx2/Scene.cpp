#include "Scene.h"
#include "Game_info.h"
#include "Camera.h"

Scene::Scene(ID3D11Device * _device, ID3D11DeviceContext * _dContext)
	: device(_device), dContext(_dContext)/*, cb(_device)*/
{
	Timer::Init();
	//Debugging::Init(_device, _dContext);

	Vector2 StartPos = Vector2(190, 400);

	map = new Map(_device);
	waveMgr = new WaveMgr(_device, map);
	player = new Player(_device, StartPos, this, waveMgr, map);
	enemyMgr = new EnemyMgr(this, waveMgr);
	screen = new ScreenQuad(_device, waveMgr, map, enemyMgr);

	Camera::Update(StartPos);
}

Scene::~Scene()
{
	delete screen;
}

void Scene::Update()
{
	Timer::Update();

	if (player)
		player->Update(dContext.Get());

	if (stone != nullptr)
	{
		stone->Update();
		if (stone->Kill())
		{
			delete stone;
			stone = nullptr;
		}
	}

	enemyMgr->Update();

	waveMgr->Update(dContext.Get());

	screen->Update();
}

void Scene::Render()
{
	screen->Render(dContext.Get());
	if(player)
		player->Render(dContext.Get());
	if(stone != nullptr)
		stone->Render(dContext.Get());
	enemyMgr->Render();
}

void Scene::ThrowStone(Vector2 pos, Vector2 dir)
{
	if (stone == nullptr)
	{
		stone = new Stone(device.Get(), pos, dir, waveMgr, map);
	}
}

void Scene::KillPlayer()
{
	for (int i = 0; i < DEATH_WAVE_NUM; ++i) {

		float angle = i * (ANGLE360 / DEATH_WAVE_NUM);
		Vector2 dir = Vector2(cos(angle), -sin(angle));

		waveMgr->Generate(player->Pos(), dir, DEATH_WAVE_LIFETIME, WAVE_REFLECT_ONLY, RED);
	}
	delete player;
	player = nullptr;
}

bool Scene::PlayerPos(Vector2& pos)
{
	if (player == nullptr)
		return false;

	pos=player->Pos();

	return true;
}

void Scene::ClearStage()
{
	for (int i = 0; i < CLEAR_WAVE_NUM; ++i) {

		float angle = i * (ANGLE360 / CLEAR_WAVE_NUM);
		Vector2 dir = Vector2(cos(angle), -sin(angle));

		waveMgr->Generate(player->Pos(), dir, CLEAR_WAVE_LIFETIME, WAVE_REFLECT_ONLY, 1, WAVE_SPEED, WAVE_DEST);
	}
	delete player;
	player = nullptr;
}
