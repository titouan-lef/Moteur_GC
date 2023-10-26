struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};


// Dans votre shader HLSL
cbuffer ConstantBuffer : register(b0)
{
    float4x4 WorldViewProjection;
};

PSInput VSMain(float4 position : POSITION, float4 color : COLOR)
{
    PSInput result;

    result.position = position;
    result.color = color;

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
}


