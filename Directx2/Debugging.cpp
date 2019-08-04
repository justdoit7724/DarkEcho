#include "Debugging.h"
#include <string>
#include "Quad.h"
#include "Shader.h"
#include "Camera.h"
#include "Game_info.h"

ID3D11Device* Debugging::device;
ID3D11DeviceContext* Debugging::dContext;
std::unordered_map<int, Debugging::ScreenTextInfo> Debugging::texts; 
std::unordered_map<UINT, Debugging::MarkInfo> Debugging::marks;
VPShader Debugging::shader;
std::unique_ptr<ConstantBuffer<Transformation>> Debugging::cb_transformation;
std::unique_ptr<ConstantBuffer<DirectX::XMVECTOR>> Debugging::cb_color;
std::unique_ptr<DirectX::SpriteBatch> Debugging::spriteBatch;
std::unique_ptr<DirectX::SpriteFont> Debugging::spriteFont;
float Debugging::curTime;

std::unique_ptr < Quad> Debugging::gridGeom;
VPShader Debugging::gridShader;
std::unique_ptr < ConstantBuffer<Debugging::CB_Grid>> Debugging::cb_grid;

Vector2 Debugging::gridCamPos;
Vector2 Debugging::gridInterval; 
bool Debugging::enableGrid;
Vector2 Debugging::gridHInterval;

