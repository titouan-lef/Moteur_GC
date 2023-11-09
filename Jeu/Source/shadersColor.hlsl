cbuffer ConstantBufferData : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
};

struct VertexIn
{
    float4 position : POSITION;
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
    
    vOut.position = mul(mul(mul(vIn.position, World), View), Projection);
    vOut.color = vIn.color;
    
    return vOut;
}

float4 PSMain(VertexOut vOut) : SV_TARGET
{
    return vOut.color;
}
