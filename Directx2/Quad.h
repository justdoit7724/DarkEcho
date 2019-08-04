#pragma once
#include "DX_info.h"
#include "Transform.h"

class Quad : public Transform
{
public:
	Quad(ID3D11Device* device, bool ignoreTransform=false);

	void Render(ID3D11DeviceContext*);
	void SetEnable(bool e) {enabled = e;}

private:

	bool enabled = true;

	ComPtr<ID3D11Buffer> vertexBuffer;
};

