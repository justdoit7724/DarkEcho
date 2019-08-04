#pragma once
#include "Debugging.h"
#include "Timer.h"
#include "Network.h"

#include "ScreenQuad.h"
#include "Player.h"
#include "Stone.h"
#include "WaveMgr.h"
#include "Map.h"
#include "EnemyMgr.h"

#define DEATH_WAVE_NUM 100
#define DEATH_WAVE_LIFETIME 10
#define CLEAR_WAVE_NUM 120
#define CLEAR_WAVE_LIFETIME 14

class Scene :public IScene
{
public:
	Scene(ID3D11Device* _device, ID3D11DeviceContext* _ID3D11DeviceContext);
	~Scene();

	void Update();
	void Render();

	void ThrowStone(Vector2 pos, Vector2 dir) override;
	void KillPlayer() override;
	bool PlayerPos(Vector2& pos) override;

	void ClearStage() override;

private:

	ScreenQuad* screen;
	Player* player;
	Stone* stone = nullptr;
	WaveMgr* waveMgr;
	Map* map;
	EnemyMgr* enemyMgr;

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> dContext;
};

