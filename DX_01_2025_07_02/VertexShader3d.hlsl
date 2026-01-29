// ==============================================================
// VertexShader3D.hlsl	
// 頂点シェーダー設定
// 
// 制作者:村上和樹		制作日付：2025/10/16	
// 更新者:村上和樹		更新日付：2025/10/16
// ==============================================================
cbuffer VS_CONSTANT_BUFFER0 : register(b0)
{
    float4x4 world;
};

cbuffer VS_CONSTANT_BUFFER1 : register(b1)
{
    float4x4 view;
};

cbuffer VS_CONSTANT_BUFFER2 : register(b2)
{
    float4x4 proj;
};

cbuffer VS_LIGHT_BUFFER : register(b3)
{
    float4x4 lightView;
    float4x4 lightProj;
};

struct VS_INPUT
{
    float3 LocalPos : POSITION0;
    float3 normalL  : NORMAL0;
    float4 color    : COLOR0;
    float2 texcood  : TEXCOOD0;
};

struct VS_OUTPUT
{
    float4 posH     : SV_POSITION;
    float3 normalW  : NORMAL0;
    float4 color    : COLOR0;
    float2 texcood  : TEXCOOD0;
    float4 shadowPos : TEXCOORD1; // Added
};

VS_OUTPUT main(VS_INPUT vs_input) 
{
    VS_OUTPUT vs_output;      //計算し終わった情報を入れるものを用意する
    
    //座標変換
    float4x4 mtxWV = mul(world,view);
    float4x4 mtxWVP = mul(mtxWV, proj);
    vs_output.posH = mul(float4(vs_input.LocalPos, 1.0f),mtxWVP);
    
    //ワールド空間の法線を作る
    vs_output.normalW = mul(float4(vs_input.normalL, 0.0f), world).xyz;
    
    vs_output.color = vs_input.color;
    vs_output.texcood = vs_input.texcood;

    // Shadow Coordinates
    float4x4 lightWVP = mul(world, mul(lightView, lightProj));
    vs_output.shadowPos = mul(float4(vs_input.LocalPos, 1.0f), lightWVP);
    
    return vs_output;
}