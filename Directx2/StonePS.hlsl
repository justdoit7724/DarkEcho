
cbuffer CB_TRANSPARENCY : register(b0)
{
    float tp;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_Target
{
    // round stone shape
    float dist = length(input.tex - float2(0.5f, 0.5f));
    if(dist < 0.55f)
        return 1.0f;
    return 0;
}