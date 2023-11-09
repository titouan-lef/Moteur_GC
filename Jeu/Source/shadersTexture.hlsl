cbuffer ConstantBufferData : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);


struct VertexIn
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VertexOut
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VertexOut VSMain(VertexIn vIn)
{
    VertexOut vOut;
    
    float4 position = { vIn.position.x, vIn.position.y, vIn.position.z, 1 };
    
    vOut.position = mul(mul(mul(position, World), View), Projection);
    //result.position = position;
    vOut.uv = vIn.uv;

    return vOut;
}

float4 PSMain(VertexOut vOut) : SV_TARGET
{
   // return float4(vOut.uv.xxx, 1);
   return g_texture.Sample(g_sampler, vOut.uv);
}