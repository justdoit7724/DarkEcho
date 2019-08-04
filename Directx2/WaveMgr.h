#pragma once
#include "DX_info.h"
#include "Game_info.h"
#include "Resource.h"
#include "Shader.h"
#include "Network.h"
#include <vector>



//wave
#define WAVE_MAX 512

// keep sync same struct in shader !!!
struct Wave {
	Vector2 pos;
	Vector2 origin;
	Vector2 dir;
	Vector3 color;
	
	float speed;
	float rad;
	float curIntensity;
	float maxIntensity;
	
	int kind;

	Wave() {

		kind = WAVE_INACTIVE;
	}
};
struct WavePartition {
	UINT idx[160];
	UINT size;

	WavePartition() {}
	WavePartition(const WavePartition& s) {}

};

class WaveMgr : public INoise, public IWaveSRV
{
public:

	WaveMgr(ID3D11Device*, IMapSpace*);
	void Update(ID3D11DeviceContext*);

	UINT Generate(const Vector2 pos, const Vector2 dir, const float lifeTime, const UINT kind, const Vector3 waveColor=Vector3(1,1,1), const float speed=WAVE_SPEED, const float rad = WAVE_RAD) override;
	void Remove(UINT idx) override;
	const WavePartition& Partition(UINT y, UINT x) override;
	const Wave& WaveInfo(UINT idx) override;
	ID3D11ShaderResourceView*const* Get() override; 

	static float WorldAttenuation() {return worldAttenuation;}

private:

	void UpdateWaves();
	void UpdatePartition();
	void UpdateRenderResource(ID3D11DeviceContext* dContext);

	static float worldAttenuation;

	UINT curWaveIdx = 0;
	Wave waves[WAVE_MAX];

	WavePartition wavePartition[MAP_DIVISION_NUM_HEIGHT][MAP_DIVISION_NUM_WIDTH];

	IMapSpace* mapSpace;

	CShader computeDraw;
	Texture2D<Vector4> renderResource;

	Buffer<Wave> data;
	ConstantBuffer<float> cb_wAtten;

	struct SPF {
		float spf;
		Vector3 padding;

		SPF(float _spf) {
			spf = _spf;
		}
	};
	ConstantBuffer<SPF> cb_time;
	
	ConstantBuffer<IVector4> cb_dest;

};






	/*struct Wave_SPartitio1n {
		IVector4 waveIdx[PARTITION_RESOLUTION];

		IVector4& Get(float x, float y)
		{
			IVector2 idx = IVector2(x / PARTITION_WIDTH, y / PARTITION_HEIGHT);

			return waveIdx[idx.x + idx.y*PARTITION_DIVISION];
		}

	}partition;*/
