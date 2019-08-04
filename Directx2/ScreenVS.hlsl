
cbuffer CB_TRANSFORMATION : register(b0)
{
    row_major float4x4 worldMat;
    row_major float4x4 viewMat;
};

struct VS_INPUT
{
    float2 pos : POSITION;
    float2 tex : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    output.pos = mul(float4(input.pos, 0, 1), mul(worldMat, viewMat));
    output.tex = input.tex;

    return output;
}