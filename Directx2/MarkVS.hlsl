
cbuffer CB_TRANSFORMATION : register(b0)
{
    row_major float4x4 worldMat;
    row_major float4x4 viewMat;
};

float4 main(float2 pos : POSITION) : SV_POSITION
{
    return mul(float4(pos, 0, 1), mul(worldMat, viewMat));

}