#pragma once
#include "DX_info.h"
#include "Transform.h"
#include "Shader.h"
#include "Quad.h"
#include "Resource.h"
#include "Network.h"

//stone
#define STONE_LIFETIME 1.25f
#define STONE_SPEED 650
#define STONE_SCALE 0.01f
#define STONE_HIT_GROUND_TIME 0.8f
#define STONE_HIT_GROUND_WAVE_NUM 15
#define STONE_HIT_GROUND_WAVE_LIFETIME 2.5f
#define STONE_HIT_WALL_WAVE_NUM 22
#define STONE_HIT_WALL_WAVE_LIFETIME 3.5f

class Stone :
	public Transform
{
public:
	Stone(ID3D11Device*, Vector2, Vector2, INoise*, IMapSpace*);
	~Stone();

	void Update();
	void Render(ID3D11DeviceContext*);

	bool Kill();

private:

	void HitGround();

	bool isHitGround = false;
	bool kill = false;
	float lifeLeft = STONE_LIFETIME;

	INoise* noiseEvent;
	IMapSpace* mapEvent;

	Quad geom;
	VPShader shader;
	ConstantBuffer<Transformation> cb_transformation;

	UINT stoneTailIdx=-1;
};

