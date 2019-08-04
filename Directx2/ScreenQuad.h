#pragma once
#include "DX_info.h"
#include "Quad.h"
#include "Shader.h"
#include "Network.h"
#include "Resource.h"
#include "WaveMgr.h"
#include "EnemyMgr.h"

class ScreenQuad
{
public:
	ScreenQuad(ID3D11Device*, IWaveSRV*, IMapSpace*, IEnemyInfo*);

	void Update();
	void Render(ID3D11DeviceContext*);

private:

	Quad screen;
	VPShader shader;


	IWaveSRV* waveSRV;
	IMapSpace* mapSpace;
	IEnemyInfo* enemyEvent;

	ConstantBuffer<Transformation> cb_transformation;
	ConstantBuffer<Vector2> cb_pos;
	ConstantBuffer<Wave> cb_waveData;
	ConstantBuffer<float> cb_wAtten;
	ConstantBuffer<CB_Enemy> cb_enemies;
};

