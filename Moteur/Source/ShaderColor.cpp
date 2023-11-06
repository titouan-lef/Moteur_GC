#include "ShaderColor.h"
#include "MyException.h"

ID3D12RootSignature* ShaderColor::s_rootSignature = ShaderColor::CreateRootSignature();

ShaderColor::ShaderColor(ConstantBufferData* cbd) : Shader(Type::color, s_rootSignature)
{

    Light initialLightData = {XMFLOAT3(-0.5f, 0.0f, 0.0f),XMFLOAT3(10000.5f, 1.0f, 1.0f),XMFLOAT3(0.5f, 0.0f, 0.0f) };
    m_constBufferLight = new ConstantBufferLight(cbd, initialLightData);
    m_constBuffer = new ConstantBuffer(cbd);

    m_descriptorHeaps = { m_constBuffer->m_cbvHeapDesc };
}

ShaderColor::~ShaderColor()
{
    delete m_constBufferLight;
}

void ShaderColor::SetGraphicsRoot()
{
    m_constBuffer->SetGraphicsRoot();
    D3D12_GPU_DESCRIPTOR_HANDLE lightGpuHandle = m_constBufferLight->m_cbvHeapDesc->GetGPUDescriptorHandleForHeapStart();

    // Supposant que Engine::CmdList est l'ID3D12GraphicsCommandList* que vous utilisez pour le rendu.
    Engine::CmdList->SetGraphicsRootDescriptorTable(1, lightGpuHandle);
}


ID3D12RootSignature* ShaderColor::CreateRootSignature()
{
    /*
    * CD3DX12_DESCRIPTOR_RANGE(a, b, c) :
    * * a : type de descripteur
    * * b : nombre de descripteur
    * * c : regsitre du shader
    */
    CD3DX12_DESCRIPTOR_RANGE ranges[2];
    ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0); // cbuffer ConstantBufferData : register(b0)
    ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1); // cbuffer LightBuffer : register(b1)

    CD3DX12_ROOT_PARAMETER rootParameters[2];
    rootParameters[0].InitAsDescriptorTable(1, &ranges[0]);
    rootParameters[1].InitAsDescriptorTable(1, &ranges[1]);

    return Shader::CreateRootSignature(_countof(rootParameters), rootParameters);
}