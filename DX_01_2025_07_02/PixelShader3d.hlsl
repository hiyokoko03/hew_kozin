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
    float4 shadowPos : TEXCOORD1;
};

Texture2D major_texture : register(t0);    //eNX`
SamplerState major_sampler : register(s0); //eNX`Tv

Texture2D shadow_map : register(t1);       // Shadow Map
SamplerState shadow_sampler : register(s1);

float4 main(PS_INPUT ps_input) : SV_TARGET
{
    float4 material = major_texture.Sample(major_sampler, ps_input.texcood) * ps_input.color * material_diffuse_color;
    float3 ambient = material.rgb * ambient_color.rgb;
    
    // Start with default brightness
    float brightness = (dot(-diffuse_world_vector.xyz, normalize(ps_input.normalW)) + 1.0f) * 0.5f;

    // Shadow Calculation
    float3 projCoords = ps_input.shadowPos.xyz / ps_input.shadowPos.w;
    projCoords.x = projCoords.x * 0.5f + 0.5f;
    projCoords.y = -projCoords.y * 0.5f + 0.5f;

    float shadow = 1.0f;
    if (projCoords.z > 0.0f && projCoords.z < 1.0f && 
        projCoords.x > 0.0f && projCoords.x < 1.0f && 
        projCoords.y > 0.0f && projCoords.y < 1.0f)
    {
        // Simple depth comparison
        // float depth = shadow_map.Sample(shadow_sampler, projCoords.xy).r;
        // Bias to prevent shadow acne
        float bias = 0.005f;
        // PCF (Percentage Closer Filtering) for softer edges
        float shadowSum = 0.0f;
        for(int x = -1; x <= 1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
                 float depth = shadow_map.SampleLevel(shadow_sampler, projCoords.xy + float2(x, y) * 0.001f, 0).r; // 0.001 depends on map size (1/1024)
                 if(depth < projCoords.z - bias)
                 {
                     shadowSum += 1.0f;
                 }
            }
        }
        shadow = 1.0f - (shadowSum / 9.0f) * 0.5f; // 0.5 shadow intensity
    }

    float3 diffuse = material.rgb * diffuse_color.rgb * brightness * shadow;
    
    float alpha = material.a * ambient_color.a;
    return float4(ambient + diffuse, alpha);
}