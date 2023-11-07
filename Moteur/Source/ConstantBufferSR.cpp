#include "ConstantBufferSR.h"
#include <wincodec.h>

ConstantBufferSR::ConstantBufferSR(XMMATRIX world, Texture texture)
    : ConstantBuffer(world, 2), m_texture(texture)// CREATION DU CBV
{
    // CREATION DU SRV

    // Décrit la Texture2D
    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.MipLevels = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.Width = imageWidth;
    textureDesc.Height = imageHeight;
    textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    textureDesc.DepthOrArraySize = 1;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;


    // Describe and create a SRV for the texture.
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;



    auto var1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);// Paramètre les propriétés de la Texture2D
    ID3D12Resource* ressource = nullptr;

    // Création de la Texture2D
    Engine::GetInstance()->Device->CreateCommittedResource(
        &var1,
        D3D12_HEAP_FLAG_NONE,
        &textureDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&ressource));

    /*************************/


    /*std::wstring filePath = L"";
    switch (m_texture)
    {
    case pierre:
        filePath = L"pierre";
        break;
    default:
        break;
    }
    filePath = L"Source/" + filePath + L".jfif";

    std::vector<UINT8> textureFile = LoadFromFile(filePath);


    D3D12_SUBRESOURCE_DATA textureData = {};
    textureData.pData = &textureFile[0];
    textureData.RowPitch = imageWidth * imagePixelSize;
    textureData.SlicePitch = textureData.RowPitch * imageHeight;
    
    
    UpdateSubresources(Engine::GetInstance()->CmdList, ressource, ressource, 0, 0, 1, &textureData);*/

    /*************************/

    Engine::GetInstance()->Device->CreateShaderResourceView(ressource, &srvDesc, m_cbvHeapDesc->GetCPUDescriptorHandleForHeapStart());// Créez le SRV

}

ConstantBufferSR::~ConstantBufferSR()
{
}

void ConstantBufferSR::SetGraphicsRoot()
{
    ConstantBuffer::SetGraphicsRoot();
    Engine::GetInstance()->CmdList->SetGraphicsRootConstantBufferView(1, m_buffer->GetGPUVirtualAddress());
}

void ConstantBufferSR::CreateTexture(ID3D12GraphicsCommandList* m_commandList)
{
    std::wstring filePath = L"";
    switch (m_texture)
    {
    case pierre:
        filePath = L"pierre";
        break;
    default:
        break;
    }
    filePath = L"Source/" + filePath + L".jfif";

    std::vector<UINT8> texture = LoadFromFile(filePath);

    ID3D12Resource* textureUploadHeap;

    // Describe and create a Texture2D.
    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.MipLevels = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.Width = imageWidth;
    textureDesc.Height = imageHeight;
    textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    textureDesc.DepthOrArraySize = 1;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

    auto var1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    ID3D12Resource* ressource = nullptr;
    Engine::GetInstance()->Device->CreateCommittedResource(
        &var1,
        D3D12_HEAP_FLAG_NONE,
        &textureDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&ressource));

    const UINT64 uploadBufferSize = GetRequiredIntermediateSize(ressource, 0, 1);

    auto var2 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto var3 = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

    // Create the GPU upload buffer.
    Engine::GetInstance()->Device->CreateCommittedResource(
        &var2,
        D3D12_HEAP_FLAG_NONE,
        &var3,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&textureUploadHeap));

    //Copy data to the intermediate upload heap and then schedule a copy
    // from the upload heap to the Texture2D.

    D3D12_SUBRESOURCE_DATA textureData = {};
    textureData.pData = &texture[0];
    textureData.RowPitch = imageWidth * imagePixelSize;
    textureData.SlicePitch = textureData.RowPitch * imageHeight;

    UpdateSubresources(m_commandList, ressource, textureUploadHeap, 0, 0, 1, &textureData);
    auto var4 = CD3DX12_RESOURCE_BARRIER::Transition(ressource, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    m_commandList->ResourceBarrier(1, &var4);
}



std::vector<UINT8> ConstantBufferSR::LoadFromFile(const std::wstring& filePath)
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

    if (SUCCEEDED(hr))
    {
        IWICBitmapDecoder* wicDecoder;

        // Charge le fichier
        hr = wicFactory->CreateDecoderFromFilename(
            filePath.c_str(),
            nullptr,
            GENERIC_READ,
            WICDecodeMetadataCacheOnDemand,
            &wicDecoder
        );

        if (SUCCEEDED(hr))
        {
            IWICBitmapFrameDecode* wicFrame;
            hr = wicDecoder->GetFrame(0, &wicFrame);

            if (SUCCEEDED(hr))
            {

                hr = wicFrame->GetSize(&imageWidth, &imageHeight);

                if (SUCCEEDED(hr))
                {
                    // Calcule la taille de l'image en octets
                    UINT imageSize = imageWidth * imageHeight * imagePixelSize; // 4 octets par pixel (RGBA)

                    // Cree un std::vector<UINT8> pour stocker les donnees de l'image
                    std::vector<UINT8> imageData(imageSize);

                    hr = wicFrame->CopyPixels(nullptr, imageWidth * imagePixelSize, imageSize, imageData.data());

                    if (SUCCEEDED(hr))
                    {
                        // imageData contient maintenant les donnees de l'image au format std::vector<UINT8>
                        return imageData;
                    }
                }
            }
        }
    }
    return std::vector<UINT8>();
}
