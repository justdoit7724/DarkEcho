#include "Player.h"
#include <WICTextureLoader.h>
#include "Keyboard.h"
#include "Camera.h"

#define FP_SCALE 0.025f
#define FP_LIFE_TIME 7.5f
#define RUN_SPEED 145
#define RUN_STRIDE 70
#define RUN_WATER_SPEED 75
#define RUN_WATER_STRIDE 60
#define WALK_SPEED 50
#define WALK_STRIDE 35
#define WALK_WATER_SPEED 30
#define WALK_WATER_STRIDE 25
#define HALF_STEP_DIST_FROM_CENTER 10.0f
#define STOMP_HOLD_MAX 0.9f
#define STOMP_LIFETIME_MAX 5.5f
#define STOMP_NUM_MAX 80
#define STOMP_NUM_MIN 35
#define STEP_RUN_NUM 17
#define STEP_RUN_WATER_NUM 25
#define STEP_WALK_NUM 12
#define STEP_WALK_WATER_NUM 20
#define STEP_RUN_LIFETIME 3.0f
#define STEP_RUN_WATER_LIFETIME 4.5f
#define STEP_WALK_LIFETIME 0.225f
#define STEP_WALK_WATER_LIFETIME 2.0f
#define STONE_DELAY 1.5f

Player::Player(ID3D11Device *_device, Vector2 start, IScene* _stoneEvent, INoise* _noiseEvent, IMapSpace* _mapEvent)
	: cb_transformation(_device), cb_fpTransparency(_device), prevStep(pos), blending(_device)
{
	device = _device;
	pos = start;
	prevStep = start;

	sceneEvent = _stoneEvent;
	noiseEvent = _noiseEvent;
	mapEvent = _mapEvent;

	steeringDir = dir;

	curSpeed = WALK_SPEED;
	curStride = WALK_STRIDE;
	curSpace = SPACE_NORMAL;

	Keyboard::Add('A');
	Keyboard::Add('D');
	Keyboard::Add('W');
	Keyboard::Add('S');
	Keyboard::Add('L');
	Keyboard::Add('K');
	Keyboard::Add(' ');

	FP_stand = Footprint(new Quad(device),FP_SCALE, L"footprint_both.png",0);
	FP_stand.geom->pos = pos;
	FP_stand.geom->dir = dir;

	for (int i = 0; i < FP_MAX; ++i) {

		FP_traces[i] = Footprint(
			new Quad(device),
			FP_SCALE,
			(i & 1) ? L"footprint_right.png" : L"footprint_left.png",
			HALF_STEP_DIST_FROM_CENTER * ((i & 1) * 2 - 1));
	}

	TextureMgr::Load(device, L"footprint_both.png");
	TextureMgr::Load(device, L"footprint_left.png");
	TextureMgr::Load(device, L"footprint_right.png");

	D3D11_INPUT_ELEMENT_DESC ILayouts[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector2), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	shader.Create(device, L"FootprintVS.cso", L"FootprintPS.cso", ILayouts, ARRAYSIZE(ILayouts));


	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	r_assert(device->CreateSamplerState(&desc, samplerState.GetAddressOf())
	);
}

Player::~Player()
{
	delete FP_stand.geom;
	for (Footprint& fp : FP_traces) {
		delete fp.geom;
	}
}

void Player::Update(ID3D11DeviceContext* dContext)
{
	Move();
	if (curSpace == SPACE_TRAP)
	{
		sceneEvent->KillPlayer();
		return;
	}
	else if (curSpace == SPACE_DEST)
	{
		sceneEvent->ClearStage();
		return;
	}

	Throw();
	Footprints();
	StompNoise();
}

void Player::Render(ID3D11DeviceContext *dContext)
{
	shader.SetPipline(dContext);
	blending.SetPipeline(dContext);

	dContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());


	for (Footprint& fp : FP_traces) {

		auto trans = Transformation(fp.geom->WorldMatrix(), Camera::ViewMat());
		cb_transformation.VSSetData(dContext, &trans, 0);
		float transparentE = fp.lifeLeft / FP_LIFE_TIME;
		cb_fpTransparency.PSSetData(dContext, &transparentE, 0);
		dContext->PSSetShaderResources(0, 1, TextureMgr::Get(fp.texName));
		fp.geom->Render(dContext);
	}

	auto trans = Transformation(FP_stand.geom->WorldMatrix(), Camera::ViewMat());
	cb_transformation.VSSetData(dContext, &trans, 0);
	float opaqueE = 1.0f;
	cb_fpTransparency.PSSetData(dContext, &opaqueE, 0);

	dContext->PSSetShaderResources(0, 1, TextureMgr::Get(FP_stand.texName));
	FP_stand.geom->Render(dContext);
}

