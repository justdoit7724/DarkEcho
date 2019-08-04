
cbuffer CB_TRANSPARENCY : register(b0)
{
    float tp;
};

Texture2D footprint : register(t0);
SamplerState samplerState : register(s0);

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_Target
{
    float3 texColor = footprint.Sample(samplerState, input.tex).xyz;
    texColor *= tp;
    return float4(texColor, texColor.x);
}