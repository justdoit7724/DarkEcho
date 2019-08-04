
cbuffer CB_TRANSFORMATION : register(b0)
{
    row_major float4x4 worldMat;
    row_major float4x4 VPMat;
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

#define ANGLE180 3.141592
#define ANGLE90 ANGLE180/2.0f
#define IDENTITY float4x4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1)

bool IsEqual(float a, float b)
{
    const float elapse = 0.00001f;
    return (a < b + elapse && a + elapse > b);
}
bool IsEqual(float4x4 m1, float4x4 m2, out int2 idx)
{
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            if (IsEqual(m1[row][col], m2[row][col])==false)
            {
                idx.x = row;
                idx.y = col;
                return false;
            }
        }
    }

    return true;
}

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.tex = input.tex;
    
    output.pos = mul(float4(input.pos, 0, 1), mul(worldMat, VPMat));
    return output;
}