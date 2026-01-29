// ==============================================================
// VertexShaderShadow.hlsl
// Shadow Pass Vertex Shader
// ==============================================================
cbuffer VS_CB0 : register(b0)
{
    float4x4 world;
};

cbuffer VS_CB1 : register(b1)
{
    float4x4 lightView;
};

cbuffer VS_CB2 : register(b2)
{
    float4x4 lightProj;
};

struct VS_INPUT
{
    float3 LocalPos : POSITION0;
    // Other inputs ignored for shadow pass
    float3 normalL  : NORMAL0;
    float4 color    : COLOR0;
    float2 texcood  : TEXCOOD0;
};

struct VS_OUTPUT
{
    float4 posH : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    float4x4 wvp = mul(world, mul(lightView, lightProj));
    output.posH = mul(float4(input.LocalPos, 1.0f), wvp);

    return output;
}
