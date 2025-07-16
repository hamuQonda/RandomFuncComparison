#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "pcg_basic.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#define STD_RAND    0
#define RLIB_RAND   1
#define PCG_RAND    2
#define NUM_RAND_FUNC 3

static uint32_t zHash, count;
static clock_t start, end;
static double cpu_time[NUM_RAND_FUNC];

void UpdateDrawFrame(void)
{
    BeginDrawing();

    DrawText("Standard rand", 10, 10, 30, RAYWHITE);
    DrawText(TextFormat("cpu time : %.4f sec", cpu_time[STD_RAND]), 10, 45, 40, YELLOW);

    DrawText("raylib's GetRandomValue", 10, 110, 30, RAYWHITE);
    DrawText(TextFormat("cpu time : %.4f sec", cpu_time[RLIB_RAND]), 10, 145, 40, YELLOW);

    DrawText("PCG's pcg32_boundedrand_r", 10, 210, 30, RAYWHITE);
    DrawText(TextFormat("cpu time : %.4f sec", cpu_time[PCG_RAND]), 10, 245, 40, YELLOW);

    EndDrawing();
}

int main(void)
{

    InitWindow(800, 600, "Random Function Comparison");

    ////////////////////////////////////////////////////
    // óêêîî≠ê∂ë¨ìxî‰är
    ////////////////////////////////////////////////////

    //-- rand()ä÷êî --
    zHash = 0;
    count = 0;
    start = clock();
    for (uint32_t i = 0; i < 10000000; i++) {
        zHash ^= rand();
    }
    end = clock();
    cpu_time[STD_RAND] = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nÅyrand_s()Åz\n");
    printf("Hash: %d\n", zHash);
    printf("CPUéûä‘ÅF%.6f ïb\n\n", cpu_time[STD_RAND]);

    //-- GetRandomValue() --
    zHash = 0;
    count = 0;
    start = clock();
    for (uint32_t i = 0; i < 10000000; i++) {
        zHash ^= GetRandomValue(0, INT_MAX);
    }
    end = clock();
    cpu_time[RLIB_RAND] = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nÅyGetRandomValue()Åz\n");
    printf("Hash: %u\n", zHash);
    printf("CPUéûä‘ÅF%.6f ïb\n\n", cpu_time[RLIB_RAND]);

    //-- pcg32_boundedrand_r() --
    pcg32_random_t rng;
    uint32_t max = 2147483647u;
    zHash = 0;
    count = 0;
    start = clock();
    for (uint32_t i = 0; i < 10000000; i++) {
        zHash ^= pcg32_boundedrand_r(&rng, max + 1);
    }
    end = clock();
    cpu_time[PCG_RAND] = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nÅypcg32_boundedrand_r()Åz\n");
    printf("Hash: %d\n", zHash);
    printf("CPUéûä‘ÅF%.6f ïb\n\n", cpu_time[PCG_RAND]);


#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }
#endif

    CloseWindow();

    return 0;
}