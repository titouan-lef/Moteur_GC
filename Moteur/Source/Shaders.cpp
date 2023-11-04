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

void Shaders::UpdateHeap(ID3D12GraphicsCommandList* m_commandList) {
    ID3D12DescriptorHeap* ppHeaps[] = { m_srvHeap };
    m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
    m_commandList->SetGraphicsRootDescriptorTable(0, m_srvHeap->GetGPUDescriptorHandleForHeapStart());
}

void Shaders::CreateHeap(ID3D12Device* m_device) {
    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};

    srvHeapDesc.NumDescriptors = 1;
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    m_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvHeap));
}

void Shaders::CreateSignature(ID3D12Device* m_device) {

    D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

    // This is the highest version the sample supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
    featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

    if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
    {
        featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
    }

    CD3DX12_DESCRIPTOR_RANGE1 ranges[1];
    ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);

    CD3DX12_ROOT_PARAMETER1 rootParameters[1];
    rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);

    D3D12_STATIC_SAMPLER_DESC sampler = {};
    sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
    sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
    sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
    sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
    sampler.MipLODBias = 0;
    sampler.MaxAnisotropy = 0;
    sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    sampler.MinLOD = 0.0f;
    sampler.MaxLOD = D3D12_FLOAT32_MAX;
    sampler.ShaderRegister = 0;
    sampler.RegisterSpace = 0;
    sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 1, &sampler, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ID3DBlob* signature;
    ID3DBlob* error;
    D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error);
    m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
}

void Shaders::PipelineStateTexture(ID3D12Device* m_device, ID3D12PipelineState** m_pipelineState, LPCWSTR pFileName) {

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
    m_device->CreateGraphicsPipelineState(&psoDescTexture, IID_PPV_ARGS(m_pipelineState));
}

void Shaders::PipelineStateColor(ID3D12Device* m_device, ID3D12PipelineState** m_pipelineState, LPCWSTR pFileName) {

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
    m_device->CreateGraphicsPipelineState(&psoDescColor, IID_PPV_ARGS(m_pipelineState));
}

void Shaders::CreateTexture(ID3D12Device* m_device, ID3D12GraphicsCommandList* m_commandList, const std::wstring& filePath)
{
    //std::vector<UINT8> texture = GenerateTextureData();
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

    m_device->CreateCommittedResource(
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
    m_device->CreateCommittedResource(
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
    m_device->CreateShaderResourceView(m_texture, &srvDesc, m_srvHeap->GetCPUDescriptorHandleForHeapStart());
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

// Generate a simple black and white checkerboard texture.
std::vector<UINT8> Shaders::GenerateTextureData()
{
    const UINT rowPitch = 500 * imagePixelSize;
    const UINT cellPitch = rowPitch >> 3;        // The width of a cell in the checkboard texture.
    const UINT cellHeight = 500 >> 3;    // The height of a cell in the checkerboard texture.
    const UINT textureSize = rowPitch * 500;

    std::vector<UINT8> data(textureSize);
    UINT8* pData = &data[0];

    for (UINT n = 0; n < textureSize; n += imagePixelSize)
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
