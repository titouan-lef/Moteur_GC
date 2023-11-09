#include "ShaderColor.h"

ShaderColor::ShaderColor() : Shader()
{
    /*
    * CD3DX12_DESCRIPTOR_RANGE(a, b, c) :
    * * a : type de descripteur
    * * b : nombre de descripteur
    * * c : regsitre du shader
    */
    std::vector<CD3DX12_ROOT_PARAMETER> rootParameters = {};

    // CREATION DU SAMPLER
    std::vector<D3D12_STATIC_SAMPLER_DESC> samplers = {};

    std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    Shader::Init(rootParameters, samplers, L"Color", inputElementDescs);
}


ShaderColor::~ShaderColor()
{
}

void ShaderColor::Init()
{
}

ShaderColor* ShaderColor::GetInstance()
{
    static ShaderColor shaderTexture;
    return &shaderTexture;
}