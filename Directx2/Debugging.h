#pragma once
#include "DX_info.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <unordered_map>
#include "Resource.h"

class Quad;
class VPShader;

class Debugging
{
public:
	static void Init(ID3D11Device* _device, ID3D11DeviceContext* _ID3D11DeviceContext);

	static void Draw(int key, std::string tex, Vector2 _pos, bool onScreen=true, DirectX::XMVECTOR _color = DirectX::Colors::White, float _scale = 1.5f);
	static void Draw(int key, int tex, Vector2 _pos, bool onScreen = true, DirectX::XMVECTOR _color = DirectX::Colors::White, float _scale = 1.5f);
	static void Draw(int key, float tex, Vector2 _pos, bool onScreen = true, DirectX::XMVECTOR _color = DirectX::Colors::White, float _scale = 1.5f);
	static void Draw(int key, std::string title, Vector2 v, Vector2 _pos, bool onScreen = true, DirectX::XMVECTOR _color = DirectX::Colors::White, float _scale = 1.5f);
	static void Mark(UINT key, Vector2 pos, float scale = 0.01f, DirectX::XMVECTOR color = DirectX::Colors::Red, float slope = 0.0f);

	static void GetResourceData(ID3D11Buffer *buffer, void *allocatedV);
	static void GetResourceData(ID3D11Texture2D *texture, void *allocatedV, int typeSize);

	static void EnableGrid(Vector2 camPos, Vector2 interval, Vector2 hInterval);
	static void DisableGrid();

	static void Render();

private:
	Debugging() {};
	static std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	static std::unique_ptr<DirectX::SpriteFont> spriteFont;

	static ID3D11Device* device;
	static ID3D11DeviceContext* dContext;
	
	static float curTime;

	struct ScreenTextInfo {
		std::string tex;
		Vector2 pos;
		DirectX::XMVECTOR color;
		float scale;

		ScreenTextInfo() {}
	};
	struct MarkInfo {
		Quad* quad;
		DirectX::XMVECTOR color;

		MarkInfo() {}
		MarkInfo(Quad* _quad, DirectX::XMVECTOR _color) {
			quad = _quad;
			color = _color;
		}
	};
	static std::unordered_map<int, ScreenTextInfo> texts;
	static std::unordered_map<UINT, MarkInfo> marks;
	static VPShader shader;
	static std::unique_ptr<ConstantBuffer<Transformation>> cb_transformation;
	static std::unique_ptr<ConstantBuffer<DirectX::XMVECTOR>> cb_color;

	static std::unique_ptr<Quad> gridGeom;
	static VPShader gridShader;
	struct CB_Grid {
		Vector2 interval;
		Vector2 camPos;
		Vector2 hInterval;
		CB_Grid(Vector2 itv, Vector2 cPos, Vector2 hItv) {
			interval = itv;
			camPos = cPos;
			hInterval = hItv;
		}
	};
	static std::unique_ptr <ConstantBuffer<CB_Grid>> cb_grid;
	static bool enableGrid;
	static Vector2 gridInterval;
	static Vector2 gridCamPos;
	static Vector2 gridHInterval;
};

