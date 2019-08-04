#include "Graphic.h"
#include "Game_info.h"

#pragma comment(lib, "d3d11.lib")

Graphic::Graphic(HWND _hwnd)
{
	hwnd = _hwnd;
	
	#pragma region Device / Context / SwapChain

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = SCREEN_WIDTH;
	scd.BufferDesc.Height = SCREEN_HEIGHT;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hwnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	r_assert(
		D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&scd,
		swapchain.GetAddressOf(),
		device.GetAddressOf(),
		NULL,
		dContext.GetAddressOf())
	);

	ComPtr<ID3D11Texture2D> backBuffer; 
	r_assert(
		swapchain->GetBuffer(
			0, 
			__uuidof(ID3D11Texture2D), 
			reinterpret_cast<void**>(backBuffer.GetAddressOf()))
	); 
	r_assert(
		device->CreateRenderTargetView(
			backBuffer.Get(), 
			0, 
			rtv.GetAddressOf())
	);
	dContext->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);

#pragma endregion

	#pragma region Viewport

	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = SCREEN_WIDTH;
	vp.Height = SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	dContext->RSSetViewports(1, &vp);
#pragma endregion
}

void Graphic::Present()
{
	swapchain->Present(0, 0);

	float bgColor[4] = { 0,0,0,0 };
	dContext->ClearRenderTargetView(rtv.Get(), bgColor);
}

	#pragma region Shaders

		/*std::wstring csPath = shaderFolder + L"CS.cso";
		Microsoft::WRL::ComPtr<ID3D10Blob> csBlob;
		R_CHECK(D3DReadFileToBlob(
			csPath.c_str(),
			csBlob.GetAddressOf()));
		R_CHECK(device->CreateComputeShader(
			csBlob->GetBufferPointer(),
			csBlob->GetBufferSize(),
			NULL,
			cs.GetAddressOf()));*/

	#pragma endregion

	#pragma region INPUT
/*
		D3D11_TEXTURE2D_DESC tex_desc;
		ZeroMemory(&tex_desc, sizeof(D3D11_TEXTURE2D_DESC));
		tex_desc.Width = width;
		tex_desc.Height = height;
		tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		tex_desc.Usage = D3D11_USAGE_IMMUTABLE;
		tex_desc.Format = DXGI_FORMAT_R32_FLOAT;
		tex_desc.MipLevels = 1;
		tex_desc.ArraySize = 1;
		tex_desc.CPUAccessFlags = 0;
		tex_desc.MiscFlags = 0;
		tex_desc.SampleDesc.Count = msCount;
		tex_desc.SampleDesc.Quality = msQuality;
		int resolution = height * width;
		static UINT* v1_float = new UINT[resolution];
		for (int h = 0; h < height; ++h)
		{
			for (int w = 0; w < width; ++w)
			{
				v1_float[w+h*width] = 1;
			}
		}

		Microsoft::WRL::ComPtr<ID3D11Texture2D> tex1;
		D3D11_SUBRESOURCE_DATA v1_data;
		ZeroMemory(&v1_data, sizeof(D3D11_SUBRESOURCE_DATA));
		v1_data.pSysMem = static_cast<const void*>(v1_float);
		v1_data.SysMemPitch = sizeof(UINT) * width;
		R_CHECK(device->CreateTexture2D(
			&tex_desc,
			&v1_data,
			tex1.GetAddressOf()));
		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
		ZeroMemory(&srv_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		srv_desc.Format = DXGI_FORMAT_R32_FLOAT;
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MipLevels = 1;
		srv_desc.Texture2D.MostDetailedMip = 0;
		R_CHECK(device->CreateShaderResourceView(
			tex1.Get(),
			&srv_desc,
			inputSRV1.GetAddressOf()));

		//-------------------------------------------------------------------INPUT 2-----------------------------------------------------------------------------------
		static UINT* v2_float = new UINT[resolution];
		for (int h = 0; h < height; ++h) {
			for (int w = 0; w < width; ++w)
			{
				v2_float[w + h * width] = 2;
			}
		}

		Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2;
		D3D11_SUBRESOURCE_DATA v2_data;
		ZeroMemory(&v2_data, sizeof(D3D11_SUBRESOURCE_DATA));
		v2_data.pSysMem = static_cast<const void*>(v2_float);
		v2_data.SysMemPitch = sizeof(UINT)*width;
		R_CHECK(device->CreateTexture2D(
			&tex_desc,
			&v2_data,
			tex2.GetAddressOf()));
		R_CHECK(device->CreateShaderResourceView(
			tex2.Get(),
			&srv_desc,
			inputSRV2.GetAddressOf()));
			*/
	#pragma endregion

	#pragma region OUTPUT

