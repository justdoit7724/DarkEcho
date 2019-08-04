#pragma once
#include "Game_info.h"

class IVector2;
class IVector4;
class Vector2;
struct ID3D11ShaderResourceView;
class MapSpace;
struct CB_Enemy;
struct WavePartition;
struct Wave;

class IScene {
public:
	virtual void ThrowStone(Vector2 pos, Vector2 dir) = 0;
	virtual void KillPlayer()=0;
	virtual bool PlayerPos(Vector2& pos)=0;

	virtual void ClearStage() = 0;
};

class INoise {
public:
	virtual UINT Generate(const Vector2 pos, const Vector2 dir, const float lifeTime, const UINT kind = WAVE_NORMAL, const Vector3 waveColor=Vector3(1,1,1), const float speed=WAVE_SPEED, const float rad = WAVE_RAD) = 0;

	virtual void Remove(UINT idx)=0;

	virtual const WavePartition& Partition(UINT y, UINT x) = 0;
	virtual const Wave& WaveInfo(UINT idx) = 0;
};

class IWaveSRV {
public:
	virtual ID3D11ShaderResourceView*const* Get() = 0;
};

class IMapSpace {
public:
	
	virtual IVector4 Dest()=0;


	virtual ID3D11ShaderResourceView*const* SpaceColorSRV() = 0;

	//edit
	virtual const Vector2 Move(Vector2 fromPt, Vector2 toPt, UINT& toSpace) = 0;
	virtual bool HitReflect(Vector2 fromPt, Vector2 toPt, Vector2 dir, Vector2& reflection) = 0;
};

class IEnemyInfo {
public:
	virtual CB_Enemy Info() = 0;
};