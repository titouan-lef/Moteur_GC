#include "Texture.h"
#include "Engine.h"
#include "MyException.h"
#include "DDSTextureLoader.h"
#include <wincodec.h>


Texture::Texture()
{
}

Texture::~Texture()
{
}

void Texture::CreateTexture(UINT id, std::wstring fileName, ID3D12DescriptorHeap* cbvSrvUavHeap, UINT m_cbvSrvUavDescriptorSize)
{
    m_id = id;
    m_fileName = L"Source/" + fileName + L".dds";

    CD3DX12_GPU_DESCRIPTOR_HANDLE gpu(cbvSrvUavHeap->GetGPUDescriptorHandleForHeapStart());// Récupération de l'emplacement prévu pour la "surface de dessin" (= render target) 0
    gpu.Offset(id, m_cbvSrvUavDescriptorSize);

    GetSizeImg();    

    // Décrit la Texture2D
    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.MipLevels = 1;
    textureDesc.Format = DXGI_FORMAT_BC1_UNORM;
    textureDesc.Width = m_width;
    textureDesc.Height = m_height;
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

    DirectX::CreateDDSTextureFromFile12(
        Engine::GetInstance()->Device, Engine::GetInstance()->CmdList,
        m_fileName.c_str(),
        m_resource, m_uploadHeap);

    m_gpu = gpu;
}

void Texture::CreateShaderResourceView(ID3D12DescriptorHeap* cbvSrvUavHeap, UINT m_cbvSrvUavDescriptorSize)
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE cpu(cbvSrvUavHeap->GetCPUDescriptorHandleForHeapStart());
    cpu.Offset(m_id, m_cbvSrvUavDescriptorSize);
    Engine::GetInstance()->Device->CreateShaderResourceView(m_resource.Get(), &m_srvDesc, cpu);// Créez le SRV
}

void Texture::GetSizeImg()
{
    IWICImagingFactory* wicFactory;
    CoInitialize(nullptr); // Initialise COM si ce n'est pas deja fait.

    // Cree une instance de l'usine WIC
    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&wicFactory)
    );

    IWICBitmapDecoder* wicDecoder;

    // Charge le fichier
    wicFactory->CreateDecoderFromFilename(
        m_fileName.c_str(),
        nullptr,
        GENERIC_READ,
        WICDecodeMetadataCacheOnDemand,
        &wicDecoder
    );

    IWICBitmapFrameDecode* wicFrame;
    hr = wicDecoder->GetFrame(0, &wicFrame);
    hr = wicFrame->GetSize(&m_width, &m_height);
}