/*
		D3D11_TEXTURE2D_DESC ot_desc;
		ZeroMemory(&ot_desc, sizeof(D3D11_BUFFER_DESC));
		ot_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		ot_desc.Usage = D3D11_USAGE_DEFAULT;
		ot_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		ot_desc.Width = width;
		ot_desc.Height = height;
		ot_desc.SampleDesc.Count = msCount;
		ot_desc.SampleDesc.Quality = msQuality;
		ot_desc.CPUAccessFlags = 0;
		ot_desc.MiscFlags = 0;
		ot_desc.ArraySize = 1;
		ot_desc.MipLevels = 1;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> outputTex;
		static UINT* v3_float = new UINT[resolution];
		for (int h = 0; h < height; ++h)
		{
			for (int w = 0; w < width; ++w)
			{
				v3_float[w + h * width] = pow(2,16)-1;
			}
		}
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = static_cast<const void*>(v3_float);
		data.SysMemPitch = width * sizeof(UINT);

		R_CHECK(device->CreateTexture2D(
			&ot_desc,
			&data,
			outputTex.GetAddressOf()));

		D3D11_UNORDERED_ACCESS_VIEW_DESC outputUAV_desc;
		ZeroMemory(&outputUAV_desc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
		outputUAV_desc.Format = ot_desc.Format;
		outputUAV_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		outputUAV_desc.Texture2D.MipSlice = 0;
		R_CHECK(device->CreateUnorderedAccessView(
			outputTex.Get(),
			&outputUAV_desc,
			outputUAV.GetAddressOf()));

		D3D11_SHADER_RESOURCE_VIEW_DESC outputSRV_desc;
		ZeroMemory(&outputSRV_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		outputSRV_desc.Format = ot_desc.Format;
		outputSRV_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		outputSRV_desc.Texture2D.MostDetailedMip = 0;
		outputSRV_desc.Texture2D.MipLevels = 1;
		R_CHECK(device->CreateShaderResourceView(
			outputTex.Get(),
			&outputSRV_desc,
			outputSRV.GetAddressOf()));
			*/
		
#pragma endregion

	#pragma region STAGING

/*
		D3D11_TEXTURE2D_DESC rt_desc;
		ZeroMemory(&rt_desc, sizeof(D3D11_TEXTURE2D_DESC));
		rt_desc.Width = width;
		rt_desc.Height = height;
		rt_desc.Format = ot_desc.Format;
		rt_desc.Usage = D3D11_USAGE_STAGING;
		rt_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		rt_desc.SampleDesc.Count = msCount;
		rt_desc.SampleDesc.Quality = msQuality;
		rt_desc.ArraySize = 1;
		rt_desc.MipLevels = 1;
		rt_desc.MiscFlags = 0;
		R_CHECK(device->CreateTexture2D(
			&rt_desc,
			nullptr,
			readTex.GetAddressOf()));
		
	*/
	#pragma endregion

	#pragma region Sampler
	/*
	D3D11_SAMPLER_DESC sp_desc;
	ZeroMemory(&sp_desc, sizeof(D3D11_SAMPLER_DESC));
	sp_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sp_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sp_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sp_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sp_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sp_desc.MinLOD = 0;
	sp_desc.MaxLOD = D3D11_FLOAT32_MAX;
	R_CHECK(device->CreateSamplerState(&sp_desc, samplerState.GetAddressOf()));
	*/
	#pragma endregion

	