void Debugging::Init(ID3D11Device* _device, ID3D11DeviceContext* _dContext)
{
	device=_device;
	dContext=_dContext;

	spriteBatch = std::make_unique<DirectX::SpriteBatch>(_dContext);
	spriteFont = std::make_unique<DirectX::SpriteFont>(_device, L"Data\\Font\\Font.spritefont");

	D3D11_INPUT_ELEMENT_DESC layouts[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	
	shader.Create(_device, L"MarkVS.cso", L"MarkPS.cso", layouts, ARRAYSIZE(layouts));
	cb_transformation.reset(new ConstantBuffer<Transformation>(_device));
	cb_color.reset(new ConstantBuffer<DirectX::XMVECTOR>(_device));

	D3D11_INPUT_ELEMENT_DESC gridLayouts[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector2), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	gridGeom.reset(new Quad(device, true));
	gridShader.Create(device, L"GridVS.cso", L"GridPS.cso", gridLayouts, 2);
	cb_grid.reset(new ConstantBuffer<CB_Grid>(device));
	enableGrid = false;
	gridInterval = 96;
	gridCamPos = 0;
	gridHInterval = 960;
}

void Debugging::Draw(int key, std::string tex, Vector2 _pos, bool onScreen, DirectX::XMVECTOR _color, float _scale)
{
	if (!onScreen) {
		Vector2 tl = Camera::Pos() + Vector2(-SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		_pos -= tl;
		_pos.y *= -1;
	}

	if (texts.find(key) == texts.end())
	{
		ScreenTextInfo newText;
		newText.tex = tex;
		newText.pos = _pos;
		newText.color = _color;
		newText.scale = _scale;

		texts.insert(std::pair<int, ScreenTextInfo>(key, newText));
	}
	else {
		texts[key].tex = tex;
		texts[key].pos = _pos;
		texts[key].color = _color;
		texts[key].scale = _scale;
	}
}
void Debugging::Draw(int key, int tex, Vector2 _pos, bool onScreen, DirectX::XMVECTOR _color, float _scale)
{
	if (!onScreen) {
		Vector2 tl = Camera::Pos() + Vector2(-SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		_pos -= tl;
		_pos.y *= -1;
	}
	if (texts.find(key) == texts.end())
	{
		ScreenTextInfo newText;
		newText.tex = std::to_string(tex);
		newText.pos = _pos;
		newText.color = _color;
		newText.scale = _scale;

		texts.insert(std::pair<int, ScreenTextInfo>(key, newText));
	}
	else {
		texts[key].tex = std::to_string(tex);
		texts[key].pos = _pos;
		texts[key].color = _color;
		texts[key].scale = _scale;
	}
}
void Debugging::Draw(int key, float tex, Vector2 _pos, bool onScreen, DirectX::XMVECTOR _color, float _scale)
{
	if (!onScreen) {
		Vector2 tl = Camera::Pos() + Vector2(-SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		_pos -= tl;
		_pos.y *= -1;
	}
	if (texts.find(key) == texts.end())
	{
		ScreenTextInfo newText;
		newText.tex = std::to_string(tex);
		newText.pos = _pos;
		newText.color = _color;
		newText.scale = _scale;

		texts.insert(std::pair<int, ScreenTextInfo>(key, newText));
	}
	else {
		texts[key].tex = std::to_string(tex);
		texts[key].pos = _pos;
		texts[key].color = _color;
		texts[key].scale = _scale;
	}
}
void Debugging::Draw(int key, std::string title, Vector2 v, Vector2 _pos, bool onScreen, DirectX::XMVECTOR _color, float _scale)
{
	if (!onScreen) {
		Vector2 tl = Camera::Pos() + Vector2(-SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		_pos -= tl;
		_pos.y *= -1;
	}
	if (texts.find(key) == texts.end())
	{
		ScreenTextInfo newText;
		newText.tex = title + "("+std::to_string(v.x) + ", " + std::to_string(v.y) +")";
		newText.pos = _pos;
		newText.color = _color;
		newText.scale = _scale;

		texts.insert(std::pair<int, ScreenTextInfo>(key, newText));
	}
	else {
		texts[key].tex = title + " = (" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
		texts[key].pos = _pos;
		texts[key].color = _color;
		texts[key].scale = _scale;
	}
}
void Debugging::Mark(UINT key, Vector2 pos, float scale, DirectX::XMVECTOR color, float slope)
{
	if (marks.find(key) == marks.end())
	{
		Quad* newGeom = new Quad(device);
		newGeom->pos = pos;
		newGeom->scale = scale;
		newGeom->dir = FORWARD * Matrix::RotationZ(-slope);

		marks.insert(std::pair<UINT, MarkInfo>(key, MarkInfo(newGeom, color)));
	}
	else {
		marks[key].quad->pos = pos;
	}
}

void Debugging::GetResourceData(ID3D11Buffer * buffer, void * allocatedData)
{
	D3D11_BUFFER_DESC bufferDesc;
	buffer->GetDesc(&bufferDesc);

	ComPtr<ID3D11Buffer> stagingBuffer;
	D3D11_BUFFER_DESC stagingDesc;
	stagingDesc.BindFlags = 0;
	stagingDesc.Usage = D3D11_USAGE_STAGING;
	stagingDesc.ByteWidth = bufferDesc.ByteWidth;
	stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	stagingDesc.MiscFlags = 0;
	stagingDesc.StructureByteStride = bufferDesc.StructureByteStride;
	r_assert(
		device->CreateBuffer(
			&stagingDesc,
			nullptr,
			stagingBuffer.GetAddressOf())
	);

	dContext->CopyResource(stagingBuffer.Get(), buffer);
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	r_assert(
		dContext->Map(stagingBuffer.Get(), 0, D3D11_MAP_READ, 0, &mappedResource)
	);
	ZeroMemory(allocatedData, bufferDesc.ByteWidth);
	CopyMemory(allocatedData, mappedResource.pData, bufferDesc.ByteWidth);
	dContext->Unmap(buffer, 0);
}
void Debugging::GetResourceData(ID3D11Texture2D * texture, void * allocatedData, int typeSize)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	texture->GetDesc(&textureDesc);

	ComPtr<ID3D11Texture2D> stagingTexture;
	D3D11_TEXTURE2D_DESC stagingDesc;
	stagingDesc.BindFlags = 0;
	stagingDesc.Usage = D3D11_USAGE_STAGING;
	stagingDesc.Format = textureDesc.Format;
	stagingDesc.Width = textureDesc.Width;
	stagingDesc.Height = textureDesc.Height;
	stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	stagingDesc.MiscFlags = 0;
	stagingDesc.SampleDesc.Count = 1;
	stagingDesc.SampleDesc.Quality = 0;
	stagingDesc.ArraySize = 1;
	stagingDesc.MipLevels = 1;

	r_assert(
		device->CreateTexture2D(
			&stagingDesc,
			nullptr,
			stagingTexture.GetAddressOf())
	);

	dContext->CopyResource(stagingTexture.Get(), texture);
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	r_assert(
		dContext->Map(stagingTexture.Get(), 0, D3D11_MAP_READ, 0, &mappedResource)
	);
	int totalByte = textureDesc.Width*textureDesc.Height*typeSize;
	ZeroMemory(allocatedData, totalByte);
	CopyMemory(allocatedData, mappedResource.pData, totalByte);
	dContext->Unmap(stagingTexture.Get(), 0);
}

void Debugging::EnableGrid(Vector2 camPos, Vector2 interval, Vector2 hInterval)
{
	enableGrid = true;
	gridCamPos = camPos;
	gridInterval = interval;
	gridHInterval = hInterval;
}

void Debugging::DisableGrid()
{
	enableGrid = false;
}

void Debugging::Render()
{
	if (enableGrid)
	{
		gridShader.SetPipline(dContext);
		cb_grid->PSSetData(dContext, &CB_Grid(gridInterval, gridCamPos, gridHInterval));
		gridGeom->Render(dContext);
	}

	spriteBatch->Begin();

	for (auto& text : texts)
	{
		spriteFont->DrawString(
			spriteBatch.get(),
			text.second.tex.c_str(),
			DirectX::XMFLOAT2(text.second.pos.x, text.second.pos.y),
			text.second.color,
			0.0f,
			DirectX::XMFLOAT2(0,0),
			DirectX::XMFLOAT2(text.second.scale, text.second.scale)
			);
	}

	spriteBatch->End();

	// clear vector for every 1 sec
	curTime += Timer::SPF();
	if (curTime >= 1)
		texts.clear();

	shader.SetPipline(dContext);
	for (auto mark : marks) {
		cb_transformation->VSSetData(dContext, &Transformation(mark.second.quad->WorldMatrix(), Camera::ViewMat()), 0);
		cb_color->PSSetData(dContext, &(mark.second.color), 0);
		mark.second.quad->Render(dContext);
	}
}
