#pragma once
#include "DX_info.h"

class VPShader
{
public:
	void Create(ID3D11Device *device, const std::wstring VSfileName, const std::wstring PSfileName, D3D11_INPUT_ELEMENT_DESC* layoutDesc, int layoutNum);

	void SetPipline(ID3D11DeviceContext*);

private:
	ComPtr<ID3D11InputLayout> iLayout = nullptr;
	ComPtr<ID3D11VertexShader> vs = nullptr;
	ComPtr<ID3D11PixelShader> ps = nullptr;
};


class CShader {
public:
	void Create(ID3D11Device *device, const std::wstring CSfileName);

	void SetPipline(ID3D11DeviceContext*);

private:
	ComPtr<ID3D11ComputeShader> cs = nullptr;
};
