
#include "ShaderInfo.cginc"

cbuffer SCREEN_MIDDLE_POS : register(b0)
{
    float2 screenPos;
};
cbuffer CB_WORLD_ATTENUATION : register(b1)
{
    float wAttenuation;
};
cbuffer CB_ENEMY : register(b2)
{
    float4 enemyInfo[ENEMY_MAX];
};

Texture2D<float4> waveRenderResource : register(t0);
Texture2D<float4> roadColor : register(t1);

struct INPUT_PS
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

float2 UV2Space(float2 uv)
{
    return float2(uv.x, 1 - uv.y);
}

float3 Mix(float3 a, float3 b, float t)
{
    return a + (b - a) * t;
}

float4 main(INPUT_PS input) : SV_Target
{
    float2 spaceIdx = (UV2Space(input.tex) - 0.5f)*2.0f;  // -1 ~ 1
    float2 offset = float2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
    float2 screenIdx = spaceIdx * offset;
    int2 curIdx = screenPos + spaceIdx * offset;

    
    float4 waveInfo = waveRenderResource[curIdx];
    float3 waveColor = waveInfo.xyz;
    float alpha = waveInfo.w - wAttenuation;
    
    //first priority - wave color & enemy area
    float sEnemyRad = ENEMY_RAD * ENEMY_RAD;
    float enemyArea = 0;
    for (int i = 0; i < ENEMY_MAX; ++i)
    {
        if (enemyInfo[i].w == ENEMY_NOT_EXIST) 
        {
            break;
        }
        else if(enemyInfo[i].w == ENEMY_OUTSCREEN)
        {
            continue;
        }
    
        float2 pos = float2(enemyInfo[i].x, enemyInfo[i].y);
        float2 distV = pos - screenIdx;
        if (dot(distV, distV) < sEnemyRad)
        {
            enemyArea = enemyInfo[i].z;
            break;
        }
    }
    
    float3 color = 0;

    if (waveColor.x != waveColor.y || waveColor.y != waveColor.z)
    {
        color=waveColor * alpha;
    }
    else
    {
        color = Mix(roadColor[curIdx].xyz, RED, enemyArea) * alpha;
    }

    return float4(max(0, color)/* + (roadColor[curIdx].w * 0.3f)*/, 1);

}






























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
            if (IsEqual(m1[row][col], m2[row][col]) == false)
            {
                idx.x = row;
                idx.y = col;
                return false;
            }
        }
    }

    return true;
}