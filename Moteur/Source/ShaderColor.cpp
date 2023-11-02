#include "ShaderColor.h"
#include "MyException.h"

ID3D12RootSignature* ShaderColor::m_rootSignature = ShaderColor::CreateRootSignature();

ShaderColor::ShaderColor() : Shader(Type::color, m_rootSignature)
{
}

ShaderColor::~ShaderColor()
{
    delete m_rootSignature;
}

ID3D12RootSignature* ShaderColor::CreateRootSignature()
{
    /*
    * CD3DX12_DESCRIPTOR_RANGE(a, b, c) :
    * * a : type de descripteur
    * * b : nombre de descripteur
    * * c : regsitre du shader
    */
    auto tmp = CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

    // Tableau des paramètres de la signature racine (ici 1 seul)
    CD3DX12_ROOT_PARAMETER rootParameters[1];
    rootParameters[0].InitAsDescriptorTable(1, &tmp); // Pour les constant buffer (registre c0)

    return Shader::CreateRootSignature(_countof(rootParameters), rootParameters);
}
