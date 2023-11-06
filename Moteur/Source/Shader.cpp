#include "Shader.h"
#include "MyException.h"

Shader::Shader(Type type) : m_type(type)
{
}

Shader::~Shader()
{
    delete m_rootSignature;
    delete m_pipelineState;
    delete m_constBuffer;
}

ID3D12RootSignature* Shader::CreateRootSignature(UINT nbParam, CD3DX12_ROOT_PARAMETER rootParameters[])
{
    // Description de la disposition de la signature racine
    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(nbParam, rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    // Transformation de la description en une structure de données qui peut être utilisée pour créer la signature racine
    ID3DBlob* serializedRootSig = nullptr;
    D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSig, nullptr);

    // Création de la signature racine
    ID3D12RootSignature* rootSignature = nullptr;
    GFX_THROW_INFO_ONLY(Engine::Device->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&rootSignature)));

    return rootSignature;
}
    

void Shader::CreatePSO()
{
    #if defined(_DEBUG) 
        UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    #else
        UINT compileFlags = 0;
    #endif

    // Récupération des shaders compilés
    ID3DBlob* vertexShader = nullptr;
    ID3DBlob* pixelShader = nullptr;
    std::wstring fileName;
    LPCSTR inputName;
    DXGI_FORMAT dxgiFormat;

    switch (m_type)
    {
    case texture:
        fileName = L"Texture";
        inputName = "TEXCOORD";
        dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
        break;
    case color:
        fileName = L"Color";
        inputName = "COLOR";
        dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
        break;
    default:
        fileName = L"ErreurNomShader";
        inputName = "ErreurInputName";
        dxgiFormat = DXGI_FORMAT::DXGI_FORMAT_YUY2;
        break;
    }

    fileName = L"source/shaders" + fileName + L".hlsl";

    GFX_THROW_INFO_ONLY(D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
    GFX_THROW_INFO_ONLY(D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));

    // Définition du vertex input layout
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { inputName, 0, dxgiFormat, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // Paramétrage de la pipeline state object (PSO).
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
    psoDesc.pRootSignature = m_rootSignature;
    psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader);
    psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader);
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;

    // Création de la PSO
    GFX_THROW_INFO_ONLY(Engine::Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
}