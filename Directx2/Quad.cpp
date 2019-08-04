#include "Quad.h"
#include "Game_info.h"

Quad::Quad(ID3D11Device * device, bool ignoreTransform)
{
	#pragma region Vertex Buffer
	
	float wRad = SCREEN_WIDTH / 2.0f;
	float hRad = SCREEN_HEIGHT / 2.0f;
	if (ignoreTransform)
	{
		wRad = 1;
		hRad = 1;
	}
	Vertex vertice[] = {
		Vertex(Vector2(-wRad, -hRad), Vector2(0, 1)),
		Vertex(Vector2(-wRad, hRad), Vector2(0,0)),
		Vertex(Vector2(wRad, hRad), Vector2(1, 0)),
		Vertex(Vector2(-wRad, -hRad), Vector2(0, 1)),
		Vertex(Vector2(wRad, hRad), Vector2(1, 0)),
		Vertex(Vector2(wRad, -hRad), Vector2(1, 1))
	};
	D3D11_BUFFER_DESC vb_desc;
	vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vb_desc.Usage = D3D11_USAGE_IMMUTABLE;
	vb_desc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertice);
	vb_desc.CPUAccessFlags = 0;
	vb_desc.MiscFlags = 0;
	vb_desc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vb_data;
	vb_data.pSysMem = vertice;
	r_assert(
		device->CreateBuffer(
			&vb_desc,
			&vb_data,
			vertexBuffer.GetAddressOf())
	);

#pragma endregion
}

void Quad::Render(ID3D11DeviceContext *ID3D11DeviceContext)
{
	if (!enabled)
		return;

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11DeviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	ID3D11DeviceContext->Draw(6, 0);
}
