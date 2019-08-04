#pragma once
#include "DX_info.h"
#include <unordered_map>

class TextureMgr
{
public:
	static void Load(ID3D11Device* device, std::wstring fileName);
	static ID3D11ShaderResourceView** Get(std::wstring fileName);

private:
	~TextureMgr();
	static std::unordered_map<std::wstring, ID3D11ShaderResourceView*> textures;
};

