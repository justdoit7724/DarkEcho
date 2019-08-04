#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <assert.h>
#include "CustomFormat.h"
#include "TextureMgr.h"
#include "Math.h"
#include "Timer.h"

using namespace Microsoft::WRL;

static const wchar_t* ShaderPath() {
#ifdef _DEBUG
#ifdef _WIN64
	return L"..\\x64\\Debug\\";
#else
	return L"..\\Debug\\";
#endif
#else
#ifdef _WIN64
	return L"..\\x64\\Release\\";
#else
	return L"..\\Release\\";
#endif
#endif
}

#define MB(s) MessageBox(0,s,0,0)

#define r_assert(r)	assert(SUCCEEDED(r))