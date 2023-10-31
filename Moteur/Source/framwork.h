// This file is used to precompile the header files that are used in the project.

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "d3dx12.h"

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <wrl.h>
#include <shellapi.h>

using namespace DirectX;

struct Vertex
{
    XMFLOAT3 m_position;
    XMFLOAT4 m_color;
};

struct ConstantBufferData
{
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX View;
	//DirectX::XMMATRIX Projection;
};