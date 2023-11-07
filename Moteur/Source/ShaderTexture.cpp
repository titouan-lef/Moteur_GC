#include "ShaderTexture.h"
#include "MyException.h"

ShaderTexture::ShaderTexture(ConstantBufferData* cbd, Texture texture) : Shader(Type::texture)
{
    m_rootSignature = CreateRootSignature();
    CreatePSO();

    // CREATION DU CBV ET DU SRV
    m_constBuffer = new ConstantBufferSR(cbd, texture);

    // CREATION DU SAMPLER
    D3D12_SAMPLER_DESC  sampler = {};
    sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;        // Filtrage de la texture
    sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;   // Mode de répétition en U
    sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;   // Mode de répétition en V
    sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;   // Mode de répétition en W
    sampler.MipLODBias = 0;
    sampler.MaxAnisotropy = 0;                              // Anisotropie (1 pour désactiver)
    sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    sampler.MinLOD = 0.0f;                                  // Niveau de détail minimum
    sampler.MaxLOD = D3D12_FLOAT32_MAX;                     // Niveau de détail maximum

    D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc = {};
    samplerHeapDesc.NumDescriptors = 1;                                 // Nombre de samplers à stocker
    samplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;          // Type de tas (pour les samplers)
    samplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;  // Doit être visible par les shaders

    Engine::GetInstance()->Device->CreateDescriptorHeap(&samplerHeapDesc, IID_PPV_ARGS(&m_samplerHeap));

    //Engine::GetInstance()->Device->CreateSampler(&sampler, samplerHeap->GetCPUDescriptorHandleForHeapStart());// Créez le Sampler

    m_descriptorHeaps = { m_constBuffer->m_cbvHeapDesc, m_samplerHeap };
}

ShaderTexture::~ShaderTexture()
{
    delete m_samplerHeap;
}

void ShaderTexture::SetGraphicsRoot()
{
    m_constBuffer->SetGraphicsRoot();

    D3D12_GPU_DESCRIPTOR_HANDLE sampler = m_samplerHeap->GetGPUDescriptorHandleForHeapStart();
    Engine::GetInstance()->CmdList->SetGraphicsRootDescriptorTable(2, sampler);
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


    rootParameters[0].InitAsDescriptorTable(1, &tmp1); // L'emplacement de registre doit correspondre à "t0" dans votre shader.
    rootParameters[1].InitAsConstantBufferView(0, 0); // L'emplacement de registre doit correspondre à "b0" dans votre shader.
    rootParameters[2].InitAsDescriptorTable(1, &tmp2);// L'emplacement de registre doit correspondre à "s0" dans votre shader.


    return Shader::CreateRootSignature(_countof(rootParameters), rootParameters);
}