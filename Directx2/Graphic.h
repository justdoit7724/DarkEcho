#pragma once

#include "DX_info.h"

class Graphic
{
public:
	Graphic(HWND _hwnd);

	void Present();

	ID3D11Device* Device() {return device.Get();}
	ID3D11DeviceContext* DContext() {return dContext.Get();}

private:

	HWND hwnd;

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> dContext;
	ComPtr<IDXGISwapChain> swapchain;
	ComPtr<ID3D11RenderTargetView> rtv;
};

