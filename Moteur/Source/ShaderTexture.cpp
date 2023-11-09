#include "ShaderTexture.h"
#include "MyException.h"


ShaderTexture::ShaderTexture() : Shader()
{
    /*
    * CD3DX12_DESCRIPTOR_RANGE(a, b, c) :
    * * a : type de descripteur
    * * b : nombre de descripteur
    * * c : regsitre du shader
    */
    auto tmp1 = CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    std::vector<CD3DX12_ROOT_PARAMETER> rootParameters = {
       CD3DX12_ROOT_PARAMETER() // registre "t0" dans le shader
    };
    rootParameters[0].InitAsDescriptorTable(1, &tmp1);

    // CREATION DU SAMPLER
    std::vector<D3D12_STATIC_SAMPLER_DESC> samplers = {
       CD3DX12_STATIC_SAMPLER_DESC(0)// registre "s0" dans le shader
    };

    std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    Shader::Init(rootParameters, samplers, L"Texture", inputElementDescs);
}


ShaderTexture::~ShaderTexture()
{
}

void ShaderTexture::Init()
{
}

ShaderTexture* ShaderTexture::GetInstance()
{
    static ShaderTexture shaderTexture;
    return &shaderTexture;
}



























//
//ShaderTexture::ShaderTexture(XMMATRIX world, Texture texture) : Shader(Type::texture)
//{
//    m_rootSignature = CreateRootSignature();
//    CreatePSO();
//
//    // CREATION DU CBV ET DU SRV
//    m_constBuffer = new ConstantBufferSR(world, texture);
//
//    // CREATION DU SAMPLER
//    D3D12_SAMPLER_DESC  sampler = {};
//    sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;        // Filtrage de la texture
//    sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;   // Mode de répétition en U
//    sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;   // Mode de répétition en V
//    sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;   // Mode de répétition en W
//    sampler.MipLODBias = 0;
//    sampler.MaxAnisotropy = 0;                              // Anisotropie (1 pour désactiver)
//    sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
//    sampler.MinLOD = 0.0f;                                  // Niveau de détail minimum
//    sampler.MaxLOD = D3D12_FLOAT32_MAX;                     // Niveau de détail maximum
//
//    D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc = {};
//    samplerHeapDesc.NumDescriptors = 1;                                 // Nombre de samplers à stocker
//    samplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;          // Type de tas (pour les samplers)
//    samplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;  // Doit être visible par les shaders
//
//    Engine::GetInstance()->Device->CreateDescriptorHeap(&samplerHeapDesc, IID_PPV_ARGS(&m_samplerHeap));
//
//    //Engine::GetInstance()->Device->CreateSampler(&sampler, samplerHeap->GetCPUDescriptorHandleForHeapStart());// Créez le Sampler
//
//    m_descriptorHeaps = { m_constBuffer->m_cbvHeapDesc, m_samplerHeap };
//}
//
//ShaderTexture::~ShaderTexture()
//{
//    delete m_samplerHeap;
//}
//
//void ShaderTexture::SetGraphicsRoot()
//{
//    m_constBuffer->SetGraphicsRoot();
//
//    D3D12_GPU_DESCRIPTOR_HANDLE sampler = m_samplerHeap->GetGPUDescriptorHandleForHeapStart();
//    Engine::GetInstance()->CmdList->SetGraphicsRootDescriptorTable(2, sampler);
//}
//
//ID3D12RootSignature* ShaderTexture::CreateRootSignature()
//{
//    /*
//    * CD3DX12_DESCRIPTOR_RANGE(a, b, c) :
//    * * a : type de descripteur
//    * * b : nombre de descripteur
//    * * c : regsitre du shader
//    */
//    auto tmp1 = CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
//    auto tmp2 = CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);
//
//    // Tableau des paramètres de la signature racine(ici 3 seul)
//    CD3DX12_ROOT_PARAMETER rootParameters[3];
//
//
//    rootParameters[0].InitAsDescriptorTable(1, &tmp1); // L'emplacement de registre doit correspondre à "t0" dans votre shader.
//    rootParameters[1].InitAsConstantBufferView(0, 0); // L'emplacement de registre doit correspondre à "b0" dans votre shader.
//    rootParameters[2].InitAsDescriptorTable(1, &tmp2);// L'emplacement de registre doit correspondre à "s0" dans votre shader.
//
//
//    return Shader::CreateRootSignature(_countof(rootParameters), rootParameters);
//}
