
float4 color;

struct PS_INPUT
{
    float4 posH : SV_POSITION;
    float4 color : COLOR0;
    float2 texcoord : TEXCOORD0;
};

Texture2D major_texture;    //テクスチャ
SamplerState major_sampler; //テクスチャサンプラー

float4 main(PS_INPUT psin) : SV_TARGET
{
    //return major_texture.Sample(major_sampler, psin.texcoord);
    
    float4 texcolor = major_texture.Sample(major_sampler, psin.texcoord);
    //return texcolor;
    
    //return color;
    return psin.color * texcolor * color;//テクスチャとカラーのRGB掛け算
}