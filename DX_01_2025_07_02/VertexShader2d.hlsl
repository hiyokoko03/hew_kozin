cbuffer VS_CONSTANT_BUFFER0 : register(b0)
{
    float4x4 proj; //プロジェクション変換
};

cbuffer VS_CONSTANT_BUFFER1 : register(b1)
{
    float4x4 world; //ワールド変換
};


struct VS_INPUT
{
    float4 posL : POSITION0;
    float4 color : COLOR0;
    float2 texcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 posH : SV_POSITION;
    float4 color : COLOR0;
    float2 texcoord : TEXCOORD0;
};

VS_OUT main(VS_INPUT vsin)
{
    VS_OUT vsout;
  
    float4x4 mtx = mul(world, proj);    //mulは行列の合成
    vsout.posH = mul(vsin.posL, mtx);
    
    vsout.color = vsin.color;
    vsout.texcoord = vsin.texcoord;
    
    return vsout;
}