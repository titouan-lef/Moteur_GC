#include "Shaders.h"
#include <wrl/client.h>
#include <wincodec.h>

using Microsoft::WRL::ComPtr;

Shaders::Shaders()
{
}

Shaders::~Shaders()
{
}

void Shaders::SetHeap(ID3D12GraphicsCommandList* m_commandList) {
    ID3D12DescriptorHeap* ppHeaps[] = { m_srvHeap };
    m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
    m_commandList->SetGraphicsRootDescriptorTable(0, m_srvHeap->GetGPUDescriptorHandleForHeapStart());
}

void Shaders::CreateHeap() {
    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};

    srvHeapDesc.NumDescriptors = 1;
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    Engine::Device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvHeap));
}
    
void Shaders::CreateSignature() {

    /*
    * Création d'un descriptor table
    * cbvTable.Init(a, b, c) :
    * * b : nombre de constant buffer par objet
    * * c : regsitre du shader
    */
    CD3DX12_DESCRIPTOR_RANGE cbvTable;
    cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

    // Liste des différent Descriptor Range
    CD3DX12_DESCRIPTOR_RANGE descriptorRange[]{
        cbvTable
    };

    /*
    * Tableau des paramètres de la signature racine (ici 1 seul)
    * il existe 3 types de paramètres différents : root constant, root descriptor et descriptor table
    */
    CD3DX12_ROOT_PARAMETER slotRootParameter = CD3DX12_ROOT_PARAMETER();

    // Initialisation des paramètres de la signature racine
    slotRootParameter.InitAsDescriptorTable(_countof(descriptorRange), descriptorRange);

    // Description de la disposition de la signature racine
    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, &slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    // Transformation de la description en une structure de données qui peut être utilisée pour créer la signature racine
    ID3DBlob* serializedRootSig = nullptr;
    D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSig, nullptr);

    // Création de la signature racine
    (Engine::Device->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
}

void Shaders::CreatePSOTexture(LPCWSTR pFileName) {

    ID3DBlob* vertexShader;
    ID3DBlob* pixelShader;

#if defined(_DEBUG)
    // Enable better shader debugging with the graphics debugging tools.
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif
    
    D3DCompileFromFile(pFileName, nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr);
    D3DCompileFromFile(pFileName, nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr);


    D3D12_INPUT_ELEMENT_DESC inputElementDescsTexture[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // Create the PSO
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDescTexture = {};
    psoDescTexture.InputLayout = { inputElementDescsTexture, _countof(inputElementDescsTexture) };
    psoDescTexture.pRootSignature = m_rootSignature;
    psoDescTexture.VS = CD3DX12_SHADER_BYTECODE(vertexShader);
    psoDescTexture.PS = CD3DX12_SHADER_BYTECODE(pixelShader);
    psoDescTexture.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDescTexture.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDescTexture.DepthStencilState.DepthEnable = FALSE;
    psoDescTexture.DepthStencilState.StencilEnable = FALSE;
    psoDescTexture.SampleMask = UINT_MAX;
    psoDescTexture.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDescTexture.NumRenderTargets = 1;
    psoDescTexture.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDescTexture.SampleDesc.Count = 1;
    Engine::Device->CreateGraphicsPipelineState(&psoDescTexture, IID_PPV_ARGS(&m_pipelineState));
}

void Shaders::CreatePSOColor(LPCWSTR pFileName) {

    ID3DBlob* vertexShader;
    ID3DBlob* pixelShader;

#if defined(_DEBUG)
    // Enable better shader debugging with the graphics debugging tools.
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif
    D3DCompileFromFile(pFileName, nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr);
    D3DCompileFromFile(pFileName, nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr);


    // Define the vertex input layout.
    D3D12_INPUT_ELEMENT_DESC inputElementDescsColors[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // Create the PSO
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDescColor = {};
    psoDescColor.InputLayout = { inputElementDescsColors, _countof(inputElementDescsColors) };
    psoDescColor.pRootSignature = m_rootSignature;
    psoDescColor.VS = CD3DX12_SHADER_BYTECODE(vertexShader);
    psoDescColor.PS = CD3DX12_SHADER_BYTECODE(pixelShader);
    psoDescColor.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDescColor.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDescColor.DepthStencilState.DepthEnable = FALSE;
    psoDescColor.DepthStencilState.StencilEnable = FALSE;
    psoDescColor.SampleMask = UINT_MAX;
    psoDescColor.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDescColor.NumRenderTargets = 1;
    psoDescColor.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDescColor.SampleDesc.Count = 1;
    Engine::Device->CreateGraphicsPipelineState(&psoDescColor, IID_PPV_ARGS(&m_pipelineState));
}

void Shaders::CreateTexture(ID3D12GraphicsCommandList* m_commandList, const std::wstring& filePath)
{
    //std::vector<UINT8> texture = GenerateTextureData(width, height, pixelSize);
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

    Engine::Device->CreateCommittedResource(
        &var1,
        D3D12_HEAP_FLAG_NONE,
        &textureDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&m_texture));

    const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_texture, 0, 1);

    auto var2 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto var3 = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

    // Create the GPU upload buffer.
    Engine::Device->CreateCommittedResource(
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

    UpdateSubresources(m_commandList, m_texture, textureUploadHeap, 0, 0, 1, &textureData);
    auto var4 = CD3DX12_RESOURCE_BARRIER::Transition(m_texture, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    m_commandList->ResourceBarrier(1, &var4);

    // Describe and create a SRV for the texture.
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    Engine::Device->CreateShaderResourceView(m_texture, &srvDesc, m_srvHeap->GetCPUDescriptorHandleForHeapStart());
}

// Generate a simple black and white checkerboard texture.
std::vector<UINT8> Shaders::GenerateTextureData(UINT width, UINT height, UINT pixelSize)
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

// Load form a file
std::vector<UINT8> Shaders::LoadFromFile(const std::wstring& filePath)
{
    ComPtr<IWICImagingFactory> wicFactory;
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
        ComPtr<IWICBitmapDecoder> wicDecoder;

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
            ComPtr<IWICBitmapFrameDecode> wicFrame;
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