#include "WaveMgr.h"
#include "Game_info.h"
#include "Debugging.h"
#include "Timer.h"

float WaveMgr::worldAttenuation;

WaveMgr::WaveMgr(ID3D11Device *device, IMapSpace* _mapSpace)
	:
	renderResource(device, CD3D11_TEXTURE2D_DESC(
		DXGI_FORMAT_R32G32B32A32_FLOAT ,
		MAP_WIDTH,
		MAP_HEIGHT, 1, 1,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS)),
	data(device, CD3D11_BUFFER_DESC(
		sizeof(Wave)*ARRAYSIZE(waves),
		D3D11_BIND_SHADER_RESOURCE,
		D3D11_USAGE_DYNAMIC,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
		sizeof(Wave))),
	cb_time(device),
	cb_dest(device),
	cb_wAtten(device)
{
	worldAttenuation = 0;

	mapSpace = _mapSpace;
	computeDraw.Create(device, L"WaveDrawCS.cso");
}

void WaveMgr::Update(ID3D11DeviceContext* dContext)
{
	UpdateWaves();

	UpdatePartition();

	UpdateRenderResource(dContext);
}

UINT WaveMgr::Generate(const Vector2 pos, const Vector2 dir, const float lifeTime, const UINT kind, const Vector3 waveColor, const float speed, const float rad)
{
	UINT idx = curWaveIdx;

	waves[curWaveIdx].pos = pos;
	waves[curWaveIdx].origin = pos;
	waves[curWaveIdx].dir = dir;

	waves[curWaveIdx].color = waveColor;

	waves[curWaveIdx].speed = speed;
	waves[curWaveIdx].rad = rad;
	waves[curWaveIdx].curIntensity = lifeTime;
	waves[curWaveIdx].maxIntensity = lifeTime;
	waves[curWaveIdx].kind = kind;

	if (++curWaveIdx == WAVE_MAX)
		curWaveIdx = 0;

	return idx;
}

void WaveMgr::Remove(UINT idx)
{
	waves[idx].kind = WAVE_INACTIVE;
}

const WavePartition& WaveMgr::Partition(UINT y, UINT x)
{
	return wavePartition[y][x];
}

const Wave& WaveMgr::WaveInfo(UINT idx)
{
	return waves[idx];
}

ID3D11ShaderResourceView*const* WaveMgr::Get()
{
	return renderResource.SRV();
}

void WaveMgr::UpdateWaves()
{
	for (int i = 0; i < WAVE_MAX; ++i) {

		if (waves[i].kind == WAVE_INACTIVE)
			continue;

		waves[i].curIntensity -= Timer::SPF();
		if (waves[i].curIntensity <= 0)
		{
			waves[i].kind = WAVE_INACTIVE;
			continue;
		}

		Vector2 newPos = waves[i].pos + (waves[i].dir * (waves[i].speed * Timer::SPF()));

		Vector2 reflectDir;
		if ((waves[i].kind == WAVE_NORMAL || waves[i].kind == WAVE_REFLECT_ONLY) && mapSpace->HitReflect(waves[i].pos, newPos, waves[i].dir, reflectDir))
		{
			waves[i].dir = reflectDir;
			waves[i].curIntensity -= 0.025f;
		}
		else
		{
			waves[i].pos = newPos;
		}
	}

}

void WaveMgr::UpdatePartition()
{
	ZeroMemory(wavePartition, sizeof(WavePartition)*(MAP_DIVISION_NUM_HEIGHT * MAP_DIVISION_NUM_WIDTH));

	const Vector2 divisionLength = Vector2(MAP_DIVISION_LENGTH_WIDTH, MAP_DIVISION_LENGTH_HEIGHT);
	for (int i = 0; i < WAVE_MAX; ++i) {

		if (waves[i].kind == WAVE_NORMAL)
		{

			int pX = waves[i].pos.x / divisionLength.x;
			int pY = waves[i].pos.y / divisionLength.y;

			int curIdx = wavePartition[pY][pX].size++;
			wavePartition[pY][pX].idx[curIdx] = i;
		}
	}
}

void WaveMgr::UpdateRenderResource(ID3D11DeviceContext* dContext)
{
	worldAttenuation += Timer::SPF()*0.66f;

	computeDraw.SetPipline(dContext);
	dContext->CSSetUnorderedAccessViews(0, 1, renderResource.UAV(), nullptr);
	data.Update(dContext, waves);
	dContext->CSSetShaderResources(0, 1, data.SRV());
	cb_dest.CSSetData(dContext, &(mapSpace->Dest()));
	cb_wAtten.CSSetData(dContext, &worldAttenuation, 1);
	dContext->Dispatch(1, 1, 1);
	dContext->CSSetUnorderedAccessViews(0, 1, renderResource.NullUAV(), nullptr);
}
