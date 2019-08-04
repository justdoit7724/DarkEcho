#pragma once
#include "DX_info.h"
#include <vector>
#include "Quad.h"
#include "Shader.h"
#include "Transform.h"
#include "BlendState.h"
#include "Network.h"
#include "Resource.h"
#include "Game_info.h"
#include "Map.h"

//PLAYER
#define FP_MAX 20

class Player : Transform
{
public:
	Player(ID3D11Device*, Vector2, IScene*, INoise*, IMapSpace*);
	~Player();

	void Update(ID3D11DeviceContext*);
	void Render(ID3D11DeviceContext*);

	Vector2 Pos() {return pos;}

private:

	void Move();
	void Throw();
	void Footprints();
	void StompNoise();
	void StepNoise(Vector2 stepPos);

	ID3D11Device* device;

	IScene* sceneEvent;
	INoise* noiseEvent;
	IMapSpace* mapEvent;

	#pragma region FootPrint
	struct Footprint {
		Quad* geom;
		std::wstring texName;
		float offset;
		float lifeLeft;

		Footprint() {}
		Footprint(Quad* _geom, float _scale, std::wstring _texName, float _offset) {
			geom = _geom;
			geom->scale = _scale;
			geom->SetEnable(true);
			texName = _texName;
			offset = _offset;
			lifeLeft = 0;
		}
	};

	Footprint FP_stand;
	std::vector<Footprint> FP_traces = std::vector<Footprint>(FP_MAX);
	int nextFPIdx = 0;
	VPShader shader;
	ComPtr<ID3D11SamplerState> samplerState;
	ConstantBuffer<Transformation> cb_transformation;
	ConstantBuffer<float> cb_fpTransparency;
	BlendState blending;
#pragma endregion

	#pragma region MOVEMENT
	enum PLAYER_MOVEMENT {
		PLAYER_MOVEMENT_STOP = 0,
		PLAYER_MOVEMENT_WALK,
		PLAYER_MOVEMENT_RUN,
		PLAYER_MOVEMENT_WALK_WATER,
		PLAYER_MOVEMENT_RUN_WATER
	}curMovement = PLAYER_MOVEMENT_STOP;
	float curSpeed=0;
	float curStride=0;

	Vector2 steeringDir;
	Vector2 prevStep;
#pragma endregion

	#pragma region Noise

	int curWaveNum = 0;
	float curWaveLifetime = 0;
	float curStompHold = 0;

#pragma endregion

	float curStoneTime=0;

	UINT curSpace = SPACE_NORMAL;
};