void Player::Move()
{
	if (Keyboard::IsPressing("ASDW"))
	{
		curStompHold = 0;
	}
	else
	{
		curMovement = PLAYER_MOVEMENT_STOP;
	}

	Vector2 movement=0;
	if (Keyboard::IsPressing('A')) 
	{
		movement += Vector2(-1, 0);
		steeringDir += Vector2(-1, 0);
	}
	else if (Keyboard::IsPressing('D'))
	{
		movement += Vector2(1, 0);
		steeringDir += Vector2(1, 0);
	}
	if (Keyboard::IsPressing('W'))
	{
		movement += Vector2(0, 1);
		steeringDir += Vector2(0, 1);
	}
	else if (Keyboard::IsPressing('S'))
	{
		movement += Vector2(0, -1);
		steeringDir += Vector2(0, -1);
	}

	if (movement == 0)
		curMovement = PLAYER_MOVEMENT_STOP;
	else
	{
		curStompHold = 0;

		movement = Vector2::Normalize(movement);
		pos = mapEvent->Move(pos, pos + movement * curSpeed * Timer::SPF(), curSpace);
		switch (curSpace)
		{
		case SPACE_NORMAL:
			if (Keyboard::IsPressing('K'))
			{
				curSpeed = WALK_SPEED;
				curStride = WALK_STRIDE;
				curWaveNum = STEP_WALK_NUM;
				curWaveLifetime = STEP_WALK_LIFETIME;
				curMovement = PLAYER_MOVEMENT_WALK;
			}
			else {
				curSpeed = RUN_SPEED;
				curStride = RUN_STRIDE;
				curWaveNum = STEP_RUN_NUM;
				curWaveLifetime = STEP_RUN_LIFETIME;
				curMovement = PLAYER_MOVEMENT_RUN;
			}
			break;
		case SPACE_WATER:
			if (Keyboard::IsPressing('K'))
			{
				curSpeed = WALK_WATER_SPEED;
				curStride = WALK_WATER_STRIDE;
				curWaveNum = STEP_WALK_WATER_NUM;
				curWaveLifetime = STEP_WALK_WATER_LIFETIME;
				curMovement = PLAYER_MOVEMENT_WALK_WATER;
			}
			else
			{
				curSpeed = RUN_WATER_SPEED;
				curStride = RUN_WATER_STRIDE;
				curWaveNum = STEP_RUN_WATER_NUM;
				curWaveLifetime = STEP_RUN_WATER_LIFETIME;
				curMovement = PLAYER_MOVEMENT_RUN_WATER;
			}
			break;
		case SPACE_DEST:
		case SPACE_TRAP:
			return;
		}

		dir += steeringDir;
		dir = Vector2::Normalize(dir);

		FP_stand.geom->pos = pos;
		FP_stand.geom->dir = dir;

		Camera::Update(pos);
	}

	steeringDir *= 0.9925f;
}

void Player::Throw()
{
	curStoneTime += Timer::SPF();

	if (Keyboard::IsPressing('L') && curStoneTime >= STONE_DELAY)
	{
		curStoneTime = 0;
		sceneEvent->ThrowStone(pos, dir);
	}
}

void Player::Footprints()
{
	FP_stand.geom->SetEnable(curMovement == PLAYER_MOVEMENT_STOP);

	if (Vector2::Length(pos - prevStep) >= curStride) {

		prevStep = pos;

		FP_traces[nextFPIdx].geom->pos = pos;
		FP_traces[nextFPIdx].geom->dir = dir;
		FP_traces[nextFPIdx].lifeLeft = FP_LIFE_TIME;

		Vector2 localRight = Vector2::Cross(dir);
		StepNoise(localRight * FP_traces[nextFPIdx].offset + pos);

		nextFPIdx = (1 + nextFPIdx) % FP_MAX;
	}

	int i = 0;
	for (Footprint& fp : FP_traces) {

		fp.lifeLeft -= Timer::SPF();
	}

}

float Lerp(float a, float b, float t) {

	return a + (b - a)*t;
}
void Player::StompNoise()
{
	if (Keyboard::IsPressing(' '))
		curStompHold += Timer::SPF();
	else if(curStompHold >= 0.1f)
	{
		float e = min(curStompHold / STOMP_HOLD_MAX,1);
		float lifeTime = e*STOMP_LIFETIME_MAX;

		int curStompNum = Lerp(STOMP_NUM_MIN, STOMP_NUM_MAX, e);
		for (int i = 0; i < curStompNum; i++)
		{
			float curRad = (ANGLE360 / curStompNum) * i;
			Vector2 curDir = Vector2(cos(curRad), -sin(curRad));

			noiseEvent->Generate(pos, curDir, lifeTime);
		}

		curStompHold = 0;
	}
}
void Player::StepNoise(Vector2 stepPos)
{
	for (int i = 0; i < curWaveNum; i++)
	{
		float curRad = (ANGLE360 / curWaveNum) * i;
		Vector2 curDir = Vector2(cos(curRad), -sin(curRad));

		noiseEvent->Generate(stepPos, curDir, curWaveLifetime);
	}
}

