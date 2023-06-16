/*******************************************************************************************
*
*   raylib [core] example - Basic window (adapted for HTML5 platform)
*
*   This example is prepared to compile for PLATFORM_WEB, PLATFORM_DESKTOP and PLATFORM_RPI
*   As you will notice, code structure is slightly diferent to the other examples...
*   To compile it for PLATFORM_WEB just uncomment #define PLATFORM_WEB at beginning
*
*   This example has been created using raylib 1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <raylib.h>

#include "raymath.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#define COLOR_00 (Color) {   4,   3,  48, 255 }
#define COLOR_01 (Color) {  28,   8, 145, 255 }
#define COLOR_02 (Color) {  74,  13, 242, 255 }
#define COLOR_03 (Color) { 162, 110, 247, 255 }
#define COLOR_04 (Color) { 237, 207, 252, 255 }
#define COLOR_05 (Color) { 218, 110, 247, 255 }
#define COLOR_06 (Color) { 219,  13, 242, 255 }
#define COLOR_07 (Color) { 145,   8, 138, 255 }
#define COLOR_08 (Color) {  44,   3,  48, 255 }
#define COLOR_09 (Color) { 116,   8, 145, 255 }
#define COLOR_10 (Color) { 166, 013, 242, 255 }
#define COLOR_11 (Color) { 162, 110, 247, 255 }
#define COLOR_12 (Color) { 219, 207, 252, 255 }
#define COLOR_13 (Color) { 130, 110, 247, 255 }
#define COLOR_14 (Color) {  20,  13, 242, 255 }
#define COLOR_15 (Color) {   8,  19, 145, 255 }

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
int screen_width = 1920;
int screen_height = 1080;

int max_iterations = 1000;
float zoom_factor = 450.0;
int x_offset = 250;

Color colors[] = {
    COLOR_00,
    COLOR_01,
    COLOR_02,
    COLOR_03,
    COLOR_04,
    COLOR_05,
    COLOR_06,
    COLOR_07,
    COLOR_08,
    COLOR_09,
    COLOR_10,
    COLOR_11,
    COLOR_12,
    COLOR_13,
    COLOR_14,
    COLOR_15
};

Image buffer;
Texture tex;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void main_loop(void);     // Update and Draw one frame

//----------------------------------------------------------------------------------
// Main Enry Point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screen_width, screen_height, "Mandelbrot Set Visualizer");

    buffer = GenImageColor(screen_width, screen_height, BLACK);

    for (int x = 0; x < screen_width; x++) {
        for (int y = 0; y < screen_height; y++) {
            float x0 = (float) (x - (screen_width / 2) - x_offset) / zoom_factor;
            float y0 = (float) (y - (screen_height / 2)) / zoom_factor;
            float x1 = 0.0f;
            float y1 = 0.0f;
            int i = 0;
            float x2 = 0.0f;
            float y2 = 0.0f;

            while (x2 + y2 <= 4 && i < max_iterations) {
                y1 = 2 * x1 * y1 + y0;
                x1 = x2 - y2 + x0;
                x2 = x1 * x1;
                y2 = y1 * y1;
                ++i;
            }

            if (i == max_iterations) {
                ImageDrawPixel(&buffer, x, y, BLACK);
            } else {
                ImageDrawPixel(&buffer, x, y, colors[i % 16]);
            }
        }
    }

    tex = LoadTextureFromImage(buffer);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        main_loop();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void main_loop(void)
{
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update your variables here
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    // DrawTexture(tex, screen_width / 2, screen_height / 2, WHITE);
    DrawTexture(tex, 0, 0, WHITE);

    EndDrawing();
    //----------------------------------------------------------------------------------
}
