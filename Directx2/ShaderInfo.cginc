

#define EPSILON 0.000001f

#define RED float3(1,0,0)
#define GREEN float3(0,1,0)
#define BLUE float3(0,0,1)
#define YELLOW float3(1,1,0)
#define BLACK float3(0,0,0)

#define SCREEN_WIDTH 960.0f
#define SCREEN_HEIGHT 960.0f
#define MAP_WIDTH (SCREEN_WIDTH*3)
#define MAP_HEIGHT (SCREEN_HEIGHT*3)
#define MAP_DIVISION_NUM_WIDTH 30
#define MAP_DIVISION_NUM_HEIGHT 30

struct WAVE
{
    float2 pos;
    float2 origin;
    float2 dir;
    float3 color;

    float speed;
    float rad;
    float curIntensity;
    float maxIntensity;
    int kind;
};

#define WAVE_INACTIVE 0
#define WAVE_MAX 512
#define WAVE_SIZE_DEST 5.0f

#define ENEMY_MAX 7
#define ENEMY_RAD 100
#define ENEMY_NOT_EXIST 0
#define ENEMY_OUTSCREEN 1
#define ENEMY_INSCREEN 2