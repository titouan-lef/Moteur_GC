#include "Texture.h"
#include "Engine.h"
#include "MyException.h"
#include "DDSTextureLoader.h"


Texture::Texture()
{
}

Texture::~Texture()
{
}

void Texture::CreateTexture(UINT id, std::wstring fileName, ID3D12DescriptorHeap* cbvSrvUavHeap, UINT rtvDescriptorSize)
{
    m_id = id;
    m_fileName = L"Source/" + fileName + L".dds";

    CD3DX12_CPU_DESCRIPTOR_HANDLE gpu(cbvSrvUavHeap->GetCPUDescriptorHandleForHeapStart());// Récupération de l'emplacement prévu pour la "surface de dessin" (= render target) 0
    gpu.Offset(id, rtvDescriptorSize);

    // Décrit la Texture2D
    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.MipLevels = 1;
    textureDesc.Format = DXGI_FORMAT_BC1_UNORM;
    textureDesc.Width = 472;
    textureDesc.Height = 472;
    textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    textureDesc.DepthOrArraySize = 1;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

    // Describe and create a SRV for the texture.
    m_srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    m_srvDesc.Format = textureDesc.Format;
    m_srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    m_srvDesc.Texture2D.MipLevels = 1;

    GFX_THROW_INFO_ONLY(Engine::GetInstance()->CmdAllocator->Reset());
    GFX_THROW_INFO_ONLY(Engine::GetInstance()->CmdList->Reset(Engine::GetInstance()->CmdAllocator, nullptr));

    DirectX::CreateDDSTextureFromFile12(
        Engine::GetInstance()->Device, Engine::GetInstance()->CmdList,
        m_fileName.c_str(),
        m_resource, m_uploadHeap);

    m_gpu = gpu;
}

void Texture::CreateShaderResourceView()
{
    Engine::GetInstance()->Device->CreateShaderResourceView(m_resource.Get(), &m_srvDesc, m_gpu);// Créez le SRV
}
