
cbuffer CB_COLOR : register(b0)
{
    float3 color;
};

float4 main(float4 pos:SV_POSITION) : SV_Target
{
    return float4(color, 1);
}