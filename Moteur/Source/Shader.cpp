#include "Shader.h"
#include "MyException.h"
#include "Engine.h"


Shader::Shader()
{
}

Shader::~Shader()
{
    //delete m_pso;
    //delete m_rootSignature;
}

void Shader::Init(std::vector<CD3DX12_ROOT_PARAMETER> rootParameters, std::vector<D3D12_STATIC_SAMPLER_DESC> samplers, std::wstring fileName, std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs)
{
    CreateRootSignature(rootParameters, samplers);
    CreatePSO(fileName, inputElementDescs);
}

void Shader::CreateRootSignature(std::vector<CD3DX12_ROOT_PARAMETER> rootParameters, std::vector<D3D12_STATIC_SAMPLER_DESC> samplers)
{
    // Description de la disposition de la signature racine
    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc((UINT)rootParameters.size(), rootParameters.data(), (UINT)samplers.size(), samplers.data(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    // Transformation de la description en une structure de données qui peut être utilisée pour créer la signature racine
    ID3DBlob* serializedRootSig = nullptr;
    D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSig, nullptr);

    // Création de la signature racine
    GFX_THROW_INFO_ONLY(Engine::GetInstance()->Device->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
}
    

void Shader::CreatePSO(std::wstring& fileName, std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs)
{
    #if defined(_DEBUG) 
        UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    #else
        UINT compileFlags = 0;
    #endif

    // Récupération des shaders compilés
    ID3DBlob* vertexShader = nullptr;
    ID3DBlob* pixelShader = nullptr;

    fileName = L"Source/shaders" + fileName + L".hlsl";

    GFX_THROW_INFO_ONLY(D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
    GFX_THROW_INFO_ONLY(D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));

    // Paramétrage de la pipeline state object (PSO).
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElementDescs.data(), (UINT)inputElementDescs.size()};
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
    GFX_THROW_INFO_ONLY(Engine::GetInstance()->Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pso)));
}