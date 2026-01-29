// ==============================================================
// PixelShader3D.hlsl	
// ピクセルシェーダー設定
// 
// 制作者:村上和樹		制作日付：2025/10/16	
// 更新者:村上和樹		更新日付：2025/11/06
// ==============================================================
cbuffer PS_CONSTANT_BUFFER0 : register(b0)
{
    float4 ambient_color;
};

cbuffer PS_CONSTANT_BUFFER1 : register(b1)
{
    float4 diffuse_color;
    float4 diffuse_world_vector;
};

cbuffer PS_CONSTANT_BUFFER2 : register(b2)
{
    float4 material_diffuse_color;
};


struct PS_INPUT
{
    float4 posH : SV_POSITION;
    float3 normalW : NORMAL0;
    float4 color : COLOR0;
    float2 texcood : TEXCOOD0;
};

Texture2D major_texture;    //テクスチャ
SamplerState major_sampler; //テクスチャサンプラ

float4 main(PS_INPUT ps_input) : SV_TARGET
{
    float4 material = major_texture.Sample(major_sampler, ps_input.texcood) * ps_input.color * material_diffuse_color;
    float3 ambient = material.rgb * ambient_color.rgb;
    
    //float brightness = max(dot(-diffuse_world_vector.xyz, normalize(ps_input.normalW)),0.0f);
    float brightness = (dot(-diffuse_world_vector.xyz, normalize(ps_input.normalW)) + 1.0f) * 0.5f;
    
    float3 diffuse = material.rgb * diffuse_color.rgb * brightness;
    
    float alpha = material.a * ambient_color.a;
    return float4(ambient + diffuse, alpha);
}