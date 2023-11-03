#include "ShaderTexture.h"
#include "MyException.h"
#include <wincodec.h>

ID3D12RootSignature* ShaderTexture::m_rootSignature = ShaderTexture::CreateRootSignature();

ShaderTexture::ShaderTexture(Texture texture) : Shader(Type::texture, m_rootSignature)
{
    switch (texture)
    {
    case pierre:
        m_texturePath = L"pierre";
        break;
    default:
        break;
    }

    m_texturePath = L"source/" + m_texturePath + L".jfif";
}

ShaderTexture::~ShaderTexture()
{
    delete m_rootSignature;
}

ID3D12RootSignature* ShaderTexture::CreateRootSignature()
{
    /*
    * CD3DX12_DESCRIPTOR_RANGE(a, b, c) :
    * * a : type de descripteur
    * * b : nombre de descripteur
    * * c : regsitre du shader
    */
    auto tmp1 = CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    auto tmp2 = CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);

    // Tableau des paramètres de la signature racine(ici 3 seul)
    CD3DX12_ROOT_PARAMETER rootParameters[3];
    
    // Paramètre 0 : Constant Buffer (matrices)
    rootParameters[0].InitAsConstantBufferView(0); // L'emplacement de registre doit correspondre à "b0" dans votre shader.

    // Paramètre 1 : Texture (g_texture)
    rootParameters[1].InitAsDescriptorTable(1, &tmp1); // L'emplacement de registre doit correspondre à "t0" dans votre shader.

    // Paramètre 2 : Sampler (g_sampler)
    rootParameters[2].InitAsDescriptorTable(1, &tmp2); // L'emplacement de registre doit correspondre à "s0" dans votre shader.


    return Shader::CreateRootSignature(_countof(rootParameters), rootParameters);
}

void ShaderTexture::CreateTexture(ID3D12GraphicsCommandList* m_commandList)
{
    std::vector<UINT8> texture = LoadFromFile();

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

    Engine::Device->CreateCommittedResource(
        &var1,
        D3D12_HEAP_FLAG_NONE,
        &textureDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&m_texture));

    m_uploadBuffer = new UploadBuffer(m_texture);
}

std::vector<UINT8> ShaderTexture::LoadFromFile()
{
    IWICImagingFactory* wicFactory;
    CoInitialize(nullptr); // Initialise COM si ce n'est pas déjà fait.

    // Crée une instance de l'usine WIC
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
            m_texturePath.c_str(),
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

                    // Crée un std::vector<UINT8> pour stocker les données de l'image
                    std::vector<UINT8> imageData(imageSize);

                    hr = wicFrame->CopyPixels(nullptr, imageWidth * imagePixelSize, imageSize, imageData.data());

                    if (SUCCEEDED(hr))
                    {
                        // imageData contient maintenant les données de l'image au format std::vector<UINT8>
                        return imageData;
                    }
                }
            }
        }
    }
    return std::vector<UINT8>();
}

// Generate a simple black and white checkerboard texture.
/*
std::vector<UINT8> ShaderTexture::GenerateTextureData(UINT width, UINT height, UINT pixelSize)
{
    const UINT rowPitch = width * pixelSize;
    const UINT cellPitch = rowPitch >> 3;        // The width of a cell in the checkboard texture.
    const UINT cellHeight = width >> 3;    // The height of a cell in the checkerboard texture.
    const UINT textureSize = rowPitch * height;

    std::vector<UINT8> data(textureSize);
    UINT8* pData = &data[0];

    for (UINT n = 0; n < textureSize; n += pixelSize)
    {
        UINT x = n % rowPitch;
        UINT y = n / rowPitch;
        UINT i = x / cellPitch;
        UINT j = y / cellHeight;

        if (i % 2 == j % 2)
        {
            pData[n] = 0x00;        // R
            pData[n + 1] = 0x00;    // G
            pData[n + 2] = 0x00;    // B
            pData[n + 3] = 0xff;    // A
        }
        else
        {
            pData[n] = 0xff;        // R
            pData[n + 1] = 0xff;    // G
            pData[n + 2] = 0xff;    // B
            pData[n + 3] = 0xff;    // A
        }
    }
    return data;
}
*/