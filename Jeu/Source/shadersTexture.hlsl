cbuffer ConstantBufferData : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);


struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

PSInput VSMain(float4 position : POSITION, float2 uv : TEXCOORD)
{
    PSInput result;
    
    result.position = mul(mul(mul(position, World), View), Projection);
    //result.position = position;
    result.uv = uv;

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return g_texture.Sample(g_sampler, input.uv);
}
