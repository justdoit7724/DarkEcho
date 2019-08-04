#include "Stone.h"
#include "Camera.h"

Stone::Stone(ID3D11Device *device, Vector2 _pos, Vector2 _dir, INoise* wm, IMapSpace* _mapEvent)
	: geom(device), cb_transformation(device)
{
	noiseEvent = wm;
	mapEvent = _mapEvent;

	pos = _pos;
	dir = _dir;
	geom.dir = _dir;
	geom.scale = STONE_SCALE;

	stoneTailIdx = noiseEvent->Generate(_pos, _dir, STONE_LIFETIME + 0.25f, WAVE_KINEMATIC, Vector3(1,1,1), STONE_SPEED);

	D3D11_INPUT_ELEMENT_DESC ILayouts[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector2), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	shader.Create(device, L"StoneVS.cso", L"StonePS.cso", ILayouts, ARRAYSIZE(ILayouts));
}

Stone::~Stone()
{
	if(lifeLeft<=0)
		for (int i = 0; i < STONE_HIT_GROUND_WAVE_NUM; i++)
		{
			float curRad = (ANGLE360 / STONE_HIT_GROUND_WAVE_NUM) * i;
			Vector2 curDir = Vector2(cos(curRad), -sin(curRad));

			noiseEvent->Generate(pos, curDir, STONE_HIT_GROUND_WAVE_LIFETIME);
		}

	noiseEvent->Remove(stoneTailIdx);
}

void Stone::Update()
{
	lifeLeft -= Timer::SPF();
	kill = lifeLeft <= 0;

	Vector2 nextPos = pos + dir * STONE_SPEED * Timer::SPF();
	Vector2 temp1, temp2;
	if (mapEvent->HitReflect(pos, nextPos, temp1, temp2))
	{
		kill = true;

		for (int i = 0; i < STONE_HIT_WALL_WAVE_NUM; i++)
		{
			float curRad = ((ANGLE360 / STONE_HIT_WALL_WAVE_NUM) * i) + 0.11f; // tweak
			Vector2 curDir = Vector2(cos(curRad), -sin(curRad));

			noiseEvent->Generate(pos, curDir, STONE_HIT_WALL_WAVE_LIFETIME);
		}
	}
	else
	{
		pos = nextPos;
		geom.pos = nextPos;

		HitGround();
	}
}

void Stone::HitGround()
{
	if (isHitGround == false && STONE_LIFETIME-lifeLeft >= STONE_HIT_GROUND_TIME)
	{
		isHitGround = true;
		for (int i = 0; i < STONE_HIT_GROUND_WAVE_NUM; i++)
		{
			float curRad = (ANGLE360 / STONE_HIT_GROUND_WAVE_NUM) * i;
			Vector2 curDir = Vector2(cos(curRad), -sin(curRad));

			noiseEvent->Generate(pos, curDir, STONE_HIT_GROUND_WAVE_LIFETIME);
		}
	}
}


void Stone::Render(ID3D11DeviceContext *dContext)
{
	shader.SetPipline(dContext);

	cb_transformation.VSSetData(dContext, &Transformation(geom.WorldMatrix(), Camera::ViewMat()), 0);

	geom.Render(dContext);
}

bool Stone::Kill()
{


	return kill;
}

