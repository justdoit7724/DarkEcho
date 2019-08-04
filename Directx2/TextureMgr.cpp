#include "TextureMgr.h"
#include <WICTextureLoader.h>

std::unordered_map<std::wstring, ID3D11ShaderResourceView*> TextureMgr::textures;

TextureMgr::~TextureMgr()
{
	for (auto& tex : textures) {

		delete tex.second;
	}
}

void TextureMgr::Load(ID3D11Device* device, std::wstring fileName)
{
	ID3D11ShaderResourceView* newSRV = nullptr;

	r_assert(
		DirectX::CreateWICTextureFromFile(
			device,
			(L"Data\\Texture\\" + fileName).c_str(),
			nullptr,
			&newSRV)
	);

	textures.insert(std::pair<std::wstring, ID3D11ShaderResourceView*>(fileName, newSRV));
}

ID3D11ShaderResourceView** TextureMgr::Get(std::wstring fileName)
{
	return &textures[fileName];
}


