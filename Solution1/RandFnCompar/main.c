#include "raylib.h"
//#include <stdio.h>
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

static uint32_t zHash;
static clock_t start, end;
static double cpu_time[NUM_RAND_FUNC];

void UpdateDrawFrame(void)
{
    BeginDrawing();

    DrawText("Standard rand()", 10, 10, 30, LIGHTGRAY);
    DrawText(TextFormat("cpu time : %00.4f sec", cpu_time[STD_RAND]), 10, 45, 40, LIGHTGRAY);

    DrawText("raylib's GetRandomValue()", 10, 130, 30, RAYWHITE);
    DrawText(TextFormat("cpu time : %.4f sec", cpu_time[RLIB_RAND]), 10, 165, 40, RAYWHITE);

    DrawText("PCG's pcg32_boundedrand_r()", 10, 250, 30, WHITE);
    DrawText(TextFormat("cpu time : %.4f sec", cpu_time[PCG_RAND]), 10, 285, 40, WHITE);

    EndDrawing();
}

int main(void)
{
    InitWindow(800, 600, "Random Function Comparison");

    //-- rand() function --
    zHash = 0;
    start = clock();
    for (uint32_t i = 0; i < 10000000; i++) zHash ^= rand();
    end = clock();
    cpu_time[STD_RAND] = (double)(end - start) / CLOCKS_PER_SEC;

    //-- GetRandomValue() function --
    zHash = 0;
    start = clock();
    for (uint32_t i = 0; i < 10000000; i++) zHash ^= GetRandomValue(0, INT_MAX);
    end = clock();
    cpu_time[RLIB_RAND] = (double)(end - start) / CLOCKS_PER_SEC;

    //-- pcg32_boundedrand_r() function --
    pcg32_random_t rng;
    uint32_t max = 2147483647u;
    zHash = 0;
    start = clock();
    for (uint32_t i = 0; i < 10000000; i++) zHash ^= pcg32_boundedrand_r(&rng, max + 1);
    end = clock();
    cpu_time[PCG_RAND] = (double)(end - start) / CLOCKS_PER_SEC;

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