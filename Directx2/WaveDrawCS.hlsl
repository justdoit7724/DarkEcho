
#include "ShaderInfo.cginc"

cbuffer CB_DEST_INFO : register(b0)
{
    int4 destTransform;
};
cbuffer CB_WORLD_ATTENUATION : register(b1)
{
    float wAttenuation;
}

StructuredBuffer<WAVE> waves;

// float3(ori color) - float(alpha)
RWTexture2D<float4> waveRenderResource : register(u0);

[numthreads(WAVE_MAX, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    if (waves[id.x].kind == WAVE_INACTIVE)
        return;

    float2 pos = waves[id.x].pos;
    float curRad = waves[id.x].rad;
    if (destTransform.x <= pos.x && pos.x <= destTransform.x + destTransform.z &&
        destTransform.y <= pos.y && pos.y <= destTransform.y + destTransform.w)
    {
        curRad = WAVE_SIZE_DEST;
    }

    float qRad = curRad * curRad;
    int fromX = max(pos.x - curRad, 0);
    int toX = min(pos.x + curRad, MAP_WIDTH);
    int fromY = max(pos.y - curRad, 0);
    int toY = min(pos.y + curRad, MAP_HEIGHT);

    
    float alpha = waves[id.x].curIntensity / waves[id.x].maxIntensity;
    float4 color = float4(waves[id.x].color, alpha + wAttenuation);
    
    for (int cx = fromX; cx <= toX; cx++)
    {
        for (int cy = fromY; cy <= toY; cy++)
        {
            float2 distV = float2(pos.x, pos.y) - float2(cx, cy);
            if (dot(distV, distV) <= qRad)
            {
                waveRenderResource[int2(cx, cy)] = float4(color.xyz, max(color.w, waveRenderResource[int2(cx, cy)].w));
            }
        }
    }
}