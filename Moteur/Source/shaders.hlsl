cbuffer ConstantBufferData : register(b0)
{
    matrix World;
    matrix View;
    //matrix Projection;
}

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PSInput VSMain(float4 position : POSITION, float4 color : COLOR)
{
    PSInput result;
    result.position = mul(mul(position, World), View);
    //result.position = mul(mul(mul(position, World), View), Projection);
    result.color = color;

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
}
