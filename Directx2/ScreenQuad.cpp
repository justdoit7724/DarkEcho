#include "ScreenQuad.h"
#include "Camera.h"
#include "Game_info.h"
#include "WaveMgr.h"

ScreenQuad::ScreenQuad(ID3D11Device * device, IWaveSRV* _waveSRV, IMapSpace* _mapEvent, IEnemyInfo* _enemyEvent)
	:
	screen(device), 
	cb_transformation(device), cb_pos(device),
	cb_waveData(device),
	cb_wAtten(device),
	cb_enemies(device)
{
	screen.pos = 0;
	screen.scale = 1;

	waveSRV = _waveSRV;
	mapSpace = _mapEvent;
	enemyEvent = _enemyEvent;

	D3D11_INPUT_ELEMENT_DESC ILayouts[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector2), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	
	shader.Create(device, L"ScreenVS.cso", L"ScreenPS.cso", ILayouts, ARRAYSIZE(ILayouts));

}

void ScreenQuad::Update()
{
	screen.pos = Camera::Pos();
}

void ScreenQuad::Render(ID3D11DeviceContext* dContext)
{
	shader.SetPipline(dContext);
	cb_transformation.VSSetData(dContext, &Transformation(screen.WorldMatrix(), Camera::ViewMat()));
	cb_pos.PSSetData(dContext, &(screen.pos));
	float wAtten = WaveMgr::WorldAttenuation();
	cb_wAtten.PSSetData(dContext, &wAtten, 1);
	CB_Enemy temp = enemyEvent->Info();
	cb_enemies.PSSetData(dContext, &(enemyEvent->Info()), 2);

	dContext->PSSetShaderResources(0, 1, waveSRV->Get());
	dContext->PSSetShaderResources(1, 1, mapSpace->SpaceColorSRV());


	screen.Render(dContext);
}
