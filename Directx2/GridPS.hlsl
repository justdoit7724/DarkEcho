
#include "ShaderInfo.cginc"

cbuffer CB_GRID_INFO : register(b0)
{
    float2 interval;
    float2 cameraPos;
    float2 highlightInterval;
}

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

float2 UV2Space(float2 uv)
{
    return float2(uv.x, 1 - uv.y);
}

float4 main(PS_INPUT input) : SV_Target
{
    float2 screenIntervalRatio = float2(
        SCREEN_WIDTH / interval.x,
        SCREEN_HEIGHT / interval.y);
    float2 ratio2 = float2(
        SCREEN_WIDTH / highlightInterval.x,
        SCREEN_HEIGHT / highlightInterval.y);
    
    float2 camOffset = float2(
        cameraPos.x / SCREEN_WIDTH,
        cameraPos.y / SCREEN_HEIGHT);

    float2 sUV = UV2Space(input.uv) - 0.5f + camOffset;
    float2 nUV = sUV;
    nUV.x *= screenIntervalRatio.x;
    nUV.y *= screenIntervalRatio.y;

    nUV = frac(nUV);

    float grid = smoothstep(0.97f, 0.99f, nUV.x) + smoothstep(0.03f, 0.01f, nUV.x);
    grid += smoothstep(0.97f, 0.99f, nUV.y) + smoothstep(0.03f, 0.01f, nUV.y);

    float2 hUV = sUV;
    hUV.x *= ratio2.x;
    hUV.y *= ratio2.y;

    hUV = frac(hUV);

    float hGrid = smoothstep(0.996f, 0.997f, hUV.x) + smoothstep(0.004f, 0.003f, hUV.x);
    hGrid += smoothstep(0.996f, 0.997f, hUV.y) + smoothstep(0.004f, 0.003f, hUV.y);

    grid = saturate(grid);
    hGrid = saturate(hGrid);

    float3 gridOnly = grid - hGrid;
    float3 r = (RED * hGrid) + gridOnly;

    return float4(r, (hGrid + grid)*0.5f);


}