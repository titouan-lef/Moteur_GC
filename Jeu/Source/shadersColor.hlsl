cbuffer ConstantBufferData : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
};

struct VertexIn
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VertexOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VertexOut VSMain(VertexIn vIn)
{
    VertexOut vOut;
    
    float4 position = { vIn.position.x, vIn.position.y, vIn.position.z, 1 };
    
    vOut.position = mul(mul(mul(position, World), View), Projection);
    vOut.color = vIn.color;
    
    return vOut;
}

float4 PSMain(VertexOut vOut) : SV_TARGET
{
    return vOut.color;
}
